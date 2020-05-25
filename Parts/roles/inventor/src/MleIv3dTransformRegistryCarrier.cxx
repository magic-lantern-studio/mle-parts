/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dTransformRegistryCarrier.cxx
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Transform Registry property
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

#include "Inventor/nodes/SoSeparator.h"

#include "mle/MleIv3dRole.h"
#include "mle/MleLoad.h"
#include "math/transfrm.h"
#include "mle/3dtranrc.h"

                                                            
MleTransform** Mle3dTransformRegistryCarrier::get(MleRole* role, unsigned int& nodeCount)
// Returns the list of transforms for each actor in the role's scene graph.
{ 
  MleTransform** transformList = NULL;

  SoSeparator* root = (SoSeparator*)(Mle3dRole::cast(role)->getRoot());
  if (root)
     {  SoSeparator* node = NULL;
        int i,j;
        nodeCount = root->getNumChildren();
        if (! transformList)
           transformList = new MleTransform*[nodeCount];
        for (int nodeNum = 1;nodeNum<nodeCount;nodeNum++)
            if (node->getChild(0) && node->getChild(0)->isOfType(SoSeparator::getClassTypeId()))
               { node=(SoSeparator*)root->getChild(nodeNum);
                 SoTransform* transform = NULL;
                 if (node->getChild(0) && node->getChild(0)->isOfType(SoTransform::getClassTypeId()))
                    transform = (SoTransform*)node->getChild(0);
                 if (transform)
                    { // Copy transform data into transform list.
                      transformList[nodeNum] = new MleTransform;
                      SbMatrix matrix,dummy;
                      transform->getRotationSpaceMatrix(matrix,dummy);
                      for (i = 0; i < 3; i++)
                          for (j = 0; j < 3; j++)
                              (*transformList[nodeNum])[i][j] = mlFloatToScalar(matrix[i][j]);
                      transform->getTranslationSpaceMatrix(matrix,dummy);
                          for (j = 0 ; j < 3; j++)
                              (*transformList[nodeNum])[3][j] = mlFloatToScalar(matrix[3][j]);
                    }
               }
     }

  return transformList;
}



int Mle3dTransformRegistryCarrier::set(MleRole* role, MleTransform** transformList,
                                       unsigned int transformCount)
// Sets the list of transforms for each actor in the role's scene graph.
{ int result = FALSE;

  SoSeparator* root = (SoSeparator*)(Mle3dRole::cast(role)->getRoot());
  if (root)
     {  SoSeparator* node=NULL;
        int nodeNum;
        for (nodeNum = 1;nodeNum<transformCount;nodeNum++)
            if ((nodeNum<root->getNumChildren() &&
                root->getChild(nodeNum) && node->getChild(nodeNum)->isOfType(SoSeparator::getClassTypeId())))
               { node = (SoSeparator*)root->getChild(nodeNum);
                 //Set the scene graph transforms from the list 
                 SoTransform* transform=NULL;
                 if (node->getChild(0) && node->getChild(0)->isOfType(SoTransform::getClassTypeId()))
                    { transform = (SoTransform*)node;
                      SbMatrix matrix;
                      matrix.makeIdentity();
                      for (int i=0;i<4;i++)
                          for (int j=0;j<3;j++)
                              matrix[i][j] = mlScalarToFloat((*transformList[nodeNum])[i][j]);
                      transform->setMatrix(matrix);
                    }
               }
        if (nodeNum == transformCount)
            result=TRUE;
     }

  return result;
}

int Mle3dTransformRegistryCarrier::set(void** toTransformList, MleTransform** fromTransformList)
//Sets the list of transforms for each actor in the role's scene graph
{ int result = FALSE;

  SoTransform** ivTransformList = (SoTransform**)toTransformList;
  SbMatrix matrix;

  int node = 0;
  if (ivTransformList && fromTransformList)
     while (ivTransformList[node] && fromTransformList[node])
        { // Set the scene graph transforms from the list 
          
          for (int i = 0; i < 4; i++)
              for (int j = 0; j < 3; j++)
                  matrix[i][j] = mlScalarToFloat((*fromTransformList[node])[i][j]);
         ivTransformList[node]->setMatrix(matrix);
         node++;
       }

  result = node;

  return result;
}
