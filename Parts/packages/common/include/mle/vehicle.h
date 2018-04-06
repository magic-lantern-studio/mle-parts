/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file vehicle.h
 * @ingroup MleParts
 *
 * This file defines a class for a Vehicle.
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

#ifndef __MLE_VEHICLE_H_
#define __MLE_VEHICLE_H_

// Include Magic Lantern header files.
#include "mle/MleScheduler.h"
#include "mle/steermas.h"
#include "mle/banker.h"
#include "mle/stabiliz.h"


/**
 * @brief Vehicle
 *
 * This package gives an actor the ability to move like a vehicle.
 *
 * Attaching this package to an actor gives it the ability to move like
 * a vehicle.  It will behave like a simple physically-based object
 * which can be moved around by applying steering forces.  Usually
 * these steering forces are determined by other packages implementing
 * "steering behaviors".  The Vehicle class inherits from SteerableMass
 * which inherits from SimpleMass.  Hence vehicles have point-mass
 * physics and a local coordiante system which is aligned with the
 * velocity of the mass.
 *
 * @see GenericVehicle, SteerableMass, SimpleMass, Reorient, Stabilizer,
 * @see mlRegisterVehiclePackageMembers
 */
class Vehicle : public SteerableMass
{
  public:

    // Constructor: initializes reference members (position, orientation,
    // scale, reorient, and stabilizer), vehicle parameters, and optionally
    // schedules the vehicle's update function.
    Vehicle(MlVector3& position,
	    MlRotation& orientation,
	    MlScalar& scale,
	    Reorient& mybanker,
	    Stabilizer& mystabilizer, 
	    MlBoolean schedFlag = TRUE);

    // Destructor: removes scheduler items associated with this instance.
    ~Vehicle();
    
    // vehicle parameters

    // Maximum speed (magnitude of velocity vector).  If non-zero,
    // velocity will be truncated to this magnitude.  (See also
    // SimpleMass::viscosity.)
    MlScalar maxSpeed;

    // Maximum magnitude of steering force.  Applied forces are
    // truncated to this magnitude.
    MlScalar maxForce;

    // Nominal radius of vehicle (or of its bounding sphere).  This is
    // a rough approximation of the size of the vehicle.
    MlScalar radius;

    // Scale factor of transform of actor to which this package is
    // attached.
    MlScalar& scale;
    
    // (currently not documented due to implementation problmes)
    // interface to the quasi-physical vehicle model:
    void applyLocalSteeringForce (const MlVector3& steering);
    void applyGlobalSteeringForce (const MlVector3& steering);

    // Returns the distance within which objects are potential
    // collision threats.  The distance is a function of current
    // velocity and maxForce.
    MlScalar collisionThreatDistance (void) const;
    
    // Gain adjustment on collision threat prediction.
    MlScalar collisionThreatGain;
    
    // Reference to associated Reorient package.
    Reorient& banker;
    
    // Reference to assoicated Stabilizer package.
    Stabilizer& stabilizer;

    // Scheduled update function
    static void update (Vehicle *sv);

  private:

    MlScalar previousScale;

    void handleScaleChange (void);
};

#define DEFAULT_VEHICLE_MAX_SPEED ML_SCALAR(0.25f)
#define DEFAULT_VEHICLE_MAX_FORCE ML_SCALAR(0.01)
#define DEFAULT_VEHICLE_RADIUS ML_SCALAR_ONE
#define DEFAULT_VEHICLE_THREAT ML_SCALAR(0.4)


// This macro can be used in an actor to register all of the "public"
// properties of the Vehicle package.  Registation of properties
// allows them to be specified in the workprint and adjusted in the
// GUI tools.
#define mlRegisterVehiclePackageMembers(actor,package)                  \
{ mlRegisterActorMember (actor, package.maxSpeed, MlScalar);            \
  mlRegisterActorMember (actor, package.maxForce, MlScalar);            \
  mlRegisterActorMember (actor, package.radius, MlScalar);              \
  mlRegisterActorMember (actor, package.collisionThreatGain, MlScalar); \
  mlRegisterSteerableMassPackageMembers (actor,package);                }


#endif /* __MLE_VEHICLE_H_ */
