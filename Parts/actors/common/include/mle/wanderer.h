/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file wanderer.h
 * @ingroup MleParts
 *
 * This file defines the class for a Wanderer Actor.
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

#ifndef __MLE_WANDERER_H_
#define __MLE_WANDERER_H_

// Include Magic Lantern header files.
#include "mle/wander.h"
#include "mle/gvehicle.h"
#include "mle/wraptest.h"
#include "mle/avoidsph.h"
#include "mle/separate.h"
#include "mle/3dmodela.h"
#include "mle/MlePtrArray.h"


/**
 * @brief Sample behavioral vehicle-based actor, wanders around, collides with
 * obstacles, used in "wope" title.
 * 
 * This simple behavioral vehicle-based actor just wanders around in
 * its world and occasionally collides with MleObstacle actors.  It is
 * chased by MlePursuer actors, and MleEvader actors flee from it.
 * 
 * @see Wander, Vehicle
 */

class MleWanderer : public Mle3dModelActor
{
    MLE_ACTOR_HEADER(MleWanderer);

  public:

    // Constructor.
    MleWanderer(void);

    // Destructor.
    ~MleWanderer(void);

    // Initialize actor (after constructor, before title runs).
    virtual void init(void);
    
    // Update function, scheduled to run every frame.
    static void behave(MleWanderer *w);

#ifdef MLE_REHEARSAL
    // Update actor after manipulation in tools.
    virtual void resolveEdit(const char *property = NULL);
#endif
    
    // Package for vehicle semantics
    GenericVehicle vehicle;
    
    // Package for wandering
    Wander wander;
    
    // Package for obstacle avoidance
    AvoidSphericalObstacle avoid;
    
    // Package for vehicle containment
    VehicleInWrapAroundSphere wrap;
    
    // Package for separation maintenance
    Separation separate;

    // Registry so other objects can find object of this class.
    static MlePtrArray registry;
};

#endif /* __MLE_WANDERER_H_ */
