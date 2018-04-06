/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file myprop2.h
 * @ingroup MleParts
 *
 * This file defines the class for a 2-member property.
 * It is an example.
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

#ifndef __MLE_MYPROP2_H_
#define __MLE_MYPROP2_H_

class MleRole;

// Replace all occurrences of PropertyNName with the name of property N
// Replace all occurrences of PropertyNType with the type of property N

struct MyTwoProperty
{ 
    MyTwoProperty(Property1Type& property1Name, Property2Type& property2Name);
    static int init(MleRole* role, 
		    Property1Type property1Name, Property2Type property2Name);
    static int push(MleRole* role, 
		    Property1Type property1Name, Property2Type property2Name);
    static int pull(MleRole* role, 
		    Property1Type& property1Name, Property2Type& property2Name);
};


// Property Convenience Macros 

#define MyTwoProperty_HEADER() \
        Property1Type  property1Name; \
        Property2Type  property2Name             

#define MyTwoProperty_CONSTRUCTOR() \
        MyTwoProperty(property1Name,property2Name)

#define MyTwoProperty_INIT_CLASS(ACTOR) \
        mleRegisterActorMember(ACTOR, property1Name, Property1Type); \
        mleRegisterActorMember(ACTOR, property2Name, Property2Type)

#define MyTwoProperty_INIT() \
        MyTwoProperty::init(getRole(),property1Name,property2Name)

#define MyTwoProperty_PUSH() \
        MyTwoProperty::push(getRole(),property1Name,property2Name)

#define MyTwoProperty_PULL() \
        MyTwoProperty::pull(getRole(),property1Name,property2Name)

#endif /* __MLE_MYPROP2_H_ */
