/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3camclp.h
 * @ingroup MleParts
 *
 * This file defines the class for the 3D Camera Clipping property.
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

#ifndef __MLE_3DCAMCLP_H_
#define __MLE_3DCAMCLP_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "mle/3dcamclc.h"

class MleActor;

class Mle3dCameraClippingProperty
{
  public:

    Mle3dCameraClippingProperty()
	{ 
	    m_clipping = MlVector2(ML_SCALAR_ZERO,ML_SCALAR(100.0)); 
    }

    MlVector2 m_clipping;

    MlVector2 operator = ( MlVector2 memberValue )
	{ 
	    return m_clipping = memberValue; 
    }

    operator MlVector2&  ()
	{ return m_clipping; }

    int push(MleActor* actor)
	{ 
	    return Mle3dCameraClippingCarrier::set(actor->getRole(),m_clipping); 
    }

    int pull(MleActor* actor)
	{ 
	    return Mle3dCameraClippingCarrier::get(actor->getRole(),m_clipping); 
    }
};


#endif /* __MLE_3DCAMCLP_H_ */
