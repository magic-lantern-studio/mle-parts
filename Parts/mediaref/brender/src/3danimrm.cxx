/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3danimrm.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Animation Registry Media Reference
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

// Include system header files.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>

#ifdef WIN32
#include <io.h>
#else //not WIN32
#include <unistd.h>
#endif 

// Include Magic Lantern header files.
#include "mle/3danimrm.h"


MLE_MEDIAREF_SOURCE(Mle3dAnimationRegistryMediaRef,MleMediaRef);


Mle3dAnimationRegistryMediaRef::Mle3dAnimationRegistryMediaRef()
{
    m_animationRegistry = NULL;
}


Mle3dAnimationRegistryMediaRef::~Mle3dAnimationRegistryMediaRef()
{
    // Do nothing.
}


Mle3dAnimationRegistry* Mle3dAnimationRegistryMediaRef::read()
{
    // Declare the return structure.
    char *buffer = m_references->buffer;
    Mle3dAnimationRegistry* animationRegistry = NULL;

    // Read Animation Registry from external reference.
    if (buffer)
	{
        // Invoke the converter to prepare the local file.
        m_converter->setReference(buffer);
        char *filename = m_converter->getFilename();

        // TRUE return means we downloaded successfully to buffer.
        if (m_converter->getFile())
		{
            animationRegistry = Mle3dAnimationRegistry::read(filename);
		}
	}

    if (! animationRegistry)
        fprintf(stderr,"Mle3dAnimationRegistryMediaRef::Warning--No animation found in animation registry.\n");

    return animationRegistry;
}
