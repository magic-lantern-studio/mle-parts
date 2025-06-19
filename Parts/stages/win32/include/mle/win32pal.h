/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file win32pal.h
 * @ingroup MlParts
 *
 * This file defines a class for enscapsulating a Win32 GDI
 * logical color palette.
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

#ifndef __MLE__WIN32PAL_H_
#define __MLE__WIN32PAL_H_

#ifdef _WINDOWS

// Include PC target header files.
#include "mle/pcpal.h"

enum MlWin32PalConfig
{
    mlWin32PalConfig_256Colors = 0,
    mlWin32PalConfig_254Colors = 1,
    mlWin32PalConfig_236Colors = 2
};

/**
 * 
 * This is a Magic Lantern class for a enscapsulating a Win32 GDI
 * logical color palette. 
 *
 * This is a Msgic Lantern class for a enscapsulating a Win32 GDI logical color 
 * palette.  MleWin32Palette provides the basic API for creating and 
 * modifying GDI color palettes, which can then be passed to the stage 
 * for realization in the hardware.  
 *
 * @see MlePalette, MlePCPalette, MleGdkPalette
 */
class MleWin32Palette : public MlePCPalette
{
  public:

    // Create a Win32 color palette.  The palEntries argument, if any,
	// provides a list of colors and palette flags for initializing
	// the logical palette.  If palEntries is NULL, the default
    // PC palette will be created.  
	//
	// The config argument, using the enumerated type MleWin32PalConfig,
    // defines three possible configurations for a GDI palette, based
	// upon the number of system "static" colors Windows will retain 
	// for itself.  The default, mlWin32PalConfig_236Colors, leaves
	// the system with its standard twenty static colors.
	//
	// The nEntries argument specifies the number of entries
	// in the palEntries list.
    MleWin32Palette(PALETTEENTRY *palEntries = NULL, 
	    MlWin32PalConfig config = mlWin32PalConfig_236Colors, 
	    unsigned int nEntries = 236);
    
    // Delete a Win32 color palette.  Releases resources allocated by the 
	// palette and destroys the palette object.
    virtual ~MleWin32Palette(void);
    
    // Set this palette in the hardware.
    virtual MlBoolean setSystemPalette(void);
    
    // Get or set a selection of n entries from the palette, beginning
    // with the specified starting index. 
    virtual MlBoolean getEntries(MlPaletteIndex start, MlPaletteIndex n, 
	    PALETTEENTRY *palEntries);

    virtual MlBoolean setEntries(MlPaletteIndex start, MlPaletteIndex n, 
	    PALETTEENTRY *palEntries);
    
	// Return the Windows handle to this palette.
    const HPALETTE getPaletteHandle(void) { return m_hPalette; }
    
#if defined(MLE_DEBUG)
    void dumpEntries(FILE *fp);
#endif /* MLE_DEBUG */

  private:

    // The Windows handle to the GDI color palette.
    HPALETTE m_hPalette;
};

#endif /* _WINDOWS */

#endif /* __MLE__WIN32PAL_H_ */
