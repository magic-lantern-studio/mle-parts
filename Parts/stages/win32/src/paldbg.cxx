/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file paldbg.cxx
 * @ingroup MlParts
 *
 * This file implements a class for debugging a color palette on the
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

// Include PC target header files.
#include "paldbg.h"


MlePalDbg::MlePalDbg(void)
{
    // Declare local variables.
    HDC globalDC;       // Device context.
    int rasterCaps;     // Raster capabilites.
    int screenX;
    int screenY;
    int borderX;

    // Retrieve the global device context.
    globalDC = GetDC(NULL);

    // Retrieve the raster capabilites.
    rasterCaps = GetDeviceCaps(globalDC,RASTERCAPS);
    rasterCaps = (rasterCaps & RC_PALETTE) ? TRUE : FALSE;

    // Determine the number of colors available on the device.
    if (rasterCaps)
        m_numColors = GetDeviceCaps(globalDC,SIZEPALETTE);
    else
        m_numColors = GetDeviceCaps(globalDC,NUMCOLORS);

    // Release the global device context.
    ReleaseDC(NULL,globalDC);

    // Determine the width of the window
    screenX = GetSystemMetrics(SM_CXSCREEN);
    screenY = GetSystemMetrics(SM_CYSCREEN);
    borderX = GetSystemMetrics(SM_XBORDER);

    sizeX = ((screenX - (2 * borderX)) / PALETTESIZE) * PALETTESIZE;

    // create a new Window
    window = CreateWindow(windowName, );
}
