/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file simpmass.h
 * @ingroup MleParts
 *
 * This file defines a class for a Simple Mass.
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

#ifndef __MLE_SIMPLEMASS_H_
#define __MLE_SIMPLEMASS_H_

#include "mle/mlTypes.h"
#include "math/vector.h"
#include "math/rotation.h"

struct MleSchedItem;

/**
 * @brief Simple Mass
 *
 * This class implements a simple physical model based on a point mass.
 * It also includes some elaborations which are not strictly physical,
 * such as the concept of a local frame of refererence (local
 * coordinate system) attached to the mass.  The mass has a velocity
 * which can be altered by applying forces.  The mass can optionally be
 * made subject to viscous drag and friction.
 */
class SimpleMass
{
  public:

    // Constructor: initializes reference members (position and
    // orientation) and optionally schedules the vehicle's update
    // function.
    SimpleMass(MlVector3& position,MlRotation& orientation,
	    MlBoolean schedFlag = TRUE);

    // Destructor.
    ~SimpleMass();
    
    // Reference member for position (typically points to members of
    // the actor which contains ths package).
    MlVector3& position;
    
    // Reference member for orientation (typically points to members of
    // the actor which contains ths package).
    MlRotation& orientation;
    
    // Mass (weight) of this object.
    MlScalar mass;

    // Current velocity (speed times heading), per frame.
    MlVector3 velocity;

    // Acceleration during the current frame.  Typically this is
    // calculated by the update function and not set by the user.
    MlVector3 acceleration;
    
    // Controlls how much (if at all) new acceleration is blended with
    // previous acceleration.  Used to filter out abrupt changes in
    // acceleration.  0 means no filtering, 0.1 is jumpy, 0.5 is
    // smooth but quick, 0.7 (the default) is nice and graceful, 0.9
    // is rather unresponsive.
    MlScalar accelDamping;

    // acceleration from last update (used by acceleration damping)
    MlVector3 lastAcceleration;

    // The viscosity (drag) factor of the medium in which the mass 
    // is travelling. The drag force will be: viscosity*velocity
    MlScalar viscosity;

    // The friction factor of the surface on which the mass is travelling
    // the friction force will be along the opposite direction of velocity
    // and the magnitude is: friction*(pressure on surface)
    MlScalar friction;

    // Applies a force (specified in local coordinates) to the mass.
    void applyLocalForce(const MlVector3& force);

    // Applies a force (specified in global coordinates) to the mass.
    void applyGlobalForce(const MlVector3& force);
    
    // Determines if a given global position is both (a) within a
    // sphere of "radius" centered on the point mass and (b) within
    // "angle" of the forward axis.
    MlBoolean pointWithinSphereAngle (const MlScalar radius,
	    const MlScalar angle, const MlVector3 *v) const;

    // Makes a prediction of the future position of the point mass
    // based on its current position and velocity and a time interval
    // expressed in fames.  A velocity-based prediction assumes that
    // no forces will be applied to the mass during the prediction
    // interval.
    void linearPredictPosition(const MlScalar futureFrames,
	    MlVector3 *futurePosition) const;

    
    // coordinate transformations

    // Transform a direction vector from global to local space.
    void localizeVector(const MlVector3& src,MlVector3& dst) const;

    // Transform a direction vector from local to global space.
    void globalizeVector(const MlVector3& src,MlVector3& dst) const;

    // Transform a position vector from global to local space.
    void localizeCoordinate(const MlVector3& src,MlVector3& dst) const;

    // Transform a position vector from local to global space.
    void globalizeCoordinate(const MlVector3& src,MlVector3& dst) const;
    
    // get and set of coordinate bases

    // Set the "forward" component of a given vector.
    void setForward(MlVector3& v,MlScalar weight) const;

    // Set the "right" component of a given vector.
    void setRight(MlVector3& v,MlScalar weight) const;

    // Set the "up" component of a given vector.
    void setUp(MlVector3& v,MlScalar weight) const;

    // Get the "forward" component of a given vector.
    MlScalar getForward(const MlVector3& v) const;

    // Get the "right" component of a given vector.
    MlScalar getRight(const MlVector3& v) const;

    // Get the "up" component of a given vector.
    MlScalar getUp(const MlVector3& v) const;

    // base coordinate system

    // Unit basis vector for local coordinate system.
    static const MlVector3 forward;

    // Unit basis vector for local coordinate system.
    static const MlVector3 right;

    // Unit basis vector for local coordinate system.
    static const MlVector3 up;
    

    // Scheduled update function.
    static void update(SimpleMass *client);
};


#define mlRegisterSimpleMassPackageMembers(actor,package)         \
{ mlRegisterActorMember (actor, package.mass, MlScalar);          \
  mlRegisterActorMember (actor, package.accelDamping, MlScalar);  \
  mlRegisterActorMember (actor, package.viscosity, MlScalar);     \
  mlRegisterActorMember (actor, package.friction, MlScalar);      }


#endif /* __MLE_SIMPLEMASS_H_ */
