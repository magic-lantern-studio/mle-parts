/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file brstmref.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a BRender Shader Table Media Reference
 * targeting the BRender platform.
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

// Include Runtime Player header files
#include "mle/brstmref.h"


MLE_MEDIAREF_SOURCE(MleBrShadeTableMediaRef,MleMediaRef);


MleBrShadeTableMediaRef::MleBrShadeTableMediaRef()
{
    // Do nothing extra
}


MleBrShadeTableMediaRef::~MleBrShadeTableMediaRef()
{
    // Do nothing
}


br_pixelmap *MleBrShadeTableMediaRef::read()
{
    // Declare local variables
    char *buffer = m_references->buffer;
    br_pixelmap *newTable = NULL;

    // Check flags to see if the reference is internal/external

    // Read BRender Shade Table from external reference
    if ( buffer )
    {
        // Invoke the converter to prepare the local file
        m_converter->setReference(buffer);
        char *filename = m_converter->getFilename();

	// Load the shade table
	newTable =  BrPixelmapLoad(filename);
	if (newTable)
	{
	    // Check to see if it's already in the registry
        br_pixelmap *oldTable = BrTableFind(newTable->identifier);
	    if (oldTable != NULL)
	    {
	        // Already registered -- throw this duplicate away
		    BrPixelmapFree(newTable);

		    // Return registered table
		    return oldTable;
	    }

	    // Add the new shade table to the BRender table registry
	    if (newTable) 
	        BrTableAdd(newTable);
	    }
    }

    return newTable;
}
