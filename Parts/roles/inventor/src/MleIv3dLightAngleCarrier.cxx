/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dLightAngleCarrier.h
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Light Angle property
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

#include <Inventor/nodes/SoSpotLight.h>

#include "mle/mlTypes.h"
#include "mle/MleIv3dRole.h"
#include "mle/3dltac.h"

// This angle [0,1] (0,360d) corresponds to the cutOffangle of light cone.
int Mle3dLightAngleCarrier::set(MleRole* role, MlScalar angle) 
{ 
  int result = FALSE;

  SoSeparator* root = Mle3dRole::cast(role)->getRoot();
  // The first child is always transform. Get the second child.
  SoNode* node = root->getChild(1);

  if (root && node && node->isOfType(SoLight::getClassTypeId()))
     { 
       // Only meaningful when it's spotlight.
       if (node->isOfType(SoSpotLight::getClassTypeId()))
	 { 
	   SoSpotLight* light = (SoSpotLight*)node;
	   light->cutOffAngle = mlScalarToFloat(mlAngleToRadians(angle));
	 }
	 
       result = TRUE;   
     }


  return result;
}


int Mle3dLightAngleCarrier::get(MleRole* role, MlScalar& angle)
{ 
  int result = FALSE;

  SoSeparator* root = Mle3dRole::cast(role)->getRoot();
  // The first child is always transform. Get the second child.
  SoNode* node = root->getChild(1);

  if (root && node && node->isOfType(SoLight::getClassTypeId()))
     { 
       // Only meaningful when it's spotlight.
       if (node->isOfType(SoSpotLight::getClassTypeId()))
	 { 
	   SoSpotLight* light=(SoSpotLight*)node;
	   angle = 
	     mlRadiansToAngle(mlFloatToScalar(light->cutOffAngle.getValue()));
	 }
	 
       result = TRUE;   
     }

  return result;
}
