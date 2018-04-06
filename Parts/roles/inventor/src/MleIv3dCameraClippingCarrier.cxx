/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dCameraClippingCarrier.h
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Camera Clipping property
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

#include "Inventor/actions/SoSearchAction.h"
#include "Inventor/nodes/SoCamera.h"
#include "Inventor/nodes/SoNode.h"
#include "Inventor/nodes/SoSeparator.h"

#include "mle/MleIv3dRole.h"
#include "math/vector.h"
#include "mle/3dcamclc.h"


int Mle3dCameraClippingCarrier::set(MleRole* role, MleVector2 clipping) 
{ int result=FALSE;

  SoSeparator* root = (SoSeparator*)(Mle3dRole::cast(role)->getRoot());
  SoNode* node;

  if (root)
     { node = NULL;
       SoSearchAction search;

       search.setFind(SoSearchAction::TYPE);
       search.setInterest(SoSearchAction::LAST);
       search.setType(SoCamera::getClassTypeId(),TRUE);
       search.apply(root);

       if (search.getPath())
          node=search.getPath()->getTail();
     }

  if (node && node->isOfType(SoCamera::getClassTypeId()))
     { SoCamera* camera = (SoCamera*)node;
       camera->nearDistance.setValue(clipping[0]);
       camera->farDistance.setValue(clipping[1]);
       result = TRUE;
     }

  return result;
}


int Mle3dCameraClippingCarrier::get(MleRole* role, MleVector2& clipping)
{ int result = FALSE;

  SoSeparator* root = (SoSeparator*)(Mle3dRole::cast(role)->getRoot());
  SoNode* node;

  if (root)
     { node = NULL;
       SoSearchAction search;

       search.setFind(SoSearchAction::TYPE);
       search.setInterest(SoSearchAction::LAST);
       search.setType(SoCamera::getClassTypeId(),TRUE);
       search.apply(root);

       if (search.getPath())
          node=search.getPath()->getTail();
     }

  if (node && node->isOfType(SoCamera::getClassTypeId()))
     { SoCamera* camera = (SoCamera*)node;
       clipping[0] = camera->nearDistance.getValue();
       clipping[1] = camera->farDistance.getValue();
       result = TRUE;
     }

  return result;
}

