/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dtexmpm.h
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Texture Map Media Reference.
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

#ifndef __MLE_3DTEXMPM_H_
#define __MLE_3DTEXMPM_H_

// Include Runtime Engine header files.
#include "mle/MleMediaRef.h"
#include "mle/Mle3dTextureMapMediaRef.h"


/**
 * @brief A Media Reference loader for texture maps.
 */
class MLE_3DTEXTUREMAPMEDIAREF_API Mle3dTextureMapMediaRef : public MleMediaRef
{
    MLE_MEDIAREF_HEADER(Mle3dTextureMapMediaRef);
    
  public:

    /**
     * Default constructor.
     */
    Mle3dTextureMapMediaRef(void);

    /**
	 * The destructor.
     */
    virtual ~Mle3dTextureMapMediaRef(void);

    /**
     *  @brief Initialize the media reference.
     */
    virtual void init(void);

    /**
	 * Read texture map data from the media reference.
     */
    void *read(void);

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

#endif /* __MLE_3DTEXMPM_H_ */
