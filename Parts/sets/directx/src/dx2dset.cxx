/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file dx2dset.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 2D Set targeting the DirectX
 * platform.
 *
 * @author Mark S. Millard
 * @date Nov 13, 2007
 */

// COPYRIGHT_BEGIN
//
//  Copyright (C) 2000-2007  Wizzer Works
//
//  Wizzer Works makes available all content in this file ("Content").
//  Unless otherwise indicated below, the Content is provided to you
//  under the terms and conditions of the Common Public License Version 1.0
//  ("CPL"). A copy of the CPL is available at
//
//      http://opensource.org/licenses/cpl1.0.php
//
//  For purposes of the CPL, "Program" will mean the Content.
//
//  For information concerning this Makefile, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include system header files.
#include <limits.h>    // For INT_MAX, INT_MIN.

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleDirector.h"

// Include DirectX target header files.
#include "mle/2drole.h"
#include "dxs/dx2dset.h"


// Macro to calculate width of image which is DWORD aligned.
#define WIDTHBYTES(bits) ((((bits) + 31) & ~31) >> 3)


MLE_SET_SOURCE(Mle2dSet, MleSet);


Mle2dSet::Mle2dSet(void)
{
    DxStage *stage;
    MleSchedulerItem *id;

    // Initialize default property values.
    position.setValue(ML_SCALAR(0), ML_SCALAR(0));
    size.setValue(ML_SCALAR(640), ML_SCALAR(480));

    // Initialize member variables.
    m_headRole = NULL;
    m_renderCBId = NULL;
    m_resizeCBId = NULL;

    m_viewBoundsCallback = NULL;
    m_lastDirtyRectangle = 0;

    // Add the Set to the Stage.  The stage is in control of the set rendering.
    // No render() function is passed to the scheduler for periodic Set
    // rendering.  The stage adds this set's render() function
    // into the Set phase of the scheduler.
    stage = DxStage::cast(MleStage::g_theStage);
    id = stage->addSet((void (*)(MleSet *)) render, this);
    setRenderCBId(id);

	// Initialize surface.
	m_offscreenSurface.init(stage->getD3dDevice());

    // Create the palette.
    // XXX -- The color pallete should come from a Set Property.
    //        The property would be a MediaRef for a color palette
    //        which can be installed into the Stage as the system
    //        palette. Note that a Stage really has no way of loading
    //        in a palette for itself (no dynamic properties).
    //
    //        For now, use the current palette entries from the Stage.
    //        How does the title set this? and where? (previously
    //        hardcoded).
 
    // Create the off-screen buffer using the resize event callback.
    Mle2dSet::resizeEventCB(WM_SIZE,NULL,(void *)this);
}


Mle2dSet::~Mle2dSet(void)
{
    // Unschedule the set render() function for the PC stage only.
    if (m_renderCBId)
        g_theTitle->m_theScheduler->remove(m_renderCBId);

    // Uninstall resize event callback.
    if (m_resizeCBId)
        g_theTitle->m_theEventMgr->uninstallEventCB(WM_SIZE,m_resizeCBId);

    // Delete all attached Roles.  The destructor of each 2D Role will
    // call this set's detach() function and update the headrole pointer.
    // So when the headrole is NULL, all the attached Roles are
    // deleted.
    while(m_headRole)
	    delete m_headRole;
}


void
Mle2dSet::init()
{
    // Insert resize callback into event dispatch manager.
    m_resizeCBId = g_theTitle->m_theEventMgr->installEventCB(
        WM_SIZE,Mle2dSet::resizeEventCB,(void *)this);

    // Bump priority of dispatched callback.
    g_theTitle->m_theEventMgr->changeEventCBPriority(WM_SIZE,m_resizeCBId,
        MLE_RESIZE_SET_PRIORITY);
}


void
Mle2dSet::attach(MleRole *curR, MleRole *newR)
{
    Mle2dRole *currentRole, *newRole;

    MLE_ASSERT(newR);
    newRole = Mle2dRole::cast(newR);

    if (curR)
	{
		// Attach after "curR" Role.
		currentRole = Mle2dRole::cast(curR);

		newRole->m_next = currentRole->m_next;
		newRole->m_prev = currentRole;
		currentRole->m_next = newRole;
		if (newRole->m_next)
			newRole->m_next->m_prev = newRole;
	}
    else
	{
		// Attach to the end of the list.
		if (m_headRole)
		{
			for (currentRole = m_headRole; currentRole->m_next; )
				currentRole = currentRole->m_next;
			currentRole->m_next = newRole;
			newRole->m_prev = currentRole;
			newRole->m_next = NULL;
		}
		else
		{
			m_headRole = newRole;
			newRole->m_next = NULL;
			newRole->m_prev = NULL;
		}
    }
}


