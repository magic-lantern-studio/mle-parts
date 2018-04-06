/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dtextrp.h
 * @ingroup MleParts
 *
 * This file defines the class for the 3D Texture Registry property.
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

#ifndef __MLE_3DTEXTRP_H_
#define __MLE_3DTEXTRP_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "mle/3dtextrc.h"

class MleActor;

class Mle3dTextureRegistryProperty
{
  public:

    Mle3dTextureRegistryProperty()
	{ m_textureRegistry = MLE_NO_MEDIA; }

    MlMediaRef m_textureRegistry;

    MlMediaRef operator = ( MlMediaRef memberValue )
    { return m_textureRegistry = memberValue; }

    operator MlMediaRef&  ()
	{ return m_textureRegistry; }

    int push(MleActor* actor)
	{ 
	    return Mle3dTextureRegistryCarrier::set(actor->getRole(),m_textureRegistry); 
    }
};

#endif /* __MLE_3DTEXTRP_H_ */
