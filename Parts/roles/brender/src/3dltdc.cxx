/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dltdc.h
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Light Drop Off Rate property
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
#include "mle/anglebr.h"
#include "mle/br3drole.h"
#include "mle/3dltdc.h"
#include "mle/3dltac.h"


int Mle3dLightDropOffRateCarrier::set(MleRole* role, MlScalar dropOffRate) 
{
  int result = FALSE;

  br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root && (root->type == BR_ACTOR_LIGHT))
     { 
       br_light* light = (br_light*)root->type_data;

       // This property is from Inventor,
       // BRender lights do not have this property.
       if (dropOffRate == ML_SCALAR_ZERO)  
	   // Correspond to constant light cone in Inventor.
	   {
	     MlScalar a;
	     Mle3dLightAngleCarrier::get(role, a);
	     light->cone_inner = mlAngleToBRAngle(mlDiv(a, ML_SCALAR(2.0)));
	   }
         else light->cone_inner = mlAngleToBRAngle(ML_ANGLE_ZERO);
	 
       result = TRUE;   
     }

  return result;
}


int Mle3dLightDropOffRateCarrier::get(MleRole* role, MlScalar& dropOffRate)
{
  int result = FALSE;

  br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root && (root->type == BR_ACTOR_LIGHT))
     {
       // BRender lights do not have this property.
       result = TRUE;
     }

  return result;
}
