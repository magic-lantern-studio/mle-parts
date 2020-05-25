/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dcharrc.cxx
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Character Registry property
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

// Include system header files.
#include <stdio.h>

#include "mle/MleLoad.h"
#include "mle/MleRole.h"
#include "mle/3dcharrm.h"
#include "mle/3dcharrc.h"


Mle3dCharacterRegistry* Mle3dCharacterRegistryCarrier::set(MleRole* role,MediaRef characterMediaRef)
{
  Mle3dCharacterRegistry* characterRegistry=NULL;

  if (characterMediaRef != MLE_NO_MEDIA)
     { Mle3dCharacterRegistryMediaRef* characterMedia =
                   (Mle3dCharacterRegistryMediaRef*)mlLoadMediaRef(characterMediaRef,NULL);
       if (characterMedia)
          { characterRegistry=characterMedia->read();
            if (!characterRegistry)
               fprintf(stderr,
                    "Mle3dCharacterRegistryCarrier::Error--could not read character registry from media \"%s\".\n",
                     characterMedia->getMediaRefBuffer(characterMedia->getNextMediaRef(NULL)));
            delete characterMedia; 
          }
     }
#ifdef MLE_REHEARSAL
   else
     fprintf(stderr,"%s%s%s",
               "Mle3dCharacterRegistryCarrier::Error--CharacterRegistry MediaRef property for Actor \"",
               role->getActor()->getName(),"\" has no value.\n");
#endif

  return characterRegistry;
}


