/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dltap.h
 * @ingroup MleParts
 *
 * This file defines the class for the 3D Light Angle property.
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

#ifndef __MLE_3DLTAP_H_
#define __MLE_3DLTAP_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "math/angle.h"
#include "mle/3dltac.h"

class MleActor;

class Mle3dLightAngleProperty
{
  public:

    Mle3dLightAngleProperty()
	{ m_angle = ML_ANGLE_PI_HALF; }

    MlScalar m_angle;

    MlScalar operator = ( MlScalar memberValue )
      { return m_angle = memberValue; }

    operator MlScalar&  ()
	{ return m_angle; }

    int push(MleActor* actor) 
	{ return Mle3dLightAngleCarrier::set(actor->getRole(),m_angle); }

    int pull(MleActor* actor)
	{ return Mle3dLightAngleCarrier::get(actor->getRole(),m_angle); }
};


#endif /* __MLE_3DLTAP_H_ */
