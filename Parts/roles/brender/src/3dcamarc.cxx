/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dcamarc.cxx
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Camera Aspect Ratio
 * property targeting the BRender platfrom.
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
#include "mle/3dcamarc.h"


int Mle3dCameraAspectRatioCarrier::set(MleRole* role, MlScalar aspectRatio) 
{ int result=FALSE;

  br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root && (root->type= = BR_ACTOR_CAMERA))
     { 
       br_camera* camera = (br_camera*)root->type_data;

       camera->aspect = mlScalarToBRScalar(aspectRatio);
       result=TRUE;
     }

  return result;
}


int Mle3dCameraAspectRatioCarrier::get(MleRole* role, MlScalar& aspectRatio)
{ int result=FALSE;

  br_actor* root=  (br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root && (root->type == BR_ACTOR_CAMERA))
     {
       br_camera* camera = (br_camera*)root->type_data;

       aspectRatio = mlBRScalarToScalar(camera->aspect);
       result=TRUE;
     }

  return result;
}
