/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dsptlta.h
 * @ingroup MleParts
 *
 * This file defines the class for a Magic Lantern Spot Light.
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

#ifndef __MLE_3DSPTLTA_H_
#define __MLE_3DSPTLTA_H_

#include "mle/3dlta.h"
#include "mle/3dltdp.h"   // dropOffRate property
#include "mle/3dltap.h"   // angle property


/**
 * @brief This class defines the Magic Lantern spot lights.
 */
class Mle3dSpotLightActor : public Mle3dBaseLightActor
{
    MLE_ACTOR_HEADER(Mle3dSpotLightActor);
    
  public:

    // Constructor: initializes member properties nodeType to "LIGHT"
    // and lightType to "SPOT".
    Mle3dSpotLightActor();

    // Destructor: turns off the light.
    ~Mle3dSpotLightActor();

    void init();

    // Push all properties (or state) to the Set.
    void update();

#ifdef MLE_REHEARSAL
    void resolveEdit(const char *);
#endif
    
    // This member property specifies the rate at which the light intensity 
    // drops off from the primary direction (0 = constant intensity; 1 =
    // sharpest drop-off).
    Mle3dLightDropOffRateProperty   dropOffRate;

    // This member property specifies the angle between the primary direction
    // and an edge of the light cone (0 = 0 degree; 1 = 180 degrees).
    Mle3dLightAngleProperty         angle;
};


#endif /* __MLE_3DSPTLTA_H_ */
