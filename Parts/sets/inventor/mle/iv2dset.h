/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file iv2dset.h
 * @ingroup MleParts
 *
 * This file contains the class for a 2D Set targeting the Inventor
 * platform.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2000-2024 Wizzer Works
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

#ifndef __MLE_IV2DSET_H_
#define __MLE_IV2DSET_H_


// Include Magic Lantern header files.
#include "math/scalar.h"
#include "math/vector.h"
#include "mle/MleSet.h"
#include "mle/MleScheduler.h"
#include "mle/Mle2dSet.h"


class MleRole;
class Mle2dRole;


/**
 * @brief Class definition for Inventor 2D Set.
 */
class MLE_2DSET_API Mle2dSet: public MleSet 
{

    MLE_SET_HEADER(Mle2dSet);

  public:

	Mle2dSet(void);

    virtual ~Mle2dSet(void);

    virtual void init(void);

    // Attach and detach a role to the 2d Set's scene graph (currently
    // implemented as a linked-list).

    // Attach newRole after curRole.  If curRole is NULL, newRole is
    // attached to the end of the list.
    virtual void attach(MleRole *curDel, MleRole *newRole);

	//  Detach curRole in the list.
    virtual void detach(MleRole *curRole);

    // This method is used to traverse the scene graph and render each
    // 2D role.
    //
    static void render(MleSet *set);

#ifdef MLE_REHEARSAL

    // This method is used to query the set geometry.
    virtual void getBounds(MlScalar min[3], MlScalar max[3]);
    
    // This method is used for picking in tools.
    virtual MleRole	*pickRole(int x, int y);

    // Rearranging the actors drawing order with this set.

    // pushActor() - Pushes the actor one layer down.  If success, 0 is
    //		     returned; otherwise, -1 is returned.  If the actor is
    //		     already at the bottom, no action is taken and -1 is
    //		     returned.
    virtual int pushActor(MleActor *actor);

    // pushActorToBottom() - Same as pushActor() except the actor is pushed
    //			     to the bottom layer.  If success, 0 is returned;
    //			     otherwise, -1 is returned.  If the actor is
    //			     already at the bottom, no action is taken and -1
    //			     is returned.
    virtual int pushActorToBottom(MleActor *actor);

    // popActor() - Pops the actor one layer up.  If success, 0 is returned;
    //		    otherwise, -1 is returned.  If the actor is already at
    //		    the top, no action is taken and -1 is returned.
    virtual int popActor(MleActor *actor);

    // popActorToTop() - Same as popActor() except the actor is popped to the
    //			 top layer.  If success, 0 is returned; otherwise, -1
    //			 is returned.  If the actor is already at the top, no
    //			 action is taken and -1 is returned.
    virtual int popActorToTop(MleActor *actor);

#endif	/* MLE_REHEARSAL */

    // Beginning of properties.

    // position - contains x and y position in screen coordinate,
    //		  (0, 0) corresponds to the lower left corner of the window.
    MlVector2 position;

    // size - contains width and height in screen coordinate.
    MlVector2 size;

  protected:

    //
    // The headRole is the first role drawn.  This makes the
    // headRole always appear in the bottom layer among all the
    // roles attached to this set.  Similiarly, the last role
    // in this list always appear in the top layer.
    //
    Mle2dRole *m_headRole;
};


#endif	/* __MLE_IV2DSET_H_ */
