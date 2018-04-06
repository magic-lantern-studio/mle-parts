/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file aumidia.h
 * @ingroup MleParts
 *
 * This file defines the class for a Magic Lantern Midi Actor.
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

#ifndef __MLE_AUMIDI_H_
#define __MLE_AUMIDI_H_


#include "mle/MleActor.h"
#include "mle/midsound.h"


/**
 * @brief The Magic Lantern Midi Actor.
 */
class MleMidiActor : public MleActor
{
    MLE_ACTOR_HEADER ( MleMidiActor );

  public:

    MleMidiActor ( void );

    virtual ~MleMidiActor ( void );
    
    void init ( void );

    static void update ( void * );
    
#ifdef MLE_REHEARSAL
    void resolveEdit ( const char * );
#endif

    void setMediaRef ( MlMediaRef mediaRef );

    void play ();

    void pause ();

    void resume ();

    void stop ();

    MlMediaRef music;		// media reference

    static MleMidiActor *midiActor;

  private:

    int firstTime;

	MleMidiSound * sound;	// sound object pointer
};


extern MleMidiActor * midiActor;

#endif /* __MLE_AUMIDI_H_ */
