/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file sphobs.h
 * @ingroup MleParts
 *
 * This file defines a class for a Spherical Obstacle.
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
