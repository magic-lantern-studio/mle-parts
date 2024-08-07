/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file wander.cxx
 * @ingroup MleParts
 *
 * This file implements the Wander steering behavior package.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2000-2024 Wizzer Works
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//  For information concerning this source file, contact Mark S. Millard,
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
