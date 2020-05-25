/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dAnimationRegistryCarrier.cxx
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Animation Registry
 * property targeting the Inventor platform.
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

#include "mle/MleLoad.h"
#include "mle/MleRole.h"
#include "mle/3danimrm.h"
#include "mle/3danimrc.h"

#ifdef MLE_REHEARSAL
#include <stdio.h>
#endif

Mle3dAnimationRegistry* Mle3dAnimationRegistryCarrier::set(MleRole* role,MediaRef animationMediaRef)
{
  Mle3dAnimationRegistry* animationRegistry = NULL;

  if (animationMediaRef != ML_NO_MEDIA)
  {
	   Mle3dAnimationRegistryMediaRef* animationMedia =
           (Mle3dAnimationRegistryMediaRef*)mlLoadMediaRef(animationMediaRef,NULL);
       if (animationMedia)
       {
		    animationRegistry = animationMedia->read();
            if (! animationRegistry)
               fprintf(stderr,
                    "MleIv3dAnimationRegistryCarrier::Error--could not read animation registry from media \"%s\".\n",
                     animationMedia->getMediaRefBuffer(animationMedia->getNextMediaRef(NULL)));
            delete animationMedia; 
       }
  }
#ifdef MLE_REHEARSAL
  else
     fprintf(stderr,"%s%s%s",
             "MleIv3dAnimationRegistryCarrier::Error--AnimationRegistry MediaRef property for Actor \"",
              role->getActor()->getName(),"\" has no value.\n");
#endif

  return animationRegistry;
}
