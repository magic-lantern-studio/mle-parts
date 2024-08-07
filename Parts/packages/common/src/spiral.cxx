/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file spiral.cxx
 * @ingroup MleParts
 *
 * This file defines a class for a Spiral behavior.
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
