/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file evader.cxx
 * @ingroup MleParts
 *
 * This file implements the class for an Evader Actor.
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


// MleEvader actor
//
// This is a prototype of a vehicle actor implementing an evasion steering
// behavior.  In addition it avoids Obstacles and maintains separation from
// other MleEvaders.  This actor is subclassed from the Pursuer actor and
// differs significantly only in its implementation of the virtual member
// function engageNearestWanderer.


// XXX this really ought to be in a Makefile somewhere
#define ANNOTATION

// Include Magic Lantern header files.
#include "mle/evader.h"
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


MLE_ACTOR_SOURCE (MleEvader, MlePursuer);


#ifdef MLE_REHEARSAL
void MleEvader::initClass(void)
{
    mlRegisterActorClass (MleEvader, MlePursuer);
    mlRegisterActorMember (MleEvader, predictive, int);
    mlRegisterSeparatePackageMembers (MleEvader, separate);
    mlRegisterGenericVehiclePackageMembers (MleEvader, vehicle);
    mlRegisterVehicleInWrapAroundSpherePackageMembers (MleEvader, wrap);
}
#endif /* MLE_REHEARSAL */


void MleEvader::init(void)
{
    // Call superclass init
    Mle3dModelActor::init();
}


// Select the nearest Wanderer and evade it.

void MleEvader::engageNearestWanderer (MlVector3 *steering)
{
    MleWanderer *w = findNearestWanderer ();
    if (w == NULL) return;

    // Draw line to selected Wanderer
    ANN_DRAW_LINE (this, position.m_translation,
	    w->position.m_translation, HI, LO, HI);

    // Get steering, conditionally using dynamic or static evasion
    if (predictive)
		pe.steeringForDynamicEvasion (&(w->vehicle), steering);
    else
		pe.steeringForStaticEvasion (&(w->position.m_translation), steering);

    // Convert global steering to local steering
    vehicle.localizeVector(*steering, *steering);
}
