/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file sphobs.cxx
 * @ingroup MleParts
 *
 * This file implements a class for a Spherical Obstacle.
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

// ------------------------------------------------------------------------
//
// SphericalObstacle package
//
// Attaching this package to an actor simply designates it as an "obstacle"
// for consideration by other packages for obstacle avoidance and collision
// detection.  Spherical obstacles are appropriate both for sphere-shaped
// objects as well as for any object whose shape can be approximated as a
// bounding sphere for simplicity.
//
// ------------------------------------------------------------------------

// Include Magic Lantern header files.
#include "mle/sphobs.h"


// implement Wanderer registry
MlePtrArray SphericalObstacle::registry(100);


// ------------------------------------------------------------------------
// constructor


SphericalObstacle::SphericalObstacle (MlVector3& p, MlScalar& s)
 : position(p), size(s)
{
    // register this instance
    registry.add(this);
}


// ------------------------------------------------------------------------
// destructor


SphericalObstacle::~SphericalObstacle (void)
{
    // unregister this instance
    registry.remove (this);
}
