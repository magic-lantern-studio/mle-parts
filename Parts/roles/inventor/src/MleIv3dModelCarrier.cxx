/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dModelCarrier.cxx
 * @ingroup MleParts
 *
 * This file contains the class implementation for setting a model on a
 * Role targeting the Inventor platform.
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
#include <string.h>
#include <stdio.h>

// Include Inventor header files.
#include <Inventor/SoLists.h>
#include <Inventor/nodes/SoSeparator.h>

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleLoad.h"

// Include Magic Lantern Inventor target header files.
#include "mle/MleIv3dRole.h"
#include "math/transfrm.h"
#include "mle/3dmodelm.h"
#include "mle/3dmodelc.h"


int Mle3dModelCarrier::set(MleRole *role, MlMediaRef model)
{ 
    int result=FALSE;
    
    SoSeparator* root = Mle3dRole::cast(role)->getRoot();
    if (root)
    {	
		// Remove any old scene graph, then
		// remove all kids except initial xform.
		while (root->getNumChildren() > 1)
		    root->removeChild(1);

		// Load the new model.
		if (model != MLE_NO_MEDIA)
		{ 
			Mle3dModelMediaRef* geometryMedia=
			    (Mle3dModelMediaRef*)mlLoadMediaRef(model,NULL);
			
			if (geometryMedia)
			{
				char* name = mlGenRegistryKeyFromMedia(model);
				SoNode* model = (SoNode*)geometryMedia->read();
				if (model)
				{
					// Add the actor to the scene graph.
					root->addChild(model);
    
					result=TRUE;
				}
#ifdef MLE_REHEARSAL
				else
					fprintf(stderr,
					   "MleIv3dModelCarrier::Error -- Could not read model from MediaRef %s.\n",name);
#endif
			}
#ifdef MLE_REHEARSAL
			else
			{
			    fprintf(stderr,
				    "MleIv3dModelCarrier::Error -- Could not read model "
				    "from MediaRef \"%s\" for actor \"%s\".\n",
				    model, role->getActor()->getName());
			}
#endif
		}
#ifdef MLE_REHEARSAL_0
//XXX This was annoying during demos
        else
            fprintf(stderr,
                "MleIv3dModelCarrier::Error -- Model MediaRef is NULL.\n");
#endif

    }

    return result;
}
