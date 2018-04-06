/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file imagmref.h
 * @ingroup MleParts
 *
 * This file defines the class for an Image Media Reference
 * targeting the Qt platform.
 *
 * @author Mark S. Millard
 * @date June 14, 2017
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2017 Wizzer Works
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
// COPYRIGHT_END

#ifndef __MLE_IMAGMREF_H_
#define __MLE_IMAGMREF_H_

// Include system header files.

// Include Qt header files.
#include <QPixmap>

// Include runtime player header files.
#include "qtmref_global.h"
#include "mle/MleMediaRef.h"
#include "mle/mlTypes.h"

// Include Qt Stage header files.

/**
 * @brief This class implements a Media Reference loader for a still image.
 */
class MLEQTMREFSHARED_EXPORT MleImageMediaRef : public MleMediaRef
{
    MLE_MEDIAREF_HEADER(MleImageMediaRef);
    
  public:

	/**
	 * The default constructor.
	 */
    MleImageMediaRef(void);

	/**
	 * The destructor.
	 */
    virtual ~MleImageMediaRef(void);

	/**
     * @brief Read the image into the supplied offscreen buffer.
	 *
     * @param image A pointer to a Qt pixmap container.
	 *
	 * @return <b>TRUE</b> is returned if the image is successfully read.
	 * Otherwise <b>FALSE</b> is returned.
	 */
    MlBoolean read(QPixmap *image);

    /**
     * @brief Load a media reference.
     *
     * Explicitly loads an image media reference without requiring Digital
     * Workprint/Playprint intervention.
     *
     * @param filename A pointer to an image file name.
     *
     * @return <b>TRUE</b> is returned if the image is successfully loaded.
     * Otherwise <b>FALSE</b> is returned.
     */
    MlBoolean loadReference(const char *filename);
};


#endif /* __MLE_IMAGMREF_H_ */
