/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file iv2dset.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 2D Set targeting the Inventor
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

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"

#include "mle/2drole.h"
#include "mle/iv2dset.h"
#include "mle/ivstage.h"


MLE_SET_SOURCE(Mle2dSet, MleSet);


#ifdef MLE_REHEARSAL

void
Mle2dSet::initClass(void)
{
    mleRegisterSetClass(Mle2dSet, MleSet);

    mleRegisterSetMember(Mle2dSet, position, MlVector2);
    mleRegisterSetMember(Mle2dSet, size, MlVector2);
}

#endif	/* MLE_REHEARSAL */


Mle2dSet::Mle2dSet(void)
{
    MleIvStage *stage;
    MleSchedulerItem *id;

    // Initialize default property values.
    position.setValue(ML_SCALAR(0), ML_SCALAR(0));
    size.setValue(ML_SCALAR(640), ML_SCALAR(480));

    // Initialize member variables.
    m_headRole = NULL;

    //
    // Add the set to the stage.  For both the rehearsal player and the
    // mastered player on SGI, the stage is in control of the set rendering.
    // No render() function is passed to the scheduler for periodic set
    // rendering.
    //
    stage = MleIvStage::cast(MleStage::g_theStage);
    id = stage->addSet((void (*)(MleSet *)) render, this);
}


Mle2dSet::~Mle2dSet(void)
{
    //
    // Delete all attached roles.  The destructor of each 2D role will
    // call this set's detach() function and update the m_headRole pointer.
    // So when the m_headRole is NULL, all the attached roles are
    // deleted.
    //
    while(m_headRole)
		delete m_headRole;
}


void
Mle2dSet::init()
{
}


void
Mle2dSet::attach(MleRole *curR, MleRole *newR)
{
    Mle2dRole *curRole, *newRole;

    MLE_ASSERT(newR);
    newRole = Mle2dRole::cast(newR);

    if (curR)
	{
		// Attach after "curRole" role..
		curRole = Mle2dRole::cast(curR);

		newRole->m_next = curRole->m_next;
		newRole->m_prev = curRole;
		curRole->m_next = newRole;
		if (newRole->m_next)
			newRole->m_next->m_prev = newRole;

    } else
	{
		// Attach to the end of the list.
		if (m_headRole)
		{
			for (curRole = m_headRole; curRole->m_next;)
				curRole = curRole->m_next;
			curRole->m_next = newRole;
			newRole->m_prev = curRole;
			newRole->m_next = NULL;

		} else
		{
			m_headRole = newRole;
			newRole->m_next = NULL;
			newRole->m_prev = NULL;
		}
    }
}


void
Mle2dSet::detach(MleRole *curRole)
{
    Mle2dRole *role;

    MLE_ASSERT(curRole);
    role = Mle2dRole::cast(curRole);

    if (role == m_headRole)
	{
		if (m_headRole->m_next)
			m_headRole->m_next->m_prev = NULL;
		m_headRole = m_headRole->m_next;

    } else
	{
		role->m_prev->m_next = role->m_next;
		if (role->m_next)
			role->m_next->m_prev = role->m_prev;
    }
}


void
Mle2dSet::render(MleSet *s)
{
    Mle2dSet *set = Mle2dSet::cast(s);
    MleIvStage *stage = MleIvStage::cast(MleStage::g_theStage);

    // Process all 2D roles attached to this set.
    Mle2dRole *nextRole = set->m_headRole;

    while(nextRole != NULL)
	{
		nextRole->draw(NULL);

        nextRole = nextRole->m_next;
    }
}


#ifdef MLE_REHEARSAL

void
Mle2dSet::getBounds(MlScalar min[3], MlScalar max[3])
{
    // XXX - Use set properties for x, y instead of hard coded numbers.
    min[0] = ML_SCALAR_ZERO;
    min[1] = ML_SCALAR_ZERO;
    min[2] = ML_SCALAR(-1);

    max[0] = ML_SCALAR(639);
    max[1] = ML_SCALAR(479);
    max[2] = ML_SCALAR_ZERO;
}


MleRole *
Mle2dSet::pickRole(int x, int y)
{
    Mle2dRole *curRole;
    MlScalar min[2], max[2];

    if (! m_headRole)
		return NULL;

    //
    // Start pick selection from the top layer.
    //
    for (curRole = m_headRole; curRole->m_next;)
		curRole = curRole->m_next;

    for(; curRole; curRole = curRole->m_prev)
	{
		curRole->getBounds(min, max);
		if(x >= mlScalarToLong(min[0]) && y >= mlScalarToLong(min[1]) &&
		   x <= mlScalarToLong(max[0]) && y <= mlScalarToLong(max[1]))
			break;
    }

    return curRole;
}


int
Mle2dSet::pushActor(MleActor *actor)
{
    MleRole *tmpRole;
    Mle2dRole *role, *prevRole;
    int retStat;

    MLE_ASSERT(actor);
    tmpRole = actor->getRole();
    MLE_ASSERT(tmpRole);
    role = Mle2dRole::cast(tmpRole);

    if (role == m_headRole)
		//
		// The role is already at the bottom layer.
		//
		retStat = -1;
    else
	{
		prevRole = role->m_prev;

		detach(role);
		if (prevRole->m_prev)
			//
			// Reattach it after prevRole->prev.
			//
			attach(prevRole->m_prev, role);
		else
		{
			//
			// Reattach it as the head of the list.
			//
			MLE_ASSERT(prevRole == m_headRole);
			prevRole->m_prev = role;
			role->m_next = prevRole;
			role->m_prev = NULL;
			m_headRole = role;
		}

		retStat = 0;
    }

    return retStat;
}


int
Mle2dSet::pushActorToBottom(MleActor *actor)
{
    MleRole *tmpRole;
    Mle2dRole *role;
    int retStat;

    MLE_ASSERT(actor);
    tmpRole = actor->getRole();
    MLE_ASSERT(tmpRole);
    role = Mle2dRole::cast(tmpRole);

    if (role == m_headRole)
		//
		// The role is already at the bottom layer.
		//
		retStat = -1;
    else
	{
		detach(role);
		//
		// Reattach it at the bottom layer (head of the list).
		//
		m_headRole->m_prev = role;
		role->m_next = m_headRole;
		role->m_prev = NULL;
		m_headRole = role;

		retStat = 0;
    }

    return retStat;
}


int
Mle2dSet::popActor(MleActor *actor)
{
    MleRole *tmpRole;
    Mle2dRole *role, *nextRole;
    int retStat;

    MLE_ASSERT(actor);
    tmpRole = actor->getRole();
    MLE_ASSERT(tmpRole);
    role = Mle2dRole::cast(tmpRole);

    if (! role->m_next)
		// The role is already at the top layer.
		retStat = -1;
    else
	{
		nextRole = role->m_next;

		detach(role);
		attach(nextRole, role);

		retStat = 0;
    }

    return retStat;
}


int
Mle2dSet::popActorToTop(MleActor *actor)
{
    MleRole *tmpRole;
    Mle2dRole *role, *nextRole;
    int retStat;

    MLE_ASSERT(actor);
    tmpRole = actor->getRole();
    MLE_ASSERT(tmpRole);
    role = Mle2dRole::cast(tmpRole);

    if (! role->m_next)
		// The role is already at the top layer.
		retStat = -1;
    else
	{
		// Get to the top layer (the end of the list).
		for (nextRole = role->m_next; nextRole->m_next;)
			nextRole = nextRole->m_next;

		detach(role);
		attach(nextRole, role);

		retStat = 0;
    }

    return retStat;
}

#endif	/* MLE_REHEARSAL */
