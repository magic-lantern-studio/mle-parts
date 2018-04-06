/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file br2dset.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 2D Set targeting the BRender
 * platform.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
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
#ifdef WIN32
#include <limits.h>    // For INT_MAX, INT_MIN.
#endif /* WIN32 */

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"

// Include Magic Lantern Runtime Engine header files.
#ifdef WIN32
#include "mle/MleDirector.h"
#endif /* WIN32 */

// Include BRender target header files.
#include "mle/2drole.h"
#include "mle/br2dset.h"
#include "mle/brstage.h"
#ifdef WIN32
#include "mle/pcstage.h"
#endif /* WIN32 */


#ifdef WIN32
// Macro to calculate width of image which is DWORD aligned.
#define WIDTHBYTES(bits) ((((bits) + 31) & ~31) >> 3)
#endif /* WIN32 */


MLE_SET_SOURCE(Mle2dSet, MleSet);


#ifdef MLE_REHEARSAL
void
Mle2dSet::initClass(void)
{
    mlRegisterSetClass(Mle2dSet, MleSet);

    mlRegisterSetMember(Mle2dSet, position, MlVector2);
    mlRegisterSetMember(Mle2dSet, size, MlVector2);
}
#endif	/* MLE_REHEARSAL */


Mle2dSet::Mle2dSet(void)
{
    BrStage *stage;
    MleSchedulerItem *id;

    //
    // Initialize default property values.
    //
    position.setValue(ML_SCALAR(0), ML_SCALAR(0));
    size.setValue(ML_SCALAR(640), ML_SCALAR(480));

    //
    // Initialize member variables.
    //
    m_headRole = NULL;
#ifdef WIN32
    m_renderCBId = NULL;
#ifndef _GDK_
    m_resizeCBId = NULL;
#endif /* _GDK_ */
#endif /* WIN32 */

#ifdef WIN32
    m_viewBoundsCallback = NULL;
    m_lastDirtyRectangle = 0;
#endif	/* WIN32 */

    //
    // Add the Set to the Stage.  For both the rehearsal player and the
    // mastered player on SGI, the stage is in control of the set rendering.
    // No render() function is passed to the scheduler for periodic Set
    // rendering.  For the PC, the stage adds this set's render() function
    // into the Set phase of the scheduler.
    //
    stage = BrStage::cast(MleStage::g_theStage);
    id = stage->addSet((void (*)(MleSet *)) render, this);
#ifdef WIN32
    setRenderCBId(id);

    //
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
    //

#if !defined(_GDK_)
    //
    // Create the off-screen buffer using the resize event callback.
    //
    Mle2dSet::resizeEventCB(WM_SIZE,NULL,(void *)this);
#endif /* !_GDK_ */
#endif /* WIN32 */
}


Mle2dSet::~Mle2dSet(void)
{
#ifdef WIN32
    //
    // Unschedule the set render() function for the PC stage only.
    //
    if (m_renderCBId)
        g_theTitle->m_theScheduler->remove(m_renderCBId);

#ifndef _GDK_
    //
    // Uninstall resize event callback.
    //
    if (m_resizeCBId)
        g_theTitle->m_theEventMgr->uninstallEventCB(WM_SIZE,m_resizeCBId);
#endif /* _GDK_ */
#endif /* WIN32 */

    //
    // Delete all attached Roles.  The destructor of each 2D Role will
    // call this set's detach() function and update the headrole pointer.
    // So when the headrole is NULL, all the attached Roles are
    // deleted.
    //
    while(m_headRole)
	    delete m_headRole;
}


void
Mle2dSet::init()
{
#ifdef WIN32
#ifndef _GDK_
    // Insert resize callback into event dispatch manager.
    m_resizeCBId = g_theTitle->m_theEventMgr->installEventCB(
        WM_SIZE,Mle2dSet::resizeEventCB,(void *)this);

    // Bump priority of dispatched callback.
    g_theTitle->m_theEventMgr->changeEventCBPriority(WM_SIZE,m_resizeCBId,
        MLE_RESIZE_SET_PRIORITY);
#endif /* _GDK_ */
#endif /* WIN32 */
}


