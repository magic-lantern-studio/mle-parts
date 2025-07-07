/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dmodelm.h
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Model Media Reference
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

#ifndef __MLE_3DMODELM_H_
#define __MLE_3DMODELM_H_


// Include Magic Lantern Runtime Engine header files.
#include "mle/MleMediaRef.h"
#include "mle/Mle3dModelMediaRef.h"


/**
 * @brief 3D Model Media Reference.
 */
class MLE_3DMODELMEDIAREF_API Mle3dModelMediaRef : public MleMediaRef
{
    MLE_MEDIAREF_HEADER(Mle3dModelMediaRef);
    
  public:

    /**
     * @brief Default constructor.
     *
     * This constructor initializes the media reference.
	 */
    Mle3dModelMediaRef();

    /**
	 * The destructor.
     */
    virtual ~Mle3dModelMediaRef();

    /**
	 * Reads the 3D model media reference.
     */
    void *read();

    /**
     * Override operator new.
     *
     * @param tSize The size, in bytes, to allocate.
     */
    void* operator new(size_t tSize);

    /**
     * Override operator new array.
     *
     * @param tSize The size, in bytes, to allocate.
     */
    void* operator new[](size_t tSize);

    /**
     * Override operator delete.
     *
     * @param p A pointer to the memory to delete.
     */
    void operator delete(void* p);

    /**
     * Override operator delete array.
     *
     * @param p A pointer to the memory to delete.
     */
    void operator delete[](void* p);
};

#endif /* __MLE_3DMODELM_H_ */
