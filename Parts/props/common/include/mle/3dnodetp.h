/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dnodetp.h
 * @ingroup MleParts
 *
 * This file defines the class for the 3D Node Type property.
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

#ifndef __MLE_3DNODETP_H_
#define __MLE_3DNODETP_H_

// Include Magic Lantern Property header files.
#include "mle/3dnodetc.h"

class MleActor;


class Mle3dNodeTypeProperty
{
  public:

    Mle3dNodeTypeProperty()
	{ m_nodeType = Mle3dNodeTypeProperty::TRANSFORM; }

    int m_nodeType;

    int operator = ( int memberValue )
	{ 
	    return m_nodeType = memberValue; 
    }

    operator int&  ()
	{ return m_nodeType; }

    int push(MleActor *actor) 
	{ 
	    return Mle3dNodeTypeCarrier::set(actor->getRole(),m_nodeType); 
    }
    
    enum Mle3dNodeType
	{ 
	    TRANSFORM, 
	    CAMERA,
	    LIGHT, 
	    GEOMETRY, 
	    BOUNDING_BOX, 
	    CLIPPING_PLANE 
    };
};


#endif /* __MLE_3DNODETP_H_ */
