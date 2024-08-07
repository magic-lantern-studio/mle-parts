/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file sphobs.cxx
 * @ingroup MleParts
 *
 * This file implements a class for a Spherical Obstacle.
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
