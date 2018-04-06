/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dTextureMapCarrier.cxx
 * @ingroup MleParts
 *
 * This file contains the class implementation for setting a texture
 * map on a  Role targeting the Inventor platform.
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

// Include Inventor header files.
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTexture2.h>

// Include Magic Lantern Runtime Enging header files.
#include "mle/MleLoad.h"

// Include Inventor target header files.
#include "mle/MleIv3dRole.h"
#include "mle/3dtexmpm.h"
#include "mle/3dtexmpc.h"


int Mle3dTextureMapCarrier::set(MleRole *role, MlMediaRef textureMap)
{
	int result = FALSE;

    if (textureMap != MLE_NO_MEDIA)
    {
		char* registryKey = mlGenRegistryKeyFromMedia(textureMap);

        SoTexture2* texture = NULL;
        Mle3dTextureMapMediaRef* mediaRef = 
            (Mle3dTextureMapMediaRef*)mlLoadMediaRef(textureMap,NULL);
        if (mediaRef && (texture = (SoTexture2*)mediaRef->read()))
        { 
			// Do nothing now.
        }

    }

   return result;
}

