/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file myprop1.h
 * @ingroup MleParts
 *
 * This file defines the class for a 1-member property.
 * It is an example
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

#ifndef __MLE_MYPROP1_H_
#define __MLE_MYPROP1_H_

// Include Magic Lantern header files.

#include "mle/prop1c.h"

class MleActor;

// Define this to be the name of the property class
#define PropertyName         myPropertyName

// Define this to be the name of the property carrier
#define PropertyCarrier      myCarrierName

// Define this to be the type name of the property member
#define MemberType           myMemberType

// Define this to be the name of the property member
#define MemberName           myMemberName

// Define this to be the inital value of the property member
#define InitialMemberValue   myInitialMemberValue

// Convenience class template to define the property class.
struct PROPERTY_API PropertyName
{
    PropertyName()
	{ MemberName = InitialMemberValue; }

    MemberType MemberName;
    MemberType operator = ( MemberType memberValue )
	{ return MemberName=memberValue; }

    operator MemberType& () 
	{ return MemberName; }

    int push(MleActor* actor)
	{ return PropertyCarrier::set(actor->getRole(),MemberName); }

    int pull(MleActor* actor)
	{ return PropertyCarrier::get(actor->getRole(),MemberName); }
};

#endif /* __MLE_MYPROP1_H_ */
