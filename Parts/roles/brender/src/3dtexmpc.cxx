/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dtexmpc.cxx
 * @ingroup MleParts
 *
 * This file contains the class implementation for setting a texture map
 * on a  Role.
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

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleLoad.h"

// Include BRender target header files.
#include "mle/brendapi.h"
#include "mle/br3drole.h"
#include "mle/3dtexmpm.h"
#include "mle/3dtexmpc.h"


int Mle3dTextureMapCarrier::set(MleRole *role, MlMediaRef textureMap)
{ 
	int result = FALSE;

    if (textureMap != MLE_NO_MEDIA)
    {
		char* registryKey = mlGenRegistryKeyFromMedia(textureMap);

         br_pixelmap *texture = BrMapFind(registryKey);
         if (! texture)
         {
            Mle3dTextureMapMediaRef* mediaRef = 
                 (Mle3dTextureMapMediaRef*)mlLoadMediaRef(textureMap,NULL);
            if (mediaRef && (texture = (br_pixelmap*)mediaRef->read()))
            {
				// Add this to the BRender registry.
                texture->identifier = registryKey;
                BrMapAdd(texture);
            }
         }

         if (texture)
         { 
             // Get the material for the node.
            br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
            br_material* material = root->material;

            // Create material if one doesn't exist for the root.
            if (! material)
            { 
				material = BrMaterialAllocate(registryKey);
                root->material = material;
            }

            // Assign texture to the material.
            material->colour_map = texture;

            // These two flags are needed for textures to be rendered.
            material->flags = BR_MATF_SMOOTH | BR_MATF_LIGHT;

            BrMaterialAdd(material);
            BrMaterialUpdate(material,BR_MATU_ALL);

         }
     }

     return result;
}

