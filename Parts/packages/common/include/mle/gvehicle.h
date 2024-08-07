/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file gvehicle.h
 * @ingroup MleParts
 *
 * This file defines a class for a Generic Vehicle.
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

#ifndef __MLE_GVEHICLE_H_
#define __MLE_GVEHICLE_H_

#include "mle/MleScheduler.h"
#include "mle/vehicle.h"

/**
 * @brief Generic Vehicle
 *
 * A simple, general-purpose, all-in-one vehicle package.
 *
 * GenericVehicle is intended to be a general-purpose vehicle model
 * which can be easily attached to an actor for rapid protyping.
 * Further refinements of the title-specifc actor may require a more
 * title-specifc vehicle model, but GenericVehicle can get you off to a
 * quick start.  In addition to be basic Vehicle functionality (gives
 * an actor the ability to move like a vehicle) the GenericVehicle
 * package comes with default Reorient and Stabilizer packages already
 * bundled inside of it.
 * 
 * @see Vehicle, mlRegisterGenericVehiclePackageMembers
 */
class GenericVehicle : public Vehicle
{
  public:

    // Constructor: initializes reference members (position,
    // orientation, scale) and optionally schedules the vehicle's
    // update function.
    GenericVehicle (MlVector3& position,
		    MlRotation& orientation,
		    MlScalar& scale,
		    MlBoolean schedFlag = TRUE);

    // A default Reorient package for the Vehicle.
    ReorientBank defaultbanker;

    // A default Stabilizer package for the Vehicle.
    Stabilizer defaultstabilizer;
};


// This macro can be used in an actor to register all of the "public"
// properties of the GenericVehicle package.  Registation of
// properties allows them to be specified in the workprint and
// adjusted in the GUI tools.

#define mlRegisterGenericVehiclePackageMembers(actor,package) \
{ mlRegisterVehiclePackageMembers (actor,package); }


#endif /* __MLE_GVEHICLE_H_ */
