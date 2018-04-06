/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dModelMediaRef.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Model Media Reference
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

// Include Inventor header files.
#include "Inventor/SoDB.h"
#include "Inventor/SoInput.h"
#include "Inventor/nodes/SoSeparator.h"

#include "mle/3dmodelm.h"


MLE_MEDIAREF_SOURCE(Mle3dModelMediaRef,MleMediaRef);


Mle3dModelMediaRef::Mle3dModelMediaRef()
{
    // Do nothing extra.
}


Mle3dModelMediaRef::~Mle3dModelMediaRef()
{
    // Do nothing.
}


void* Mle3dModelMediaRef::read()
{
    // Declare local variables.
    char *buffer = m_references->buffer;

    // Check flags to see if the reference is internal/external.

    // Read Inventor Model from external reference.

    if (!buffer) return NULL;

    SoInput in;
    if (!in.openFile((char*) buffer))
    {
		printf("Mle3dModelMediaRef(iv): can't read file %s\n", buffer);
		return NULL;
    }
    
    SoSeparator *myGraph = SoDB::readAll(&in);
    if (myGraph == NULL)
    {
		printf("Mle3dModelMediaRef(iv): problem reading file %s\n", buffer);
		return NULL;
    }

    in.closeFile();
    return myGraph;
}

