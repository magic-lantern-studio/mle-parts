/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv2dImageRole.h
 * @ingroup MleParts
 *
 * This file defines the class for a 2D Image Role.
 *
 * @author Mark S. Millard
 * @date February 22, 2011
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Wizzer Works
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

#ifndef __MLE_2DIMGD_H_
#define __MLE_2DIMGD_H_

// Include system header files.
#if defined(MLE_REHEARSAL) || defined(__linux__)

// Include files for image processing library.
#include <FreeImage.h>

#elif defined(WIN32)

// Include files for image processing library.
#include "mle/dib.h"

#endif /* MLE_REHEARSAL */

// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "math/vector.h"
#include "mle/2drole.h"
#include "mle/Mle2dImageRole.h"

// Declare external classes.
class MleActor;


/**
 * This class defines a Role for displaying 2D images.
 */
class MLE_2DIMAGEROLE_API Mle2dImgRole : public Mle2dRole 
{

    MLE_ROLE_HEADER(Mle2dImgRole);

  public:

	Mle2dImgRole(MleActor *actor);

    virtual ~Mle2dImgRole(void);

    // Update the position where the image is drawn.  The position is
    // in screen coordinate and lower left is the image origin.
    void screenLocation(MlVector2 &pos);

    // This will read in a single pixelmap image.  It can also be used to
    // read in a different pixelmap image during runtime.
    void load(MlMediaRef img);

    void load(MleMediaRef *img);

    // The display of the image can be toggle on (with TRUE) or off
    // (with FALSE).
    void display(int state);

    // The render() function of the 2D Set calls this function to update
    // this role every cycle.
    virtual void draw(void *data);

  protected:

	// Image origin (lower left) in screen coordinate.
    MlVector2 screenPosition;

	// TRUE for on, FALSE for off
    int displayState;

#if defined(MLE_REHEARSAL) || defined(__linux__)

    unsigned char *imageData;

  private:

	// The FreeImage DIB object.
	FIBITMAP *inImg;
	FIBITMAP *imageOp;

#elif defined(WIN32)

	// The Microsoft DIB object.
    MleDIB dib;

#endif /* MLE_REHEARSAL */
};


#endif /* __MLE_2DIMGD_H_ */
