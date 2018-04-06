/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3drgeomc.cxx
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Rehearsal Geometry property
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

#include <string.h>
#include <stdio.h>

#include "mle/brendapi.h"
#include "mle/scalarbr.h"
#include "mle/br3drole.h"
#include "mle/mlLoad.h"
#include "math/transfrm.h"
#include "mle/3dscenem.h"
#include "mle/3drgeomc.h"
#include "mle/3dscenec.h"

int Mle3dRehearsalGeometryCarrier::set
	(MleRole* role, int onOff, MediaRef geometry)
{ 
    int result = FALSE;

    br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
    if (root)
    { 
       if (geometry != MLE_NO_MEDIA)
	   { 
	     //Load the rehearsal geometry registry
	     Mle3dSceneGraphMediaRef* geometryMedia =
	       (Mle3dSceneGraphMediaRef*)mlLoadMediaRef(geometry,NULL);          
	     if (!geometry)
	     {
	       fprintf(stderr,
		       "Mle3dRehearsalGeometryCarrier::Error\n");
	       fprintf(stderr, "from rehearsalGeometry media \"%s\".\n",
		       geometryMedia->getMediaRefBuffer(geometryMedia->getNextMediaRef(NULL)));
	     }
	       
	     if (geometryMedia)
	     {
	       geometryMedia->read();

	       //Add the geometry actor nodes to the scene graph
	       br_actor** nodes = (br_actor**)geometryMedia->getNodeList();
	       int i=0;
	       while (nodes[i])
		   { 
		     BrActorAdd(root,nodes[i]);
		     i++;
		   }
              
	       delete geometryMedia;

	       if (onOff != 0) // rehearsal geometry on
		     root->render_style = BR_RSTYLE_FACES;
	       else
		     root->render_style = BR_RSTYLE_NONE;
		 
	       result = TRUE;
		 }
	   }
#ifdef MLE_REHEARSAL
       else
	     fprintf(stderr,
		   "Mle3dRehearsalGeometryCarrier::\
		   Error--RehearsalGeometry MediaRef property for actor \"%s\" has no value.\n",
		   role->getActor()->getName());
#endif

	}

    return result;
}

