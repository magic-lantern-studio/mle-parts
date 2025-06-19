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

#if defined(_WINDOWS)
#if defined(MLE_REHEARSAL)
// Use FreeImage library for Rehearsal Player on Windows.
#include <FreeImage.h>
#else /* ! MLE_REHEARSAL */
#include "mle/dib.h"
#endif
#else /* ! _WINDOWS */
// Use FreeImage library.
#include <FreeImage.h>
#endif


/**
 * @brief This class implements a Media Reference loader for a still image.
 */
class MLE_2DIMAGEMEDIAREF_API MleImageMediaRef : public MleMediaRef
{
    MLE_MEDIAREF_HEADER(MleImageMediaRef);

#if defined(__linux__) || defined(MLE_REHEARSAL)

  private:

    /** The IO handler for FreeImage. */
    FreeImageIO m_io;

#endif /* MLE_REHEARSAL */

  public:

    MleImageMediaRef(void);

    virtual ~MleImageMediaRef(void);

#if defined(__linux__) || defined(MLE_REHEARSAL)

    MlBoolean read(FIBITMAP *&inImg);

#elif defined(_WINDOWS)

    MlBoolean read(MleDIB &dib);

#endif /* _WINDOWS */
};


#endif /* __MLE_IMAGMREF_H_ */
