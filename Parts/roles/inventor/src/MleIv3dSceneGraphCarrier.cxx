/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dSceneGraphCarrier.cxx
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Scene Graph property
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

#include "Inventor/Sb.h"
#include "Inventor/nodes/SoSeparator.h"
#include "Inventor/nodes/SoSeparator.h"

#include "mle/MleIv3dRole.h"
#include "mle/MleLoad.h"
#include "mle/3dscenem.h"
#include "mle/3dscenec.h"

#ifdef MLE_REHEARSAL
#include <stdio.h>
#endif


int Mle3dSceneGraphCarrier::set(MleRole* role, MediaRef sceneGraph,
                                char**& nameList, void**& nodeList,
                                void**& geometryList, void**& materialList, 
                                void**& textureList, void**& transformList)
{ int result = FALSE;

  SoSeparator* root = (SoSeparator*)(Mle3dRole::cast(role)->getRoot());
  if (root)
     { 
       if (sceneGraph != MLE_NO_MEDIA)
          {  
            Mle3dSceneGraphMediaRef* sceneGraphMedia =
                  (Mle3dSceneGraphMediaRef*)mlLoadMediaRef(sceneGraph,NULL);
            if (sceneGraphMedia)
               { 
                 sceneGraphMedia->read();

                 nameList = sceneGraphMedia->getNameList();
                 nodeList = sceneGraphMedia->getNodeList();
                 geometryList = sceneGraphMedia->getGeometryList();
                 materialList = sceneGraphMedia->getMaterialList();
                 textureList = sceneGraphMedia->getTextureList();

                 if (nodeList)
                    // Can only attach the geometry, materials, and textures if there are valid nodes
                    { // Count the nodes in the node list
                      int nodeCount=0;
                      while (nodeList[nodeCount]) nodeCount++;

                      // Remove any existing nodes for this role
                      if (nodeCount)
                         { if (root->getNumChildren()>3)
                              for (int i = root->getNumChildren()-1; i > 3; i--)
                                  root->removeChild(i);
                               
                         }
                  
                      // Add the nodes in the node list into the scene graph and
                      // store the list of transformation matrices for the nodes
                      transformList = (void**)new SbMatrix*[nodeCount+1];
                      for (int i = 0; i < nodeCount; i++)
                          { /* XXX Only add a transform when one doesn't exist
                            SoSeparator* separator = new SoSeparator;
                            separator->ref();
                            SoSeparator* node = (SoSeparator*)nodeList[i];
                            if (node) 
                               { SoTransform* transform = new SoTransform;
                                 transformList[i]=&(transform);
                                 separator->addChild(transform);
                                 separator->addChild(node);
                               }
                            root->addChild(separator);
                            */
                            root->addChild((SoNode*)nodeList[i]);
                            SoSeparator* node = (SoSeparator*)nodeList[i];
                            if (node->getChildren())
                               { if (node->getChild(0)->isOfType(SoTransform::getClassTypeId()))
                                    transformList[i] = (SoTransform*)node->getChild(0);
                                 else
                                    transformList[i] = NULL;
                               }
                            
                          }
                      transformList[nodeCount] = NULL;
                      result = nodeCount;
                 delete sceneGraphMedia;
                    }
                  else
                    fprintf(stderr,"%s%s%s",
                            "MleIv3dSceneGraphCarrier::Error--SceneGraph MediaRef property for Actor \"",
                            role->getActor()->getName(),"\" has no file list.\n");
     
               }
          }
#ifdef MLE_REHEARSAL
        else
          fprintf(stderr,"%s%s%s",
                    "MleIv3dSceneGraphCarrier::Error--SceneGraph MediaRef property for Actor \"",
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


