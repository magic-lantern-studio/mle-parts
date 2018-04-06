/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dcamfvp.h
 * @ingroup MleParts
 *
 * This file defines the class for the 3D Field of View property.
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

#ifndef __MLE_3DCAMFVP_H_
#define __MLE_3DCAMFVP_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "mle/3dcamfvc.h"

class MleActor;

class Mle3dCameraFieldOfViewProperty
{
  public:
 
    Mle3dCameraFieldOfViewProperty()
	{ m_fieldOfView = ML_SCALAR(45.0); }

    MlScalar m_fieldOfView;

    MlScalar operator = ( MlScalar memberValue )
	{ 
	    return m_fieldOfView = memberValue; 
    }

    operator MlScalar&  ()
	{ return m_fieldOfView; }

    int push(MleActor* actor) 
	{ 
	    return Mle3dCameraFieldOfViewCarrier::set(actor->getRole(),m_fieldOfView); 
    }

    int pull(MleActor* actor)
	{ 
	    return Mle3dCameraFieldOfViewCarrier::get(actor->getRole(),m_fieldOfView); 
    }
};


#endif /* __MLE_3DCAMFVP_H_ */
