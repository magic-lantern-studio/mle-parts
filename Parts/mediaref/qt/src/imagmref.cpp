/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file imagmref.cpp
 * @ingroup MleParts
 *
 * This file implements the class for an Image Media Reference
 * targeting the Qt platform.
 *
 * @author Mark S. Millard
 * @date June 15, 2017
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

// Include system header files

// Include runtime player header files
#include "mle/mlFileio.h"

// Include Qt Stage header files.
#include "imagmref.h"
#ifdef PNGMREFCONVERTER
#include "pngcnvtr.h"
#else
#include "qtpngcnvtr.h"
#endif /* PNGMREFCONVERTER */
#include "mle/qt/qtstage.h"


MLE_MEDIAREF_SOURCE(MleImageMediaRef,MleMediaRef)


MleImageMediaRef::MleImageMediaRef()
{
	// Delete default converter.
	deleteConverter();

#ifdef PNGMREFCONVERTER
	// Use the PNG Media Reference converter.
	PngMrefConverter *converter = new PngMrefConverter();
#else
    // Use the Qt PNG Media Reference converter.
    QtPngMrefConverter *converter = new QtPngMrefConverter();
#endif /* PNGMREFCONVERTER */
	setConverter(converter);
}


MleImageMediaRef::~MleImageMediaRef()
{
	// The PNG Media Reference converter will be deleted by the
	// base class, MleMediaRef.
}


MlBoolean MleImageMediaRef::read(QPixmap *image)
{
    // Declare local variables
    MlBoolean status = ML_FALSE;
    char *buffer = m_references->buffer;
	char *filename;

    // Retrieve the data from the media reference converter.
    if (buffer)
	{
        // Invoke the converter to prepare the local file.
	    m_converter->setReference(buffer);
	    filename = m_converter->getFilename();

	    // TRUE return means we downloaded successfully to buffer.
    	if (m_converter->getFile())
		{
	        // File was successully downloaded, use the provided surface
			// for loading our image.
            status = ML_TRUE;
        }
    }
    if (ML_FALSE == status)
	{
        return ML_FALSE;
    }

    // Get the Qt stage and associated device.
    //QtStage *theStage = QtStage::cast(MleStage::g_theStage);

    // Create a pixmap the size of the image we are retrieving.
    // Load the image from the file into our Magic Lantern offscreen buffer.
    unsigned int width = ((ImagMrefConverter *)m_converter)->getWidth();
    unsigned int height = ((ImagMrefConverter *)m_converter)->getHeight();
#ifdef PNGMREFCONVERTER
    unsigned int imageDataSize = ((PngMrefConverter *)m_converter)->getDataSize();
    unsigned char *imageData = ((PngMrefConverter *)m_converter)->getData();
    QImage qimage = QImage(imageData, width, height, QImage::Format_RGB444);
    **image = QPixmap::fromImage(qimage);
    //QPixmap *pixmap = new QPixmap();
    //status = pixmap->loadFromData(imageData, imageDataSize, "PNG");

	// XXX - The PngMrefConverter is holding on to the PNG image data. Instead of
    // creating the pixmap from a file, we should find a way to transfer the data
    // directly into the surface.

    // Clean-up and return.
	((PngMrefConverter *)m_converter)->releaseFile();
#else
    QImage *qimage = ((QtPngMrefConverter *)m_converter)->getData();
    QPixmap pixmap = QPixmap::fromImage(*qimage);
    *image = pixmap;

    // Clean-up and return.
    ((QtPngMrefConverter *)m_converter)->releaseFile();
#endif /* PNGMREFCONVERTER */

    return status;
}

MlBoolean MleImageMediaRef::loadReference(const char *filename)
{
    MlBoolean status = ML_FALSE;

    // Allocate a new reference.
    MleMediaLoadReference *newReference = (MleMediaLoadReference *) mlMalloc(
        sizeof(MleMediaLoadReference));

    newReference->bufsiz = strlen(filename);
    newReference->buffer = (char *) malloc(newReference->bufsiz + 1);
    strcpy(newReference->buffer, filename);
    newReference->flags = 0;
    newReference->next = NULL;

    // Attach next reference.
    MleMediaLoadReference *nextReference = m_references;
    if (! nextReference)
    {
        // First entry on the list.
        m_references = newReference;
    } else
    {
        // Add entry to end of list.
        while (nextReference->next != NULL)
            nextReference = nextReference->next;
        nextReference->next = newReference;
    }
    m_numReferences++;

    status = ML_TRUE;
    return status;
}
