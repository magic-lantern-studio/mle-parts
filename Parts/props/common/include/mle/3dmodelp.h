/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dmodelp.h
 * @ingroup MleParts
 *
 * This file defines the class for the 3D Model property.
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

#ifndef __MLE_3DMODELP_H_
#define __MLE_3DMODELP_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

// Include Carrier support.
#include "mle/3dmodelc.h"

class MleActor;

class PROPERTY_API Mle3dModelProperty
{
  public:

    Mle3dModelProperty()
	{ m_model = MLE_NO_MEDIA; }

    MlMediaRef m_model;

    MlMediaRef operator = ( MlMediaRef memberValue )
	{ 
	    return m_model = memberValue; 
    }

    operator MlMediaRef&  ()
	{ return m_model; }

    int push(MleActor *actor) 
	{ 
	    return Mle3dModelCarrier::set(actor->getRole(),m_model); 
    }
};


#endif /* __MLE_3DMODELP_H_ */