void
Mle2dSet::detach(MleRole *curR)
{
    Mle2dRole *role;

    MLE_ASSERT(curR);
    role = Mle2dRole::cast(curR);

    if (role == m_headRole)
	{
		if (m_headRole->m_next)
			m_headRole->m_next->m_prev = NULL;
		m_headRole = m_headRole->m_next;
    }
    else
	{
		role->m_prev->m_next = role->m_next;
		if (role->m_next)
			role->m_next->m_prev = role->m_prev;
    }
}


// I would be interested in finding out if the ViewBoundsCallback and the
// g_curSet variable could be made into private static data members of the
// class - not important, but wraps things up a bit neater than using
// friend functions.

static void ViewBoundsCallback(Mle2dRole *);

// Keep a copy of the current set for use by the bounds callback.
static Mle2dSet *g_curSet;


void
Mle2dSet::render(MleSet *s)
{
    Mle2dSet *set = Mle2dSet::cast(s);
    DxStage *theStage = DxStage::cast(MleStage::g_theStage);
	IDirect3DSurface9 *offscreenBuffer = set->getOffscreenBuffer();

	// Obtain the region to render to.
	D3DLOCKED_RECT lockedRect;
	offscreenBuffer->LockRect(&lockedRect, NULL, 0);
	DWORD *pixelBuffer = (DWORD *)lockedRect.pBits;

    // Set clearBounds based on the union of dirty rectangles,
    // this was computed as the renderBounds of the previous frame.
    set->m_clearBounds = set->m_renderBounds;

    // Clear off-screen buffer using dirty rectangle list.
    int rectIndex;
    DxStageDirtyRect *dp;

    for (rectIndex = 0, dp = set->m_dirtyRectangles; 
	     rectIndex < set->m_lastDirtyRectangle; 
	     rectIndex++, dp++)
	{
	    unsigned int xStart = dp->min_x;
		unsigned int xEnd = dp->max_x + 1;
		for (unsigned int i = xStart; i < xEnd; i++)
		{
			unsigned int yStart = dp->min_y;
			unsigned int yEnd = dp->max_y + 1;
			for (unsigned int j = yStart; j < yEnd; j++)
			{
				// Index into the surface, note we use the pitch and
				// divide by four since the pitch is given in bytes and
				// there are 4 bytes per DWORD.
				int index = i * lockedRect.Pitch / 4 + j;
				pixelBuffer[index] = 0x00000000;  // Black - should be background parameter.

			}
		}
    }
	
	// Release the Direct3D surface. Any rendering performed by the Roles below
	// will be responsible for managing the locking/unlocking of this buffer.
	offscreenBuffer->UnlockRect();

    // Reset dirty rectangle list.
    set->m_lastDirtyRectangle = 0;
    set->m_renderBounds.min_x = INT_MAX;
    set->m_renderBounds.min_y = INT_MAX;
    set->m_renderBounds.max_x = INT_MIN;
    set->m_renderBounds.max_y = INT_MIN;

    // Hook dirty rectangle callback, and render view
    g_curSet = set;  // Use static to pass to ViewBoundsCallback
    Mle2dSetViewBoundsCallback old_cb =
        set->setViewBoundsCallback(ViewBoundsCallback);

    // Process all 2D Roles attached to this Set.
    Mle2dRole *nextRole = set->m_headRole;

    while (nextRole != NULL)
	{
		// XXX - we should be passing in the IDirect3DSurface9 that the Role
		// Should be rendering to. For now, Mle2DRole ignores the argument.
		nextRole->draw(NULL);

        // Invoke the callback function on this Role.
        if (set->m_viewBoundsCallback)
            set->m_viewBoundsCallback(nextRole);

        nextRole = nextRole->m_next;
    }

    set->setViewBoundsCallback(old_cb);

    // Tell the Stage the latest regions that need to be blitted.
    theStage->dirtyRegion(&(set->m_renderBounds));
    //theStage->dirtyRegion(&(set->m_clearBounds));

	// Transfer the off-screen buffer to the Stage.
	RECT bounds;
	bounds.left = set->m_renderBounds.min_x;
	//bounds.bottom = set->m_renderBounds.min_y;
	bounds.top = set->m_renderBounds.min_y;
	bounds.right = set->m_renderBounds.max_x;
	//bounds.top = set->m_renderBounds.max_y;
	bounds.bottom = set->m_renderBounds.max_y;
	IDirect3DSurface9 *backBuffer = theStage->getPixelBuffer();
	IDirect3DDevice9 *device = theStage->getD3dDevice();
	set->m_offscreenSurface.bltFast(backBuffer, 0, 0, bounds);
	ULONG count = backBuffer->Release();
}

