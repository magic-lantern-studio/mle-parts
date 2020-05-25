/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file win32pal.h
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 * @date Jan 24, 2006
 *
 * This file defines a class for enscapsulating a Win32 GDI
 * logical color palette.
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

#ifndef __MLE_WIN32PAL_H_
#define __MLE_WIN32PAL_H_

#ifdef WIN32

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

#endif /* WIN32 */

#endif /* __MLE_WIN32PAL_H_ */
