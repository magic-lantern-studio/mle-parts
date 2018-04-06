/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dTextureMapMediaRef.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Texture Map Media Reference
 * targeting the Inventor platform.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
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

// Include system header files.
#ifdef WIN32
#include <windows.h>
#endif /* WIN32 */
#ifdef psx
#include <stdio.h>
#endif

// Include Runtime Player header files
#include "Inventor/nodes/SoTexture2.h"
#include "mle/3dtexmpm.h"


MLE_MEDIAREF_SOURCE(Mle3dTextureMapMediaRef,MleMediaRef);


Mle3dTextureMapMediaRef::Mle3dTextureMapMediaRef()
{
    // Do nothing extra.
}


Mle3dTextureMapMediaRef::~Mle3dTextureMapMediaRef()
{
    // Do nothing.
}


void* Mle3dTextureMapMediaRef::read()
{
    // Declare local variables.
    char *buffer = m_references->buffer;
    void* result = NULL;

    // Check flags to see if the reference is internal/external.

    if ( buffer ) 
    {
	    // Invoke the converter to prepare the local file.
	    m_converter->setReference(buffer);
	    char *filename = m_converter->getFilename();

        if (m_converter->getFile()) 
        {
            // Do Nothing.
	    }
    }
    return result;
}

