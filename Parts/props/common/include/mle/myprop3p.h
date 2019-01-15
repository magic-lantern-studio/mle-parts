/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file myprop3.h
 * @ingroup MleParts
 *
 * This file defines the class for a 3-member property.
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

#ifndef __MLE_MYPROP3_H_
#define __MLE_MYPROP3_H_

// Include Magic Lantern header files.

class MleRole;

// Replace all occurrences of PropertyNName with the name of property N
// Replace all occurrences of PropertyNType with the type of property N

struct PROPERTY_API MyThreeProperty
{ 
    MyThreeProperty(Property1Type& property1Name, 
		    Property2Type& property2Name,
		    Property3Type& property3Name);
    static int init(MleRole* role, 
		    Property1Type property1Name, 
		    Property2Type property2Name, 
		    Property3Type property3Name);
    static int push(MleRole* role, 
		    Property1Type property1Name, 
		    Property2Type property2Name, 
		    Property3Type property3Name);
    static int pull(MleRole* role, 
		    Property1Type& property1Name, 
		    Property2Type& property2Name, 
		    Property3Type& property3Name);
};


//Property Convenience Macros 

#define MyThreeProperty_HEADER() \
        Property1Type  property1Name; \
        Property1Type  property2Name; \
        Property3Type  property3Name             

#define MyThreeProperty_CONSTRUCTOR() \
        MyThreeProperty(property1Name,property2Name,property3Name)

#define MyThreeProperty_INIT_CLASS(ACTOR) \
        mleRegisterActorMember(ACTOR, property1Name, Property1Type); \
        mleRegisterActorMember(ACTOR, property2Name, Property2Type); \
        mleRegisterActorMember(ACTOR, property3Name, Property3Type)

#define MyThreeProperty_INIT() \
        MyThreeProperty::init(getRole(),property1Name,property2Name,property3Name)

#define MyThreeProperty_PUSH() \
        MyThreeProperty::push(getRole(),property1Name,property2Name,property3Name)

#define MyThreeProperty_PULL() \
        MyThreeProperty::pull(getRole(),property1Name,property2Name,property3Name)

#endif /* __MLE_MYPROP3_H_ */
