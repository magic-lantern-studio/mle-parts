/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dcomapc.cxx
 * @ingroup MleParts
 *
 * This file contains the class implementation for setting a color map
 * on a Role.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2000-2025 Wizzer Works
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//  For information concerning this header file, contact Mark S. Millard,
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
#ifdef _WINDOWS
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

#ifdef _WINDOWS
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
#endif /* _WINDOWS material setup */

                BrMaterialAdd(material);
                BrMaterialUpdate(material,BR_MATU_ALL);
                result=TRUE;
            }
    }

    return result;
}
