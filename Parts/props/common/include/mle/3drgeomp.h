/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3drgeomp.h
 * @ingroup MleParts
 *
 * This file defines the class for the 3D Rehearsal Geometry property.
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

#ifndef __MLE_3DRGEOMP_H_
#define __MLE_3DRGEOMP_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "mle/3drgeomc.h"

class MleActor;

class Mle3dRehearsalGeometryProperty
{
  public:

    int m_onOff;

    MlMediaRef m_geometry;
  
    Mle3dRehearsalGeometryProperty()
	{ 
      m_onOff = 1;
      m_geometry = MLE_NO_MEDIA;
	}
 
    int push(MleActor* actor)
	{ 
      return Mle3dRehearsalGeometryCarrier::set(actor->getRole(), 
	    m_onOff, m_geometry); 
	}
};

#endif /* __MLE_3DRGEOMP_H_ */
