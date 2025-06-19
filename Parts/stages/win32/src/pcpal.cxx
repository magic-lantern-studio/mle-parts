/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file pcpal.h
 * @ingroup MlParts
 *
 * This file implements an abstract class for a Windows color palette.
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

// Include Magic Lantern headers.
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"

// Include PC target header files.
#include "mle/pcpal.h"


// Pack RGB values into a palette entry.
PALETTEENTRY *MlePCPalette::rgbToPaletteEntry(BYTE r, BYTE g, BYTE b)
{
    PALETTEENTRY    *pe = (PALETTEENTRY *)mlMalloc(sizeof(PALETTEENTRY));
    
    MLE_ASSERT(pe != NULL);

    pe->peRed = r;
    pe->peGreen = g;
    pe->peBlue = b;

    return pe;
}


// MlePCPalette class - implementation of PC color palette.

void MlePCPalette::defaultPalette(
    PALETTEENTRY *palEntries,
    BYTE defaultFlags, 
    PALETTEENTRY *defaultColor)
{
    HDC screen_dc;
    PALETTEENTRY peEnt[MLE_PC_PALETTE_SIZE];
    PALETTEENTRY *pe;
    int i;

    //
    // Fill the palette with system colours.
    //
    screen_dc = GetDC(NULL);
    GetSystemPaletteEntries(screen_dc, 0, MLE_PC_PALETTE_SIZE, peEnt);
    ReleaseDC(NULL, screen_dc);
 
    // Set the flags.
    for(i=0; i < 10; i++)
        peEnt[i].peFlags = 0;

    for (i=MLE_PC_PALETTE_SIZE-1; i > MLE_PC_PALETTE_SIZE-11; i--)
        peEnt[i].peFlags = 0;

    for( i=10; i < MLE_PC_PALETTE_SIZE-10; i++)
        peEnt[i].peFlags = defaultFlags;
 
    //
    // Clear all the non-system colours.
    //
    for (i=10; i < MLE_PC_PALETTE_SIZE-10; i++)
	{
        peEnt[i].peRed = defaultColor->peRed;
        peEnt[i].peGreen = defaultColor->peGreen;
        peEnt[i].peBlue = defaultColor->peBlue;
    }

    //
    // Fill in color ramps.
    //
    pe = &peEnt[10];

    makeRange(rgbToPaletteEntry(0,0,0),rgbToPaletteEntry(147,147,147),
              rgbToPaletteEntry(255,255,255), pe+  0, 32);
    makeRange(rgbToPaletteEntry(0,0,0),rgbToPaletteEntry(60,60,238),
              rgbToPaletteEntry(255,255,255), pe+ 32, 32);
    makeRange(rgbToPaletteEntry(0,0,0),rgbToPaletteEntry(60,238,60),
              rgbToPaletteEntry(255,255,255), pe+ 64, 32);
    makeRange(rgbToPaletteEntry(0,0,0),rgbToPaletteEntry(60,238,238),
              rgbToPaletteEntry(255,255,255), pe+ 96, 32);
    makeRange(rgbToPaletteEntry(0,0,0),rgbToPaletteEntry(238,60,60),
              rgbToPaletteEntry(255,255,255), pe+128, 32);
    makeRange(rgbToPaletteEntry(0,0,0),rgbToPaletteEntry(238,60,238),
              rgbToPaletteEntry(255,255,255), pe+160, 32);
    makeRange(rgbToPaletteEntry(0,0,0),rgbToPaletteEntry(238,238,60),
               rgbToPaletteEntry(255,255,255), pe+192, 32);

    // Write values back to our caller.
    pe = palEntries;
    for (i=0; i < MLE_PC_PALETTE_SIZE; i++)
	{
        *pe++ = peEnt[i];
    }
}


