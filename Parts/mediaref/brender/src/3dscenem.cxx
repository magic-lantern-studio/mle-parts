/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dscenem.cxx
 * @ingroup MleParts
 *
 * This file defines the class for a 3D Scene Graph Media Reference
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
#include "mle/brendapi.h"

// Include Runtime Player header files
#include "mle/3dscenem.h"

#define MLE_MEDIA_REF_FILENAME_SEPARATOR ","

br_actor**    mlLoadBrActorRegistry(char* filename);
br_model**    mlLoadBrModelRegistry(char* filename);
br_material** mlLoadBrMaterialRegistry(char* filename);
br_pixelmap** mlLoadBrPixelmapRegistry(char* filename);


MLE_MEDIAREF_SOURCE(Mle3dSceneGraphMediaRef,MleMediaRef);


Mle3dSceneGraphMediaRef::Mle3dSceneGraphMediaRef()
{
    nameList = NULL;
    nodeList = geometryList=materialList = textureList = NULL;
}


Mle3dSceneGraphMediaRef::~Mle3dSceneGraphMediaRef()
{
}


void* Mle3dSceneGraphMediaRef::read()
{
    // Declare local variables.
    char *buffer = m_references->buffer;
    void* result = NULL;

    // Need to distinguish media reloading or recycling       

    // Read BRender Scene Graph from external reference
    if (buffer) 
	{
        // Invoke the converter to prepare the local file
        m_converter->setReference(buffer);
        char* target = m_converter->getFilename();

        // TRUE return means we downloaded successfully to buffer
        if (m_converter->getFile()) 
            result = loadSceneGraph(target);
	}

    if (! result)
        fprintf(stderr,"Mle3dSceneGraphMediaRef::Warning--no scene graph found in media file.\n");

    return result;
}


void* Mle3dSceneGraphMediaRef::loadSceneGraph(char* target)
{
	void* result = NULL;

    //XXX This section will be replaced by calls to the base MleMediaRef
    //    to get the file list and file types
    char** fileType = new char*[256];
    char** fileList = new char*[256];
    int fileCount=0;
    fileType[fileCount] = strtok(target,MLE_MEDIA_REF_FILENAME_SEPARATOR);
    fileList[fileCount] = strtok(NULL,MLE_MEDIA_REF_FILENAME_SEPARATOR);

    while (fileType[fileCount] && fileList[fileCount])
	{
		fileCount++;
        fileType[fileCount] = strtok(NULL,MLE_MEDIA_REF_FILENAME_SEPARATOR);
        fileList[fileCount] = strtok(NULL,MLE_MEDIA_REF_FILENAME_SEPARATOR);
	}

    if (fileCount)
	{ 
        // Load the textures from the BRender texture registry
        // (This must be done before loading the material registry)
        int textureFileCount = 0;
		int i;
        for (i = 0; i < fileCount; i++)
            if (strcmp(fileType[i],"textures") == 0)
			{
                textureList = (void**)mlLoadBrPixelmapRegistry(fileList[i]);
                textureFileCount++;
			}

        // Load the materials from the BRender material registry
        // (This must be done before loading the geometry registry)
        int materialFileCount = 0;
        for (i = 0; i < fileCount; i++)
            if (strcmp(fileType[i],"materials") == 0)
			{
                materialList = (void**)mlLoadBrMaterialRegistry(fileList[i]);
                materialFileCount++;
			}

        // Load the geometry from the BRender model registry
        // (This must be done before loading the actor registry)
        int geometryFileCount = 0;
        for (i = 0; i < fileCount; i++)
            if (strcmp(fileType[i],"geometry") == 0)
			{ 
                geometryList = (void**)mlLoadBrModelRegistry(fileList[i]);
                geometryFileCount++;
			}

        // Load the scene graph from the BRender actor registry
        int sceneGraphFileCount = 0;
        for (i = 0; i < fileCount; i++)
            if (strcmp(fileType[i],"sceneGraph")==0)
			{ 
                nodeList = (void**)mlLoadBrActorRegistry(fileList[i]);
                sceneGraphFileCount++;
			}
       
        nodeCount = 0;
        if (nodeList)
		{
			while (nodeList[nodeCount]) nodeCount++;
            nameList = new char*[nodeCount];
            i = 0;
            while (nodeList[i]) 
			{
				nameList[i] = strdup(((br_actor*)nodeList[i])->identifier);
                i++;
			}
		}
        else
            fprintf(stderr,"Mle3dSceneGraphMediaRef::Error--No sceneGraph media found in media ref.\n");
    
        result=nodeList;
	}
    else
       fprintf(stderr,"Mle3dSceneGraphMediaRef::Error--No filenames found in media ref.\n");

    delete[] fileType;
    delete[] fileList;
    return result;
}



