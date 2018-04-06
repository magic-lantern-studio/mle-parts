/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dgeorwp.h
 * @ingroup MleParts
 *
 * This file defines the class for the 3D Geometry Registry property
 * with Web I/O.
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

#ifndef __MLE_3DGEORWP_H_
#define __MLE_3DGEORWP_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "mle/3dgeorwc.h"

class MleActor;

// This property makes a possibly asynchronous call to the property
// delegate to set the media.  If the set fails for lack of media
// the property will have null transformList, and there will be no
// way to set it.  

class Mle3dGeometryRegistryWebProperty
{
  public:

    Mle3dGeometryRegistryWebProperty()
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
		     Mle3dGeometryRegistryWebCarrier::set(actor->getDelegate(),m_geometryRegistry,
							 m_size))!=NULL);
    }

    MlMediaRef m_geometryRegistry;
    
    int m_size;

    void** m_transformList;
};


#endif /* __MLE_3DGEORWP_H_ */
