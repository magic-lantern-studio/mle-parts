/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file brpalmrf.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a Palette Media Reference
 * targeting the BRender platform.
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


// Include system header files
#ifdef _WINDOWS

#include <windows.h>

#ifdef _GDK_
#include "mle/gdkpal.h"
#else
#include "mle/win32pal.h"
#endif

#endif /* _WINDOWS */

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
