/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv2dImageMediaRef.cxx
 * @ingroup MleParts
 *
 * This file implements the class for an Image Media Reference
 * targeting the Inventor platform.
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
#if defined(_WINDOWS)
#include <windows.h>
#endif /* _WINDOWS */

// Include runtime player header files
#include "mle/imagmref.h"
#include "mle/mlFileio.h"
#include "mle/mlMalloc.h"


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
	printf("%s",message);
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

void*
MleImageMediaRef::operator new(size_t tSize)
{
	void* p = mlMalloc(tSize);
	return p;
}

void*
MleImageMediaRef::operator new[](size_t tSize)
{
	void* p = mlMalloc(tSize);
	return p;
}

void
MleImageMediaRef::operator delete(void* p)
{
	mlFree(p);
}

void
MleImageMediaRef::operator delete[](void* p)
{
	mlFree(p);
}
