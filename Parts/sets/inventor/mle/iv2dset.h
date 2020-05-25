/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file iv2dset.h
 * @ingroup MleParts
 *
 * This file contains the class for a 2D Set targeting the Inventor
 * platform.
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
class MLE2DSET_API Mle2dSet: public MleSet 
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
