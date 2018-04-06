/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file qtpngcnvtr.h
 * @ingroup MleParts
 *
 * @author Mark S. Millard
 * @created July 5, 2017
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

#ifndef __MLE_QTPNGCNVTR_H_
#define __MLE_QTPNGCNVTR_H_

// Include Qt header files.
#include <QImage>

// Include Magic Lantern header files.
#include "qtmref_global.h"
#include "imagcnvtr.h"

/**
 * @brief The class provides a Media Reference Converter for the PNG
 * image file format.
 *
 * This class uses the QtImage object to load and process the PNG
 * file.
 */
class MLEQTMREFSHARED_EXPORT QtPngMrefConverter : public ImagMrefConverter
{
  public:

    /**
     * Default constructor.
     */
    QtPngMrefConverter(void);

    /**
     * The destructor.
     */
    virtual ~QtPngMrefConverter(void);

    /**
     * @brief Get the PNG file, validating the file and
     * retrieving image information.
     */
    virtual MlBoolean getFile(void);

    /**
     * @brief Release the PNG file.
     */
    virtual MlBoolean releaseFile(void);

    /**
     * @brief Get the width of the retrieved file.
     */
    virtual unsigned int getWidth(void);

    /**
     * @brief Get the height of the retrieved file.
     */
    virtual unsigned int getHeight(void);

    /**
     * @brief Get the PNG image data.
     *
     * @return A pointer to the Qt QImage object containing the PNG data
     * is returned. <b>NULL</b> may be returned if the PNG file has not
     * been loaded (see getFile());
     *
     * @see getFile(void)
     */
    QImage *getData(void);

  private:

    // Open the specified file and determine if it is a real PNG file.
    //MlBoolean checkIfPng(char *filename);

  private:

    // A pointer to QImage.
    QImage *m_png;
};

#endif /* __MLE_QTPNGCNVTR_H_ */
