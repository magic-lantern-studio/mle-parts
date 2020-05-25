/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file avoidsph.h
 * @ingroup MleParts
 *
 * This file defines a class for a vehicle that is capable of
 * steering around spherical objects.
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

#ifndef __MLE_AVOIDSPH_H_
#define __MLE_AVOIDSPH_H_

// Include Magic Lantern header files.
#include "mle/sphobs.h"
#include "mle/vehicle.h"
#include "math/vector.h"

/**
 * @brief Avoid Spherical Obstacle
 *
 * This package gives an actor the ability to deal with spherical obstacles:
 * steering around them, and detecting/reacting to collisions with them.
 */
class AvoidSphericalObstacle
{
  public:

    // Constructor: requires a reference to this actor's Vehicle
    // package.
    AvoidSphericalObstacle(Vehicle& vehicleRef);

    // reference member for Vehicle package
    Vehicle& vehicle;
    
    // used only for annotation
    SphericalObstacle *lastObstacleAvoided;
    
    // Determines if any obstacles pose a risk of imminent collision
    // to our vehicle.  If so, this function returns a steering
    // direction vector that will turn the vehicle away from the
    // potential collision.  Only obstacles that lie along the current
    // flight path and with a certain distance are considered to pose
    // a risk of collision.  If no potential collision is found this
    // function returns a steering value of (0,0,0).
    void steeringForObstacleAvoidance (MlVector3 *steering);
    
    // Checks whether our Vehicle has penetrated any of the known
    // SphericalObstacles.  If not, "displacement" and "opposingForce"
    // are set to (0,0,0).  Otherwise "displacement" is set to a
    // translation vector specifying how to move out of the
    // interpentration, and "opposingForce" is set to the force needed
    // to counteract all of the vehicle's velocity in the direction of
    // the obstacle.  This corresponds to assuming the obstacle is
    // much more massive than the vehicle.
    void checkForObstaclePenetration (MlVector3 *displacement,
		MlVector3 *opposingForce);

    // Determines if our vehicle is in collision with any obstacles in
    // the world.  When interpenetration is detected, the vehicle's
    // position and velocity is adjusted to model collision.
    MlBoolean checkForAndReactToCollisions (void);
};

#endif /* __MLE_AVOIDSPH_H_ */
