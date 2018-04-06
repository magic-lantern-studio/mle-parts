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

#ifndef __MLE_MYPROP3_H_
#define __MLE_MYPROP3_H_

class MleRole;

// Replace all occurrences of PropertyNName with the name of property N
// Replace all occurrences of PropertyNType with the type of property N

struct MyThreeProperty
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
