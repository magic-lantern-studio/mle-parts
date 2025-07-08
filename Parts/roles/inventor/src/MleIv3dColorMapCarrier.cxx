/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dColorMapCarrier.cxx
 * @ingroup MleParts
 *
 * This file contains the class implementaation for setting a color map
 * on a Role targeting the Inventor platform.
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

// Include Inventor header files.
#include <Inventor/nodes/SoSeparator.h>

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleLoad.h"

// Include Magic Lantern header files.
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

