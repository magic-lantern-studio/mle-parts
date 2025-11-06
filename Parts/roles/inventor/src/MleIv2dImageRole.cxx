/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv2dImageRole.cxx
 * @ingroup MleParts
 *
 * This file implements the 2D Image Role targeting the Inventor platform.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2017-2025 Wizzer Works
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

#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif /* _WINDOWS */

// Include system header files.
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

// Include Magic Lantern header files.
#include "mle/MleLoad.h"
#include "mle/mlMalloc.h"
#include "mle/imagmref.h"
#include "mle/ivstage.h"
#include "mle/2dimga.h"
#include "mle/2dimgd.h"


MLE_ROLE_SOURCE(Mle2dImgRole, Mle2dRole);


Mle2dImgRole::Mle2dImgRole(MleActor *actor)
 : Mle2dRole(actor)
{
    screenPosition.setValue(ML_SCALAR(0), ML_SCALAR(0));
    displayState = FALSE;
    imageData = NULL;
    imageOp = NULL;
    inImg = NULL;
}


Mle2dImgRole::~Mle2dImgRole(void)
{
    if (imageData)
		mlFree(imageData);
    if (imageOp)
		FreeImage_Unload(imageOp);
    if (inImg)
		FreeImage_Unload(inImg);
}


void
Mle2dImgRole::screenLocation(MlVector2 &pos)
{
    screenPosition.setValue(pos.getValue());

    if (imageOp)
	{
		// Update its bounding box.

		int imageWidth, imageHeight;
		MlScalar min[2], max[2];

		imageWidth = FreeImage_GetWidth(imageOp);
		imageHeight = FreeImage_GetHeight(imageOp);
		min[0] = pos[0];
		min[1] = pos[1];
		max[0] = pos[0] + mlLongToScalar(imageWidth) - ML_SCALAR_ONE;
		max[1] = pos[1] + mlLongToScalar(imageHeight) - ML_SCALAR_ONE;
		setBounds(min, max);
    }
}


void
Mle2dImgRole::load(MlMediaRef img)
{
    MleImageMediaRef *imageMediaRef;
    int width, height;
    MlScalar min[2], max[2];

    imageMediaRef = (MleImageMediaRef *) mlLoadMediaRef(img, NULL);
    imageMediaRef->read(inImg);
    MLE_ASSERT(inImg);

    // Obtain the image data.
	imageOp = FreeImage_ConvertTo32Bits(inImg);
    width = FreeImage_GetWidth(imageOp);
    height = FreeImage_GetHeight(imageOp);
    imageData = (unsigned char *) mlMalloc(width * height * 4);
    char *pixels = (char *) FreeImage_GetBits(imageOp);
	for (int j = 0; j < width * height; j++)
	{
		imageData[j*4+0]= pixels[j*4+0];
		imageData[j*4+1]= pixels[j*4+1];
		imageData[j*4+2]= pixels[j*4+2];
		imageData[j*4+3]= pixels[j*4+3];
	}

    // Update its bounding box.
    min[0] = screenPosition[0];
    min[1] = screenPosition[1];
    max[0] = screenPosition[0] + mlLongToScalar(width) - ML_SCALAR_ONE;
    max[1] = screenPosition[1] + mlLongToScalar(height) - ML_SCALAR_ONE;
    setBounds(min, max);
}

void
Mle2dImgRole::load(MleMediaRef *img)
{
	// Todo: load image media reference from provided reference.
}

void
Mle2dImgRole::display(int state)
{
    displayState = state;
}


void
Mle2dImgRole::draw(void *)
{
    int stageWidth, stageHeight, imageWidth, imageHeight;
    MlScalar x, y;

    if (displayState)
	{
		screenPosition.getValue(x, y);
		imageWidth = FreeImage_GetWidth(imageOp);
		imageHeight = FreeImage_GetHeight(imageOp);

#if defined(MLE_REHEARSAL)
		MleIvStage::g_theStage->getSize(&stageWidth, &stageHeight);
#elif defined(__linux__) || defined(_WINDOWS)
		// Todo: need to fix this for non-rehearsal stage.
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
		glDrawPixels(imageWidth, imageHeight, GL_BGRA_EXT, GL_UNSIGNED_BYTE,
			imageData);

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glPopAttrib();
    }
}

void*
Mle2dImgRole::operator new(size_t tSize)
{
	void* p = mlMalloc(tSize);
	return p;
}

void*
Mle2dImgRole::operator new[](size_t tSize)
{
	void* p = mlMalloc(tSize);
	return p;
}

void
Mle2dImgRole::operator delete(void* p)
{
	mlFree(p);
}

void
Mle2dImgRole::operator delete[](void* p)
{
	mlFree(p);
}
