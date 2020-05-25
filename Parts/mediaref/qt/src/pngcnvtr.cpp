/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file pngcnvtr.cpp
 * @ingroup MleParts
 *
 * @author Mark S. Millard
 * @created June 15, 2017
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

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "pngcnvtr.h"


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
    if (m_fp) {
        fclose(m_fp);
        m_fp = NULL;
    }

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

unsigned int
PngMrefConverter::getDataSize()
{
    unsigned int size = sizeof(png_byte) * getWidth() * getHeight() * 3;
    return size;
}

unsigned char *
PngMrefConverter::getData()
{
    unsigned char *data = NULL;

    if (m_pngInfo) {
        int width = getWidth();
        int height = getHeight();
        data = (png_byte *) mlMalloc(sizeof(png_byte) * width * height * 3);

        for (int y = 0; y < height; y++) {
            png_bytep row = m_rowPointers[y];
            for (int x = 0; x < width; x++) {
                int xoffset = x * 3;
                png_bytep px = &(row[xoffset]);

                int yoffset = y * png_get_rowbytes(m_png, m_pngInfo);
                data[yoffset + xoffset] = px[0];     // red component
                data[yoffset + xoffset + 1] = px[1]; // green component
                data[yoffset + xoffset + 2] = px[2]; // blue component
                //data[yoffset + xoffset + 3] = px[3]; // alpha component
            }
        }
    }

    return data;
}
