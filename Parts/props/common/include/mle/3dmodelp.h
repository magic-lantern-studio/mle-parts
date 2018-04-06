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

#ifndef __MLE_3DMODELP_H_
#define __MLE_3DMODELP_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

// Include Carrier support.
#include "mle/3dmodelc.h"

class MleActor;

class Mle3dModelProperty
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
