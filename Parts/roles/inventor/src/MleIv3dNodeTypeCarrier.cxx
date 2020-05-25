/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dNodeTypeCarrier.h
 * @ingroup MleParts
 *
 * This file contains the class implementation for setting a node
 * type on a Role.
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

// Include system header files.
#include <stdio.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>

#include "mle/mlTypes.h"
#include "mle/MleIv3dRole.h"
#include "mle/3dnodetp.h"
#include "mle/3dnodetc.h"


int Mle3dNodeTypeCarrier::set(MleRole* role, int nodeType)
{ 

  SoSeparator *root = Mle3dRole::cast(role)->getRoot();
  if (!root)
    return FALSE;

  switch (nodeType)
    { 
	case Mle3dNodeTypeProperty::CAMERA:
	    SoPerspectiveCamera *cam = new SoPerspectiveCamera;
	    root->addChild(cam);
	    break;
	case Mle3dNodeTypeProperty::LIGHT:
	    SoDirectionalLight *light = new SoDirectionalLight;
	    root->addChild(light);
	    break;
	case Mle3dNodeTypeProperty::TRANSFORM:
	case Mle3dNodeTypeProperty::GEOMETRY:
	case Mle3dNodeTypeProperty::BOUNDING_BOX:
	case Mle3dNodeTypeProperty::CLIPPING_PLANE:
	default:
	    break;
    }
      
  return TRUE;    
}


int Mle3dNodeTypeCarrier::get(MleRole* role, int& nodeType)
{
    SoSeparator *root = Mle3dRole::cast(role)->getRoot();
    if (!root)
	return FALSE;
	
    // Always have transform as first node under root, so
    // get second node.
    SoNode *node = root->getChild(1);

    if (! node)
	 nodeType = Mle3dNodeTypeProperty::TRANSFORM;
    else if (node->isOfType(SoCamera::getClassTypeId()))
	 nodeType = Mle3dNodeTypeProperty::CAMERA;
    else if (node->isOfType(SoLight::getClassTypeId()))
	 nodeType = Mle3dNodeTypeProperty::LIGHT;
    else
	 nodeType = Mle3dNodeTypeProperty::GEOMETRY;

    return TRUE;
}
