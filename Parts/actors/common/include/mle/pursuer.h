/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file pursuer.h
 * @ingroup MleParts
 *
 * This file defines the class for a Pursuer Actor.
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

#ifndef __MLE_PURSUER_H_
#define __MLE_PURSUER_H_

#include "mle/MleActor.h"
#include "mle/separate.h"
#include "mle/gvehicle.h"
#include "mle/wraptest.h"
#include "mle/avoidsph.h"
#include "mle/wanderer.h"
#include "mle/pe.h"
#include "mle/3dmodela.h"


/**
 * @brief Magic Lantern Pursuer Actor
 *
 * Sample behavioral vehicle-based actor, chases MleWanderer actors.
 *
 * This vehicle-based actor has three behavioral components: (1) pursuer the
 * nearest MleWanderer actor, (2) avoid MleObstacle actors, and avoid crowding
 * any nearby MlePursuer, MleEvader, or MleWanderer actors.
 * 
 * @see PursueEvade, Vehicle, MleEvader
 */
class MlePursuer : public Mle3dModelActor
{
    MLE_ACTOR_HEADER (MlePursuer);
    
  public:

    // Constructor.
    MlePursuer(void);

    // Destructor.
    ~MlePursuer(void);
    
    // Initialize actor (after constructor, before title runs).
    virtual void init(void);
    
    MlVector3 targetPoint;

    // Switch to select between static (0) or dynamic (1) pursuit.
    int predictive;
    
    // include package for vehicle semantics
    GenericVehicle vehicle;
    
    // include package for pursuit and evasion
    PursueEvade pe;
    
    // Package for obstacle avoidance
    AvoidSphericalObstacle avoid;
    
    // Package for vehicle containment
    VehicleInWrapAroundSphere wrap;
    
    // Package for separation maintenance
    Separation separate;
    
    // Update function, scheduled to run every frame.
    static void update (MlePursuer *client);
    
    // Returns a pointer to the nearest MleWanderer actor.
    MleWanderer *MlePursuer::findNearestWanderer (void);

    // Pursues the nearest MleWanderer actor.
    virtual void MlePursuer::engageNearestWanderer (MlVector3 *steering);
    
#ifdef MLE_REHEARSAL
    // Update actor after manipulation in tools.
    virtual void resolveEdit(const char *property = NULL);
#endif
};

#endif /* __MLE_PURSUER_H_ */
