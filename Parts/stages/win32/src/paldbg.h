/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file paldbg.h
 * @ingroup MlParts
 *
 * This file defines a class for debugging a color palette on the
 * Windows platform.
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

#ifndef __MLE_PALDBG_H_
#define __MLE_PALDBG_H_


// Include system header files.
#include <windows.h>

// Include Magic Lantern header files.
#include "mle/mlTypes.h"


class MlePalDbg
{
  // Declare member variables.
  private:

    HWND m_window;        // Handle to window.

    int m_sizeX;          // Height of palette window.
    int m_sizeY;          // Width of palette window.

    int m_numColors;      // Number of device colors.

  // Declare member functions.
  public:

    MlePalDbg(void);

    virtual ~MlePalDbg(void);

    //
    // This method is used to update the display of the palette.
    //
    MlBoolean draw(void);

};


#endif /* __MLE_PALDBG_H_ */
