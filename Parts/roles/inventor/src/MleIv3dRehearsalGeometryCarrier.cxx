/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dRehearsalGeometryCarrier.cxx
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Rehearsal Geometry property
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

#include <string.h>
#include <stdio.h>

#include "Inventor/nodes/SoSeparator.h"
#include "Inventor/nodes/SoDrawStyle.h"

#include "mle/MleLoad.h"
#include "mle/MleIv3dRole.h"
#include "math/transfrm.h"
#include "mle/3dscenem.h"
#include "mle/3drgeomc.h"
#include "mle/3dscenec.h"


int Mle3dRehearsalGeometryCarrier::set(MleRole* role, int onOff,
    MediaRef geometry)
{ 
    int result = FALSE;
    int nodeIndex;
    
    SoSeparator* root = Mle3dRole::cast(role)->getRoot();
    SoNode* node = root->getChild(1); // Get the second child.
    if (node == NULL)
      {
	fprintf(stderr, "MleIv3dRehearsalGeometryCarrier::Error--No light or camera node is found! \n");
        nodeIndex = 1;
      }
    else nodeIndex = 2;
    if (root)
      {     
 
	    // Load the rehearsal geometry.
        if (geometry != MLE_NO_MEDIA)
        { 
	       Mle3dSceneGraphMediaRef* geometryMedia =
	         (Mle3dSceneGraphMediaRef*)fwLoadMediaRef(geometry,NULL);
            
	  if (! geometry)
	    {
	      fprintf(stderr,
		      "Mle3dRehearsalGeometryCarrier::Error\n");
	      fprintf(stderr, "from rehearsalGeometry media \"%s\".\n",
		      geometryMedia->getMediaRefBuffer(geometryMedia->getNextMediaRef(NULL)));
	    }

	  if (geometryMedia)
	    {
	      geometryMedia->read();

	      SoNode** nodes=(SoNode**)geometryMedia->getNodeList();
	      int i=0;
	      while (nodes[i])
		{ 
		  root->addChild(nodes[i]);
		  i++;
		}
              
	      delete geometryMedia;

	      SoDrawStyle* dnode = new SoDrawStyle;
	      
	      if (onOff != 0) // Rehearsal geometry on.
		dnode->style = SoDrawStyle::FILLED;
	      else
		dnode->style = SoDrawStyle::INVISIBLE;
	      
	      // Insert a SoDrawStyle node as the third child of root.
	      root->insertChild(dnode, nodeIndex); 
	    
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


 
