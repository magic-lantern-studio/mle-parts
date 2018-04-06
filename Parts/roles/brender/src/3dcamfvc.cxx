/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dcamfvc.cxx
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Camera Field of View property
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
#include "mle/3dcamfvc.h"


int Mle3dCameraFieldOfViewCarrier::set(MleRole* role, MlScalar fieldOfView) 
{ int result = FALSE;

  br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root && (root->type == BR_ACTOR_CAMERA))
     { 
       br_camera* camera = (br_camera*)root->type_data;

       camera->field_of_view = BrDegreeToAngle(mlScalarToBRScalar(fieldOfView));

       result = TRUE;
     }

  return result;
}


int Mle3dCameraFieldOfViewCarrier::get(MleRole* role, MlScalar& fieldOfView)
{ int result = FALSE;

  br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root && (root->type == BR_ACTOR_CAMERA))
     {
       br_camera* camera = (br_camera*)root->type_data;

       fieldOfView = mlBRScalarToScalar(BrAngleToDegree(camera->field_of_view));
       result = TRUE;
     }

  return result;
}

