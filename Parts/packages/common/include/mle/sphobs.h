/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file sphobs.h
 * @ingroup MleParts
 *
 * This file defines a class for a Spherical Obstacle.
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

#ifndef __MLE_SPHOBS_H_
#define __MLE_SPHOBS_H_

#include "math/vector.h"
#include "math/scalar.h"
#include "mle/MlePtrArray.h"


/**
 * @brief Spherical Obstacle
 *
 * Designates an actor as an "obstacle" for obstacle
 * avoidance and collision detection by the AvoidSphericalObstacle package.
 *
 * Attaching this package to an actor simply designates it as an "obstacle"
 * for consideration by the AvoidSphericalObstacle package for obstacle
 * avoidance and collision detection.  Spherical obstacles are appropriate
 * both for sphere-shaped objects as well as for any object whose shape can
 * be approximated as a bounding sphere for simplicity.
 */
class SphericalObstacle
{
  public:

    // Constructor: adds the new instance to SphericalObstacles registry,
    // initializes references to position (or center point) and size
    // (diameter) of spherical obstacle.  These references typically point
    // to members of the actor which contains the SphericalObstacle
    // package.
    SphericalObstacle (MlVector3& position, MlScalar& size);

    // Destructor: removes instance from registry.
    ~SphericalObstacle (void);

    // Reference to center point of sphere.
    MlVector3& position;

    // Reference to diameter of sphere.
    MlScalar& size;
    
    // Public registry of all SphericalObstacle instances.  Other code can
    // index through this registry like an array to inspect each obstacle.
    static MlePtrArray registry;
};

#endif /* __MLE_SPHOBS_H_ */