void MlePCPalette::clearSystemPalette()
{
    // Use a dummy palette setup.
    struct
    {
        WORD Version;
        WORD NumberOfEntries;
        PALETTEENTRY entries[256];
    } palette =
    {
        0x300,
        256
    };

    HPALETTE screenPalette = 0;
    HDC      screenDC;
    int      counter;
    UINT     mapped = 0;
    BOOL     bOK = FALSE;
    int      nOK = 0;
  
    // Reset everything in the system palette to black.
    for (counter = 0; counter < 256; counter++)
    {
        palette.entries[counter].peRed = 0;
        palette.entries[counter].peGreen = 0;
        palette.entries[counter].peBlue = 0;
        palette.entries[counter].peFlags = PC_NOCOLLAPSE;
    }

    // Create, select, realize, deselect, and delete the palette.
    screenDC = GetDC(NULL);
    screenPalette = CreatePalette((LOGPALETTE *)&palette);

    if (screenPalette)
    {
        screenPalette = SelectPalette(screenDC,screenPalette,FALSE);
        mapped = RealizePalette(screenDC);
        screenPalette = SelectPalette(screenDC,screenPalette,FALSE);
        bOK = DeleteObject(screenPalette);
    }

    nOK = ReleaseDC(NULL, screenDC);
}


//
// makeColor()
//
// Generate a shaded colour at 'level' along a ramp from
//
// 0                    0.75       1.0
// ambient -----------> diffuse -> specular
//
void MlePCPalette::makeColor(
    PALETTEENTRY *a,
    PALETTEENTRY *d,
    PALETTEENTRY *s,
    float level, 
    PALETTEENTRY *pe)
{
    float    invLevel;

    if (level < 0.75f)
	{
        level /= 0.75f;
        invLevel = 1.0f - level;

        pe->peRed = (BYTE)(invLevel * a->peRed + level * d->peRed);
        pe->peGreen = (BYTE)(invLevel * a->peGreen + level * d->peGreen);
        pe->peBlue = (BYTE)(invLevel * a->peBlue + level * d->peBlue);
    } else
	{
        level = (level-0.75f)/0.25f;
        invLevel = 1.0f - level;

        pe->peRed = (BYTE)(invLevel * d->peRed + level * s->peRed);
        pe->peGreen = (BYTE)(invLevel * d->peGreen + level * s->peGreen);
        pe->peBlue = (BYTE)(invLevel * d->peBlue + level * s->peBlue);
    }
}


//
// makeRange()
//
// Fill a given array of PALETTEENTRYs with a ramp of colours using makeColor().
//
void MlePCPalette::makeRange(
    PALETTEENTRY *a,
    PALETTEENTRY *d,
    PALETTEENTRY *s,
    PALETTEENTRY *pe,
    int npe)
{
    int i;

    for(i=0; i < npe; i++)
	{
        makeColor(a,d,s, i/ (float)npe, &pe[i]);
    }
}


#if defined(MLE_DEBUG)
void MlePCPalette::dumpEntries(FILE *fp,PALETTEENTRY *palEntries)
{
    //char buf[BUFSIZ];

    for (int i = 0; i < MLE_PC_PALETTE_SIZE; i++)
	{
        fprintf(fp,"Palette Entry %d\n",i);
        fprintf(fp,"\tRed Value: %d\n",(short)palEntries[i].peRed);
        fprintf(fp,"\tGreen Value: %d\n",(short)palEntries[i].peGreen);
        fprintf(fp,"\tBlue Value: %d\n",(short)palEntries[i].peBlue);
        switch ((int)palEntries[i].peFlags)
		{
            case NULL:
                fprintf(fp,"\tFlag Value: %s\n","NULL");
                break;
            case PC_EXPLICIT:
                fprintf(fp,"\tFlag Value: %d %s\n",
                        (short)palEntries[i].peFlags,"PC_EXPLICIT");
                break;
            case PC_NOCOLLAPSE:
                fprintf(fp,"\tFlag Value: %d %s\n",
                        (short)palEntries[i].peFlags,"PC_NOCOLLAPSE");
                break;
            case PC_RESERVED:
                fprintf(fp,"\tFlag Value: %d %s\n",
                        (short)palEntries[i].peFlags,"PC_RESERVED");
                break;
            default:
                fprintf(fp,"\tFlag Value: %d %s\n",
                        (short)palEntries[i].peFlags,"UNKNWOWN");
        }
    }
}
#endif /* MLE_DEBUG */
