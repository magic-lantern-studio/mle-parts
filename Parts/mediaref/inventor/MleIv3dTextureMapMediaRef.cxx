/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dTextureMapMediaRef.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Texture Map Media Reference
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
#ifdef psx
#include <stdio.h>
#endif
#include <mle/mlMalloc.h>

// Include Runtime Player header files
#include "Inventor/nodes/SoTexture2.h"
#include "mle/3dtexmpm.h"


MLE_MEDIAREF_SOURCE(Mle3dTextureMapMediaRef,MleMediaRef);


Mle3dTextureMapMediaRef::Mle3dTextureMapMediaRef()
{
    // Do nothing extra.
}

Mle3dTextureMapMediaRef::~Mle3dTextureMapMediaRef()
{
    // Do nothing.
}

void Mle3dTextureMapMediaRef::init(void)
{
    // Nothing to initialize.
}


void* Mle3dTextureMapMediaRef::read()
{
    // Declare local variables.
    char *buffer = m_references->buffer;
    void* result = NULL;

    // Check flags to see if the reference is internal/external.

    if ( buffer ) 
    {
	    // Invoke the converter to prepare the local file.
	    m_converter->setReference(buffer);
	    char *filename = m_converter->getFilename();

        if (m_converter->getFile()) 
        {
            // Do Nothing.
	    }
    }
    return result;
}

void*
Mle3dTextureMapMediaRef::operator new(size_t tSize)
{
    void* p = mlMalloc(tSize);
    return p;
}

void*
Mle3dTextureMapMediaRef::operator new[](size_t tSize)
{
    void* p = mlMalloc(tSize);
    return p;
}

void
Mle3dTextureMapMediaRef::operator delete(void* p)
{
    mlFree(p);
}

void
Mle3dTextureMapMediaRef::operator delete[](void* p)
{
    mlFree(p);
}
