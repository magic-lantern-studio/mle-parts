/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dModelCarrier.cxx
 * @ingroup MleParts
 *
 * This file contains the class implementation for setting a model on a
 * Role targeting the Inventor platform.
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
#include <string.h>
#include <stdio.h>

// Include Inventor header files.
#include <Inventor/SoLists.h>
#include <Inventor/nodes/SoSeparator.h>

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleLoad.h"

// Include Inventor target header files.
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
