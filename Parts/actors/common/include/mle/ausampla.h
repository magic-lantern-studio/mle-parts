/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file ausampla.h
 * @ingroup MleParts
 *
 * This file defines the class for a Magic Lantern Sound Actor.
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

#ifndef __MLE_AUSAMPLE_H_
#define __MLE_AUSAMPLE_H_


#include "mle/MleActor.h"
#include "mle/smpsound.h"

class MleSampleActor : public MleActor
{
    MLE_ACTOR_HEADER ( MleSampleActor );

  public:

    MleSampleActor ( void );

    virtual ~MleSampleActor ( void );
    
    void init ( void );

    static void update ( void * );
    
#ifdef MLE_REHEARSAL
    void resolveEdit ( const char * );
#endif

    void setMediaRef ( MlMediaRef mediaRef );
    
    void setVolume ( float v );

    void play ();

    void pause ();

    void resume ();

    void stop ();
    
    MlMediaRef music;		// media reference

    float volume;

    int bufferSize;

    float stoppingTime;

    // The global sampled sound actor that exists iff some sample actor
    // exists.
    static MleSampleActor *sampleActor;

  private:

    int firstTime;

	MleSampleSound * sound;	// sound object pointer
};

extern MleSampleActor * sampleActor;

#endif /* MLE_REHEARSAL */
