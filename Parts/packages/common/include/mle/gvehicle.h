/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file gvehicle.h
 * @ingroup MleParts
 *
 * This file defines a class for a Generic Vehicle.
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
