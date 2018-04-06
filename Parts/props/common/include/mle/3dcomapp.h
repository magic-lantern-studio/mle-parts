/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dcomapp.h
 * @ingroup MleParts
 *
 * This file defines the class for the 3D Color Map property.
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

#ifndef __MLE_3DCOMAPP_H_
#define __MLE_3DCOMAPP_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

// Include BRender target header files.
#include "mle/3dcomapc.h"

class MleActor;

class Mle3dColorMapProperty
{
  public:

    Mle3dColorMapProperty()
	{ m_colorMap = MLE_NO_MEDIA; }

    MlMediaRef m_colorMap;

    MlMediaRef operator = ( MlMediaRef memberValue )
	{ 
	    return m_colorMap = memberValue; 
    }

    operator MlMediaRef&  ()
	{ return m_colorMap; }

    int push(MleActor* actor) 
	{ 
	    return Mle3dColorMapCarrier::set(actor->getRole(),m_colorMap); 
    }
};


#endif /* __MLE_3DCOMAPP_H_ */
