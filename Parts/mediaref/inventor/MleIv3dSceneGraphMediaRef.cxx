/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dSceneGraphMediaRef.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Scene Graph Media Reference
 * targeting the Inventor platform.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2000-2025 Wizzer Works
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//  For information concerning this header file, contact Mark S. Millard,
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

#ifdef _WINDOWS
#include <io.h>
#include <windows.h>
#else /* ! _WINDOWS */
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
