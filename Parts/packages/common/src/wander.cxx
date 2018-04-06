/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file wander.cxx
 * @ingroup MleParts
 *
 * This file implements the Wander steering behavior package.
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


// Include Magic Lantern header files.
#include "mle/wander.h"
#include "mle/behavlib.h"


// sqrt(2)/2
#define DIAGONAL ML_SCALAR(0.70710678)


// constructor

Wander::Wander (Vehicle& vehicleRef) : vehicle(vehicleRef)
{
    force = MlVector3::zero;
    rate = DEFAULT_WANDER_RATE;
    strength = DEFAULT_WANDER_STRENGTH;
}


// updates and returns a new local steering vector for random wandering

void Wander::steeringToWander (MlVector3 *steering)
{
    // Define geometry of force envelope: a sphere centered on the point
    // one unit down the forward axis, with radius from 0 to sqrt(2)/2
    MlVector3 center = MlVector3::zero;
    vehicle.setForward (center, ML_SCALAR_ONE);
    MlScalar radius = mlMul (strength, DIAGONAL);

    // add a random increment to the existing wander force
    MlVector3 increment;
    setToRandomPointInCube (&increment, mlMul (strength, rate));
    force += increment;

    // move the sphere to be centered at the origin
    force -= center;

    // map force back to surface of (approximate) sphere
    MlScalar length = force.approximateLength ();
    force *= mlDiv (radius, length);

    // move the sphere back to its original position
    force += center;

    // globalize the force vector to generate the global steering vector
    vehicle.globalizeVector(force, *steering);
}
