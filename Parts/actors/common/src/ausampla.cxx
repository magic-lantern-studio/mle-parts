/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file ausampla.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a Magic Lantern Sound Actor.
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
#include "mle/ausampla.h"



MleSampleActor *MleSampleActor::sampleActor = NULL;

MLE_ACTOR_SOURCE(MleSampleActor,MleActor);

#ifdef MLE_REHEARSAL
void MleSampleActor::initClass ( void )
{
    mlRegisterActorClass ( MleSampleActor, MleActor );

    mlRegisterActorMember ( MleSampleActor, music, MediaRef );

    mlRegisterActorMember ( MleSampleActor, volume, float );

    mlRegisterActorMember ( MleSampleActor, bufferSize, int );

    mlRegisterActorMember ( MleSampleActor, stoppingTime, float );
}

void MleSampleActor::resolveEdit ( const char * property )
{
    if (!property) return;

    if ( strcmp ( property, "music" ) == 0 )
    {
		delete sound;
		sound = NULL;
	} else if ( strcmp ( property, "volume" ) == 0 )
    {
		if ( sound )
			sound->setVolume ( volume );
    } else if ( strcmp ( property, "bufferSize" ) == 0 )
    {
		if ( sound )
			sound->setMaxBuffSize ( bufferSize );
    } else if ( strcmp ( property, "stoppingTime" ) == 0 )
    {
		if ( sound )
			sound->setStoppingTime ( stoppingTime );
    }
}
#endif /* MLE_REHEARSAL */

// Audio actor default constructor for music.

MleSampleActor::MleSampleActor ( void ) : MleActor ()
{
    if ( MleSampleActor::sampleActor == NULL )
		MleSampleActor::sampleActor = this;

    sound = NULL;
    music = MLE_NO_MEDIA;
    volume = 1.0f;
    bufferSize = 0;
    stoppingTime = 0.0f;

    firstTime = TRUE;
}

MleSampleActor::~MleSampleActor ( void )
{
    if ( sound )
		delete sound;

    g_theTitle->m_theScheduler->remove ( this );

    if ( MleSampleActor::sampleActor == this )
		MleSampleActor::sampleActor = NULL;
}

void MleSampleActor::init ( void )
{
    if ( music != MLE_NO_MEDIA )
    {
		g_theTitle->m_theScheduler->insertFunc ( PHASE_ACTOR, update, this, this);
    }
}

void MleSampleActor::setMediaRef ( MediaRef mediaRef )
{
    music = mediaRef;

    init ();
}

void MleSampleActor::update ( void * client )
{
    MleSampleActor * actor = (MleSampleActor *) client;

    if ( actor->firstTime )
    {
		actor->firstTime = FALSE;
    } else if ( ! actor->sound )
    {
		// The following operations must be done in this order!

		// Create a new MleSampleSound object.

		actor->sound = new MleSampleSound;

		// The MaxBuffSize and the StoppingTime are used by the open
		// function to allocate resources at the same time as the audio
		// file is opened and, optionally, read into memory.

		if ( actor->bufferSize )
			actor->sound->setMaxBuffSize ( actor->bufferSize );
		if ( actor->stoppingTime )
			actor->sound->setStoppingTime ( actor->stoppingTime );

		// Open the audio file, allocate buffers, load the data, etc.

		actor->sound->open ( actor->music );

		// These functions affect data structures created by the open.

		actor->sound->setRepeat ();	// play music forever
		actor->sound->setVolume ( actor->volume );
		actor->sound->play ();
    }
}

void MleSampleActor::setVolume ( float v )
{
    sound->setVolume ( v );
}

void MleSampleActor::play ()
{
    if ( sound )
		sound->play ();
}

void MleSampleActor::pause ()
{
    if ( sound )
		sound->pause ();
}

void MleSampleActor::resume ()
{
    if ( sound )
		sound->resume ();
}

void MleSampleActor::stop ()
{
    if ( sound )
		sound->stop ();
}
