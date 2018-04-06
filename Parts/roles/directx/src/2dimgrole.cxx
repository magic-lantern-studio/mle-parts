/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 2dimgrole.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 2D Image Role targeting
 * the DirectX platform.
 *
 * @author Mark S. Millard
 * @date Nov 20, 2007
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


// Include system header files

// Include Magic Lantern header files
#include "mle/MleActor.h"
#include "mle/MleLoad.h"
#include "mle/mlMalloc.h"

// Include Magic Lantern DirectX platform header files.
#include "dxs/2dimgrole.h"
#include "dxs/imagmref.h"
#include "dxs/dx2dset.h"


MLE_ROLE_SOURCE(Mle2dImgRole, Mle2dRole);


Mle2dImgRole::Mle2dImgRole(MleActor *actor)
 : Mle2dRole(actor)
{
    screenPosition.setValue(ML_SCALAR(0), ML_SCALAR(0));
    displayState = FALSE;

	// Get the DirectX stage for the associated device.
	DxStage *theStage = DxStage::cast(MleStage::g_theStage);

	// Create a new Magic Lantern surface and initialize it.
	m_surface = new DxsSurface();
	m_surface->init(theStage->getD3dDevice());
}


Mle2dImgRole::~Mle2dImgRole(void)
{
    if (m_surface)
		delete m_surface;
}


void
Mle2dImgRole::screenLocation(MlVector2 &pos)
{
    screenPosition.setValue(pos.getValue());
}


void
Mle2dImgRole::load(MlMediaRef img)
{
    MleImageMediaRef *imageMediaRef;

    imageMediaRef = (MleImageMediaRef *) mlLoadMediaRef(img, NULL);

    if (imageMediaRef)
	{
		// Remember the Media Reference identifier.
		m_imgMediaRef = img;

		// Read the image data into the supplied surface.
	    imageMediaRef->read(m_surface);

		// Register with the stage to free up resources when DirectX resets.
		DxStage *theStage = DxStage::cast(MleStage::g_theStage);
		theStage->addResetCallback(Mle2dImgRole::onReset, this);
	} else
	{
		m_imgMediaRef = MLE_NO_MEDIA;
	    return;
	}
}


void
Mle2dImgRole::display(int state)
{
    displayState = state;
}


void
Mle2dImgRole::draw(void *data)
{
    // Declare local variables.
    MlScalar xPos, yPos, min[2], max[2];
	IDirect3DSurface9 *srcSurface;
    IDirect3DSurface9 *destSurface;
    int xs, ys, xd, yd;
    int srcWidth,srcHeight,destWidth,destHeight;

    if (displayState)
	{
		D3DSURFACE_DESC srcDesc, destDesc;

        screenPosition.getValue(xPos,yPos);
        xd = mlScalarToLong(xPos);
        yd = mlScalarToLong(yPos);

		srcSurface = m_surface->getD3DSurface();
        destSurface = ((Mle2dSet *)m_set)->getOffscreenBuffer();

		// Get the boundaries for clipping.
		srcSurface->GetDesc(&srcDesc);
		destSurface->GetDesc(&destDesc);
        srcWidth = srcDesc.Width;
        srcHeight = srcDesc.Height;
        destWidth = destDesc.Width;
        destHeight = destDesc.Height;

        // Check for trivial clipping.
	    if ((xd >= destWidth) || (yd >= destHeight))
	        return;
	    if ((destWidth <= 0) || (destHeight <= 0))
	        return;
    	if ((srcWidth <= 0) || (srcHeight <= 0))
	        return;

    	xs = ys = 0;

		if ((xd + srcWidth) > destWidth)
			srcWidth = destWidth - xd;

		if ((yd + srcHeight) > destHeight)
		{
			//int origHeight = srcHeight;
			srcHeight = destHeight - yd;
			//ys = origHeight - srcHeight;
		}

		// On Windows, the origin is the upper, left-hand corner;
		// therefore, we have to compensate for the vertical offset.
		//yd = destHeight - yd - srcHeight;

        // Copy pixel data into rendering buffer.
		RECT srcRect;
		srcRect.left = xs;
		srcRect.top = ys;
		//srcRect.bottom = ys;
		srcRect.right = xs + srcWidth;
		//srcRect.top = ys + srcHeight;
		srcRect.bottom = ys + srcHeight;
		m_surface->bltFast(destSurface, xd, yd, srcRect);

        // Set extent of Role for dirty rectangle calculation.
        min[0] = mlLongToScalar(xd);
        min[1] = mlLongToScalar(yd);
        max[0] = mlLongToScalar(xd + srcWidth - 1);
        max[1] = mlLongToScalar(yd + srcHeight - 1);
        setBounds(min, max);
    }
}

void
Mle2dImgRole::onReset(unsigned int ev, void *calldata)
{
	Mle2dImgRole *role = (Mle2dImgRole *)calldata;

	if (ev == RELEASE_RESOURCE)
	{
		// Delete the surface associated with the Role's image.
		role->m_surface->deleteSurface();

	} else if (ev == CREATE_RESOURCE)
	{
		// Reallocate the surface and load in the Role's image.
		if (role->m_imgMediaRef != MLE_NO_MEDIA)
			role->load(role->m_imgMediaRef);
	}
}
