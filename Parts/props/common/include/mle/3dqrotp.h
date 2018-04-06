/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dqrotp.h
 * @ingroup MleParts
 *
 * This file defines the class for the 3D Quaternion Rotation property.
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

#ifndef __MLE_3DQROTP_H_
#define __MLE_3DQROTP_H_

//Title:    3D QUATERNION ROTATION PROPERTY

// Include Magic Lantern Math header files.
#include "math/transfrm.h"

// Include Carrier support.
#include "mle/3dtransc.h"

class MleActor;


class Mle3dQuaternionRotationProperty
{
  public:

    Mle3dQuaternionRotationProperty()
	{ 
	    m_rotation = MlRotation(ML_SCALAR_ZERO,ML_SCALAR_ZERO,ML_SCALAR_ZERO,ML_SCALAR_ONE); 
    }

    MlRotation operator = ( MlRotation memberValue )
	{ 
	    return m_rotation = memberValue; 
    }

    operator MlRotation&  ()
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

    MlRotation m_rotation;
};


#endif /* __MLE_3DQROTP_H_ */
