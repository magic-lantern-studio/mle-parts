/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dmodelc.h
 * @ingroup MleParts
 *
 * This file contains the class for setting a model on a
 * Role.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2003-2019 Wizzer Works
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

#ifndef __MLE_3DMODELC_H_
#define __MLE_3DMODELC_H_

//Title:    3D MODEL PROPERTY CARRIER

// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "mle/MleMediaRef.h"

class MleRole;
class MlTransform;

/**
 * @brief 3D Model property carrier.
 *
 * This sets the model on a Mle3dRole.
 *
 * @see Mle3dRole
*/
class Mle3dModelCarrier 
{ 
  public:

    static int set(MleRole *role, MlMediaRef model);
};

#endif /* __MLE_3DMODELC_H_ */
