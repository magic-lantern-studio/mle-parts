/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dtextrm.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Texture Registry Media Reference
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
#include <windows.h>
#else /* not WIN32 */
#include <unistd.h>
#endif

// Include Magic Lantern header files.
#include "mle/mlFileIO.h"
#include "math/scalarbr.h"
#include "math/vector.h"
#include "mle/brendapi.h"


// Include Runtime Player header files
#include "mle/3dtextrm.h"


MLE_MEDIAREF_SOURCE(Mle3dTextureRegistryMediaRef,MleMediaRef);


Mle3dTextureRegistryMediaRef::Mle3dTextureRegistryMediaRef()
{
    // Do nothing extra.
}


Mle3dTextureRegistryMediaRef::~Mle3dTextureRegistryMediaRef()
{
    // Do nothing.
}

// XXX - This is a hokey max on loading BRender registries
#define MLE_MAX_TEXTURES 1024



br_pixelmap** Mle3dTextureRegistryMediaRef::read()
{
    // Declare local variables
    char *buffer = m_references->buffer;

    // XXX Need to distinguish media reloading or recycling; see note in
    // mlLoadBrPixelmapRegistry() in 3dscenem.cxx 

    // Set up for failure mode.
    m_textureRegistryList = NULL;

    // Read BRender Texture Registry from external reference
    if (buffer)
	{
        // Invoke the converter to prepare the local file
        m_converter->setReference(buffer);
        char *filename = m_converter->getFilename();

        // TRUE return means we downloaded successfully to buffer
        if (m_converter->getFile())
		{
            m_textureRegistryList = loadTextureRegistry(filename,
                m_textureRegistrySize);
		}
	}
    if (! (m_textureRegistryList && m_textureRegistrySize))
        fprintf(stderr,"Mle3dTextureRegistryMediaRef::Warning--no textures found in texture registry.\n");

    return m_textureRegistryList;
}


br_pixelmap** Mle3dTextureRegistryMediaRef::loadTextureRegistry(char* filename, int& numTextures)
{
    br_pixelmap*  tempRegistry[MLE_MAX_TEXTURES];
    br_pixelmap** textureRegistry = NULL;
    numTextures = 0;

    if (filename)
	{
        int filecheck = mlOpen(filename,O_RDONLY);
        if (filecheck > -1)
		{
			mlClose(filecheck);

            numTextures = BrPixelmapLoadMany(filename,tempRegistry,MLE_MAX_TEXTURES);

            if (numTextures)
			{
				textureRegistry = new br_pixelmap*[numTextures];
				int i;
                for (i = 0; i < numTextures; i++)
                    textureRegistry[i] = tempRegistry[i];
                BrMapAddMany(textureRegistry,i);
			}
            else
               fprintf(stderr,"Mle3dTextureRegistryMediaRef::Error--Could not load textures from file %s.\n",filename);
		}
        else
            fprintf(stderr,"Mle3dTextureRegistryMediaRef::Error--Could not find texture registry file %s.\n",filename);
	}
    else
        fprintf(stderr,"Mle3dTextureRegistryMediaRef::Error--No filename given for texture registry file.\n");

    return textureRegistry;
}
