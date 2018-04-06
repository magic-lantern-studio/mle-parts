/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file win32pal.h
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 * @date Jan 24, 2006
 *
 * This file implements a class for enscapsulating a Win32 GDI
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

#if !defined(_GDK_)

// Include system header files.
#if defined(MLE_DEBUG)
#include <stdio.h>
#endif /* MLE_DEBUG */

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

// Include PC target header files.
#include "mle/win32pal.h"
#include "mle/pcstage.h"


// Create a Win32 color palette.
MleWin32Palette::MleWin32Palette(
    PALETTEENTRY *palEntries,
    MlWin32PalConfig config, 
    unsigned int nEntries)
{
    LOGPALETTE *logical_palette;

    //
    // Build the logical palette
    //
    logical_palette = (LOGPALETTE *)mlMalloc(sizeof(LOGPALETTE) + 
        (sizeof(PALETTEENTRY) * MLE_PC_PALETTE_SIZE));
    MLE_ASSERT(logical_palette);

    logical_palette->palVersion    = 0x300;
    logical_palette->palNumEntries = MLE_PC_PALETTE_SIZE;

    if (palEntries == NULL)
	{
        // Fill palette with default colors.
        defaultPalette(logical_palette->palPalEntry, PC_NOCOLLAPSE,
            MlePCPalette::rgbToPaletteEntry(255,0,255));
    }
    else
	{
        HDC screenDC;
        unsigned int start, end;

        // Fill the palette with system colours.
        screenDC = GetDC(NULL);
        GetSystemPaletteEntries(screenDC, 0, MLE_PC_PALETTE_SIZE,
            logical_palette->palPalEntry);

        // Check for valid number of entries.
        switch (config)
		{
			case mlWin32PalConfig_256Colors:
				// Leave system palette use as is.
				if (nEntries > MLE_PC_PALETTE_SIZE)
					nEntries = MLE_PC_PALETTE_SIZE;
				start = 0; end = MLE_PC_PALETTE_SIZE;
				break;
			case mlWin32PalConfig_254Colors:
				// Set system to have only 2 static colors.
				SetSystemPaletteUse(screenDC, SYSPAL_NOSTATIC);
				if (nEntries > MLE_PC_PALETTE_SIZE - 2)
					nEntries = MLE_PC_PALETTE_SIZE - 2;
				start = 1; end = MLE_PC_PALETTE_SIZE - 1;
				break;
			case mlWin32PalConfig_236Colors:
				// Set system to have 20 static colors.
				SetSystemPaletteUse(screenDC, SYSPAL_STATIC);
				if (nEntries > MLE_PC_PALETTE_SIZE - 20)
					nEntries = MLE_PC_PALETTE_SIZE - 20;
				start = 10; end = MLE_PC_PALETTE_SIZE - 10;
				break;
        };

        // Copy the requested colors and flags into the logical palette.
        // So far, we're assuming that the flags have been set correctly.
        for (unsigned int i = start; i < end; i++)
		{
            logical_palette->palPalEntry[i] = palEntries[i];
        }

        // Release screen device context.
        ReleaseDC(NULL,screenDC);
    }

    m_hPalette = CreatePalette(logical_palette);
    MLE_ASSERT(m_hPalette);

    mlFree(logical_palette);
}


// Delete a Win32 color palette.
MleWin32Palette::~MleWin32Palette()
{
    // The palette should have been removed from the palette manager's
    // registry prior to deletion.
    // XXX - note to me - have palmgr deletion return the pointer.
    MLE_ASSERT(~(m_flags & MLE_PALETTE_REGISTERED));

    // Test whether the object is successfully deleted.  For debugging.
    BOOL rval = DeleteObject(m_hPalette);
    MLE_ASSERT(rval == TRUE);
}


