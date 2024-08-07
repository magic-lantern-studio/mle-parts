/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file steermass.h
 * @ingroup MleParts
 *
 * This file defines a class for a Steerable Mass.
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
