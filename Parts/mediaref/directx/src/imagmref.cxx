/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file imagmref.cxx
 * @ingroup MleParts
 *
 * This file implements the class for an Image Media Reference
 * targeting the DirectX platform.
 *
 * @author Mark S. Millard
 * @date Nov 16, 2007
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

// Include system header files
#include <windows.h>

// Include runtime player header files
#include "mle/mlFileIO.h"

// Include DirectX Stage header files.
#include "dxs/imagmref.h"
#include "dxs/dxstage.h"
#include "dxs/pngcnvtr.h"


MLE_MEDIAREF_SOURCE(MleImageMediaRef,MleMediaRef);


MleImageMediaRef::MleImageMediaRef()
{
	// Delete default converter.
	deleteConverter();

	// Use the PNG Media Reference converter.
	PngMrefConverter *converter = new PngMrefConverter();
	setConverter(converter);
}


MleImageMediaRef::~MleImageMediaRef()
{
	// The PNG Media Reference converter will be deleted by the
	// base class, MleMediaRef.
}


MlBoolean MleImageMediaRef::read(LPDXS_SURFACE image)
{
    // Declare local variables
    BOOL status = FALSE;
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
	        status = TRUE;
        }
    }
    if (FALSE == status)
	{
        return FALSE;
    }

	// Get the DirectX stage and associated device.
	DxStage *theStage = DxStage::cast(MleStage::g_theStage);

	// Get the display mode.
	D3DDISPLAYMODE d3dMode;
	HRESULT hr = theStage->getD3dObj()->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3dMode);
	if (hr != D3D_OK)
	{
		DXTrace(__FILE__, __LINE__, hr, "GetAdapterDisplayMode() failed when reading Image Media Reference.", true);
		((PngMrefConverter *)m_converter)->releaseFile();
		return FALSE;
	}

    // Create a surface the size of the image we are retrieving.
	// Load the image from the file into our Magic Lantern surface.
	unsigned int width = ((PngMrefConverter *)m_converter)->getWidth();
	unsigned int height = ((PngMrefConverter *)m_converter)->getHeight();
	hr = image->createSurfaceFromFile(filename, d3dMode.Format);
	if (hr != DXS_OK)
	{
		DXTrace(__FILE__, __LINE__, hr, "CreateSurfaceFromFile() failed when reading Image Media Reference.", true);
		((PngMrefConverter *)m_converter)->releaseFile();
		return FALSE;
	}

	// XXX - The PngMrefConverter is holding on to the PNG image data. Instead of
	// creating the surface from a file, we should use image->CreateSurface and
	// transfer the data directly into the surface.

    // Clean-up and return.
	((PngMrefConverter *)m_converter)->releaseFile();
    return status;
}
