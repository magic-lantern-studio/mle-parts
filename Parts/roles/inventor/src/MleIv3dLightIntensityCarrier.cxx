/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dLightIntensityCarrier.h
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Light Intensity property
 * targeting the Inventor platform.
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

#include "mle/mlTypes.h"
#include "mle/MleIv3dRole.h"
#include "mle/3dltic.h"

int Mle3dLightIntensityCarrier::set(MleRole* role, MlScalar intensity) 
{ 
  int result = FALSE;

  SoSeparator* root = Mle3dRole::cast(role)->getRoot();
  // First child is always transform, so get the second child.
  SoNode* node = root->getChild(1);

  if (root && node && node->isOfType(SoLight::getClassTypeId()))
     { 
       SoLight* light = (SoLight*)node;
       light->intensity = mlScalarToFloat(intensity);
	 
       result = TRUE;
     }

  return result;
}


int Mle3dLightIntensityCarrier::get(MleRole* role, MlScalar& intensity)
{ 
  int result = FALSE;
  
  SoSeparator* root = Mle3dRole::cast(role)->getRoot();
  // First child is always transform, so get the second child.
  SoNode* node = root->getChild(1);

  if (root && node && node->isOfType(SoLight::getClassTypeId()))
     { 
       SoLight* light = (SoLight*)node;
       intensity = mlFloatToScalar(light->intensity.getValue());
	 
       result = TRUE;
     }
   

  return result;
}
