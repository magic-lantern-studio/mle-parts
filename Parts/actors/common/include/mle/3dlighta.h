/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dlighta.cxx
 * @ingroup MleParts
 *
 * This file defines the actor for a Magic Lantern Light.
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

#ifndef __MLE_3DLIGHTA_H_
#define __MLE_3DLIGHTA_H_

#include "mle/MleActor.h"
#include "mle/3dnodetp.h"
#include "mle/3dqrotp.h"
#include "mle/3dtranlp.h"
#include "mle/3dlttp.h"
#include "mle/3dltsp.h"
#include "mle/3dltip.h"

/**
 * @brief This class defines a Magic Lantern 3d light.
 */
class Mle3dLightActor : public MleActor
{
    MLE_ACTOR_HEADER(Mle3dLightActor);
    
  public:

    // Constructor: initializes member properties nodeType to "LIGHT"
    // and lightType to "DIRECTIONAL".
    Mle3dLightActor();

    // Destructor: turns off the light.
    virtual ~Mle3dLightActor();

    virtual void init();

    //  Push all properties (or state) to the Set.
    void update();

#ifdef MLE_REHEARSAL
    virtual void resolveEdit(const char* property);
#endif

    // The member properties

    // This is the 3D position of the light
    Mle3dTranslationProperty        position;     

    // This member property specifies the direction of the light.
    Mle3dQuaternionRotationProperty orientation;  // actor transform

    // This member property is to indicate that the general type of the 
    // actor is of "LIGHT" type.
    Mle3dNodeTypeProperty           nodeType;

    // This member property specifies the type of the light. In this case,
    // it is set to "DIRECTIONAL".
    Mle3dLightTypeProperty          lightType;

    // This member property specifies whether the light is on or off. 
    // If the light is off, it is set to zero, any nonzero value will
    // indicate that the light is on.
    Mle3dLightSwitchProperty        onOffSwitch;

    // This member property specifies the intensity of the directional 
    // light. It's value is from 0 (minimum intensity) and 1 (maximum
    // intensity).
    Mle3dLightIntensityProperty     intensity;

};


#endif /* __MLE_3DDIRLTA_H_ */
