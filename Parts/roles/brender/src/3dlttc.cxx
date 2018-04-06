/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dlttc.cxx
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Light Type property
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
#include "mle/scalarbr.h"
#include "mle/3dlttp.h"
#include "mle/3dlttc.h"


int Mle3dLightTypeCarrier::set(MleRole* role, int lightType) 
{
  int result = FALSE;

  br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root && (root->type == BR_ACTOR_LIGHT))
     { 
       br_light* light = (br_light*)root->type_data;

       light->attenuation_c = mlScalarToBRScalar(FW_SCALAR_ONE);
       light->attenuation_l = mlScalarToBRScalar(FW_SCALAR_ZERO);
       light->attenuation_q = mlScalarToBRScalar(FW_SCALAR_ZERO);
       light->colour = BR_COLOUR_RGB(255,255,255);
       switch (lightType)
	   {
	     case Mle3dLightTypeProperty::DIRECTIONAL:
	       light->type = BR_LIGHT_DIRECT;
	       break;
	     case Mle3dLightTypeProperty::POINT:
	       light->type = BR_LIGHT_POINT;
	       break;
	     case Mle3dLightTypeProperty::SPOT:
	       light->type = BR_LIGHT_SPOT;
	       break;
	     default:
	       light->type = BR_LIGHT_DIRECT;
	       break;
	   }

       result = TRUE;
     }

  return result;
}


int Mle3dLightTypeCarrier::get(MleRole* role, int& lightType)
{
  int result = FALSE;

  br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root && (root->type == BR_ACTOR_LIGHT))
     {
       br_light* light = (br_light*)root->type_data;

       switch (light->type)
	   {
	     case BR_LIGHT_DIRECT:
	       lightType = Mle3dLightTypeProperty::DIRECTIONAL;
	       break;
	     case BR_LIGHT_POINT:
	       lightType = Mle3dLightTypeProperty::POINT;
	       break;
	     case BR_LIGHT_SPOT:
	       lightType = Mle3dLightTypeProperty::SPOT;
	       break;	
	   }
       result=TRUE;
     }

  return result;
}
