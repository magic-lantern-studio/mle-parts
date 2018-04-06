/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file pcpal.h
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 * @date Jan 24, 2006
 *
 * This file defines an abstract class for a Windows color palette.
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

#ifndef __MLE_PCPAL_H_
#define __MLE_PCPAL_H_


#ifdef WIN32

// Include system header files.
#include <windows.h>

// Include Win32 target header files.
#include "mle/palette.h"


#define MLE_PC_PALETTE_SIZE    256

// Default palette colors - define the beginning of each 32-color ramp.
#define MLE_PCPAL_BASE_GRAY    10
#define MLE_PCPAL_BASE_BLUE    10 + 32
#define MLE_PCPAL_BASE_GREEN   10 + 64
#define MLE_PCPAL_BASE_YELLOW  10 + 96
#define MLE_PCPAL_BASE_RED     10 + 128
#define MLE_PCPAL_BASE_MAGENTA 10 + 160
#define MLE_PCPAL_BASE_ORANGE  10 + 192

/**
 *
 * @brief This is an abstract base class for a Windows color palette.
 *
 * This is an abstract base class for a Windows color palette.
 * It is derived from the abtract class MlePalette, which is the base
 * class for all Magic Lantern color palette classes.  The MlePCPalette 
 * class is the base class for the Windows-based PC color palette
 * classes MleWin32Palette (for Win32 GDI palettes) and MleGdkPalette
 * (for DirectDraw palettes).  It contains no palette data, and is 
 * necessarily abstract since the GDI and DirectDraw palette APIs 
 * are so different.
 * 
 * MlePCPalette provides the basic API for creating and modifying 
 * Windows color palettes, which can then be passed to the stage for 
 * realization in the hardware.  It also provides some utilities
 * for building the list of palette entries or a default palette.
 * 
 * The macro MLE_PC_PALETTE_SIZE is defined to be 256, the size of 
 * a PC's 8-bit color palette.
 *
 * @see MlePalette, MleWin32Palette, MleGdkPalette
 */
class MlePCPalette : public MlePalette
{
  public:

	// Create a PC palette.
	MlePCPalette(void) {}

	// Delete a PC palette.
	virtual ~MlePCPalette(void) {}

	// Get or set a selection of n entries from the palette, beginning
	// with the specified starting index.  Pure virtual functions.
	virtual MlBoolean getEntries(MlPaletteIndex start, MlPaletteIndex n,
				 PALETTEENTRY *palEntries) = 0;
	virtual MlBoolean setEntries(MlPaletteIndex start, MlPaletteIndex n,
				 PALETTEENTRY *palEntries) = 0;

	// rgbToPaletteEntry() allocates a PALETTEENTRY and sets it to
	// the specified RGB value.
	static PALETTEENTRY *rgbToPaletteEntry(BYTE r, BYTE g, BYTE b);

	// makeColor() interpolates a single palette entry for a specified level 
	// of ambient, diffuse, and specular colors.
	static void makeColor(PALETTEENTRY *a, PALETTEENTRY *d, PALETTEENTRY *s, 
	    float level, PALETTEENTRY *pe);

	// makeRange() interpolates a ramp of palette entries for the specified 
	// ambient, diffuse, and specular colors.  The argument npe specifies
    // the number of ramp values to be interpolated.
	static void makeRange(PALETTEENTRY *a, PALETTEENTRY *d, PALETTEENTRY *s,
	    PALETTEENTRY *pe, int npe);

	// clearSystemPalette() clears the system palette to all black in 
	// preparation for setting new colors.
	static void clearSystemPalette(void);

#if defined(MLE_DEBUG)
	static void dumpEntries(FILE *fp,PALETTEENTRY *palEntries);
#endif /* MLE_DEBUG */

  protected:

	// defaultPalette() creates a list of PALETTEENTRIES containing a 
	// default set of 32-color ramps starting at index 10.  A set of 
	// macros are defined specifying the base index for each ramp in 
	// the default palette. These macros are named FW_PCPAL_BASE_<color> 
	// where color is GRAY, BLUE, GREEN, YELLOW, RED, MAGENTA, 
	// or ORANGE.
	void defaultPalette(PALETTEENTRY *palEntries, BYTE defaultFlags, 
				 PALETTEENTRY *defaultColor);
};

#endif /* WIN32 */

#endif /* __MLE_PCPAL_H_ */
