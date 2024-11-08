/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dcomapc.h
 * @ingroup MleParts
 *
 * This file contains the class for setting a color map
 * on a Role.
 */

 // COPYRIGHT_BEGIN
 //
 // The MIT License (MIT)
 //
 // Copyright (c) 2000-2024 Wizzer Works
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


#ifndef __MLE_3DCOMAPC_H_
#define __MLE_3DCOMAPC_H_

// Include Magic Lantern header files.
#include "mle/MleMediaRef.h"

#include "mle/MleCarrier.h"

class MleRole;

/**
 * @brief 3D Color Map property carrier.
 *
 * This sets a colormap on a Mle3dRole.
 *
 * @see Mle3dRole
 */
class MLE_CARRIER_API Mle3dColorMapCarrier 
{
  public:
 
	/**
	 * Set() passes a colormap to a Mle3dRole.
	 */
    static int set(MleRole *role, MlMediaRef colorMap);
};

#endif /* __MLE_3DCOMAPC_H_ */
