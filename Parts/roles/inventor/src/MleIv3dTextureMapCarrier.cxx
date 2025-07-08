/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dTextureMapCarrier.cxx
 * @ingroup MleParts
 *
 * This file contains the class implementation for setting a texture
 * map on a  Role targeting the Inventor platform.
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
#include <Inventor/nodes/SoTexture2.h>

// Include Magic Lantern Runtime Enging header files.
#include "mle/MleLoad.h"

// Include Magic Lantern Inventor target header files.
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

