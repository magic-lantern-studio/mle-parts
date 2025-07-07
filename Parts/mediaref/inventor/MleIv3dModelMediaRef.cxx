/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dModelMediaRef.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Model Media Reference
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

// Include Inventor header files.
#include "Inventor/SoDB.h"
#include "Inventor/SoInput.h"
#include "Inventor/nodes/SoSeparator.h"

#include "mle/3dmodelm.h"


MLE_MEDIAREF_SOURCE(Mle3dModelMediaRef,MleMediaRef);


Mle3dModelMediaRef::Mle3dModelMediaRef()
{
    // Do nothing extra.
}


Mle3dModelMediaRef::~Mle3dModelMediaRef()
{
    // Do nothing.
}


void* Mle3dModelMediaRef::read()
{
    // Declare local variables.
    char *buffer = m_references->buffer;

    // Check flags to see if the reference is internal/external.

    // Read Inventor Model from external reference.

    if (!buffer) return NULL;

    SoInput in;
    if (!in.openFile((char*) buffer))
    {
		printf("Mle3dModelMediaRef(iv): can't read file %s\n", buffer);
		return NULL;
    }
    
    SoSeparator *myGraph = SoDB::readAll(&in);
    if (myGraph == NULL)
    {
		printf("Mle3dModelMediaRef(iv): problem reading file %s\n", buffer);
		return NULL;
    }

    in.closeFile();
    return myGraph;
}

void*
Mle3dModelMediaRef::operator new(size_t tSize)
{
    void* p = mlMalloc(tSize);
    return p;
}

void*
Mle3dModelMediaRef::operator new[](size_t tSize)
{
    void* p = mlMalloc(tSize);
    return p;
}

void
Mle3dModelMediaRef::operator delete(void* p)
{
    mlFree(p);
}

void
Mle3dModelMediaRef::operator delete[](void* p)
{
    mlFree(p);
}
