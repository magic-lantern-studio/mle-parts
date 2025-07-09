/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file escapea.h
 * @ingroup MleParts
 *
 * This file defines the class for an Actor that processes the escape
 * key.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2000-2025 Wizzer Works
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//  For information concerning this header file, contact Mark S. Millard,
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

    /**
     * Override operator new.
     *
     * @param tSize The size, in bytes, to allocate.
     */
    void* operator new(size_t tSize);

    /**
     * Override operator new array.
     *
     * @param tSize The size, in bytes, to allocate.
     */
    void* operator new[](size_t tSize);

    /**
     * Override operator delete.
     *
     * @param p A pointer to the memory to delete.
     */
    void operator delete(void* p);

    /**
     * Override operator delete array.
     *
     * @param p A pointer to the memory to delete.
     */
    void operator delete[](void* p);

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
