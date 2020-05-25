/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dLightTypeCarrier.cxx
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Light Type property
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
#include <Inventor/nodes/SoPointLight.h>

 
#include "mle/mlTypes.h"
#include "mle/MleIv3dRole.h"
#include "mle/3dlttp.h"
#include "mle/3dlttc.h"


int Mle3dLightTypeCarrier::set(MleRole* role, int lightType) 
{ int result = FALSE;

  SoSeparator* root = (Mle3dRole::cast(role)->getRoot());
  // Since the first child is always TRANSFORM, get the second child.
  SoNode* node = root->getChild(1);

  if (root && node && node->isOfType(SoLight::getClassTypeId()))
     { 
       switch (lightType)
	 {
	 case Mle3dLightTypeProperty::DIRECTIONAL: // Default light node.
	   SoDirectionalLight* mydirlight = (SoDirectionalLight*)node;
	   mydirlight->direction.setValue(0,0,0);
	   break;
	 case Mle3dLightTypeProperty::POINT:
	   root->removeChild(node);
	   SoPointLight* mypointlight = new SoPointLight;
	   mypointlight->location.setValue(0,0,0);
	   root->addChild(mypointlight);
	   break;
	 case Mle3dLightTypeProperty::SPOT:
	   root->removeChild(node);
	   SoSpotLight* myspotlight = new SoSpotLight;
	   myspotlight->location.setValue(0,0,0);
	   myspotlight->direction.setValue(0,0,0);
	   root->addChild(new SoSpotLight);
	   break;
	 }

       SoLight *light = (SoLight*) node;
       light->color.setValue(1, 1, 1);
       light->intensity = mlScalarToFloat(FW_SCALAR(1.0));
       light->on = TRUE;
       
       result = TRUE;
     }

  return result;
}


int Mle3dLightTypeCarrier::get(MleRole* role, int& lightType)
{ int result = FALSE;

  SoSeparator* roo = Mle3dRole::cast(role)->getRoot();
  // Since the first child is always TRANSFORM, get the second child.
  SoNode* node = root->getChild(1);

  if (root && node && node->isOfType(SoLight::getClassTypeId()))
     {
       SoLight* light = (SoLight*)node;

       if (light->isOfType(SoDirectionalLight::getClassTypeId()))
	   lightType = Mle3dLightTypeProperty::DIRECTIONAL;
       else if (light->isOfType(SoPointLight::getClassTypeId()))
	   lightType = Mle3dLightTypeProperty::POINT;
       else if (light->isOfType(SoSpotLight::getClassTypeId()))
	   lightType = Mle3dLightTypeProperty::SPOT;

       result = TRUE;
     }

  return result;
}
