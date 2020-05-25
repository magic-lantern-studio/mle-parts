/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file evader.h
 * @ingroup MleParts
 *
 * This file defines the class for an Evader Actor.
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

#ifndef __MLE_EVADER_H_
#define __MLE_EVADER_H_

// Include Magic Lantern header files.
#include "mle/pursuer.h"


/**
 * @brief Evader Actor
 *
 * Sample behavioral vehicle-based actor, flees from MleWanderer actors,
 * used in "wope" title.
 *
 * This vehicle-based actor has three behavioral components: (1) fly
 * away from the nearest MleWanderer actor, (2) avoid MleObstacle actors,
 * and avoid crowding any nearby MlePursuer, MleEvader, or MleWanderer
 * actors.  Note that this actor is derived from, and is almost
 * identical to, the MlePursuer actor.
 *
 * @see PursueEvade, Vehicle, MlePursuer
 */
class MleEvader : public MlePursuer
{
    MLE_ACTOR_HEADER (MleEvader);
    
  public:

	// Initialize actor (after constructor, before title runs).
    virtual void init(void);

    // Evades the nearest MleWanderer actor.
    virtual void MleEvader::engageNearestWanderer (MlVector3 *steering);
};


#endif /* __MLE_EVADER_H_ */
