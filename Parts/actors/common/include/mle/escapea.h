/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file escapea.h
 * @ingroup MleParts
 *
 * This file defines the class for an Actor that processes the escape
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

#ifndef __MLE_ESCAPEACTOR_H_
#define __MLE_ESCAPEACTOR_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleActor.h"
#include "mle/MleEscapeActor.h"


/**
 * @brief This actor quits a title when the user presses the escape key
 * on the keyboard.
 *
 * This is strictly a convenience actor. Titles which take up the whole
 * screen, for example, need to provide users with a way to quit. During
 * early prototyping of the title, programmers can use MleEscapeActor
 * to avoid having to write keyboard handling code to do this by
 * simply inserting this actor into their title's workprint.
 * 
 * @see MleKeyboardPolled.
 */
class MLE_ESCAPEACTOR_API MleEscapeActor : public MleActor 
{

    MLE_ACTOR_HEADER(MleEscapeActor);

  public:

    /**
	 * The default constructor.
	 */
	MleEscapeActor(void);

	/**
	 * The destructor.
	 */
    virtual ~MleEscapeActor(void);

    /**
	 * Creates a poll-based keyboard manager, if necessary, and
     * schedules this actor's update function.
	 */
    virtual void init(void);

#ifdef MLE_REHEARSAL

    /*
	 * Resolve modifications made to the specified property.
	 *
	 * @param property A pointer to a character array containing
	 * the name of the property to update/resolve.
	 */
    virtual void resolveEdit(const char *property);

#endif  /* MLE_REHEARSAL */

    /**
	 * Update function will keep polling the keyboard
     * to see if the escape key is down. If so, the
     * title's quit flag is set so that the Magic Lantern
	 * Runtime Engine's main loop
     * will terminate at the end of the current frame.
	 *
	 * @param self The actor, a pointer to itself, to update.
	 */
    static void update(MleEscapeActor *self);

};


#endif	/* __MLE_ESCAPEACTOR_H_ */
