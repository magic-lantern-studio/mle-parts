/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file br3drole.cxx
 * @ingroup MleParts
 *
 * This file contains the class implementation for a 3D Role targeting
 * the BRender platfrom.
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

// Include system header files.
#include <stdio.h>

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

// Include Magic Lantern Math header files.
#include "math/scalarbr.h"
#include "math/transfrm.h"

// Inlclude BRender target header files.
#include "mle/3dtransc.h"
#include "mle/br3drole.h"


MLE_ROLE_SOURCE(Mle3dRole,MleRole);


Mle3dRole::Mle3dRole(MleActor *actor, int nodeType)
  : MleRole(actor)
{
    m_root=NULL;
    setNodeType(nodeType);
}


Mle3dRole::~Mle3dRole()
{
	// Remove the BRender node from the graph.
	if ( m_root && m_root->prev )
	{
		BrActorRemove(m_root);
		BrActorFree(m_root);
	}
}


int Mle3dRole::setNodeType(int nodeType)
{
	int result = FALSE;

    switch (nodeType)
    {
	    case Mle3dNodeTypeProperty::TRANSFORM:
            m_root = BrActorAllocate(BR_ACTOR_NONE, this);
            break;
        case Mle3dNodeTypeProperty::CAMERA:
            m_root = BrActorAllocate(BR_ACTOR_CAMERA, this);
            break;
        case Mle3dNodeTypeProperty::LIGHT:
            m_root = BrActorAllocate(BR_ACTOR_LIGHT, this);
            break;
        case Mle3dNodeTypeProperty::GEOMETRY:
            m_root = BrActorAllocate(BR_ACTOR_MODEL, this);
            break;
        case Mle3dNodeTypeProperty::BOUNDING_BOX:
            m_root = BrActorAllocate(BR_ACTOR_BOUNDS, this);
            break;
        case Mle3dNodeTypeProperty::CLIPPING_PLANE:
            m_root = BrActorAllocate(BR_ACTOR_CLIP_PLANE, this);
            break;
        default:
            m_root = BrActorAllocate(BR_ACTOR_NONE, this);
            break;
    }

    if (m_root)
    {
        m_root->t.type = BR_TRANSFORM_MATRIX34;
        Mle3dTransformCarrier::set(this,MlTransform::identity());
        result = TRUE;
    }

    if (! result)
        printf("Mle3dRole::Error -- could not create Role node.\n");
#ifdef MLE_REHEARSAL
    else
        // Default is to set picking on.
        pickRegistry.set(root,this);
#endif /* MLE_REHEARSAL */


  return result; 
}


void Mle3dRole::addChild(Mle3dRole *child)
{
    MLE_ASSERT(m_root);
    MLE_ASSERT(child);

    // Get the scene graph root of the new child.
    br_actor *childRoot = child->getRoot();
    MLE_ASSERT(childRoot);

    // Attach the child to the scene graph.
    BrActorAdd(m_root,childRoot);
}
