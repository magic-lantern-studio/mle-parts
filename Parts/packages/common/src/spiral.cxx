/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file spiral.cxx
 * @ingroup MleParts
 *
 * This file defines a class for a Spiral behavior.
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


// This package makes an actor to spiral down when being shot.

// Include Magic Lanter header files.
#include "mle/spiral.h"
#include "mle/behavlib.h"
#include "math/scalar.h"


SpiralDown::SpiralDown (Vehicle& v)
 : vehicle(v) 
{
    // randomize the spiral direction
    MlScalar abc = frand2(ML_SCALAR(0.0), ML_SCALAR(2.10));
    spiralToRight = (abc > ML_SCALAR_ONE) ? TRUE : FALSE;

    // default values
    radius_factor = ML_SCALAR(1.0);
    // should be made to 9.81 for gravitational acceleration, but...
    fall_factor = ML_SCALAR(1.0);
}


// destructor

SpiralDown::~SpiralDown (void)
{
}


// Spiral down when being shot

void SpiralDown::steeringForSpiral (MlVector3 *steering)
{
    // initialize returned steering value to zero
    (*steering) = MlVector3::zero;
      
    // global down direction
    MlVector3 Gdown = vehicle.up * ML_SCALAR(-1.0);
    MlScalar mass = vehicle.mass;
    MlVector3 temp, dir;
 
    // temp points to the right of the vehicle in the global coords
    vehicle.globalizeVector(vehicle.right, temp);

    if ( spiralToRight == TRUE) 
      dir = temp;
    else
      dir =  temp * ML_SCALAR(-1.0);

    MlVector3 a = dir * (mlMul(radius_factor, vehicle.maxSpeed));
    MlVector3 b = Gdown * (mlMul(fall_factor, mass));
   	
    *steering = a + b;

}
