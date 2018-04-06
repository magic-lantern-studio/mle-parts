/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dGeometryRegistryCarrier.h
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Geometry Registery property
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
#include <string.h>
#include <stdio.h>

#include "Inventor/SoLists.h"
#include "Inventor/nodes/SoSeparator.h"

#include "mle/MleLoad.h"
#include "mle/MleIv3dRole.h"
//#include "math/transfrm.h"
#include "mle/3dmodelm.h"
#include "mle/3dgeomrc.h"


void** Mle3dGeometryRegistryCarrier::set(MleRole* role, MediaRef geometryRegistry)
{ 
    SoSeparator* root = Mle3dRole::cast(role)->getRoot();
    if (root)
    {	
	// Remove any old scene graph --
	//   remove all kids except initial xform.
	while (root->getNumChildren() > 1)
	      root->removeChild(1);

    // Load the new geometryRegistry.
	if (geometryRegistry != MLE_NO_MEDIA)
        { 
	    Mle3dModelMediaRef* geometryMedia =
                (Mle3dModelMediaRef*)mlLoadMediaRef(geometryRegistry,NULL);
	    
	    if (geometryMedia)
	    {
		char* name = mlGenRegistryKeyFromMedia(geometryRegistry);
		SoNode* geometryRegistry = (SoNode*)geometryMedia->read();
		if (geometryRegistry)
		{
		    // Add the actor to the scene graph
		    root->addChild(geometryRegistry);
		}
#ifdef MLE_REHEARSAL
		else
		    fprintf(stderr,
			   "MleIv3dGeometryRegistryCarrier::Error--Could not read geometryRegistry from MediaRef %s.\n",name);
#endif
	    }
#ifdef MLE_REHEARSAL
	    else
	    {
	       fprintf(stderr,
		    "MleIv3dGeometryRegistryCarrier::Error--Could not read geometryRegistry "
		    "from MediaRef \"%s\" for actor \"%s\".\n",
		    geometryRegistry, role->getActor()->getName());
	    }
#endif
	}

    }

    // This is a hack for now.
    // Really it should return a list of pointers to all of the SoShape-based nodes.
    return (void**)root;
}
