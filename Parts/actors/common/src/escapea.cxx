/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file escapea.cxx
 * @ingroup MleParts
 *
 * This file implements the class for an Actor that processes the escape
 * key.
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

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleDirector.h"
#include "mle/MleScheduler.h"
#include "mle/MleKeyboardPolled.h"

// Include Magic Lantern Extension header files.
#include "mle/escapea.h"


MLE_ACTOR_SOURCE(MleEscapeActor, MleActor);


#ifdef MLE_REHEARSAL
void
MleEscapeActor::initClass(void)
{
    mleRegisterActorClass(MleEscapeActor, MleActor);
}
#endif /* MLE_REHEARSAL */


MleEscapeActor::MleEscapeActor(void)
  : MleActor()
{
	// Do nothing extra.
}


MleEscapeActor::~MleEscapeActor(void)
{
	// Do nothing extra.
}


void
MleEscapeActor::init(void)
{
    // Create a polling keyboard manager, if necessary.
    if (MleKeyboardPolled::g_keyboardManager == NULL)
        new MleKeyboardPolled;

    // Schedule update function.
    g_theTitle->m_theScheduler->insertFunc(PHASE_ACTOR,(MleSchedulerFunc) MleEscapeActor::update,
	    this, this, 1, 1);
}


#if MLE_REHEARSAL
void
MleEscapeActor::resolveEdit(const char *)
{
	// Do nothing extra.
}
#endif /* MLE_REHEARSAL */


void
MleEscapeActor::update(MleEscapeActor* /* self */)
{
    if (MleKeyboardPolled::g_keyboardManager // Make sure no one else deleted it
        &&
       (MleKeyboardPolled::g_keyboardManager->keyDown(MLE_KEY_Escape)))
    g_theTitle->m_quit = TRUE;
}