void
Mle2dSet::attach(MleRole *curR, MleRole *newR)
{
    Mle2dRole *currentRole, *newRole;

    MLE_ASSERT(newR);
    newRole = Mle2dRole::cast(newR);

    if (curR)
	{
		//
		// Attach after "curR" Role.
		//
		currentRole = Mle2dRole::cast(curR);

		newRole->m_next = currentRole->m_next;
		newRole->m_prev = currentRole;
		currentRole->m_next = newRole;
		if (newRole->m_next)
			newRole->m_next->m_prev = newRole;
	}
    else
	{
		//
		// Attach to the end of the list.
		//
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


#ifdef WIN32
// I would be interested in finding out if the ViewBoundsCallback and the
// g_curSet variable could be made into private static data members of the
// class - not important, but wraps things up a bit neater than using
// friend functions.

static void ViewBoundsCallback(Mle2dRole *);

// Keep a copy of the current set for use by the bounds callback.
static Mle2dSet *g_curSet;
#endif /* WIN32 */


void
Mle2dSet::render(MleSet *s)
{
    Mle2dSet *set = Mle2dSet::cast(s);
    BrStage *stage = BrStage::cast(MleStage::g_theStage);

#ifdef WIN32
    //
    // Set clearBounds based on the union of dirty rectangles,
    // this was computed as the renderBounds of the previous frame.
    //
    set->m_clearBounds = set->m_renderBounds;

    //
    // Clear screen and Z buffer using dirty rectangle list,
    // the screen clear probably should be done in the stage.
    //
    int rectIndex;
    brwin_dirty_rect *dp;
    int xoffset = stage->getPixelBuffer()->origin_x;
    int yoffset = stage->getPixelBuffer()->origin_y;

    for (rectIndex = 0, dp = set->m_dirtyRectangles; 
	     rectIndex < set->m_lastDirtyRectangle; 
	     rectIndex++, dp++)
	{
		BrPixelmapRectangleFill(stage->getPixelBuffer(),
		    (br_int_16) (dp->min_x - xoffset),
			(br_int_16) (dp->min_y - yoffset),
			(br_int_16) (dp->max_x - dp->min_x + 1),
			(br_int_16) (dp->max_y - dp->min_y + 1),
			0);

		BrPixelmapRectangleFill(stage->getDepthBuffer(),
			(br_int_16) (dp->min_x - xoffset),
			(br_int_16) (dp->min_y - yoffset),
			(br_int_16) (dp->max_x - dp->min_x + 1),
			(br_int_16) (dp->max_y - dp->min_y + 1),
			0xFFFFFFFF);
    }

    //
    // Reset dirty rectangle list.
    //
    set->m_lastDirtyRectangle = 0;
    set->m_renderBounds.min_x = INT_MAX;
    set->m_renderBounds.min_y = INT_MAX;
    set->m_renderBounds.max_x = INT_MIN;
    set->m_renderBounds.max_y = INT_MIN;

    // Hook dirty rectangle callback, and render view
    g_curSet = set;  // Use static to pass to ViewBoundsCallback
    Mle2dSetViewBoundsCallback old_cb =
        set->setViewBoundsCallback(ViewBoundsCallback);
#endif	/* WIN32 */

    //
    // Process all 2D Roles attached to this set.
    //
    Mle2dRole *nextRole = set->m_headRole;
    br_pixelmap *pixelBuffer = stage->getPixelBuffer();

    while (nextRole != NULL)
	{
		nextRole->draw((void *) pixelBuffer);

#ifdef WIN32
        //
        // Invoke the callback function on this Role.
        //
        if (set->m_viewBoundsCallback)
            set->m_viewBoundsCallback(nextRole);
#endif	/* WIN32 */

        nextRole = nextRole->m_next;
    }

#ifdef WIN32
    set->setViewBoundsCallback(old_cb);

    //
    // Tell the Stage the latest regions that need to be blitted.
    //
    stage->dirtyRegion(&(set->m_renderBounds));
    stage->dirtyRegion(&(set->m_clearBounds));
#endif	/* WIN32 */
}


#ifdef MLE_REHEARSAL
void
Mle2dSet::getBounds(MlScalar min[3], MlScalar max[3])
{
    //
    // XXX - Use Set properties for x, y instead of hard coded numbers.
    //
    min[0] = MLE_SCALAR_ZERO;
    min[1] = MLe_SCALAR_ZERO;
    min[2] = MLE_SCALAR(-1);

    max[0] = MLE_SCALAR(639);
    max[1] = MLE_SCALAR(479);
    max[2] = MLE_SCALAR_ZERO;
}


MleRole *
Mle2dSet::pickDelegate(int x, int y)
{
    Mle2dRole *curRole;
    MlScalar min[2], max[2];

    if (! m_headRole)
		return NULL;

    //
    // Start pick selection from the top layer.
    //
    for (curRole = m_headRole; curRole->next; )
		curRole = curRole->next;

    for(; curRole; curRole = curRole->prev)
	{
		curRole->getBounds(min, max);
		if (x >= mlScalarToLong(min[0]) && y >= mlScalarToLong(min[1]) &&
			x <= mlScalarToLong(max[0]) && y <= mlScalarToLong(max[1]))
			break;
    }

    return curRole;
}


int
Mle2dSet::pushActor(MleActor *actor)
{
    MleRole *tmpRole;
    Mle2dRole *role, *prevRole;
    int retStat;

    MLE_ASSERT(actor);
    tmpRole = actor->getRole();
    MLE_ASSERT(tmpRole);
    role = Mle2dRole::cast(tmpRole);

    if (role == m_headRole)
		//
		// The Role is already at the bottom layer.
		//
		retStat = -1;
    else
	{
		prevRole = role->prev;

		detach(role);
		if (prevrole->prev)
			//
			// Reattach it after prevrole->prev.
			//
			attach(prevrole->prev, role);
		else
		{
			//
			// Reattach it as the head of the list.
			//
			MLE_ASSERT(prevRole == m_headRole);
			prevrole->prev = role;
			role->next = prevRole;
			role->prev = NULL;
			m_headRole = role;
		}

		retStat = 0;
    }

    return retStat;
}


int
Mle2dSet::pushActorToBottom(MleActor *actor)
{
    MleRole *tmpRole;
    Mle2dRole *role;
    int retStat;

    MLE_ASSERT(actor);
    tmpRole = actor->getRole();
    FW_ASSERT(tmpRole);
    role = Mle2dRole::cast(tmpRole);

    if (role == m_headRole)
		//
		// The Role is already at the bottom layer.
		//
		retStat = -1;
    else
	{
		detach(role);
		//
		// Reattach it at the bottom layer (head of the list).
		//
		m_headRole->prev = role;
		role->next = m_headRole;
		role->prev = NULL;
		m_headrole = role;

		retStat = 0;
    }

    return retStat;
}


int
Mle2dSet::popActor(MleActor *actor)
{
    MleRole *tmpRole;
    Mle2dRole *role, *nextRole;
    int retStat;

    MLE_ASSERT(actor);
    tmpRole = actor->getRole();
    MLE_ASSERT(tmpRole);
    role = Mle2dRole::cast(tmpRole);

    if (! role->next)
		//
		// The Role is already at the top layer.
		//
		retStat = -1;
    else
	{
		nextRole = role->next;

		detach(role);
		attach(nextRole, role);

		retStat = 0;
    }

    return retStat;
}


int
Mle2dSet::popActorToTop(MleActor *actor)
{
    Mlerole *tmpRole;
    Mle2drole *role, *nextRole;
    int retStat;

    MLE_ASSERT(actor);
    tmpRole = actor->getRole();
    MLE_ASSERT(tmpRole);
    role = Mle2dRole::cast(tmpRole);

    if (! role->next)
		//
		// The Role is already at the top layer.
		//
		retStat = -1;
    else
	{
		//
		// Get to the top layer (the end of the list).
		//
		for (nextRole = role->next; nextRole->next;)
			nextRole = nextRole->next;

		detach(role);
		attach(nextRole, role);

		retStat = 0;
    }

    return retStat;
}
#endif	/* MLE_REHEARSAL */


#ifdef WIN32
#if defined(_GDK_)
int
Mle2dSet::resizeEventCB(
    MlEvent /*event*/,void * /*callData*/,void * /*clientData*/)
{
    return(TRUE);
}
#else /* !_GDK_ */
int
Mle2dSet::resizeEventCB(MleEvent event,void *callData,void *clientData)
{
    // Declare local variables.
    Mle2dSet *set = (Mle2dSet *) clientData;
    PCBRenderStage *stage = PCBRenderStage::cast(MleStage::g_theStage);

    // XXX -- this code may need to be in the init() method. The init() method
    //        will be available when Sets have access to their properties.
    //        Most of the data below will exist as properties on the 2D
    //        set and the info structure will be initialized from that
    //        information.
    struct {
        BITMAPINFOHEADER header;
        RGBQUAD colors[MLE_PALETTE_SIZE];
    } info;
    int width,height;

    //
    // Retrieve width and height of the stage.
    // XXX --  This is not really correct; the width and height should be
    //         obtained from 2D set properties.
    //
    stage->getSize(&width,&height);

    //
    // Fill in the header info.
    //
    memset(&info,0,sizeof(info));
    info.header.biSize = sizeof(BITMAPINFOHEADER);
    info.header.biWidth = width;
    info.header.biHeight = height;
    info.header.biPlanes = 1;
    info.header.biBitCount = MLE_BIT_COUNT;
    info.header.biCompression = BI_RGB;
    info.header.biSizeImage = WIDTHBYTES(width * MLE_BIT_COUNT) * height;
    info.header.biClrUsed = MLE_PALETTE_SIZE;

    //
    // Fill in the color table info. Note that this will construct
    // a 1-to-1 index translation table into the color palette.
    //
    short *index = (short *)info.colors;
    for (int i = 0; i < MLE_PALETTE_SIZE; i++)
	{
        index[i] = i;
    }

    //
    // Create the off-screen buffer. Note that the actual buffer will be
    // the same one we use for rendering the BRender Scene Graph.
    //
    char *osb = stage->getOffScreenBuffer();
    set->m_dib.create((BITMAPINFO *)&info,(BYTE *)osb);

    return(TRUE);
}
#endif /* !_GDK_ */


void
ViewBoundsCallback(Mle2dRole *role)
{
    // Declare local variables.
    brwin_dirty_rect *dp = g_curSet->m_dirtyRectangles;
    MlScalar x,y;
    MlScalar width,height;
    MlScalar min[2], max[2];
    PCBRenderStage *stage = PCBRenderStage::cast(MleStage::g_theStage);
    int xoffset = stage->getPixelBuffer()->origin_x;
    int yoffset = stage->getPixelBuffer()->origin_y;

    role->getBounds(min, max);
    x = min[0] - ML_SCALAR(xoffset);
    y = min[1] - ML_SCALAR(yoffset);
    width = max[0] - min[0] + ML_SCALAR_ONE;
    height = max[1] - min[1] + ML_SCALAR_ONE;

    //
    // Accumulate total bounding rectangle.
    //
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

    //
    // Store into dirty rectangle list; if list is full, merge current
    // into last dirty rectangle.
    //
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
#endif /* WIN32 */
