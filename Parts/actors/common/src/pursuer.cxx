/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file pursuer.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a Pursuer Actor.
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


// MlePursuer actor
//
// This is a prototype of a vehicle actor implementing a pursuit steering
// behavior.  This actor will pursue the nearest MleWanderer actor, while
// avoiding Obstacles and maintaining separation from fellow MlePursuers.

#define ANNOTATION

#include "mle/MleScheduler.h"
#include "mle/MleDirector.h"
#include "mle/pursuer.h"
#include "mle/behavlib.h"
#include "mle/3dannoa.h"

// This is a work-around for an apparent bug in BRender.  The facility
// used to draw annotation (prelit edges) seems to allow only "binary"
// colors, where the RGB components are either 0 or 100%.  Originally
// the annotation lines associated with this actor used fairly subtle
// colors that were dark and desaturated.  BRender Made these all
// white so they were indistinguishable.  The temporary (we hope!) fix
// is to use bright saturated colors.  But those are distracting and
// visually dominate the motion of the vehicles.  So now the RGB
// components are defined in terms of preprocessor variables HI and
// LO.  At some point in the future we hope to go back to the subtle
// color values.  (XXX)

//#define HI 0.2
//#define LO 0.1

#define HI 1.0
#define LO 0.0


MLE_ACTOR_SOURCE (MlePursuer, Mle3dModelActor);


#ifdef MLE_REHEARSAL
void
MlePursuer::initClass(void)
{
    mlRegisterActorClass (MlePursuer, Mle3dModelActor);
    mlRegisterActorMember (MlePursuer, targetPoint, MlVector3);
    mlRegisterActorMember (MlePursuer, predictive, int);
    mlRegisterSeparatePackageMembers (MlePursuer, separate);
    mlRegisterGenericVehiclePackageMembers (MlePursuer, vehicle);
    mlRegisterVehicleInWrapAroundSpherePackageMembers (MlePursuer, wrap);
}
#endif /* MLE_REHEARSAL */


MlePursuer::MlePursuer(void)
 : vehicle(position.m_translation, orientation.m_rotation, scale.m_scale),
   wrap(position.m_translation),
   separate(vehicle),
   avoid(vehicle),
   pe(vehicle)
{
    MlScalar o = ML_SCALAR_ZERO;

    // Initialize variables
    vehicle.velocity.setValue(o, o, ML_SCALAR(-0.2));
    targetPoint = MlVector3::zero;
    predictive = 1;

    // Schedule actor's update function, save pointer to scheduler item
    g_theTitle->m_theScheduler->insertFunc(PHASE_ACTOR,(MleSchedulerFunc)update,this,this);
}


MlePursuer::~MlePursuer()
{
    g_theTitle->m_theScheduler->remove(this);
}


void MlePursuer::init(void)
{
    // Call superclass init
    Mle3dModelActor::init();
}


#ifdef MLE_REHEARSAL
void MlePursuer::resolveEdit (const char *property)
{
    // Call superclass resolve
    Mle3dModelActor::resolveEdit(property);
}
#endif /* MLE_REHEARSAL */


MleWanderer *MlePursuer::findNearestWanderer (void)
{
    MleWanderer *nearest = NULL;
    MlScalar minDistance = INFINITY;
    for ( int i = 0; i < MleWanderer::registry.getSize(); i++ )
	{
	    MleWanderer *w = (MleWanderer *)MleWanderer::registry[i];
	    MlVector3 offset = (w->position.m_translation - 
				this->position.m_translation);
	    MlScalar d = offset.approximateLength();
	    if (d < minDistance)
		{
		    nearest = w;
		    minDistance = d;
		}
	}

    return nearest;
}


// Select the nearest MleWanderer and pursue it.

void MlePursuer::engageNearestWanderer (MlVector3 *steering)
{
    MleWanderer *w = findNearestWanderer ();
    if (w == NULL) return;

    // draw line to selected MleWanderer
    ANN_DRAW_LINE (this, position.m_translation,
	    w->position.m_translation, HI, LO, LO);

    // Get steering, conditionally using dynamic or static pursuit
    if (predictive)
	{
	    pe.steeringForDynamicPursuit (&(w->vehicle), steering);
	    // Draw line to predicted position
 	    ANN_DRAW_LINE (this, position.m_translation, pe.target, HI, HI, HI);
	}
    else
	{
	    pe.steeringForStaticPursuit (&w->position.m_translation, steering);
	}

    // Convert global steering to local steering
    vehicle.localizeVector(*steering, *steering);
}


#define SEPARATION_CHECK_LIKELIHOOD (FRAND01 > ML_SCALAR(0.5))


void MlePursuer::update (MlePursuer *p)
{
    // Push transfomation state into delegate
    p->Mle3dModelActor::update();

    // Clear annotation for this actor
    ANN_CLEAR (p);

    MlVector3 steering = MlVector3::zero;
    MlVector3 separation = MlVector3::zero;

    // Check for imminent collision
    MlVector3 oa;
    p->avoid.steeringForObstacleAvoidance (&oa);
    if (oa != MlVector3::zero)
	{
	    // Steer to avoid obstacle
	    steering = oa;
	    {
			// Give it a small forward component, this keeps
			// the vehicle from "stalling" when pursuit and
			// obstacle avoidance directions are far apart.
			MlScalar s = ML_SCALAR(0.5);
			MlScalar f = mlMul (oa.approximateLength(), s);
			p->vehicle.setForward(oa, f);
	    }
	    // Draw line to obstacle
	    ANN_DRAW_LINE (p,p->position.m_translation,
			   (p->avoid.lastObstacleAvoided)->
			   position, LO, LO, HI);
	}
    else
	{
	    // Occasionally check for crowding
	    if (SEPARATION_CHECK_LIKELIHOOD)
			p->separate.steeringForSeparation(&separation);

	    if (separation != MlVector3::zero)
		{
		    // Draw line along separation maintenance direction
		    MlVector3 v = p->position.m_translation +
			(separation * ML_SCALAR(5));
		    ANN_DRAW_LINE (p, p->position.m_translation, v, LO, HI, LO);
		    p->vehicle.localizeVector(separation, steering);
		}
	    else
		{
		    // Otherwise, select a MleWanderer and pursue it:
		    p->engageNearestWanderer (&steering);
		}
	}
    // Apply global steering force to our momentum
    p->vehicle.applyLocalSteeringForce (steering);
}
