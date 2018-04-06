/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file pngcnvtr.h
 * @ingroup MleParts
 *
 * @author Mark S. Millard
 * @created Nov 19, 2007
 */

// COPYRIGHT_BEGIN
//
//  Copyright (C) 2000-2007  Wizzer Works
//
//  Wizzer Works makes available all content in this file ("Content").
//  Unless otherwise indicated below, the Content is provided to you
//  under the terms and conditions of the Common Public License Version 1.0
//  ("CPL"). A copy of the CPL is available at
//
//      http://opensource.org/licenses/cpl1.0.php
//
//  For purposes of the CPL, "Program" will mean the Content.
//
//  For information concerning this Makefile, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

#ifndef __MLE_PNGCNVTR_H_
#define __MLE_PNGCNVTR_H_

// Inlucde PNG header files.
#include "mle/png.h"

// Include Magic Lantern header files.
#include "dxs/imagcnvtr.h"

/**
 * @brief The class provides a Media Reference Converter for the PNG
 * image file format.
 */
class PngMrefConverter : public ImagMrefConverter
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
	unsigned int getWidth(void);

	/**
	 * Get the height of the retrieved file.
	 */
	unsigned int getHeight(void);

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
