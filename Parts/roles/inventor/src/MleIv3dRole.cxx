/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dRole.h
 * @ingroup MleParts
 *
 * This file contains the class implementation for a 3D Role targeting
 * the Inventor platfrom.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
//  Copyright (C) 2003-2018  Wizzer Works
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

// Include Inventor header files.
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>

// Include Magic Lantern math header files.
#include "math/transfrm.h"

// Inlclude Magic Lantern parts header files.
#include "mle/3dtransc.h"
#include "mle/MleIv3dRole.h"


MLE_ROLE_SOURCE(Mle3dRole,MleRole);


// Declare picking registry.
#if defined(MLE_REHEARSAL)
MleDwpDict Mle3dRole::g_pickRegistry;
#endif /* MLE_REHEARSAL */


Mle3dRole::Mle3dRole(MleActor *actor, int nodeType)
 : MleRole(actor)
{
    m_root = NULL;
    setNodeType(nodeType);
}

Mle3dRole::~Mle3dRole()
{
    // Remove the node from the graph.
    if ( m_root )
    {
#if defined(MLE_REHEARSAL)
		g_pickRegistry.remove(m_root);

		// Remove ourselves from our parent - this should bring
		// refcount to zero and delete us.
		m_ivParent->removeChild(m_root);
#endif /* MLE_REHEARSAL */
    }
}

int Mle3dRole::setNodeType(int nodeType)
{
    m_root = new SoSeparator;

    SoTransform *xform = new SoTransform;
    m_root->addChild(xform);
    
    switch (nodeType)
    { 
		case Mle3dNodeTypeProperty::CAMERA:
			{
			SoPerspectiveCamera *cam = new SoPerspectiveCamera;
			m_root->addChild(cam);
			}
			break;
		case Mle3dNodeTypeProperty::LIGHT:
			{
			SoDirectionalLight *light = new SoDirectionalLight;
			m_root->addChild(light);
			}
			break;
		case Mle3dNodeTypeProperty::TRANSFORM:
		case Mle3dNodeTypeProperty::GEOMETRY:
		case Mle3dNodeTypeProperty::BOUNDING_BOX:
		case Mle3dNodeTypeProperty::CLIPPING_PLANE:
		default:
			break;
    }
    
    Mle3dTransformCarrier::set(this,MlTransform::identity());

#if defined(MLE_REHEARSAL)
    // Default is to set picking on.
    g_pickRegistry.set(m_root,this);
#endif /* MLE_REHEARSAL */

    return TRUE; 
}


void
Mle3dRole::addChild(Mle3dRole *child)
{
    MLE_ASSERT(m_root);
    MLE_ASSERT(child);
    MLE_ASSERT(m_root->getTypeId().isDerivedFrom(
		SoGroup::getClassTypeId()));

    ((SoGroup *)m_root)->addChild(child->getRoot());
}
