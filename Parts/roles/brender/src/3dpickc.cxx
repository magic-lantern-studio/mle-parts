/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dpickc.cxx
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Pickable property
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

#include <stdio.h>

#include "mle/brendapi.h"
#include "mle/br3drole.h"
#include "mle/3dpickc.h"


int Mle3dPickableCarrier::set(MleRole* role, int pickable)
{ int result = FALSE;

#ifdef MLE_REHEARSAL
  br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root)
     { 
       // Register node for picking.
       if (pickable)
          MleRole::pickRegistry.set(root,role);
       else
          MleRole::pickRegistry.set(root,NULL);

       result = TRUE;
     }
#endif

  return result;
}

