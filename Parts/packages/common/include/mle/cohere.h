/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file cohere.h
 * @ingroup MleParts
 *
 * This file defines a class for a vehicle that exhibits
 * collective steering behavior.
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

#ifndef __MLE_COHERE_H_
#define __MLE_COHERE_H_

#include <math/scalar.h>
#include "mle/vehicle.h"
#include "mle/MlePtrArray.h"


/**
 * @brief Cohesion (group-forming) collective steering behavior package.
 *
 * This package gives an vehicle the ability to cohere with (approach
 * and form a group with) other nearby vehicles.  The parameters are a
 * distance threshold (which determines when two vehicles are"nearby")
 * and an angle (which defines the perceptual "field of view").
 * 
 * @see Separation, Alignment, mlRegisterCoherePackageMembers
 */
class Cohesion
{
  public:

    // Constructor.  The constructor requires a reference to the
    // Vehicle of the Actor containing the package.  It must be called
    // from the Actor's constructor.  This function initializes the
    // package instance and adds it to the class' registry.
    Cohesion (Vehicle& vehicle);

    // Destructor.  Removes the instance from the class' registry.
    ~Cohesion (void);
    
    // Reference to the vehicle of the actor to which we are attached
    Vehicle& vehicle;

    // Distance threshold. Cohesion  behavior is triggered when two
    // vehicles get closer than this distance from each other.
    MlScalar nearDist;

    // Perception angle (in degrees, measured from the vehicle's
    // forward axis).  If set to 180, perception of other vehicle is
    // omnidirectional.  If set to 90, perception is limited to
    // vehicles ahead of ours.
    MlScalar frontAngle;

    // Allows for multiple "groups" of vehicles.  By default all
    // vehicles have groupID of 0.  Separation behavior for one
    // vehicle considers only vehicles with the same groupID.
    int groupID;
    
    // This function determines which (if any) other vehicles (a) have
    // a matching groupID, (b) are within the nearDist neighborhood
    // and (c) are within the frontAngle sector.  It then computes a
    // steering force to cohere with (approach) them.  It returns a
    // zero vector (MlVector3::zero) when no correction is necessary.
    void steeringForCohesion (MlVector3 *steering);

    // Used by each instance of the package to find the others.
    static MlePtrArray registry;
};


// This macro can be used in an actor to register all of the "public"
// properties of the Cohesion package.  Registation of properties
// allows them to be specified in the workprint and adjusted in the
// GUI tools.
#define mlRegisterCoherePackageMembers(actor,package)          \
{ mlRegisterActorMember (actor, package.nearDist, MlScalar);   \
  mlRegisterActorMember (actor, package.frontAngle, MlScalar); \
  mlRegisterActorMember (actor, package.groupID, MlScalar); }


#endif /* __MLE_COHERE_H_ */
