/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file pngcnvtr.cxx
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

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "dxs/pngcnvtr.h"


PngMrefConverter::PngMrefConverter()
  : m_opened(FALSE), m_infoRead(FALSE)
{
	m_png = NULL;
	m_pngInfo = NULL;
	m_fp = NULL;
}


PngMrefConverter::~PngMrefConverter()
{
	cleanup();
}


void PngMrefConverter::cleanup()
{
	if (m_rowPointers)
	{
    	png_uint_32 height;

		//height = m_pngInfo->height;
		height = png_get_image_height(m_png, m_pngInfo);
		for (unsigned int y = 0; y < height; y++)
			mlFree(m_rowPointers[y]);

		mlFree(m_rowPointers);
		m_rowPointers = NULL;
	}

	if (m_pngInfo)
	{
		png_destroy_info_struct(m_png, &m_pngInfo);
		m_pngInfo = NULL;
	}

	if (m_png)
	{
		png_destroy_read_struct(&m_png, (png_infopp)NULL, (png_infopp)NULL);
		m_png = NULL;
	}


	if (m_fp)
	{
		fclose(m_fp);
		m_fp = NULL;
	}

	m_infoRead = FALSE;
	m_opened = FALSE;
}


// Check to see if a file is a PNG file using png_sig_cmp().  png_sig_cmp()
// returns zero if the image is a PNG and nonzero if it isn't a PNG.
// 
// The function checkIfPng() shown here, returns nonzero (true)
// if the file can be opened and is a PNG, 0 (false) otherwise.
// 
// If this call is successful, and you are going to keep the file open,
// you should call png_set_sig_bytes(png_ptr, PNG_BYTES_TO_CHECK); once
// you have created the png_ptr, so that libpng knows your application
// has read that many bytes from the start of the file.  Make sure you
// don't call png_set_sig_bytes() with more than 8 bytes read or give it
// an incorrect number of bytes read, or you will either have read too
// many bytes (your fault), or you are telling libpng to read the wrong
// number of magic bytes (also your fault).
// 
// Many applications already read the first 2 or 4 bytes from the start
// of the image to determine the file type, so it would be easiest just
// to pass the bytes to png_sig_cmp() or even skip that if you know
// you have a PNG file, and call png_set_sig_bytes().

#define PNG_BYTES_TO_CHECK 8
MlBoolean PngMrefConverter::checkIfPng(char *filename)
{
   unsigned char buf[PNG_BYTES_TO_CHECK];

   // Open the prospective PNG file.
   if ((m_fp = fopen(filename, "rb")) == NULL)
      return FALSE;

   // Read in some of the signature bytes.
   if (fread(buf, 1, PNG_BYTES_TO_CHECK, m_fp) != PNG_BYTES_TO_CHECK)
      return FALSE;

   // Compare the first PNG_BYTES_TO_CHECK bytes of the signature.
   // Return nonzero (true) if they match.
   if (! png_sig_cmp(buf, (png_size_t)0, PNG_BYTES_TO_CHECK))
	   m_opened = TRUE;

   return TRUE;
}


MlBoolean PngMrefConverter::readPngInfo(unsigned int sigRead)
{
	// Make sure we are working with a valid PNG file.
	if (! m_opened)
		return FALSE;

    // Create and initialize the png_struct with the desired error handler
    // functions.  Since we want to use the default stderr and longjump method,
    // we supply NULL for the last three parameters.  We also supply the
    // the compiler header file version, so that we know if the application
    // was compiled with a compatible version of the library.  REQUIRED

    m_png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (m_png == NULL)
    {
        return FALSE;
    }

    // Allocate/initialize the memory for image information.  REQUIRED.

    m_pngInfo = png_create_info_struct(m_png);
    if (m_pngInfo == NULL)
    {
        png_destroy_read_struct(&m_png, (png_infopp)NULL, (png_infopp)NULL);
        return FALSE;
    }

    // Set error handling since we are using the setjmp/longjmp method (this is
    // the normal method of doing things with libpng).  REQUIRED unless we
    // set up our own error handlers in the png_create_read_struct() earlier.

    if (setjmp(png_jmpbuf(m_png)))
    {
		cleanup();

        // If we get here, we had a problem reading the file.
        return FALSE;
    }

    // Set up the input control since we are using standard C streams.
    png_init_io(m_png, m_fp);

    // If we have already read some of the signature, increment the number of bytes read.
    png_set_sig_bytes(m_png, sigRead);

	// Read image information.
    png_read_info(m_png, m_pngInfo);

	m_infoRead = TRUE;

	return 1;
}


MlBoolean
PngMrefConverter::readPngData()
{
	png_uint_32 height;
	int numberOfPasses;

	if ((! m_opened) || (! m_infoRead))
		return FALSE;

    // Set error handling since we are using the setjmp/longjmp method (this is
    // the normal method of doing things with libpng).  REQUIRED unless we
    // set up our own error handlers in the readPngInfo() earlier.

    if (setjmp(png_jmpbuf(m_png)))
    {
		cleanup();

        // If we get here, we had a problem reading the file.
        return FALSE;
    }

	//height = m_pngInfo->height;
	height = png_get_image_height(m_png, m_pngInfo);

	// Set interlace handling.
	numberOfPasses = png_set_interlace_handling(m_png);
	png_read_update_info(m_png, m_pngInfo);

	// Allocate enough space to read the image data.
	m_rowPointers = (png_bytep *) mlMalloc(sizeof(png_bytep) * height);
	for (unsigned int y = 0; y < height; y++)
		//m_rowPointers[y] = (png_byte *) mlMalloc(m_pngInfo->rowbytes);
        m_rowPointers[y] = (png_byte *) mlMalloc(png_get_rowbytes(m_png, m_pngInfo));

	// Read the entire image in one pass.
	png_read_image(m_png, m_rowPointers);

	// Read rest of file, and get additional chunks in info_ptr - REQUIRED.
    png_read_end(m_png, m_pngInfo);

	return TRUE;
}


MlBoolean
PngMrefConverter::getFile()
{
	MlBoolean status = TRUE;

	// Open the file and check that it is a valid PNG file.
	MlBoolean isPng = checkIfPng(m_reference);
	if (isPng)
	{
		// Read the png file into a buffer.
		if (! readPngInfo(PNG_BYTES_TO_CHECK))
		{
			status = FALSE;
		} else if (! readPngData())
		{
			status = FALSE;
		}

	} else
	{
		// Not a valid PNG file.
		status = FALSE;
	}

	// Close the file opened by checkIfPng().
	if (m_fp) fclose(m_fp);

	return status;
}


MlBoolean
PngMrefConverter::releaseFile()
{
	cleanup();
	return TRUE;
}

unsigned int
PngMrefConverter::getWidth()
{
	if (m_pngInfo)
		//return m_pngInfo->width;
		return png_get_image_width(m_png, m_pngInfo);
	else
		return 0;
}


unsigned int
PngMrefConverter::getHeight()
{
	if (m_pngInfo)
		//return m_pngInfo->height;
		return png_get_image_height(m_png, m_pngInfo);
	else
		return 0;
}
