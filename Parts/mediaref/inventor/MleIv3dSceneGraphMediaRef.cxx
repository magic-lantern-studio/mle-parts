/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dSceneGraphMediaRef.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Scene Graph Media Reference
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

// Include Inventor header files.
#include "Inventor/SoDB.h"
#include "Inventor/SoInput.h"
#include "Inventor/actions/SoCallbackAction.h"
#include "Inventor/nodes/SoSeparator.h"
#include "Inventor/nodes/SoShape.h"


// Include Runtime Player header files
#include "mle/3dscenem.h"

#define MLE_MEDIA_REF_FILENAME_SEPARATOR ","


MLE_MEDIAREF_SOURCE(Mle3dSceneGraphMediaRef,MleMediaRef);


Mle3dSceneGraphMediaRef::Mle3dSceneGraphMediaRef()
{
    nodeList = geometryList = materialList = textureList = NULL;
}


Mle3dSceneGraphMediaRef::~Mle3dSceneGraphMediaRef()
{
    // Do nothing
}


void* Mle3dSceneGraphMediaRef::read()
{
    // Declare local variables
    char *buffer = m_references->buffer;
    void* result = FALSE;

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
	void* result = FALSE;

    if (target)
	{ 
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
           // Load the scene graph from the Inventor file
           int sceneGraphFileCount = 0;
           for (int i = 0; i < fileCount; i++)
               if (strcmp(fileType[i],"sceneGraph")==0)
			   { 
                   SoInput in;
                   if (in.openFile((char*) fileList[i]))
				   {
					   SoSeparator* result = SoDB::readAll(&in);
                       if (!result)
					   {
                           fprintf(stderr,"Mle3dSceneGraphMediaRef::Error--Could not read Inventor file %s\n", getMediaRefBuffer(getNextMediaRef(NULL)));
                           return NULL;
					   }
                       in.closeFile();
     
                       if (nodeCount = result->getNumChildren())
					   {
						   ((SoSeparator*)result)->ref();
                           nodeList = new void*[nodeCount];
                           nameList = new char*[nodeCount];
                           for (int i = 0; i < nodeCount; i++)
						   {
                               nodeList[i] = (void*)result->getChild(i);
                               nameList[i] = strdup(result->getChild(i)->getName().getString());
						   }
                           nodeList[nodeCount] = NULL;
					   }
				   }
                   sceneGraphFileCount++;
			   }
               result=nodeList;
		}
        else
            fprintf(stderr,"Mle3dSceneGraphMediaRef::Error--No filenames found in media ref.\n");
	}
     
    return result;
}