br_actor** mlLoadBrActorRegistry(char* filename)
{
    br_actor*  tempRegistry[MLE_MAX_NODES];
    br_actor** actorRegistry = NULL;

    if (filename)
	{
        int filecheck = mlOpen(filename,O_RDONLY);
        if (filecheck > -1)
		{
			mlClose(filecheck);

            int actorCount = BrActorLoadMany(filename,tempRegistry,MLE_MAX_NODES);

            if (actorCount)
			{
				actorRegistry = new br_actor*[actorCount+1];
                for (int i = 0; i < actorCount; i++)
                    actorRegistry[i]=tempRegistry[i];
                actorRegistry[actorCount]=NULL;
			}
            else
                fprintf(stderr,"Mle3dSceneGraphMediaRef::Error--Could not load actor registry from file %s.\n",filename);
		}
        else
            fprintf(stderr,"Mle3dSceneGraphMediaRef::Error--Could not find actor registry file %s.\n",filename);
	}
    else
        fprintf(stderr,"Mle3dSceneGraphMediaRef::Error--No filename given for actor registry file.\n");

    return actorRegistry;
}


br_model** mlLoadBrModelRegistry(char* filename)
// Loads a model registry from the given file and returns a list of models.  
// Also sets the number of models read in the modelCount parameter
{ 
    br_model** modelList = NULL;
    br_model* tempList[MLE_MAX_NODES];

    if (filename)
	{ 
       int filecheck = mlOpen(filename,O_RDONLY);
       if (filecheck > -1)
	   {
		   mlClose(filecheck);

           int modelCount = BrModelLoadMany(filename,tempList,MLE_MAX_NODES);
           if (modelCount)
		   { 
               modelList = new br_model*[modelCount+1];
               for (int i = 0; i < modelCount; i++)
                   modelList[i] = tempList[i];
               BrModelAddMany(modelList,modelCount);
               modelList[modelCount]=NULL;
		   }
           else
               fprintf(stderr,"Mle3dSceneGraphMediaRef::Error--Could not load model registry from file %s.\n",filename);
          }
       else
          fprintf(stderr,"Mle3dSceneGraphMediaRef::Error--Could not find model registry file %s.\n",filename);
	} 
    else
        fprintf(stderr,"Mle3dSceneGraphMediaRef::Error--No filename given for model registry.\n");

    return modelList;
}


br_material** mlLoadBrMaterialRegistry(char* filename)
{
    br_material*  tempRegistry[MLE_MAX_NODES];
    br_material** materialRegistry = NULL;

    if (filename)
	{
        int filecheck = mlOpen(filename,O_RDONLY);
        if (filecheck > -1)
		{
			mlClose(filecheck);

            int materialCount = BrMaterialLoadMany(filename,tempRegistry,MLE_MAX_NODES);
            if (materialCount)
			{
				char* string;
                char* textureName;
                materialRegistry = new br_material*[materialCount+1];
                for (int i = 0; i < materialCount; i++)
				{
					materialRegistry[i]=tempRegistry[i];
                    string=strdup(materialRegistry[i]->identifier);
                    // XXX Texture name is the second half of the BRender material name
                    strtok(string,"\n");
                    textureName = strtok(NULL,"");
                    if (textureName)
					{
                        br_pixelmap* texture = BrMapFind(textureName);
                        if (texture)
                            materialRegistry[i]->colour_map = texture;
					}
                    BrMaterialAdd(materialRegistry[i]);
                    BrMaterialUpdate(materialRegistry[i],BR_MATU_ALL);
				}
                materialRegistry[materialCount] = NULL;
                delete string;
			}
            else
                fprintf(stderr,"Mle3dSceneGraphMediaRef::Error--Could not load material registry from file %s.\n",filename);
		}
        else
            fprintf(stderr,"Mle3dSceneGraphMediaRef::Error--Could not find material registry file %s.\n",filename);
	}
    else
        fprintf(stderr,"Mle3dSceneGraphMediaRef::Error--No filename given for material registry file.\n");

    return materialRegistry;
}


br_pixelmap** mlLoadBrPixelmapRegistry(char* filename)
{
    br_pixelmap*  tempRegistry[MLE_MAX_NODES];
    br_pixelmap** pixelmapRegistry = NULL;

    if (filename)
	{
        int filecheck = mlOpen(filename,O_RDONLY);
        if (filecheck > -1)
		{
			mlClose(filecheck);

            int pixelmapCount = BrPixelmapLoadMany(filename,tempRegistry,MLE_MAX_NODES);
            if (pixelmapCount)
			{
				pixelmapRegistry = new br_pixelmap*[pixelmapCount+1];
                for (int i = 0; i < pixelmapCount; i++)
                    pixelmapRegistry[i] = tempRegistry[i];

                // XXX Might want to check whether textures with the same
                // identifiers are already in the BRender registry.
                // Duplicates could be thrown away to reclaim memory.
                BrMapAddMany(pixelmapRegistry,pixelmapCount);
                pixelmapRegistry[pixelmapCount] = NULL;
			}
            else
                fprintf(stderr,"Mle3dSceneGraphMediaRef::Error--Could not load texture registry from file %s.\n",filename);
		}
        else
            fprintf(stderr,"Mle3dSceneGraphMediaRef::Error--Could not find texture registry file %s.\n",filename);
	}
    else
        fprintf(stderr,"Mle3dSceneGraphMediaRef::Error--No filename given for texture registry file.\n");

    return pixelmapRegistry;
}
