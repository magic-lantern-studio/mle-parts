/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dmodelc.cxx
 * @ingroup MleParts
 *
 * This file contains the class implementation for setting a model on a
 * Role.
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

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleLoad.h"

// Include Magic Lantern Math header files.
#include "math/transfrm.h"

// Include BRender target header files.
#include "mle/brendapi.h"
#include "math/scalarbr.h"
#include "mle/br3drole.h"
#include "mle/3dmodelm.h"
#include "mle/3dmodelc.h"


int Mle3dModelCarrier::set(MleRole *role, MlMediaRef geometryRegistry)
{
	int result=FALSE;

    br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
    if (root)
    {
        // Remove any existing actors for this node.
        br_actor* oldActor = root->children;
        if (oldActor)
            BrActorRemove(oldActor);

		// Load the geometry registry.
        if (geometryRegistry != MLE_NO_MEDIA)

		{
			Mle3dModelMediaRef* geometryMedia =
                  (Mle3dModelMediaRef*)mlLoadMediaRef(geometryRegistry,NULL);
            
            char* name = mlGenRegistryKeyFromMedia(geometryRegistry);
	        if (geometryMedia)
			{
		         br_model* model=(br_model*)geometryMedia->read();
		         if (model)
				 { 
					 // Set the name of the model
		             model->identifier = strdup(name);
     
		             // Create a new actor to hold the model.
		             br_actor* myActor = BrActorAllocate(BR_ACTOR_MODEL,NULL);
		             myActor->model = BrModelAdd(model);
     
		             // The actor will get the name of the media ref.
		             myActor->identifier = strdup(name);
		             myActor->t.type = BR_TRANSFORM_MATRIX34;

		             // Add the actor to the scene graph.
		             BrActorAdd(root,myActor);
		             result=TRUE;
				 }
#ifdef MLE_REHEARSAL
		         else
		             fprintf(stderr,
			             "Mle3dModelCarrier::Error -- Could not read model from MediaRef \"%s\" for actor \"%s\".\n",
                         name,role->getActor()->getName());
#endif /* MLE_REHEARSAL */
            }
#ifdef MLE_REHEARSAL
	        else
                fprintf(stderr,
                    "Mle3dModelCarrier::Error -- Could not find MediaRef \"%s\" for actor \"%s\".\n",
                    name,role->getActor()->getName());
#endif /* MLE_REHEARSAL */
		}
#ifdef MLE_REHEARSAL
       else
           fprintf(stderr,
               "Mle3dModelCarrier::Error -- Model MediaRef property for actor \"%s\" has no value.\n",
               role->getActor()->getName());
#endif /* MLE_REHEARSAL */
    }

    return result;
}


