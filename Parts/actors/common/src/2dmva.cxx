/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 2dmva.h
 * @ingroup MleParts
 *
 * This file defines the class for a 2D Movie Actor.
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

// Include Magic Lantern header files.
#include "mle/2dmva.h"
#include "mle/2dmvd.h"


MLE_ACTOR_SOURCE(Mle2dMvActor, MleActor);


#ifdef MLE_REHEARSAL
void
Mle2dMvActor::initClass(void)
{
    mlRegisterActorClass(Mle2dMvActor, MleActor);

    mlRegisterActorMember(Mle2dMvActor, position, MlVector2);
    mlRegisterActorMember(Mle2dMvActor, movie, MlMediaRef);
    mlRegisterActorMember(Mle2dMvActor, displayState, int);

    //
    // Mark all the properties that belongs to the "transform" property
    // data set.
    //
    mlRegisterActorMemberDataset(Mle2dMvActor, position, MLE_PROP_DATASET_TRANSFORM);
}
#endif /* MLE_REHEARSAL */


Mle2dMvActor::Mle2dMvActor(void)
 : MleActor()
{
    //
    // Set default property values.
    //
    position.setValue(ML_SCALAR(0), ML_SCALAR(0));
    movie = MLE_NO_MEDIA;
    displayState = FALSE;
}


Mle2dMvActor::~Mle2dMvActor(void)
{
}


void
Mle2dMvActor::init(void)
{
    Mle2dMvRole *role = Mle2dMvRole::cast(getRole());

    //
    // Push initial data got from the workprint to the role.
    //
    role->screenLocation(position);
    role->display(displayState);
    role->load(movie);
}


#ifdef MLE_REHEARSAL
void
Mle2dMvActor::resolveEdit(const char *property)
{
    Mle2dMvRole *role = Mle2dMvRole::cast(getRole());

    if(! property || strcmp(property, "position") == 0)
	role->screenLocation(position);
    else if(strcmp(property, "movie") == 0)
	role->load(movie);
    else if(strcmp(property, "displayState") == 0)
	role->display(displayState);
}
#endif /* MLE_REHEARSAL */


void
Mle2dMvActor::setPosition(MlVector2 &pos)
{
    Mle2dMvRole *role = Mle2dMvRole::cast(getRole());

    //
    // Update position property and push the update to the role.
    //
    position.setValue(pos.getValue());
    role->screenLocation(position);
}


void
Mle2dMvActor::setMovie(MlMediaRef mv)
{
    Mle2dMvRole *role = Mle2dMvRole::cast(getRole());

    //
    // Update movie property and push the update to the role.
    //
    movie = mv;
    role->load(mv);
}


void
Mle2dMvActor::setVisible(int state)
{
    Mle2dMvRole *role = Mle2dMvRole::cast(getRole());

    //
    // Update displayState property and push the update to the role.
    //
    displayState = state;
    role->display(state);
}


int
Mle2dMvActor::getVisible(void)
{
    return displayState;
}
