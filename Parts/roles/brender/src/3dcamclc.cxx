/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dcamclc.h
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Camera Clipping property
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
#include "math/vector.h"
#include "mle/3dcamclc.h"


int Mle3dCameraClippingCarrier::set(MleRole* role, MleVector2 clipping) 
{ int result=FALSE;

  br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root && (root->type == BR_ACTOR_CAMERA))
     { 
       br_camera* camera = (br_camera*)root->type_data;

       MlScalar nearPlane = clipping[0];
       MlScalar farPlane = clipping[1];

       camera->hither_z = mlScalarToBRScalar(
                mlMulDiv(farPlane,nearPlane,farPlane + farPlane - nearPlane));
       camera->yon_z = mlScalarToBRScalar(farPlane);
       result = TRUE;
     }

  return result;
}


int Mle3dCameraClippingCarrier::get(MleRole* role, MleVector2& clipping)
{ int result=FALSE;

  br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root && (root->type == BR_ACTOR_CAMERA))
     {
       br_camera* camera = (br_camera*)root->type_data;

       MlScalar nearPlane = mlBRScalarToScalar(camera->hither_z);
       MlScalar farPlane = mlBRScalarToScalar(camera->yon_z);

       clipping[0] = mlMulDiv(nearPlane + nearPlane,farPlane,farPlane + nearPlane);
       clipping[1] = farPlane;
       result = TRUE;
     }

  return result;
}

