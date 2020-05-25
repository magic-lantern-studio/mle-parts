/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dcamtc.h
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Camera Type property
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
#include "mle/br3drole.h"
#include "mle/3dcamtp.h"
#include "mle/3dcamtc.h"


int Mle3dCameraTypeCarrier::set(MleRole* role, int cameraType) 
{ int result = FALSE;

  br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root && (root->type == BR_ACTOR_CAMERA))
     { 
       br_camera* camera = (br_camera*)root->type_data;

       switch (cameraType)
              { case Mle3dCameraTypeProperty::PERSPECTIVE:
                     camera->type = BR_CAMERA_PERSPECTIVE_FOV;
                     break;
                case Mle3dCameraTypeProperty::ORTHOGRAPHIC:
                     camera->type = BR_CAMERA_PARALLEL;
                     break;
                default:
                     camera->type = BR_CAMERA_PERSPECTIVE_FOV;
                     break;
              }

       result=TRUE;
     }

  return result;
}


int Mle3dCameraTypeCarrier::get(MleRole* role, int& cameraType)
{ int result = FALSE;

  br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root && (root->type == BR_ACTOR_CAMERA))
     {
       br_camera* camera = (br_camera*)root->type_data;

       switch (camera->type)
              { case BR_CAMERA_PERSPECTIVE_FOV:
                     cameraType = Mle3dCameraTypeProperty::PERSPECTIVE;
                     break;
                case BR_CAMERA_PARALLEL:
                     cameraType = Mle3dCameraTypeProperty::ORTHOGRAPHIC;
                     break;
              }
       result = TRUE;
     }

  return result;
}
