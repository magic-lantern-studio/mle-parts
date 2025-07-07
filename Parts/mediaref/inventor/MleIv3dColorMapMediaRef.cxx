/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dColorMapMediaRef.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Colormap Media Reference
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

// Include system header files.
#ifdef _WINDOWS
#include <windows.h>
#endif /* _WINDOWS */
#include <mle/mlMalloc.h>

// Include Runtime Player header files.
#include "mle/mlTypes.h"
#include "mle/3dcomapm.h"


MLE_MEDIAREF_SOURCE(Mle3dColorMapMediaRef,MleMediaRef);


Mle3dColorMapMediaRef::Mle3dColorMapMediaRef()
{
    // Do nothing extra.
}


Mle3dColorMapMediaRef::~Mle3dColorMapMediaRef()
{
    // Do nothing.
}


void *Mle3dColorMapMediaRef::read()
{
    // Declare local variables.
    char *buffer = m_references->buffer;
    void* result = NULL;

    // Check flags to see if the reference is internal/external.

    // Read Color Map from external reference.
    if ( buffer ) 
    {
       // Invoke the converter to prepare the local file.
       m_converter->setReference(buffer);
       char *filename = m_converter->getFilename();

    } 

    // TRUE return means we downloaded successfully to buffer.
    if (m_converter->getFile()) 
    {
		//result = (void*) loadColorMap(filename);
    }
	    
    return result;
}

void*
Mle3dColorMapMediaRef::operator new(size_t tSize)
{
    void* p = mlMalloc(tSize);
    return p;
}

void*
Mle3dColorMapMediaRef::operator new[](size_t tSize)
{
    void* p = mlMalloc(tSize);
    return p;
}

void
Mle3dColorMapMediaRef::operator delete(void* p)
{
    mlFree(p);
}

void
Mle3dColorMapMediaRef::operator delete[](void* p)
{
    mlFree(p);
}
