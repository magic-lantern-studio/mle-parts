/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file wander.h
 * @ingroup MleParts
 *
 * This file defines the Wander steering behavior package.
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

#ifndef __MLE_WANDER_H_
#define __MLE_WANDER_H_

// Include Magic Lantern header files.
#include "mle/vehicle.h"
#include "math/vector.h"


#define DEFAULT_WANDER_STRENGTH ML_SCALAR(0.5)
#define DEFAULT_WANDER_RATE ML_SCALAR(0.1)


/**
 * @brief This package gives an actor the ability to "wander".  
 *
 * This package gives an actor the ability to "wander".  Wandering is a type
 * of random steering: the steering direction on one frame is based on the
 * direction in the previous frame, plus a random offset.  That is, the
 * steering direction performs a random walk.  The envelope of the wander
 * steering force is the surface of a sphere located slightly ahead of the
 * vehicle.  The radius of that sphere is based on the package's member
 * variable "strength".  The speed of the random walk is controlled by
 * package's member variable "rate".
 */
class Wander
{
  public:

    // Constructor: requires a reference to this actor's Vehicle
    // package.
    Wander (Vehicle& vehicleRef);

    // reference member for Vehicle package
    Vehicle& vehicle;

    // The "strength" parameter determines how sharply the vehicle can
    // turn.  The amount of turn varies over time, this controls the
    // maximum tuning possible.  If strength is zero the vehicle does not
    // diviate from its initial direction.  When the strength is one, the
    // steering angle can diviate as much as 45 degrees from the direction
    // of travel.  The default value is halfway inbetween.
    MlScalar strength;
    
    // The "rate" parameter determines the amount of noise added to the
    // steering direction.  Zero means no change.  Small values mean small
    // changes: the steering direction changes slowly and smoothly.  Higher
    // values cause the steering value to change faster and so less
    // smoothly.  A value of one means that the steering on one frame is
    // essentially unrelated to the steering on the previous frame, this is
    // the "white noise" of steering behaviors.  The default value of 0.1
    // gives lively changes of direction with relatively smooth transition.
    MlScalar rate;
    
    //////////
    // This function computes and returns a new steering force each frame.
    // The force is specified in the 3d global space as are other packages.
    void steeringToWander (MlVector3 *steering);
    
  private:

    // current steering force
    MlVector3 force;
};


#define mlRegisterWanderPackageMembers(actor,package)         \
{ mlRegisterActorMember (actor, package.strength, MlScalar);  \
  mlRegisterActorMember (actor, package.rate, MlScalar);      }


#endif /* __MLE_WANDER_H_ */
