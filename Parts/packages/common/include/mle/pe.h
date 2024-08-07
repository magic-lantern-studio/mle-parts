/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file pe.h
 * @ingroup MleParts
 *
 * This file defines a class for a pursuer or evader vehicle.
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

#ifndef __MLE_PE_H_
#define __MLE_PE_H_

// Include Magic Lantern header files.
#include "mle/vehicle.h"
#include "math/vector.h"


/**
 * @brief This package provides pursuit and evasion behavior.
 *
 * This package provides simple pursuit (and evasion) behavior for static
 * targets and predictive pursuit (and evasion) for moving targets, such as
 * other vehicles.  Evasion is essentially the inverse of pursuit.
 * 
 * Nothing is scheduled by this package, calls must be made from the actor's
 * own scheduled code into functions provided by this package.
 */
class PursueEvade
{
  public:

    PursueEvade(Vehicle& vehicleRef);

    // reference member for Vehicle package
    Vehicle& vehicle;
    
    // Steer towards a static global location.
    void steeringForStaticPursuit  (const MlVector3 *location,
				    MlVector3 *steering);
    
    // Steer away from a static global location.
    void steeringForStaticEvasion  (const MlVector3 *location,
				    MlVector3 *steering);
    
    // Steer to chase or catch a moving vehicle.
    void steeringForDynamicPursuit (const Vehicle *opponent,
				    MlVector3 *steering);
    
    // Steer to escape from a moving vehicle.
    void steeringForDynamicEvasion (const Vehicle *opponent,
				    MlVector3 *steering);

    // Steer to chase a moving vehicle by aiming at a point offsetDist
    // away from it.  That is: fly toward, but not directly at, the
    // other vehicle.
    void steeringForOffsetPursuit (const Vehicle *opponent,
				   const MlScalar offsetDist,
				   MlVector3 *steering);

    // Steer towards a global location, eventually slowing so as to
    // come to a stop when we reach the location.  The vehicle will
    // begin to slow when its travel time to the target is within
    // "decelerationTime".  This value is specified in units of frames
    // (simulation steps).  The conversion from time to distance
    // assumes the vehicle is moving at its maximum speed.
    void steeringForGoToStopAt (const MlVector3 *location,
				const MlScalar decelerationTime,
				MlVector3 *steering);

    
    // "target" contains the target position used by the last steering
    // calculation.  It could be the location being pursued or evaded.
    // It could represent either a static location or a predicted
    // future location for the dynamic case.  Note that "the last
    // call" would be ill defined in a multiprocessed environment, but
    // as of now (11-9-95) FireWalker is not multiprocessed.
    MlVector3 target;
    
    
    // predictionGainForPursuit and predictionGainForEvasion are
    // tuning parameters for dynamic pursuit and evasion.  They are
    // multiplicative convertion factors from distance-to-target to an
    // approximate time-to-intercept which is used to predict future
    // target position.  These parameters effect the time scale of
    // prediction, and are dependent upon the vehicle's maximum
    // velocity.
    MlScalar predictionGainForPursuit;
    MlScalar predictionGainForEvasion;
    
    
  private:

    void predictionForDynamicPursuit (const Vehicle *opponent,
				      const MlScalar predictGain,
				      const MlBoolean noBowRide,
				      MlVector3 *futurePosition);

    void steeringDynamicPursuitInternal (const Vehicle *opponent,
					 const MlScalar predictGain,
					 const MlBoolean noBowRide,
					 MlVector3 *steering);
};

// Default for predictionGainForPursuit and predictionGainForEvasion.
#define DEFAULT_PREDICTION_GAIN ML_SCALAR (6)


#define mlRegisterPursueEvadePackageMembers(actor,package)                   \
{ mlRegisterActorMember (actor, package.predictionGainForPursuit, MlScalar); \
  mlRegisterActorMember (actor, package.predictionGainForEvasion, MlScalar); }


#endif /* __MLE_PE_H_ */
