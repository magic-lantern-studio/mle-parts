/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file aumidia.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a Magic Lantern Midi Actor.
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

#include "mle/MleScheduler.h"
#include "mle/MleDirector.h"
#include "mle/aumidia.h"


MleMidiActor *MleMidiActor::midiActor = NULL;

MLE_ACTOR_SOURCE(MleMidiActor,MleActor);

#ifdef MLE_REHEARSAL
void MleMidiActor::initClass ( void )
{
    mlRegisterActorClass ( MleMidiActor, MleActor );

    mlRegisterActorMember ( MleMidiActor, music, MlMediaRef );
}

void MleMidiActor::resolveEdit ( const char * property )
{
    if (!property) return;

    if ( strcmp ( property, "music" ) == 0 )
    {
	    delete sound;
	    sound = NULL;
    }
}
#endif /* MLE_REHEARSAL */

// Audio actor default constructor for music.

MleMidiActor::MleMidiActor ( void ) : MleActor ()
{
    if ( MleMidiActor::midiActor == NULL )
	    MleMidiActor::midiActor = this;

    sound = NULL;
    music = MLE_NO_MEDIA;

    firstTime = TRUE;
}

MleMidiActor::~MleMidiActor ( void )
{
    if ( sound )
	    delete sound;

    g_theTitle->m_theScheduler->remove ( this );

    if ( MleMidiActor::midiActor == this )
	    MleMidiActor::midiActor = NULL;
}

void MleMidiActor::init ( void )
{
    if ( music != MLE_NO_MEDIA )
    {
	    g_theTitle->m_theScheduler->insertFunc ( PHASE_ACTOR, update, this, this);
    }
}

void MleMidiActor::setMediaRef ( MlMediaRef mediaRef )
{
    music = mediaRef;

    init ();
}

void MleMidiActor::update ( void * client )
{
    MleMidiActor * actor = (MleMidiActor *) client;

    if ( actor->firstTime )
    {
		actor->firstTime = FALSE;
    } else if ( ! actor->sound )
    {
		actor->sound = new MleMidiSound;
		actor->sound->open ( actor->music );
		actor->sound->setRepeat ();	// play music forever
		actor->sound->play ();
    }
}

void MleMidiActor::play ()
{
    if ( sound )
	    sound->play ();
}

void MleMidiActor::pause ()
{
    if ( sound )
	    sound->pause ();
}

void MleMidiActor::resume ()
{
    if ( sound )
	    sound->resume ();
}

void MleMidiActor::stop ()
{
    if ( sound )
	    sound->stop ();
}
