/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dCameraFieldOfViewCarrier.cxx
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Camera Field of View property
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
#include "Inventor/nodes/SoOrthographicCamera.h"
#include "Inventor/nodes/SoPerspectiveCamera.h"
#include "Inventor/nodes/SoNode.h"
#include "Inventor/nodes/SoSeparator.h"

#include "mle/MleIv3dRole.h"
#include "mle/3dcamfvc.h"


int Mle3dCameraFieldOfViewCarrier::set(MleRole* role, MlScalar fieldOfView) 
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
          node = search.getPath()->getTail();
     }

  if (node)
     { if (node->isOfType(SoPerspectiveCamera::getClassTypeId()))
          { SoPerspectiveCamera* camera = (SoPerspectiveCamera*)node;
            camera->heightAngle.setValue(fieldOfView);
            result = TRUE;
          }
       else
       if (node->isOfType(SoOrthographicCamera::getClassTypeId()))
          { SoOrthographicCamera* camera = (SoOrthographicCamera*)node;
            camera->height.setValue(fieldOfView);
            result = TRUE;
          }
     }

  return result;
}



int Mle3dCameraFieldOfViewCarrier::get(MleRole* role, MlScalar& fieldOfView)
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


  if (node)
     { if (node->isOfType(SoPerspectiveCamera::getClassTypeId()))
          { SoPerspectiveCamera* camera = (SoPerspectiveCamera*)node;
            fieldOfView = camera->heightAngle.getValue();
            result = TRUE;
          }
       else
       if (node->isOfType(SoOrthographicCamera::getClassTypeId()))
          { SoOrthographicCamera* camera = (SoOrthographicCamera*)node;
            fieldOfView = camera->height.getValue();
            result = TRUE;
          }
     }

  return result;
}

