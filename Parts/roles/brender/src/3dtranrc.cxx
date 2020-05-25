/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dtranrc.cxx
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

#include "mle/brendapi.h"
#include "mle/scalarbr.h"
#include "mle/br3drole.h"
#include "mle/MleLoad.h"
#include "mle/transfrm.h"
#include "mle/3dtranrc.h"

                                                            
MleTransform** Mle3dTransformRegistryCarrier::get(MleRole* role, unsigned int& actorCount)
// Returns the list of transforms for each actor in the role's scene graph.
{ 
  MleTransform** transformList = NULL;

  br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root)
     {  br_actor* actor = root->children;
        int numActors = 0;
        while (actor)
              { numActors++;
                actor = actor->next;
              }
        if (!transformList)
           transformList = new MleTransform*[numActors];
        actor = root->children;
        actorCount = numActors;
        numActors = 0;
        while (actor)
          { 
            // Store transforms in a list for easy access
            transformList[numActors] = new MleTransform;
            for (int i=0;i<4;i++)
                for (int j=0;j<3;j++)
                    (*transformList[numActors])[i][j] = mlBRScalarToScalar(actor->t.t.mat.m[i][j]);

            actor=actor->next;
            numActors++;
          }
     }

  return transformList;
}



int Mle3dTransformRegistryCarrier::set(MleRole* role, MleTransform** transformList,
                                       unsigned int transformCount)
// Sets the list of transforms for each actor in the role's scene graph.
{ int result = FALSE;

  br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root)
     {  br_actor* actor = root->children;
        unsigned int actorNum = 0;
        while (actor && (actorNum<transformCount))
          { // Set the scene graph transforms from the list.
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 3; j++)
                    actor->t.t.mat.m[i][j] = mlScalarToBRScalar(
                                            (*transformList[actorNum])[i][j]);
            actor = actor->next;
            actorNum++;
          }
        if (actorNum == transformCount)
            result=TRUE;
     }

  return result;
}


int Mle3dTransformRegistryCarrier::set(void** toTransformList, MleTransform** fromTransformList)
// Sets the list of transforms for each actor in the role's scene graph
{ int result = FALSE;

  br_matrix34** brTransformList = (br_matrix34**)toTransformList;

  int node = 0;
  if (toTransformList && fromTransformList)
     while (brTransformList[node] && fromTransformList[node])
        { // Set the scene graph transforms from the list 
          for (int i = 0; i < 4; i++)
              for (int j = 0; j < 3; j++)
                  brTransformList[node]->m[i][j] = mlScalarToBRScalar(
                                              (*fromTransformList[node])[i][j]);
         node++;
       }

  result = node;

  return result;
}
