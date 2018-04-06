/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dtranrp.h
 * @ingroup MleParts
 *
 * This file defines the class for the 3D Transform Registry property.
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

#ifndef __MLE_3DTRANRP_H_
#define __MLE_3DTRANRP_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "mle/3dtranrc.h"

class MleActor;

class Mle3dTransformRegistryProperty
{
  public:

    MlTransform** m_transformList;

    unsigned int m_transformCount;
    
    Mle3dTransformRegistryProperty()
	{ m_transformList = NULL; transformCount=0; }

    MlTransform** operator = ( MlTransform** memberValue )
    { return m_transformList = memberValue; }

    operator MlTransform**&  ()
	{ return m_transformList; }

    MlTransform& operator [](int i)        
    { return *((MlTransform*) m_transformList[i]); }

    int push(MleActor* actor)
	{
		return Mle3dTransformRegistryCarrier::
		    set(actor->getRole(),m_transformList,m_transformCount);
	}

    int pull(MleActor* actor)
	{
		return (transformList = Mle3dTransformRegistryCarrier::
		    get(actor->getRole(),m_transformCount))!=NULL; 
     }
};


#endif /* __MLE_3DTRANRP_H_ */

