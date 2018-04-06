/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dltic.h
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Light Intensity property
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
#include "mle/3dltic.h"


int Mle3dLightIntensityCarrier::set(MleRole* role, MlScalar intensity) 
{ int result = FALSE;

  br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root && (root->type == BR_ACTOR_LIGHT))
     { 
       br_light* light = (br_light*)root->type_data;

       if (intensity == MLE_SCALAR_ZERO) // no illumination
	     light->attenuation_c = mlScalarToBRScalar(MLE_SCALAR(10.0));
       else
	   {  
       	   MlScalar a = mlDiv(MLE_SCALAR(0.1), intensity);
       	   light->attenuation_c = mlScalarToBRScalar(a);
       }
       result = TRUE;   
     }

  return result;
}


int Mle3dLightIntensityCarrier::get(MleRole* role, MlScalar& intensity)
{ int result = FALSE;

  br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root && (root->type == BR_ACTOR_LIGHT))
     {
       br_light* light = (br_light*)root->type_data;

       MlScalar a = mlBRScalarToScalar(light->attenuation_c);
       if ( a >= MLE_SCALAR(10) )
	     intensity = MLE_SCALAR_ZERO;
       else
          intensity = mlDiv(MLE_SCALAR(0.1), a);

       result = TRUE;
     }

  return result;
}
