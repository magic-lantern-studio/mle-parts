/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file wanderer.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a Wanderer Actor.
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


// MleWanderer actor
//
// This is a vehicle actor based on the Wander steering behavior package.
// Because this actor does not avoid obstacles, it will occasionally bump
// into them.  A simple model of collision causes the vehicle to glance off
// from these encounters.  The obstacle and vehicle are treated as a sphere
// for purposes of this test.

// Include Magic Lantern header files.
#include "mle/MleScheduler.h"
#include "mle/MleDirector.h"

#include "mle/behavlib.h"
#include "mle/wanderer.h"

#ifdef ANNOTATION
#include "mle/Annotation.h"
#endif

// implement MleWanderer registry
MlePtrArray MleWanderer::registry(100);


MLE_ACTOR_SOURCE(MleWanderer,Mle3dModelActor);


#ifdef MLE_REHEARSAL
void MleWanderer::initClass(void)
{
    mlRegisterActorClass(MleWanderer,Mle3dModelActor);
    mlRegisterSeparatePackageMembers (MleWanderer, separate);
    mlRegisterGenericVehiclePackageMembers (MleWanderer, vehicle);
    mlRegisterVehicleInWrapAroundSpherePackageMembers (MleWanderer, wrap);
}
#endif


MleWanderer::MleWanderer(void)
 : vehicle(position.m_translation, orientation.m_rotation, scale.m_scale), 
    wrap(position.m_translation),
    separate(vehicle),
    wander(vehicle),
    avoid(vehicle)
{
    // initialize variables
    vehicle.velocity.setValue(ML_SCALAR_ZERO,ML_SCALAR_ZERO,ML_SCALAR(-.2));

    // register this instance
    registry.add(this);

    // schedule actor's update function, save pointer to scheduler item
    g_theTitle->m_theScheduler->insertFunc(PHASE_ACTOR,(MleSchedulerFunc)behave,this,this);
}


MleWanderer::~MleWanderer()
{
    g_theTitle->m_theScheduler->remove(this);
    registry.remove(this);
}


void
MleWanderer::init(void)
{
    // call superclass init
    Mle3dModelActor::init();
}


#ifdef MLE_REHEARSAL
void
MleWanderer::resolveEdit(const char *property)
{
   // call superclass resolve
   Mle3dModelActor::resolveEdit(property);
}
#endif


void MleWanderer::behave(MleWanderer *w)
{
    // push geometric state into delegate
    w->orientation.push(w);
    w->position.push(w);

    // apply local wander force to our momentum
    MlVector3 steering;
    w->wander.steeringToWander (&steering);
    w->vehicle.applyGlobalSteeringForce (steering);

    // react to collisions with any of the obstacles
    w->avoid.checkForAndReactToCollisions ();
}	
