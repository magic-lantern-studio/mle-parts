/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file brpalmrf.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a Palette Media Reference
 * targeting the BRender platform.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
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


// Include system header files
#ifdef WIN32

#include <windows.h>

#ifdef _GDK_
#include "mle/gdkpal.h"
#else
#include "mle/win32pal.h"
#endif

#endif /* WIN32 */

// Include Runtime Player header files
#include "mle/mlMalloc.h"
#include "mle/brpalmrf.h"


MLE_MEDIAREF_SOURCE(MleBrPaletteMediaRef,MleMediaRef);


MleBrPaletteMediaRef::MleBrPaletteMediaRef()
{
    // Do nothing extra
}


MleBrPaletteMediaRef::~MleBrPaletteMediaRef()
{
    // do nothing
}


br_pixelmap *MleBrPaletteMediaRef::readBrPalette()
{
    // declare local variables
    br_pixelmap *colorPixelmap = NULL;
    char *buffer = m_references->buffer;

    // check flags to see if the reference is internal/external

    // read BRender Palette Pixelmap from external reference
    if ( buffer )
	{
	    // Invoke the converter to prepare the local file
	    m_converter->setReference(buffer);
	    char *filename = m_converter->getFilename();

		// load the palette
		colorPixelmap =  BrPixelmapLoad(filename);

		// although a palette is a pixelmap, it doesn't get added
		// to the map registry because that is for textures.
    }

    return colorPixelmap;
}


MlBoolean MleBrPaletteMediaRef::read(MlePCPalette *pal)
{
	// Declare local variables
	PALETTEENTRY *paletteEntries;
	LOGPALETTE *paletteHdr;
#ifdef _GDK_
	MleGdkPalette *palette = (MleGdkPalette *)pal;
#else
	MleWin32Palette *palette = (MleWin32Palette *)pal;
#endif /* _GDK_ */
	MlBoolean status = FALSE;

	// Read the br_pixelmap containg the palette information
	br_pixelmap *pm = readBrPalette();
	if (pm)
	{
		// Create a win32 logical palette
		paletteHdr = (LOGPALETTE *) mlMalloc(sizeof(LOGPALETTE));
		paletteHdr->palVersion = 0x300;
		paletteHdr->palNumEntries = pm->height;
		paletteEntries = paletteHdr->palPalEntry;

		// Copy the colors into the logical palette
		br_uint_32 *pixPtr = (br_uint_32 *)pm->pixels;
		br_uint_16 i;
		for (i = 0; i < pm->height; i++)
		{
			paletteEntries[i].peRed = (BYTE)BR_RED(*pixPtr);
			paletteEntries[i].peGreen = (BYTE)BR_GRN(*pixPtr);
			paletteEntries[i].peBlue = (BYTE)BR_BLU(*pixPtr);
			paletteEntries[i].peFlags = 0;

			pixPtr++;
		}

		// Set palette entries
		status = palette->setEntries(0,paletteHdr->palNumEntries,paletteEntries);

		BrPixelmapFree(pm);
	}

	return(status);
}

PALETTEENTRY *MleBrPaletteMediaRef::read(void)
{
	PALETTEENTRY *paletteEntries = NULL;

	// Read the br_pixelmap containing the palette information
	br_pixelmap *pm = readBrPalette();
	if (pm)
	{
		// Create a table of Windows palette entries
		paletteEntries = (PALETTEENTRY *) mlMalloc(sizeof(PALETTEENTRY)*pm->height);
		if (paletteEntries)
		{
			// Copy the colors into the logical palette
			br_uint_32 *pixPtr = (br_uint_32 *)pm->pixels;
			br_uint_16 i;
			for (i = 0; i < pm->height; i++)
			{
				paletteEntries[i].peRed = (BYTE)BR_RED(*pixPtr);
				paletteEntries[i].peGreen = (BYTE)BR_GRN(*pixPtr);
				paletteEntries[i].peBlue = (BYTE)BR_BLU(*pixPtr);
				paletteEntries[i].peFlags = 0;
				pixPtr++;
			}
		}
		// XXX else handle error

		BrPixelmapFree(pm);
	}
	// XXX else handle error

	return(paletteEntries);
}
