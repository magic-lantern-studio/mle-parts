/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file brmtmref.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Material Media Reference
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

// Include system header files
#ifdef WIN32
#include <windows.h>
#endif /* WIN32 */

// Include Runtime Player header files
#include "mle/mlTypes.h"
#include "mle/brmtmref.h"


MLE_MEDIAREF_SOURCE(brmtmref,MleMediaRef);


brmtmref::brmtmref()
{
    // Do nothing extra
}


brmtmref::~brmtmref()
{
    // Do nothing
}


br_material *brmtmref::read()
{
    // Declare local variables
    char *buffer = m_references->buffer;
    br_material *result = NULL;

    // Check flags to see if the reference is internal/external

    // Read BRender Material from external reference
    if ( buffer )
	{
        // Invoke the converter to prepare the local file
        m_converter->setReference(buffer);
        char *filename = m_converter->getFilename();

        // TRUE return means we downloaded successfully to buffer
        if (m_converter->getFile())
		{
            result =  BrMaterialLoad(filename);
        }

	    // XXX Where does this get added to the BRender registry?
    } 

    return result;
}
