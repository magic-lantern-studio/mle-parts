/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3derotp.h
 * @ingroup MleParts
 *
 * This file defines the class for the 3D Euler Angle Rotation property.
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

#ifndef __MLE_3DEROTP_H_
#define __MLE_3DEROTP_H_

// Include Magic Lantern header files.
#include "math/transfrm.h"
#include "mle/3dtransc.h"

class MleActor;

class Mle3dEulerAngleRotationProperty
{
  public:

    Mle3dEulerAngleRotationProperty()
	{ 
	    m_rotation = MlVector3(ML_SCALAR_ZERO,ML_SCALAR_ZERO,ML_SCALAR_ZERO); 
    }

    MlVector3 m_rotation;

    MlVector3 operator = ( MlVector3 memberValue )
	{ 
	    return m_rotation = memberValue; 
    }

    operator MlVector3&  ()
	{ return m_rotation; }

    int push(MleActor* actor)
	{ 
	    MlTransform temp;
	    if (Mle3dTransformCarrier::get(actor->getRole(),temp))
	    { 
		    temp.setRotationOnly(m_rotation);
		    return Mle3dTransformCarrier::set(actor->getRole(),temp);
	    }
	    else 
		    return FALSE;
    }
};

#endif /* __MLE_3DEROTP_H_ */
