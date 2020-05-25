/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dmatlrc.cxx
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Material Registry property
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
#include "mle/mlLoad.h"
#include "mle/3dmatlrm.h"
#include "mle/3dmatlrc.h"


int Mle3dMaterialRegistryCarrier::set(MleRole* role, MediaRef materialRegistry)
{
  int result = FALSE;

  br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
  if (root)
     { 
       if (materialRegistry! = MLE_NO_MEDIA)
          { Mle3dMaterialRegistryMediaRef* materialMedia=
                  (Mle3dMaterialRegistryMediaRef*)mlLoadMediaRef(materialRegistry,NULL);
            if (materialMedia)
               { if (!materialMedia->read())
                    fprintf(stderr,
                       "Mle3dMaterialRegistryCarrier::Error--could not read material registry from media \"%s\".\n",
                        materialMedia->getMediaRefBuffer(materialMedia->getNextMediaRef(NULL)));

                 //XXX Need to get the material list and apply it to the geometry in the
                 //    case where the geometry has already been read in for this root node
                 delete materialMedia;
               }
          }
#ifdef MLE_REHEARSAL
        else
          fprintf(stderr,"%s%s%s",
                    "Mle3dMaterialRegistryCarrier::Error--MaterialRegistry MediaRef property for Actor \"",
                    role->getActor()->getName(),"\" has no value.\n");
#endif
       result=TRUE;
     }

  return result;
}

