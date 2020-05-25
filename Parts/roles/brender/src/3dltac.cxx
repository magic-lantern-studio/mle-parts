/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dltac.h
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Light Angle property
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
#include "math/anglebr.h"
#include "mle/br3drole.h"
#include "mle/3dltac.h"

// This angle corresponds to the outer cone angle in br_light.
int Mle3dLightAngleCarrier::set(MleRole* role, MlScalar angle) 
{ int result = FALSE;

  br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root && (root->type == BR_ACTOR_LIGHT))
     { 
       br_light* light = (br_light*)root->type_data;

       // in BRender and MAgic Lantern, angle = 1.0 corresponds to 360d.
       light->cone_outer = mlAngleToBRAngle(mlDiv(angle, ML_SCALAR(2.0)));
	
       result = TRUE;   
     }

  return result;
}


int Mle3dLightAngleCarrier::get(MleRole* role, MlScalar& angle)
{ int result = FALSE;

  br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root && (root->type == BR_ACTOR_LIGHT))
     {
       br_light* light = (br_light*)root->type_data;

       MlScalar a = mlBRAngleToAngle(light->cone_outer);
       angle = mlMul(a, ML_SCALAR(2.0));

       result = TRUE;
     }

  return result;
}
