/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dColorMapCarrier.cxx
 * @ingroup MleParts
 *
 * This file contains the class implementaation for setting a color map
 * on a Role targeting the Inventor platform.
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

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleLoad.h"

// Include Inventor target header files.
#include "mle/MleIv3dRole.h"
#include "mle/3dcomapm.h"
#include "mle/3dcomapc.h"


int Mle3dColorMapCarrier::set(MleRole *role, MlMediaRef colorMapMedia)
{
	int result = FALSE;

    if (colorMapMedia != MLE_NO_MEDIA)
    {
	    char *registryKey = mlGenRegistryKeyFromMedia(colorMapMedia);

        void *colorMap = NULL;
        {
			Mle3dColorMapMediaRef *mediaRef = 
              (Mle3dColorMapMediaRef *)mlLoadMediaRef(colorMapMedia,NULL);

            if (mediaRef && (colorMap = mediaRef->read()))
            {
				// Do nothing for now.
            }
        }
            
        if (! colorMap)
        {
			// Do nothing.
            result = TRUE;
        }
        else
            fprintf(stderr,"MleIv3dColorMapCarrier::Warning -- Could not load color map media\n");
    }

    return result;
}

