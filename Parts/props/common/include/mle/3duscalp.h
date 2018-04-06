/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3duscalp.h
 * @ingroup MleParts
 *
 * This file defines the class for the 3D Uniform Scale property.
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

#ifndef __MLE_3DUSCALP_H_
#define __MLE_3DUSCALP_H_

// Include Magic Lantern header files.
#include "math/transfrm.h"
#include "mle/3dtransc.h"

class MleActor;

class Mle3dUniformScaleProperty
{
  public:

    Mle3dUniformScaleProperty()
	{ m_scale = ML_SCALAR_ONE; }

    MlScalar m_scale;

    MlScalar operator = ( MlScalar memberValue )
    { return m_scale = memberValue; }

    operator MlScalar&  ()
	{ return m_scale; }

    int push(MleActor* actor)
	{ 
	    MlTransform temp;
	    if (Mle3dTransformCarrier::get(actor->getRole(),temp))
	    { 
		    temp.setScaleOnly(MlVector3(m_scale,m_scale,m_scale));
		    return Mle3dTransformCarrier::set(actor->getRole(),temp);
	    }
	    else 
		    return FALSE;
    }
};

#endif /* __MLE_3DUSCALP_H_ */
