/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file behavlib.h
 * @ingroup MleParts
 *
 * This file defines utilities for the Magic Lantern
 * behavior library.
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

#ifndef __MLE_BEHAVLIB_H_
#define __MLE_BEHAVLIB_H_

#include <stdlib.h>	// for rand()

#include "mle/mlTypes.h"
#include "mle/MleActor.h"
#include "math/vector.h"

// Needed only for Vehicle Library which might belong in another file:
#include "mle/mlMacros.h"

class SteerableMass;

// Prototypes for functions defined in the behavlib c++ file.


// Math and Geometry utilities:


MlScalar frand2 (MlScalar min, MlScalar max);

MlBoolean pointsWithinDistance (MlScalar distance,
				const MlVector3 *v1, const MlVector3 *v2);

void truncateVectorLength (MlVector3 *vector, const MlScalar maxLength);

void setToRandomPointInCube (MlVector3 *vector, MlScalar radius);


// Time Library

MlScalar frameRate (void);

MlScalar frameDuration (void);


// Vehicle Library:

MlScalar vehicleSpeed (const SteerableMass *vehicle);

void steerTowardLocalTarget (const SteerableMass *vehicle,
    const MlVector3 *localTarget, MlVector3 *steering);

void steerTowardGlobalTarget (const SteerableMass *vehicle,
    const MlVector3 *globalTarget, MlVector3 *steering);

void adjustSteeringForVelocity (const SteerableMass *vehicle,
    const MlScalar maxSpeed,
				MlVector3 *steering);

MlBoolean targetNearForwardAxis (const SteerableMass *vehicle,
    const MlVector3 *localTarget);

MlBoolean overshootingTarget (const SteerableMass *vehicle,
    const MlVector3 *globalTarget, const MlScalar futureTime);

// Just some arbitrarily large number, intended as an "infinity key".
// This should be defined more carefully and in some centralized def
// file.  It should probably be of type MlScalar for portability.

#define INFINITY ML_SCALAR(10000)

// The macro FRAND01 returns a random floating point value uniformly
// distributed on the interval between 0 and 1.

#define FRAND01 (mlDiv(mlFloatToScalar(rand()), ML_SCALAR(32767.0f)))

// basic linear interpolation macro.  As "weight" ranges from 0 to 1, the
// returned value is a linear combination of v0 and v1.

#define ML_LERP(weight,v0,v1) (mlMul((weight),(v1)) + mlMul((ML_SCALAR_ONE - (weight)), (v0)))


#if ML_FIXED_POINT
#define VECTOR_WITHIN_MAGNITUDE(radius, vector) \
(((vector).length()) < (radius))
#else /* ML_FIXED_POINT */
#define VECTOR_WITHIN_MAGNITUDE(radius, vector) \
(((vector).dot(vector)) < (mlSquare(radius)))
#endif /* ML_FIXED_POINT */


#define VECTOR_WITHIN_CUBE(radius, vector) \
((((-(radius)) < vector[0]) && ((radius) > vector[0])) && \
 (((-(radius)) < vector[1]) && ((radius) > vector[1])) && \
 (((-(radius)) < vector[2]) && ((radius) > vector[2])))


#endif /* __MLE_BEHAVLIB_H_ */
