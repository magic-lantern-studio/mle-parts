/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv2dImageMediaRef.cxx
 * @ingroup MleParts
 *
 * This file implements the class for an Image Media Reference
 * targeting the Inventor platform.
 *
 * @author Mark S. Millard
 * @date February 22, 2011
 */

// COPYRIGHT_BEGIN
//
//  Copyright (C) 2000-2011  Wizzer Works
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
#if defined(WIN32)
#include <windows.h>
#endif /* WIN32 */

// Include runtime player header files
#include "mle/imagmref.h"
#include "mle/mlFileio.h"


MLE_MEDIAREF_SOURCE(MleImageMediaRef,MleMediaRef);

/**
 * FreeImage error handler.
 *
 * @param fif Format / Plugin responsible for the error.
 * @param message Error message.
*/
void
FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message)
{
	printf("\n*** "); 
	if (fif != FIF_UNKNOWN)
	{
		printf("%s Format\n", FreeImage_GetFormatFromFIF(fif));
	}
	printf(message);
	printf(" ***\n");
}

unsigned DLL_CALLCONV
fif_ReadProc(void *buffer, unsigned size, unsigned count, fi_handle handle)
{
	return fread(buffer, size, count, (FILE *)handle);
}

unsigned DLL_CALLCONV
fif_WriteProc(void *buffer, unsigned size, unsigned count, fi_handle handle)
{
	return fwrite(buffer, size, count, (FILE *)handle);
}

int DLL_CALLCONV
fif_SeekProc(fi_handle handle, long offset, int origin)
{
	return fseek((FILE *)handle, offset, origin);
}

long DLL_CALLCONV
fif_TellProc(fi_handle handle)
{
	return ftell((FILE *)handle);
}

MleImageMediaRef::MleImageMediaRef()
{

}


MleImageMediaRef::~MleImageMediaRef()
{
    // Do nothing
}

MlBoolean
MleImageMediaRef::read(FIBITMAP *&inImg)
{
    // Declare local variables
    char *filename;
    MlBoolean status = FALSE;
    char *buffer = m_references->buffer;

    if (buffer) 
	{
		// Invoke the converter to prepare the local file.
		m_converter->setReference(buffer);
		filename = m_converter->getFilename();

		// TRUE return means we downloaded successfully to buffer.
		if (m_converter->getFile())
		{
			inImg = NULL;

			// Open the image file and return the file image handle.
			FILE *file;

			// Open the file.
			file = fopen(filename, "rb");
			if (file != NULL)
			{
		        // Find the buffer format.
		        FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromHandle(&m_io, (fi_handle)file, 0);

		        if (fif != FIF_UNKNOWN)
				{
			        // Load from the file handle.
			        inImg = FreeImage_LoadFromHandle(fif, &m_io, (fi_handle)file, 0);
				}

		        // Done with the file handle, close it.
		       fclose(file);
			}

            // Set the return status.
			if (inImg)
			    status = TRUE;
		}
    }

    return(status);
}
