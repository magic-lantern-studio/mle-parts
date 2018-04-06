/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dmatlrm.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Material Registry Media Reference
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
#else //not WIN32
#include <unistd.h>
#endif

// Include Magic Lantern header files.
#include "mle/mlFileIO.h"
#include "math/scalarbr.h"
#include "math/vector.h"
#include "mle/brendapi.h"
#include "mle/3dmatlrm.h"


MLE_MEDIAREF_SOURCE(Mle3dMaterialRegistryMediaRef,MleMediaRef);


Mle3dMaterialRegistryMediaRef::Mle3dMaterialRegistryMediaRef()
{
    m_materialRegistrySize = 0;
    m_materialRegistryList = NULL;
}


Mle3dMaterialRegistryMediaRef::~Mle3dMaterialRegistryMediaRef()
{
    // Do nothing.
}


// XXX - This is a hokey max on loading BRender registries
#define MLE_MAX_MATERIALS 1024


br_material** Mle3dMaterialRegistryMediaRef::read()
{
    // Set up for failure mode
    char *buffer = m_references->buffer;
    m_materialRegistryList = NULL;

    // Read BRender MaterialRegistry Registry from external reference
    if (buffer)
	{
	    // Invoke the converter to prepare the local file
	    m_converter->setReference(buffer);
	    char *filename = m_converter->getFilename();

	    // TRUE return means we downloaded successfully to buffer
	    if (m_converter->getFile())
		{
	        m_materialRegistryList =
		        loadMaterialRegistry(filename,m_materialRegistrySize);
		}
	}

    if (! (m_materialRegistryList && m_materialRegistrySize))
        fprintf(stderr,"Mle3dMaterialRegistryMediaRef::Warning--no materials found in material registry.\n");

    return m_materialRegistryList;
}


br_material** Mle3dMaterialRegistryMediaRef::loadMaterialRegistry(char* filename, int& numMaterials)
{
    br_material*  tempRegistry[MLE_MAX_MATERIALS];
    br_material** materialRegistry=NULL;
    numMaterials = 0;

    if (filename)
	{
        int filecheck = mlOpen(filename,O_RDONLY);
        if (filecheck>-1)
		{
			mlClose(filecheck);

            numMaterials = BrMaterialLoadMany(filename,tempRegistry,MLE_MAX_MATERIALS);

            if (numMaterials)
			{
				char* string;
                char* textureName;
                materialRegistry = new br_material*[numMaterials];
                for (int i = 0; i < numMaterials; i++)
				{
					materialRegistry[i] = tempRegistry[i];

                    string = strdup(materialRegistry[i]->identifier);
                    // XXX Texture name is the second half of the BRender material name
                    strtok(string,"\n");
                    textureName = strtok(NULL,"");
                    if (textureName)
					{
                         br_pixelmap* texture = BrMapFind(textureName);
                         if (texture)
                             materialRegistry[i]->colour_map=texture;
					}
                    if (materialRegistry[i]) 
					{
						BrMaterialAdd(materialRegistry[i]);
                        BrMaterialUpdate(materialRegistry[i],BR_MATU_ALL);
					}
				}
                delete string;
			}
            else
                fprintf(stderr,"Mle3dMaterialRegistryMediaRef::Error--Could not load materials from file %s.\n",filename);
		}
        else
            fprintf(stderr,"Mle3dMaterialRegistryMediaRef::Error--Could not find material registry file %s.\n",filename);
	}
    else
         fprintf(stderr,"Mle3dMaterialRegistryMediaRef::Error--No filename given for material registry file.\n");

    return materialRegistry;
}

