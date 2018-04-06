/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dltsc.cxx
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Light Switch proeprty
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
#include "mle/3dltsc.h"

// Since BrLightEnable() and BrLightDisable() don't
// return any value, the get() function returns no meaningful value.

int Mle3dLightSwitchCarrier::set(MleRole* role, int onOffSwitch) 
{ int result = FALSE;

  br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root && (root->type == BR_ACTOR_LIGHT))
     { 
       if (onOffSwitch != 0) 
	     BrLightEnable(root);
       else 
	     BrLightDisable(root);
	 
       result = TRUE;
     }

  return result;
}


int Mle3dLightSwitchCarrier::get(MleRole* role, int& onOffSwitch)
{ int result = FALSE;

  br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root && (root->type == BR_ACTOR_LIGHT))
     { 
       result = TRUE;
     }

  return result;
}
