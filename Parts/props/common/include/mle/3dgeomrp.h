/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dgeomrp.h
 * @ingroup MleParts
 *
 * This file defines the class for the 3D Geometry Registry property.
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

#ifndef __MLE_3DGEOMRP_H_
#define __MLE_3DGEOMRP_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "mle/3dgeomrc.h"

class MleActor;

class Mle3dGeometryRegistryProperty
{
  public:
 
    Mle3dGeometryRegistryProperty() 
	{ 
	    m_geometryRegistry = MLE_NO_MEDIA; 
	    m_size = 0;
	    m_transformList = NULL;
    }

    MlMediaRef operator = ( MlMediaRef memberValue )
	{ 
	    return m_geometryRegistry = memberValue; 
    }

    operator MlMediaRef&  ()
	{ return m_geometryRegistry; }

    int push(MleActor* actor)
	{ 
	    return ((m_transformList = (void**)
		     Mle3dGeometryRegistryCarrier::set(actor->getRole(),m_geometryRegistry,
						      m_size))!=NULL);
    }

    MlMediaRef m_geometryRegistry;
    
    int m_size;

    void** m_transformList;
};


#endif /* __MLE_3DGEOMRP_H_ */
