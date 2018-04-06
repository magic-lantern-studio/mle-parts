/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file paldbg.cxx
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 * @date Jan 24, 2006
 *
 * This file implements a class for debugging a color palette on the
 * Windows platform.
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
