/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file paletted.h
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D palette property.
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


#ifndef __MLE_PALETTED_H_
#define __MLE_PALETTED_H_

// In order to bind the palette actor to the palette role, we need
// a set.  Presently we're using the 2d set, and so we have to have dummy
// getBounds, setBounds, and draw methods.
#include <limits.h>

// Include Magic Lantern header files.
#include "mle/2drole.h"
#include "mle/brpalmrf.h"
#ifdef _WINDOWS
    // XXX currently only implemented for the PC stage.
#include "mle/pcstage.h"
#endif


/**
 * @brief This is a role that loads and sets a Windows color palette from a 
 * mediaref.
 *
 * @see MleWin32PaletteMediaRef,MlePaletteActor
 */
class MlePaletteRole : public Mle2dRole
{
    MLE_ROLE_HEADER(MlePaletteRole);

  public:
    // Create a palette role.
    MlePaletteRole(MleActor *actor);

    // Delete a palette role.
    virtual ~MlePaletteRole();

#ifdef _WINDOWS
    // Load palette from mediaref.
    void loadPalette(MlMediaRef palmref);

    // Pointer to palette object.
    MlePaletteType *palette;
#else
    // Pointer to palette object (non-Windows). Not currently used.
    void *palette;
#endif

    // Dummy bounding box get routine (needed because MlePaletteRole
    // is derived from Mle2dRole. Returns a harmless bounding box.
    void        getBounds(MlScalar min[2], MlScalar max[2])
    {
        min[0] = ML_SCALAR(INT_MAX);
        min[1] = ML_SCALAR(INT_MAX);
        max[0] = ML_SCALAR(INT_MIN);
        max[1] = ML_SCALAR(INT_MIN);
    }

    // Dummy bounding box set routine (needed because MlePaletteRole
    // is derived from Mle2dRole. Does nothing.
    void        setBounds(MlScalar min[2], MlScalar max[2]) {}

    // Dummy draw routine (needed because MlePaletteRole
    // is derived from Mle2dRole. Does nothing.
    void        draw(void *data) {}

};

#endif /* __MLE_PALETTED_H_ */
