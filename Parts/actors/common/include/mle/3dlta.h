/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dlta.h
 * @ingroup MleParts
 *
 * This file defines the base class for a Magic Lantern Light.
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

#ifndef __MLE_3DLTA_H_
#define __MLE_3DLTA_H_

 
#include "mle/MleActor.h"
#include "mle/3dnodetp.h"
#include "mle/3dqrotp.h"
#include "mle/3dtranlp.h"
#include "mle/3dlttp.h"
#include "mle/3dltsp.h"
#include "mle/3dltip.h"
#include "mle/3drgeomp.h"
#include "mle/3duscalp.h"

/**
 * @brief This is the base class for the FireWalker light actors.
 */
class Mle3dBaseLightActor : public MleActor
{
    MLE_ACTOR_HEADER(Mle3dBaseLightActor);
    
  public:

     // Constructor: initializes member properties nodeType to "LIGHT"

     Mle3dBaseLightActor();
     ~Mle3dBaseLightActor() {}

     virtual void init() {}

     virtual void update() {}

#ifdef MLE_REHEARSAL
    virtual void resolveEdit(const char *) {}

    // This is a piece of 3D geometry representing the directional light
    // at the rehearsal stage.
    Mle3dRehearsalGeometryProperty  RG;
 
    // This is the scale of the rehearsal geometry.
    Mle3dUniformScaleProperty       scale;
#endif
    
    // The member properties

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

    // This member property specifies the direction of the light.
    Mle3dQuaternionRotationProperty orientation;  // actor transform

    // This member property specifies the 3D position of the light.
    Mle3dTranslationProperty        position;
};


#endif /* __MLE_3DLTA_H_ */
