/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dscenec.cxx
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Scene Graph property
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

#include <stdio.h>

#include "mle/brendapi.h"
#include "mle/br3dd.h"
#include "mle/mlLoad.h"
#include "mle/3dscenem.h"
#include "mle/3dscenec.h"



int Mle3dSceneGraphCarrier::set(MleRole* role, MediaRef sceneGraph,
                                char**& nameList, void**& nodeList,
                                void**& geometryList, void**& materialList, 
                                void**& textureList, void**& transformList)
{ int result = FALSE;

  br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root)
     { 
       if (sceneGraph != MLE_NO_MEDIA)
          { Mle3dSceneGraphMediaRef* sceneGraphMedia =
                  (Mle3dSceneGraphMediaRef*)mlLoadMediaRef(sceneGraph,NULL);
            if (sceneGraphMedia)
               { 

                 if (!sceneGraphMedia->read())
                    fprintf(stderr,
                         "Mle3dSceneGraphCarrier::Error--could not read scene graph from media \"%s\".\n",
                          sceneGraphMedia->getMediaRefBuffer(sceneGraphMedia->getNextMediaRef(NULL)));
                   
                 nameList = sceneGraphMedia->getNameList();
                 nodeList = sceneGraphMedia->getNodeList();
                 geometryList = sceneGraphMedia->getGeometryList();
                 materialList = sceneGraphMedia->getMaterialList();
                 textureList = sceneGraphMedia->getTextureList();

                 if (nodeList)
                    // Must have valid node list to attach geometry, materials, and textures
                    { 
                      // Count the nodes in the node list
                      int nodeCount = 0;
                      while (nodeList[nodeCount]) nodeCount++;

                      // Remove any scene graph nodes already in tree
                      if (nodeCount)
                         { br_actor* node = root->children;
                           while (node)
                                 { br_actor* temp = node->next;
                                   BrActorRemove(node);
                                   node = temp;
                                 }
                         }
             
                      // Add the nodes in the node list into the scene graph and
                      // store the list of transformation matrices for the nodes
                      transformList = (void**)new br_matrix34*[nodeCount+1];
                      for (int i = 0; i < nodeCount; i++)
                          { // Nodes must be added in reverse order to preserve hierarchy
                            BrActorAdd(root,(br_actor*)nodeList[nodeCount-i-1]);
                            if (nodeList[i])
                               transformList[i] = &(((br_actor*)nodeList[i])->t.t.mat);
                          }
                      transformList[nodeCount] = NULL;
                      result=nodeCount;
                    }

                 delete sceneGraphMedia;
               }
          }
#ifdef MLE_REHEARSAL
        else
          fprintf(stderr,"%s%s%s",
                    "Mle3dSceneGraphCarrier::Error--SceneGraph MediaRef property for Actor \"",
                    role->getActor()->getName(),"\" has no value.\n");
#endif
     }

  return result;
}



int Mle3dSceneGraphCarrier::set(MleRole* role, MediaRef sceneGraph)
{
  char** name = NULL;
  void** dummy = NULL;
  return Mle3dSceneGraphCarrier::set(role,sceneGraph,name,dummy,dummy,dummy,dummy,dummy);
}


