/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dcomapc.cxx
 * @ingroup MleParts
 *
 * This file contains the class implementation for setting a color map
 * on a Role.
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
#include "mle/3dcomapm.h"
#include "mle/3dcomapc.h"
#ifdef WIN32
#include "mle/pcpal.h" // For MLE_PCPAL_BASE_GRAY.
#endif

int Mle3dColorMapCarrier::set(MleRole *role, MlMediaRef colorMap)
{ 
	int result=FALSE;

    if (colorMap != MLE_NO_MEDIA)
    {
		char* registryKey = mlGenRegistryKeyFromMedia(colorMap);

        br_pixelmap* shadeTable = NULL;
        if (registryKey)
            shadeTable = BrMapFind(registryKey);
        if (shadeTable == NULL)
        { 
			 Mle3dColorMapMediaRef *mediaRef = 
                 (Mle3dColorMapMediaRef *)mlLoadMediaRef(colorMap,NULL);

             if (mediaRef && (shadeTable = (br_pixelmap*)mediaRef->read()))
             { 
				 // Add this to the BRender registry.
                 shadeTable->identifier = registryKey;
                 BrTableAdd(shadeTable);
            }
        }
            
        if (shadeTable)
            for (br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
                 root!=NULL; root = root->next)
			{ 
                br_material* material = root->material;
                if (!material)
                {
                    material = BrMaterialAllocate(registryKey);
                    root->material = material;
                }
                material->index_shade = shadeTable;

#ifdef WIN32
                material->colour = BR_COLOUR_RGB(255,255,255);   /* colour */
                material->opacity = 255;                         /* opacity */
                material->ka = BR_UFRACTION(0.10);               /* ka */
                material->kd = BR_UFRACTION(0.70);               /* kd */
                material->ks = BR_UFRACTION(0.0);                /* ks */
                material->power =  BR_SCALAR(20);                           /* power */
                material->flags =  BR_MATF_LIGHT|BR_MATF_SMOOTH; /* flags */
                material->map_transform.m[0][0] = BR_SCALAR(1);
                material->map_transform.m[0][1] = BR_SCALAR(0);
                material->map_transform.m[1][0] = BR_SCALAR(0);
                material->map_transform.m[1][1] = BR_SCALAR(1);
                material->map_transform.m[2][0] = BR_SCALAR(0);
                material->map_transform.m[2][1] = BR_SCALAR(0);
			    // We don't have any knowledge here about the organization
			    // of the color palette. The best guess is to set the 
			    // index base and range to the first ramp in the default
			    // palette, which is a grayscale, since the default RGB 
			    // material color is white.
                material->index_base = MLE_PCPAL_BASE_GRAY;
                material->index_range = 31;
#else /* SGI */
                material->colour = BR_COLOUR_RGB(255,255,255);      // white
                material->ka = BR_UFRACTION(0.50);
                material->kd = BR_UFRACTION(0.20);
                material->ks = BR_UFRACTION(0.30);
                material->power = BR_SCALAR(20);
                material->flags = BR_MATF_SMOOTH;
                material->flags |= BR_MATF_LIGHT;
                material->index_base = 0;
                material->index_range = 0;
                material->index_shade = NULL;
#endif /* WIN32 material setup */

                BrMaterialAdd(material);
                BrMaterialUpdate(material,BR_MATU_ALL);
                result=TRUE;
			}
    }

    return result;
}
