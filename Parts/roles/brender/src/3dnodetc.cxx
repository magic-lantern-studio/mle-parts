/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dnodetc.h
 * @ingroup MleParts
 *
 * This file contains the class implementation for setting a node
 * type on a Role.
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

//Include Magic Lantern header files.
#include "mle/mlTypes.h"

// Include BRender target header files.
#include "math/scalarbr.h"
#include "mle/br3drole.h"
#include "mle/3dnodetp.h"
#include "mle/3dnodetc.h"


int Mle3dNodeTypeCarrier::set(MleRole *role, int nodeType)
{ 
	int result = FALSE;

    // Declare pointers to BRender actor and its parent actor.
    br_actor *root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
    br_actor *parent = NULL;

    if ( root )
	{
        // See if we have already created this root node.
        switch (nodeType) 
        {
            case Mle3dNodeTypeProperty::TRANSFORM:
                if (root->type == BR_ACTOR_NONE)
                    result=TRUE;
                 break;
	        case Mle3dNodeTypeProperty::CAMERA:
	            if (root->type == BR_ACTOR_CAMERA)
	                result=TRUE;
	            break;
	        case Mle3dNodeTypeProperty::LIGHT:
	            if (root->type == BR_ACTOR_LIGHT)
	                result=TRUE;
	            break;
	        case Mle3dNodeTypeProperty::GEOMETRY:
	            if (root->type == BR_ACTOR_MODEL)
	                result=TRUE;
	            break;
	        case Mle3dNodeTypeProperty::BOUNDING_BOX:
	            if (root->type == BR_ACTOR_BOUNDS)
	                result=TRUE;
	            break;
	        case Mle3dNodeTypeProperty::CLIPPING_PLANE:
	            if (root->type == BR_ACTOR_CLIP_PLANE)
                    result=TRUE;
	            break;
		}

        if (!result)
        {
	        // Remember the parent.
	        parent = root->parent;

#ifdef MLE_REHEARSAL
            // Remove previous root node from picking registry.
            role->pickRegistry.set(root,NULL);
#endif /* MLE_REHEARSAL */

	        // Take out of the hierarchy.
	        BrActorRemove(root);

	        // and get rid of it
	        //BrActorFree(root); //For some reason this doesn't work properly.
        }
	}

    if (! result)
    {
        // Create a BRender actor of the proper type.
        switch (nodeType) 
        {
	        case Mle3dNodeTypeProperty::TRANSFORM:
	            root = BrActorAllocate(BR_ACTOR_NONE, role);
	            break;
	        case Mle3dNodeTypeProperty::CAMERA:
	            root = BrActorAllocate(BR_ACTOR_CAMERA, NULL);
	            break;
	        case Mle3dNodeTypeProperty::LIGHT:
	            root = BrActorAllocate(BR_ACTOR_LIGHT, NULL);
	            break;
	        case Mle3dNodeTypeProperty::GEOMETRY:
	            root = BrActorAllocate(BR_ACTOR_MODEL, role);
	            break;
	        case Mle3dNodeTypeProperty::BOUNDING_BOX:
	            root = BrActorAllocate(BR_ACTOR_BOUNDS, NULL);
	            break;
	        case Mle3dNodeTypeProperty::CLIPPING_PLANE:
	            root = BrActorAllocate(BR_ACTOR_CLIP_PLANE, NULL);
	            break;
	        default:
	            root = BrActorAllocate(BR_ACTOR_NONE, role);
	            break;
        }
     
        MLE_ASSERT(root);
     
        // Add back to the parent if there is one.
        if ( parent )
	        BrActorAdd(parent,root);
     
        // Assign the new BRender actor to the Role.
        Mle3dRole::cast(role)->m_root = root;

#ifdef MLE_REHEARSAL
        // Default is to set picking on.
        role->pickRegistry.set(root,role);
#endif /* MLE_REHEARSAL */

        result = TRUE;
    }

    return result;
}


int Mle3dNodeTypeCarrier::get(MleRole *role, int& nodeType)
{ 
	int result = FALSE;

    br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
    if (root)
    { 
		switch (root->type)
        {
		    case BR_ACTOR_NONE:
                nodeType = Mle3dNodeTypeProperty::TRANSFORM;
                break;
            case BR_ACTOR_CAMERA:
                 nodeType = Mle3dNodeTypeProperty::CAMERA;
                 break;
            case BR_ACTOR_LIGHT:
                nodeType = Mle3dNodeTypeProperty::LIGHT;
                break;
            case BR_ACTOR_MODEL:
                nodeType = Mle3dNodeTypeProperty::GEOMETRY;
                break;
            case BR_ACTOR_BOUNDS:
                nodeType = Mle3dNodeTypeProperty::BOUNDING_BOX;
                break;
            case BR_ACTOR_CLIP_PLANE:
                nodeType = Mle3dNodeTypeProperty::CLIPPING_PLANE;
                break;
            default:
                nodeType = Mle3dNodeTypeProperty::TRANSFORM;
                break;
        }
        result = TRUE;
    }

    return result;
}

