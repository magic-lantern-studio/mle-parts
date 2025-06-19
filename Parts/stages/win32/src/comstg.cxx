/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file comstg.cxx
 * @ingroup MlParts
 *
 * This file implements common class utilities for Stages targeting the
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

// Include system header files.
#include <limits.h>
#include <stdio.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleScheduler.h"
#include "mle/MleDirector.h"

// Include PC target header files.
#include "mle/comstg.h"


//
// Implementation of Blit helper class
//

#if 0
/* 8 bit palettized colors */
#define DIB_USAGE DIB_PAL_COLORS
#else
/* 24 bit DIB colors */
#define DIB_USAGE DIB_RGB_COLORS
#endif


// Define type for CreateDIBSection function.
typedef HBITMAP (*_CreateDIBSection)
    (HDC,CONST BITMAPINFO *,UINT,VOID **,HANDLE,DWORD);


// Initialize static member variables.
MlBoolean MleDIBSectionBlit::g_initState = FALSE;
FARPROC MleDIBSectionBlit::g_CreateDIBSection_P = NULL;


MleDIBSectionBlit::MleDIBSectionBlit()
{
    // Declare local variables.
    HMODULE hgdi;

    // Try to get the CreateDIBSection process address.
    if (g_initState == FALSE)
	{
        g_CreateDIBSection_P = NULL;
        hgdi = GetModuleHandle("GDI32");
        g_initState = TRUE;

        if (hgdi != NULL)
		{
            g_CreateDIBSection_P = GetProcAddress(hgdi, "CreateDIBSection");
        }
    }

	// Initialize member variables.
    m_bitmap = NULL;
    m_width = 0;
    m_height = 0;
}

  
MleDIBSectionBlit::~MleDIBSectionBlit()
{
    deallocate();
}


//
// Macro to convert a number of bits to longword aligned bytes.
//
#define WIDTHBYTES(bits) ((((bits) + 31) & ~31) >> 3)


// This method returns TRUE if bitmap was actually allocated.
MlBoolean MleDIBSectionBlit::allocate(
    HDC screen, HPALETTE hPalette, int w, int h)
{
    struct {
        BITMAPINFOHEADER header;
        RGBQUAD colors[MLE_PALETTE_SIZE];
    } info;
    int i;

    if (g_CreateDIBSection_P == NULL) return FALSE;

#if DIB_USAGE == DIB_RGB_COLORS
    PALETTEENTRY palentries[MLE_PALETTE_SIZE];
#else
    short *index;
#endif
  
    //
    // Create the bitmap info and palette.
    //
    memset(&info,0,sizeof(info));
    info.header.biSize = sizeof(BITMAPINFOHEADER);
    info.header.biWidth = w;
    info.header.biHeight = h;
    info.header.biPlanes = 1;
    info.header.biBitCount = MLE_BIT_COUNT;
    info.header.biCompression = BI_RGB;
    info.header.biSizeImage = WIDTHBYTES(w * MLE_BIT_COUNT) * h;
    info.header.biClrUsed = 256;

#if DIB_USAGE == DIB_RGB_COLORS
    GetPaletteEntries(hPalette,0,MLE_PALETTE_SIZE,palentries);

    for(i = 0; i < MLE_PALETTE_SIZE; i++)
	{
        info.colors[i].rgbRed = palentries[i].peRed;
        info.colors[i].rgbGreen = palentries[i].peGreen;
        info.colors[i].rgbBlue = palentries[i].peBlue;
    }
#else
    index = (short *)info.colors;

    for(i = 0; i < MLE_PALETTE_SIZE; i++)
        index[i] = i;
#endif

    // Create the DIB Section.
    m_bitmap = (HBITMAP)((_CreateDIBSection)CreateDIBSection)(
	screen, (LPBITMAPINFO)&info, DIB_USAGE, &m_bits,  0, 0);
    if (m_bitmap == NULL)
        return FALSE;

    m_width = w;
    m_height = h;

    return TRUE;
}


void MleDIBSectionBlit::deallocate()
{
    if (m_bitmap != NULL)
        DeleteObject(m_bitmap);
    m_bitmap = NULL;
    m_bits = NULL;
    m_width = 0;
    m_height = 0;
}


void MleDIBSectionBlit::blit(HDC hdcWindow, int dest_x, int dest_y,
			    int src_x, int src_y,int width, int height)
{
    // declare local variables.
    HDC dc;
    HBITMAP oldBitmap;

    dc = CreateCompatibleDC(hdcWindow);
    oldBitmap = (HBITMAP)SelectObject(dc, m_bitmap);

    BitBlt(hdcWindow,dest_x,dest_y,width,height,dc,src_x,src_y,SRCCOPY);

    SelectObject(dc,oldBitmap);
    DeleteDC(dc);

    // flush cached GDI calls.
    GdiFlush();
}


#ifndef _GDK_
void
MleDIBSectionBlit::mapColorPalette(HDC hdcWindow,HPALETTE palette)
{
    // Patch DIB section color table..
    PALETTEENTRY peColors[MLE_PALETTE_SIZE];
    RGBQUAD rgbColors[MLE_PALETTE_SIZE];

#if 0
    short *index;
    index = (short *)rgbColors;
    for (int i = 0; i < MLE_PALETTE_SIZE; i++)
        index[i] = i;
#endif
    GetPaletteEntries(palette,0,MLE_PALETTE_SIZE,peColors);
    for (int i = 0; i < MLE_PALETTE_SIZE; i++)
	{
        rgbColors[i].rgbRed = peColors[i].peRed;
        rgbColors[i].rgbGreen = peColors[i].peGreen;
        rgbColors[i].rgbBlue = peColors[i].peBlue;
        rgbColors[i].rgbReserved = (BYTE)0;
    }

    HDC dc;
    HBITMAP oldBitmap;
    UINT mapped;
    dc = CreateCompatibleDC(hdcWindow);
    oldBitmap = (HBITMAP)SelectObject(dc,m_bitmap);
    mapped = SetDIBColorTable(dc,0,MLE_PALETTE_SIZE,rgbColors);
    SelectObject(dc,oldBitmap);
    DeleteDC(dc);
}
#endif /*! _GDK_ */
