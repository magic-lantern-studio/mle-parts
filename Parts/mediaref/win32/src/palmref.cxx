/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file palmref.cxx
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
#endif /* WIN32 */

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
