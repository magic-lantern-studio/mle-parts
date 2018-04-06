/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file pngcnvtr.h
 * @ingroup MleParts
 *
 * @author Mark S. Millard
 * @created June 14, 2017
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

#ifndef __MLE_PNGCNVTR_H_
#define __MLE_PNGCNVTR_H_

// Inlucde PNG header files.
#include <png.h>

// Include Magic Lantern header files.
#include "qtmref_global.h"
#include "imagcnvtr.h"

/**
 * @brief The class provides a Media Reference Converter for the PNG
 * image file format.
 */
class MLEQTMREFSHARED_EXPORT PngMrefConverter : public ImagMrefConverter
{
  public:

	/**
	 * Default constructor.
	 */
    PngMrefConverter(void);

	/**
	 * The destructor.
	 */
	virtual ~PngMrefConverter(void);

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
	 * Get the width of the retrieved file.
	 */
    virtual unsigned int getWidth(void);

	/**
	 * Get the height of the retrieved file.
	 */
    virtual unsigned int getHeight(void);

    /**
     * @brief Get the image data of the retrieved file.
     *
     * @return A pointer to the image data is returned.
     */
    virtual unsigned char *getData(void);

    /**
     * @brief Get the size of the image data of the retrieved file.
     *
     * Assumption that PNG file is in the RGB format.
     *
     * @return The number of bytes is returned.
     */
    virtual unsigned int getDataSize(void);

  private:

	// Open the specified file and determine if it is a real PNG file.
	MlBoolean checkIfPng(char *filename);

	// Read the PNG image info.
	MlBoolean readPngInfo(unsigned int sigRead);

	// Read the PNG image data.
	MlBoolean readPngData(void);

	// Cleanup.
	void cleanup(void);

  private:

	// A pointer to png_struct.
	png_structp m_png;
	// A pointer to the png_info.
	png_infop   m_pngInfo;
	// A pointer to an array of bytes for each row in the image.
	png_bytep  *m_rowPointers;
	// The file pointer.
	FILE *m_fp;

	// Flag indicating the file has been opened and is a valid PNG file.
	MlBoolean m_opened;
	// Flag indicating that the info_struct has been read.
	MlBoolean m_infoRead;

};

#endif /* __MLE_PNGCNVTR_H_ */
