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
struct PropertyName
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
