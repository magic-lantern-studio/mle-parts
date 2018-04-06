/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dcharrp.h
 * @ingroup MleParts
 *
 * This file defines the class for the 3D Character Registry property.
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

#ifndef __MLE_3DCHARRP_H_
#define __MLE_3DCHARRP_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "mle/3dcharrm.h"
#include "mle/3dcharrc.h"

class MleActor;

class Mle3dCharacterRegistryProperty
{
  public:

    Mle3dCharacterRegistryProperty()
	{ 
	    m_characterMediaRef = MLE_NO_MEDIA; 
	    m_characterRegistry = NULL;
    }

    MlMediaRef m_characterMediaRef;

    MlMediaRef operator = ( MlMediaRef memberValue )
	{ 
	    return m_characterMediaRef = memberValue; 
    }

    operator Mle3dCharacterRegistry* & ()
	{ 
	    return m_characterRegistry; 
    }

    int push(MleActor* actor)
	{ 
	    return (m_characterRegistry = Mle3dCharacterRegistryCarrier::
		    set(actor->getRole(),m_characterMediaRef))!=NULL; 
    }
    
    Mle3dCharacterRegistry* m_characterRegistry;
};


#endif /* __MLE_3DCHARRP_H_ */
