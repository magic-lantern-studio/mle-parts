/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file align.h
 * @ingroup MleParts
 *
 * This file defines a class for a vehicle with collective
 * steering behavior.
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

#ifndef __MLE_ALIGN_H_
#define __MLE_ALIGN_H_

#include <math/scalar.h>
#include "mle/vehicle.h"
#include "mle/MlePtrArray.h"


/**
 * @brief Alignment collective steering behavior package.
 *
 * This package gives an vehicle the ability to align itself with (that
 * is, head in the same direction as) other nearby vehicles.  The
 * parameters are a distance threshold (which determines when two
 * vehicles are "nearby") and an angle (which defines the perceptual
 * "field of view").
 * 
 * @see Cohesion, Separation, mlRegisterAlignPackageMembers
  */
class Alignment
{
  public:

    // Constructor.  The constructor requires a reference to the
    // Vehicle of the Actor containing the package.  It must be called
    // from the Actor's constructor.  This function initializes the
    // package instance and adds it to the class' registry.
    Alignment (Vehicle& vehicle);

    // Destructor.  Removes the instance from the class' registry.
    ~Alignment (void);
       
    // Reference to the vehicle of the actor to which we are attached.
    Vehicle& vehicle;
    
    // Distance threshold.  Alignment behavior is triggered when two
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
    // steering force to align with them.  It returns a zero vector
    // (MlVector3::zero) when no correction is necessary.
    void steeringForAlignment (MlVector3 *steering);
    
    // Used by each instance of the package to find the others.
    static MlePtrArray registry;
};


// This macro can be used in an actor to register all of the "public"
// properties of the Alignment package.  Registation of properties
// allows them to be specified in the workprint and adjusted in the
// GUI tools.
#define mlRegisterAlignPackageMembers(actor,package)           \
{ mlRegisterActorMember (actor, package.nearDist, MlScalar);   \
  mlRegisterActorMember (actor, package.frontAngle, MlScalar); \
  mlRegisterActorMember (actor, package.groupID, MlScalar); }


#endif /* __MLE_ALIGN_H_ */
