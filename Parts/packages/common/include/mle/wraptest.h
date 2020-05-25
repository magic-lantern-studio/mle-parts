/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file wraptest.h
 * @ingroup MleParts
 *
 * This file defines a class for managing spherical constraints.
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
