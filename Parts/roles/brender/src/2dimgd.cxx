/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 2dimgd.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 2D Image Role.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2000-2025 Wizzer Works
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END


// Include system header files
#if defined(MLE_REHEARSAL)
#include <GL/gl.h>
#endif /* MLE_REHEARSAL */

// Include Magic Lantern header files
#include "mle/MleLoad.h"
#include "mle/2dimgd.h"
#include "mle/mlMalloc.h"
#include "mle/imagmref.h"

#if defined(_WINDOWS)
#include "mle/MleActor.h"
#include "mle/pcstage.h"
#include "mle/br2dset.h"
#elif defined(MLE_REHEARSAL)
#include "mle/sgibrstage.h"
#include "mle/2dimga.h"
#endif /* _WINDOWS */


MLE_ROLE_SOURCE(Mle2dImgRole, Mle2dRole);


Mle2dImgRole::Mle2dImgRole(MleActor *actor)
 : Mle2dRole(actor)
{
    screenPosition.setValue(ML_SCALAR(0), ML_SCALAR(0));
    displayState = FALSE;
#if defined(MLE_REHEARSAL)
    imageData = NULL;
    imageOp = NULL;
    inImg = NULL;
#endif /* MLE_REHEARSAL */
}


Mle2dImgRole::~Mle2dImgRole(void)
{
#if defined(MLE_REHEARSAL)
    if (imageData)
        mlFree(imageData);
    if (imageOp)
        delete imageOp;
    if (inImg)
        delete inImg;
#endif /* MLE_REHEARSAL */
}


void
Mle2dImgRole::screenLocation(MlVector2 &pos)
{
    screenPosition.setValue(pos.getValue());

#if defined(MLE_REHEARSAL)

    // Update its bounding box.

    int imageWidth, imageHeight;
    MlScalar min[2], max[2];

    if(imageOp)
    {
        imageWidth = imageOp->getWidth();
         imageHeight = imageOp->getHeight();
    }
    else
    {
        // This is to handle the case where media is not loaded in correctly.

        getBounds(min, max);
        imageWidth = mlScalarToLong(max[0]) - mlScalarToLong(min[0]) + 1;
        imageHeight = mlScalarToLong(max[1]) - mlScalarToLong(min[1]) + 1;
    }
    min[0] = pos[0];
    min[1] = pos[1];
    max[0] = pos[0] + mlLongToScalar(imageWidth) - ML_SCALAR_ONE;
    max[1] = pos[1] + mlLongToScalar(imageHeight) - ML_SCALAR_ONE;
    setBounds(min, max);
#endif /* MLE_REHEARSAL */
}


void
Mle2dImgRole::load(MlMediaRef img)
{
    MleImageMediaRef *imageMediaRef;

    imageMediaRef = (MleImageMediaRef *) mleLoadMediaRef(img, NULL);

#if defined(_WINDOWS)
    // Declare local variables.
    MlePaletteType *dibPalette;

    if (imageMediaRef)
        imageMediaRef->read(dib);
    else
        return;

    // Map the palette here to that of the forum's palette.
    dibPalette = ((Mle2dSet *)forum)->getPalette();
    dib.mapColorsToPalette(dibPalette->getPaletteHandle());

    // XXX -- Set the tranparency value on the dib here

#elif defined(MLE_REHEARSAL)

    int width, height;
    MlScalar min[2], max[2];

    // Free the old image if needed.

    if (imageData)
    {
        mlFree(imageData);
        imageData = NULL;
    }
    if (imageOp)
    {
        delete imageOp;
        imageOp = NULL;
    }
    if (inImg)
    {
        delete inImg;
        inImg = NULL;
    }

    // Set a default bounding box and return if no image media ref object
    // or it cannot load in media.

    if ((! imageMediaRef) || (! imageMediaRef->read(inImg)))
    {
        screenPosition.getValue(min[0], min[1]);
        max[0] = min[0] + ML_SCALAR(63);
        max[1] = min[1] + ML_SCALAR(63);
        setBounds(min, max);
        return;
    }

    // Obtain the image data.

    imageOp = new ilABGRImg(inImg);
    imageOp->setCoordSpace(ilLowerLeftOrigin);
    width = imageOp->getWidth();
    height = imageOp->getHeight();
    imageData = (unsigned char *) mlMalloc(width * height * 4);
    imageOp->getTile(0, 0, width, height, imageData);

    //
    // Update its bounding box.
    //
    min[0] = screenPosition[0];
    min[1] = screenPosition[1];
    max[0] = screenPosition[0] + mlLongToScalar(width) - ML_SCALAR_ONE;
    max[1] = screenPosition[1] + mlLongToScalar(height) - ML_SCALAR_ONE;
    setBounds(min, max);

    //
    // Delete the image media ref.
    // XXX might want to store in some media ref registry for latter use.

    delete imageMediaRef;
#endif /* _WINDOWS */
}


void
Mle2dImgRole::display(int state)
{
    displayState = state;
}


void
Mle2dImgRole::draw(void *)
{
#if defined(_WINDOWS)
    // Declare local variables.
    MlScalar xPos, yPos, min[2], max[2];
    MleDIB *destDib;
    int xs, ys, xd, yd;
    int srcWidth,srcHeight,destWidth,destHeight;

    if (displayState)
    {
        screenPosition.getValue(xPos,yPos);
        xd = mlScalarToLong(xPos);
        yd = mlScalarToLong(yPos);

        destDib = ((Mle2dSet *)forum)->getDib();
        srcWidth = dib.getWidth();
        srcHeight = dib.getHeight();
        destWidth = destDib->getWidth();
        destHeight = destDib->getHeight();

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
            int origHeight = srcHeight;
            srcHeight = destHeight - yd;
            ys = origHeight - srcHeight;
        }

        // On Windows, the origin is the upper, left-hand corner;
        // therefore, we have to compensate for the vertical offset.
        yd = destHeight - yd - srcHeight;

        // Copy pixel data into rendering buffer.
        dib.copyBits(destDib,xd,yd,srcWidth,srcHeight,xs,ys);

        // Set extent of delegate for dirty rectangle calculation.
        min[0] = mlLongToScalar(xd);
        min[1] = mlLongToScalar(yd);
        max[0] = mlLongToScalar(xd + srcWidth - 1);
        max[1] = mlLongToScalar(yd + srcHeight - 1);
        setBounds(min, max);
    }

#elif defined(MLE_REHEARSAL)

    int stageWidth, stageHeight, imageWidth, imageHeight;
    MlScalar x, y;

    // Don't draw if media is not loaded in.
    if (! inImg)
        return;

    if (displayState)
    {
        screenPosition.getValue(x, y);
        imageWidth = imageOp->getWidth();
        imageHeight = imageOp->getHeight();

#if defined(MLE_REHEARSAL)
        MleSGIBrStage::theStage->getSize(&stageWidth, &stageHeight);
#else
        stageWidth = 640;
        stageHeight = 480;
#endif

        glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT |
            GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT |
            GL_TEXTURE_BIT | GL_VIEWPORT_BIT | GL_TRANSFORM_BIT);

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_TEXTURE_1D);
        glDisable(GL_TEXTURE_2D);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0.0, stageWidth - 1.0, 0.0, stageHeight - 1.0, 0.0, 1.0);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        glRasterPos3i(mlScalarToLong(x), mlScalarToLong(y), 0);
        glDrawPixels(imageWidth, imageHeight, GL_ABGR_EXT, GL_UNSIGNED_BYTE,
            imageData);

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        glPopAttrib();
    }
#endif /* MLE_REHEARSAL */
}
