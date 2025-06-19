/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv3dRole.h
 * @ingroup MleParts
 *
 * This file contains the class implementation for a 3D Role targeting
 * the Inventor platfrom.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2017-2025 Wizzer Works
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

#if defined(_WINDOWS)
// Make sure that the registry can be shared if the library is
// included as part of a DLL.
#pragma data_seg( ".GLOBALS" )
#endif
// Declare picking registry.
#if defined(MLE_REHEARSAL)
MleDwpDict Mle3dRole::g_pickRegistry;
#endif /* MLE_REHEARSAL */
#if defined(_WINDOWS)
#pragma data_seg()
#pragma comment(linker, "/section:.GLOBALS,rws")
#endif


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
