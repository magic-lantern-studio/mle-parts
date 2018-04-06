/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dltip.h
 * @ingroup MleParts
 *
 * This file defines the class for the 3D Light Intensisty property.
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

#ifndef __MLE_3DLTIP_H_
#define __MLE_3DLTIP_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "mle/3dltic.h"

class MleActor;

class Mle3dLightIntensityProperty
{
  public:

    Mle3dLightIntensityProperty()
	{ m_intensity = ML_SCALAR(1.0); }

    MlScalar m_intensity;

    MlScalar operator = ( MlScalar memberValue )
    { return m_intensity = memberValue; }

    operator MlScalar&  ()
	{ return m_intensity; }

    int push(MleActor* actor)
	{ 
      return Mle3dLightIntensityCarrier::set(actor->getRole(),m_intensity); 
    }

    int pull(MleActor* actor)
	{ 
      return Mle3dLightIntensityCarrier::get(actor->getRole(),m_intensity); 
    }
};


#endif /* __MLE_3DLTIP_H_ */