// Set this palette in the hardware.
MlBoolean MleWin32Palette::setSystemPalette(void)
{
    // declare local variables.
    BOOL result = FALSE;
    int i,sysColors,palEntries;
    HPALETTE palOld;

    // Get a screen DC to work with.
    HWND hwndActive = GetActiveWindow();
    HDC hdcScreen = GetDC(hwndActive);
    MLE_ASSERT(hdcScreen);

    // Make sure we are on a palettized device.
    if (! (GetDeviceCaps(hdcScreen,RASTERCAPS) &RC_PALETTE))
	{
#if defined(MLE_DEBUG)
        fprintf(stderr,"MleWin32Palette:: Not a palettized device.");
#endif /* MLE_DEBUG */
        goto abort;
    }

    // Get the number of system colors and the number of palette
    // entries. Note that on a palletized device the number of
    // colors is the number of guaranteed colors, i.e., the number
    // of reserved system colors.
    sysColors = GetDeviceCaps(hdcScreen,NUMCOLORS);
    palEntries = GetDeviceCaps(hdcScreen,SIZEPALETTE);

    // If there are more than 256 colors we are wasting our time.
    if (sysColors > 256) goto abort;

    // Now we force the palette manager to reset its tables so that
    // the next palette to be realized will get its colors in the order they
    // are in the logical palette. This is done by changing the number of
    // reserved colors.
    SetSystemPaletteUse(hdcScreen,SYSPAL_NOSTATIC);
    SetSystemPaletteUse(hdcScreen,SYSPAL_STATIC);

    // Select our palette into the screen DC and realize it so that
    // its colors will be entered into the free slots in the physical palette.
    palOld = SelectPalette(hdcScreen,
                           (HPALETTE) m_hPalette,
                           FALSE);
    RealizePalette(hdcScreen);

    // Now replace the old palette (but don't realize it).
    SelectPalette(hdcScreen,palOld,FALSE);

    // The physical palette now has our colors set in place and its own
    // reserved colors at either end. We can grab the lot now.
    PALETTEENTRY pe[256];
    GetSystemPaletteEntries(hdcScreen, 
                            0,
                            palEntries,
                            pe);

    // Set the PC_NOCOLLAPSE flag for each of our colors so that the GDI
    // won't merge them. Be careful not to set PC_NOCOLLAPSE for the 
    // system color entries so that we won't get multiple copies of these
    // colors in the palette when we realize it.
    for (i = 0; i < sysColors/2; i++)
	{
        pe[i].peFlags = 0;
    }
    for (; i < palEntries-sysColors/2; i++)
	{
        pe[i].peFlags = PC_NOCOLLAPSE;
    }
    for (; i < palEntries; i++)
	{
        pe[i].peFlags = 0;
    }

    // Resize the palette in case it was smaller.
    ResizePalette(m_hPalette,palEntries);

    // Update the palette entries with what is now in the physical palette.
    SetPaletteEntries(m_hPalette,0,palEntries,pe);
    result = TRUE;

abort:
    ReleaseDC(hwndActive,hdcScreen);
    return(result);

#if 0
    // Set the application palette.
    HDC screenDC;
    UINT numMapped;

    // Retrieve the screen device context.
    screenDC = GetDC(NULL);

    // Select and realize the palette in the screen dc.
    if (SelectPalette(screenDC, m_hPalette, FALSE) == NULL)
        return FALSE;
    numMapped = RealizePalette(screenDC);
    if (numMapped == GDI_ERROR)
        return FALSE;

    // Release the screen device context.
    ReleaseDC(NULL,screenDC);

    return TRUE;
#endif /* 0 */
}

// Get or set entries in color palette.
MlBoolean MleWin32Palette::getEntries(
    MlPaletteIndex start,
	MlPaletteIndex n, 
    PALETTEENTRY *palEntries)
{
    unsigned int nEntries;

    MLE_ASSERT(palEntries);
    nEntries = GetPaletteEntries(m_hPalette, start, n, palEntries);
    if (! nEntries)
	{
        return FALSE;
    }

    return TRUE;
}

MlBoolean MleWin32Palette::setEntries(
	MlPaletteIndex start,
	MlPaletteIndex n, 
    PALETTEENTRY *palEntries)
{
    unsigned int nEntries;
    MlBoolean status = TRUE;

    MLE_ASSERT(palEntries);
    nEntries = SetPaletteEntries(m_hPalette, start, n, palEntries);
    if (! nEntries)
	{
        status = FALSE;
    }

    return status;
}


#if defined(MLE_DEBUG)
void MleWin32Palette::dumpEntries(FILE *fp)
{
    // Declare local variables.
    PALETTEENTRY palEntries[MLE_PC_PALETTE_SIZE];

    fprintf(fp,"Dumping MleWin32Palette entries.\n");

    getEntries(0,MLE_PC_PALETTE_SIZE,palEntries);
    MlePCPalette::dumpEntries(fp,palEntries);
}
#endif /* MLE_DEBUG */

#endif /* !_GDK_ */
