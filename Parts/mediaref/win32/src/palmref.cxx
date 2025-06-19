/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file palmref.cxx
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
#endif /* _WINDOWS */

// Include Runtime Player header files
#include "mle/palmref.h"
#include "mle/mlMalloc.h"
#ifdef _GDK_
#include "mle/gdkpal.h"
#else
#include "mle/win32pal.h"
#endif /* _GDK_ */


MLE_MEDIAREF_SOURCE(MleWin32PaletteMediaRef,MleMediaRef);


MleWin32PaletteMediaRef::MleWin32PaletteMediaRef()
{
    // Do nothing extra
}


MleWin32PaletteMediaRef::~MleWin32PaletteMediaRef()
{
    // Do nothing
}


MlBoolean MleWin32PaletteMediaRef::read(MlePCPalette *pal)
{
    // Declare local variables
    HMMIO fp;
    MMCKINFO chunkInfoTop,chunkInfoData;
    PALETTEENTRY *paletteEntries;
    LOGPALETTE *paletteHdr;
#ifdef _GDK_
    MleGdkPalette *palette = (MleGdkPalette *)pal;
#else
    MleWin32Palette *palette = (MleWin32Palette *)pal;
#endif /* _GDK_ */
    MlBoolean status = FALSE;
    char *buffer = m_references->buffer;

    // open PAL file
    // Note: Microsoft distinguishes between text and binary mode;
    //       make sure the file is opened in binary mode so that
    //       translations are suppressed.
    if (buffer)
    {
        // Invoke the converter to prepare the local file
        m_converter->setReference(buffer);
        char *filename = m_converter->getFilename();

        // TRUE return means we downloaded successfully to buffer
        if (m_converter->getFile())
        {
            fp = mmioOpen(filename,NULL,MMIO_READ);
            if (fp)
            {
                status = TRUE;
            }
        }
    }
    if (FALSE == status)
    {
        return(FALSE);
    }

    // Load PAL file

    // Read RIFF PAL chunk
    chunkInfoTop.fccType = mmioFOURCC('P','A','L',' ');
    if (! mmioDescend(fp,&chunkInfoTop,NULL,MMIO_FINDRIFF))
    {
        // Read PAL data chunk
        chunkInfoData.ckid = mmioFOURCC('d','a','t','a');
        if (! mmioDescend(fp,&chunkInfoData,&chunkInfoTop,MMIO_FINDCHUNK))
        {
//        if (! mmioDescend(fp,&chunkInfoData,NULL,MMIO_FINDCHUNK)) {

            paletteHdr = (LOGPALETTE *) mlMalloc(chunkInfoData.cksize);
            paletteEntries = paletteHdr->palPalEntry;

            if (mmioRead(fp,(char *)paletteHdr,chunkInfoData.cksize))
            {
                // Set palette entries
                palette->setEntries(0,(unsigned char)paletteHdr->palNumEntries,
                                    paletteEntries);
                status = TRUE;
            }

            mmioAscend(fp,&chunkInfoData,0);
        }

        mmioAscend(fp,&chunkInfoTop,0);
    }

    // Close PAL file and return
    mmioClose(fp,0);
    return(status);
}

PALETTEENTRY *MleWin32PaletteMediaRef::load(void)
{
    // Declare local variables
    HMMIO fp;
    MMCKINFO chunkInfoTop,chunkInfoData;
    LOGPALETTE *paletteHdr;
    PALETTEENTRY *paletteEntries = NULL;
    char *buffer = m_references->buffer;


    // Open PAL file
    // Note: Microsoft distinguishes between text and binary mode;
    //       make sure the file is opened in binary mode so that
    //       translations are suppressed.
    if (buffer)
    {
        // Invoke the converter to prepare the local file
        m_converter->setReference(buffer);
        char *filename = m_converter->getFilename();

        // TRUE return means we downloaded successfully to buffer
        if (m_converter->getFile())
        {
            fp = mmioOpen(filename,NULL,MMIO_READ);
            if (!fp)
            {
                return NULL;
            }
        }
    }

    // Load PAL file

    // Read RIFF PAL chunk
    chunkInfoTop.fccType = mmioFOURCC('P','A','L',' ');
    if (! mmioDescend(fp,&chunkInfoTop,NULL,MMIO_FINDRIFF))
    {

        // Read PAL data chunk
        chunkInfoData.ckid = mmioFOURCC('d','a','t','a');
        if (! mmioDescend(fp,&chunkInfoData,&chunkInfoTop,MMIO_FINDCHUNK))
        {
//        if (! mmioDescend(fp,&chunkInfoData,NULL,MMIO_FINDCHUNK)) {

            paletteHdr = (LOGPALETTE *) mlMalloc(chunkInfoData.cksize);
            paletteEntries = paletteHdr->palPalEntry;

            if (mmioRead(fp,(char *)paletteHdr,chunkInfoData.cksize))
            {

                // Create a table of Windows palette entries
                paletteEntries = (PALETTEENTRY *) mlMalloc(sizeof(PALETTEENTRY)*paletteHdr->palNumEntries);
                if (paletteEntries)
                { 
                    // Copy the palette entries into this table for return
                    PALETTEENTRY *entryPtr = paletteHdr->palPalEntry;
                    for (int i = 0; i < paletteHdr->palNumEntries; i++)
                        paletteEntries[i] = *entryPtr++;
                }
            }
            mlFree(paletteHdr);

            mmioAscend(fp,&chunkInfoData,0);
        }

        mmioAscend(fp,&chunkInfoTop,0);
    }

    return(paletteEntries);
}