MlBoolean
Mle2dSet::resize(int width, int height)
{
	DxStage *theStage = DxStage::cast(MleStage::g_theStage);

	// Remove previous surface.
	m_offscreenSurface.deleteSurface();

    // Create the off-screen buffer.
	HRESULT hr;
	hr = m_offscreenSurface.createSurface(width, height, D3DFMT_X8R8G8B8);
	if (hr != DXS_OK)
	{
		return FALSE;
	} else
	{
		// Add a callback to be notified if the DirectX device is reset.
		theStage->addResetCallback(Mle2dSet::onReset, this);
	}

	return TRUE;
}

int
Mle2dSet::resizeEventCB(MleEvent event,void *callData,void *clientData)
{
    // Declare local variables.
	MlBoolean status = FALSE;

	// Dereference the Stage and Set.
    Mle2dSet *set = (Mle2dSet *)clientData;
    DxStage *theStage = DxStage::cast(MleStage::g_theStage);

	// We don't want to resize while the Stage is resetting the DirectX device.
	if (theStage->isResetting())
		return status;

    // Retrieve width and height of the stage.
    // XXX --  This is not really correct; the width and height should be
    //         obtained from 2D set properties.
	int width, height;
    theStage->getSize(&width,&height);

	// Resize the set.
	status = set->resize(width, height);

    return status;
}


void
ViewBoundsCallback(Mle2dRole *role)
{
    // Declare local variables.
    DxStageDirtyRect *dp = g_curSet->m_dirtyRectangles;
    MlScalar x,y;
    MlScalar width,height;
    MlScalar min[2], max[2];
    DxStage *stage = DxStage::cast(MleStage::g_theStage);
    int xoffset = 0;
    int yoffset = 0;

    role->getBounds(min, max);
    x = min[0] - ML_SCALAR(xoffset);
    y = min[1] - ML_SCALAR(yoffset);
    width = max[0] - min[0] + ML_SCALAR_ONE;
    height = max[1] - min[1] + ML_SCALAR_ONE;

    // Accumulate total bounding rectangle.
    if (x < ML_SCALAR(g_curSet->m_renderBounds.min_x))
        g_curSet->m_renderBounds.min_x = mlScalarToLong(x);
    if (y < ML_SCALAR(g_curSet->m_renderBounds.min_y))
        g_curSet->m_renderBounds.min_y = mlScalarToLong(y);
    if (x + width > ML_SCALAR(g_curSet->m_renderBounds.max_x + 1))
        g_curSet->m_renderBounds.max_x =
			mlScalarToLong(x) + mlScalarToLong(width) - 1;
    if (y + height > ML_SCALAR(g_curSet->m_renderBounds.max_y + 1))
        g_curSet->m_renderBounds.max_y =
			mlScalarToLong(y) + mlScalarToLong(height) - 1;

    // Store into dirty rectangle list; if list is full, merge current
    // into last dirty rectangle.
    if (g_curSet->m_lastDirtyRectangle >= MAX_DIRTY_RECTANGLES)
	{
        dp = g_curSet->m_dirtyRectangles + g_curSet->m_lastDirtyRectangle - 1;
        if (x < ML_SCALAR(dp->min_x))
            dp->min_x = mlScalarToLong(x); 
        if (y < ML_SCALAR(dp->min_y))
            dp->min_y = mlScalarToLong(y);
        if (x + width > ML_SCALAR(dp->max_x + 1))
            dp->max_x = mlScalarToLong(x) + mlScalarToLong(width) - 1;
        if (y + height > ML_SCALAR(dp->max_y + 1))
            dp->max_y = mlScalarToLong(y) + mlScalarToLong(height) - 1;
    } else
	{
        dp = g_curSet->m_dirtyRectangles + g_curSet->m_lastDirtyRectangle;
        dp->min_x = mlScalarToLong(x);
        dp->min_y = mlScalarToLong(y);
        dp->max_x = mlScalarToLong(x) + mlScalarToLong(width) - 1;
        dp->max_y = mlScalarToLong(y) + mlScalarToLong(height) - 1;

        g_curSet->m_lastDirtyRectangle++;
    }
}

void
Mle2dSet::onReset(unsigned int ev, void *calldata)
{
	Mle2dSet *set = (Mle2dSet *)calldata;

	if (ev == RELEASE_RESOURCE)
	{
		// Remove the DirectX surface since we are being reset.
		set->m_offscreenSurface.deleteSurface();
	} else if (ev == CREATE_RESOURCE)
	{
		// Reallocate the DirectX surface since we are done resetting.
		DxStage *theStage = DxStage::cast(MleStage::g_theStage);

	    // Retrieve width and height of the stage.
		// XXX --  This is not really correct; the width and height should be
		//         obtained from 2D set properties.
		int width, height;
		theStage->getSize(&width,&height);

		// Resize the set.
		set->resize(width, height);
	}
}
