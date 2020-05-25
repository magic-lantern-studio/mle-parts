/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file steermass.h
 * @ingroup MleParts
 *
 * This file defines a class for a Steerable Mass.
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

#ifndef __MLE_STEERMASS_H_
#define __MLE_STEERMASS_H_

#include "mle/simpmass.h"
#include <math/scalar.h>

/**
 * @brief Steerable Mass
 *
 * This class augments a SimpleMass with an interface of three
 * parametric controls: thrust, turn (yaw), and climb (pitch).
 */
class SteerableMass : public SimpleMass
{
  public:

    // Constructor: initializes reference members (position and
    // orientation) and optionally schedules the vehicle's update
    // function.
    SteerableMass(MlVector3& position,MlRotation& orientation,
	    MlBoolean schedFlag = TRUE);


    // Apply a force along the mass' local forward axis (negative
    // values imply braking force).
    void thrust(MlScalar value);


    // Apply a lateral (sideways) turning force: yaw.
    void turn(MlScalar value);


    // Apply a vertical turning force: pitch.
    void climb(MlScalar value);
};


#define mlRegisterSteerableMassPackageMembers(actor,package) \
{ mlRegisterSimpleMassPackageMembers (actor, package); }


#endif /* __MLE_STEERMASS_H_ */
