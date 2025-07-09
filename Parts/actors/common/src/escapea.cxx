/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file escapea.cxx
 * @ingroup MleParts
 *
 * This file implements the class for an Actor that processes the escape
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
    // Make sure the keyboard manager exists.
    if (MleKeyboardPolled::g_keyboardManager  &&
       (MleKeyboardPolled::g_keyboardManager->keyDown(MLE_KEY_Escape)))
    {
        g_theTitle->m_quit = TRUE;
    }
}

void*
MleEscapeActor::operator new(size_t tSize)
{
    void* p = mlMalloc(tSize);
    return p;
}

void*
MleEscapeActor::operator new[](size_t tSize)
{
    void* p = mlMalloc(tSize);
    return p;
}

void
MleEscapeActor::operator delete(void* p)
{
    mlFree(p);
}

void
MleEscapeActor::operator delete[](void* p)
{
    mlFree(p);
}
