/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file wraptest.h
 * @ingroup MleParts
 *
 * This file defines a class for managing spherical constraints.
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

#ifndef __MLE_WRAPTEST_H_
#define __MLE_WRAPTEST_H_

// Include Magic Lantern header files.
#include "mle/MleScheduler.h"
#include "math/vector.h"
#include "math/rotation.h"


/**
 * @brief This package constrains a position vector to remain inside a certain
 * sphere. 
 * 
 * This package constrains a position vector to remain inside a certain
 * sphere.  It schedules itself to automatically run each frame.  This
 * package is usually attached to an actor with a Vehicle package and
 * manipulates its "position" property.  When the vehicle ventures
 * outside of the sphere, it is wrapped around to the antipode, the
 * polar-opposite point.  (Note that despite its name, this package can
 * constrain any MlVector3 and need not be associated with a Vehicle.)
 */
class VehicleInWrapAroundSphere
{
  public:

    // Constructor: requires a reference to this actor's Vehicle
    // package.
    VehicleInWrapAroundSphere (MlVector3& position);

    // Destructor.
    ~VehicleInWrapAroundSphere ();
    
    // Reference to position vector being constrained.
    MlVector3& position;
    
    // Diameter of constraining sphere.
    MlScalar diameter;

    // Center point of constraining sphere.
    MlVector3 center;
    
    // Automatically scheduled update function.
    static void update (VehicleInWrapAroundSphere *v);
};


#define mlRegisterVehicleInWrapAroundSpherePackageMembers(actor,package) \
{ mlRegisterActorMember (actor, package.diameter, MlScalar);  \
  mlRegisterActorMember (actor, package.center, MlVector3);}


#endif /* __MLE_WRAPTEST_H_ */
