/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dgeomrc.h
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Geometry Registery property
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
#include <stdio.h>

#include "mle/brendapi.h"
#include "mle/scalarbr.h"
#include "mle/br3drole.h"
#include "mle/MleLoad.h"
#include "mle/transfrm.h"
#include "mle/3dgeomrm.h"
#include "mle/3dgeomrc.h"


br_matrix34** ml3dGeometryRegistryRoleAddGeometry(br_actor* parent,
                                                  unsigned int  geometryRegistrySize,
                                                  br_model**    geometryRegistryList)
// Sets up the role scene graph with geometry groups from the geometry registry.
{ int result = FALSE;
  br_matrix34** transformList = NULL;

  if (parent && geometryRegistryList && geometryRegistrySize)
     {
       br_actor** actorList = NULL;
       unsigned int actorCount = 0,tempCount = 0;
       actorList = mlBrGetModelGroups(geometryRegistryList, geometryRegistrySize, actorCount);
       if (actorCount && actorList)
          { transformList = new br_matrix34*[actorCount];
            for (unsigned int i = 0; i < actorCount; i++)
                if (actorList[i] &&
                    BrActorAdd(parent,actorList[actorCount-i-1]))
                  {
                    // Store transforms in a list for easy access
                    transformList[i]=&(actorList[i]->t.t.mat);
                    tempCount++;
                  }
            if (tempCount == actorCount)
               result = TRUE;
            delete actorList;
          }
     }

  return transformList;
}


void ml3dGeometryRegistryRoleRemoveGeometry(br_actor* parent)
// Removes any exisiting geometry nodes from the scene graph starting at the given parent node.
{
  if (parent)
     {  br_actor* node = parent->children;
        while (node)
              { br_actor* temp = node->next;
                BrActorRemove(node);
                node = temp;
              }
     }
}


void** Mle3dGeometryRegistryCarrier::set(MleRole* role, MediaRef geometryRegistry)
{
  int geometryRegistrySize;
  return (set(role,geometryRegistry,geometryRegistrySize));
}


void** Mle3dGeometryRegistryCarrier::set(MleRole* role, MediaRef geometryRegistry,
                                         int& geometryRegistrySize)
// Reads in a list of geometry from a geometry registry media ref and sets up
// the BRender scene graph with the geometry nodes.
{ int result = FALSE;

  br_matrix34** transformList = NULL;
  geometryRegistrySize = 0;

  br_actor* root=(br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root)
     { if (geometryRegistry != MLE_NO_MEDIA)
          // Load the geometry registry.
          { 
            Mle3dGeometryRegistryMediaRef* geometryMedia =
                  (Mle3dGeometryRegistryMediaRef*)mlLoadMediaRef(geometryRegistry,NULL);
            if (geometryMedia)
               { // Remove the current set of geometry in this node, if any.
                 ml3dGeometryRegistryRoleRemoveGeometry(root); 

                 geometryMedia->read();
                 geometryRegistrySize = geometryMedia->getSize();

                 if (geometryMedia->getList() && geometryRegistrySize)
                    // Set up the scene graph with the models from the geometry registry.
                    transformList=ml3dGeometryRegistryRoleAddGeometry(root, geometryRegistrySize, 
                                                                      geometryMedia->getList());
                 delete geometryMedia;
               }
          }
#ifdef MLE_REHEARSAL
       else
          fprintf(stderr,"%s%s%s",
                    "Mle3dGeometryRegistryCarrier::Error--GeometryRegistry MediaRef property for Actor \"",
                    role->getActor()->getName(),"\" has no value.\n");
#endif

       result=TRUE;
     }

  return (void**)transformList;
}



