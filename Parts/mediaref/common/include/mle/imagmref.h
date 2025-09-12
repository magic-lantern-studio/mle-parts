/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file imagmref.h
 * @ingroup MleParts
 *
 * This file defines the class for an Image Media Reference.
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


#ifndef __MLE_IMAGMREF_H_
#define __MLE_IMAGMREF_H_

// Include runtime player header files.
#include "mle/mlTypes.h"
#include "mle/MleMediaRef.h"
#include "mle/Mle2dImageMediaRef.h"

// Use FreeImage library.
#include <FreeImage.h>


/**
 * @brief This class implements a Media Reference loader for a still image.
 */
class MLE_2DIMAGEMEDIAREF_API MleImageMediaRef : public MleMediaRef
{
    MLE_MEDIAREF_HEADER(MleImageMediaRef);

  private:

    /** The IO handler for FreeImage. */
    FreeImageIO m_io;

  public:

    /**
	 * Default constructor.
     * 
     * This constructor initializes the media reference.
     */
    MleImageMediaRef(void);

    /**
	 * The destructor.
     */
    virtual ~MleImageMediaRef(void);

    /*
     * Read a FreeImage bitmap from a file.
     *
     * @param inImg A reference to a pointer to the FreeImage bitmap.
     * @return TRUE if successful, FALSE otherwise.
	 */
    MlBoolean read(FIBITMAP *&inImg);

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


#endif /* __MLE_IMAGMREF_H_ */
