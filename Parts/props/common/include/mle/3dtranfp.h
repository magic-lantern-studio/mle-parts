/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dtranfp.h
 * @ingroup MleParts
 *
 * This file defines the class for the 3D Transform property.
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

#ifndef __MLE_3DTRANFP_H_
#define __MLE_3DTRANFP_H_

// Include Magic Lantern header files.
#include "math/transfrm.h"
#include "mle/3dtransc.h"

class MleActor;

class Mle3dTransformProperty
{
  public:

    Mle3dTransformProperty()
	{ m_transform = MlTransform::identity(); }

    MlTransform m_transform;

    MlTransform operator = ( MlTransform memberValue )
    { return m_transform = memberValue; }

    operator MlTransform&  ()
	{ return m_transform; }

    int push(MleActor* actor)
	{ 
	    return Mle3dTransformCarrier::set(actor->getRole(),m_transform); 
    }
};

#endif /* __MLE_3DTRANFP_H_ */
