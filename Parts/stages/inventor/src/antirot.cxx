/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file antirot.cxx
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 * @date Jan 24, 2006
 *
 * This file implements an Inventor node that undoes any rotation applied
 * in the current transformation matrix during scene graph traversal.
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

// Include Inventor header files.
#include <Inventor/SoDB.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/elements/SoCacheElement.h>

#include "antirot.h"

// Do standard node creation things.
SO_NODE_SOURCE(MleAntiRotate);

// Constructor.
MleAntiRotate::MleAntiRotate(void)
{
	SO_NODE_CONSTRUCTOR(MleAntiRotate);

	// No fields.

	// In the future, there could be a field to define a preferred
	// axis of rotation.
}

// Destructor.
MleAntiRotate::~MleAntiRotate()
{
}

void
MleAntiRotate::initClass(void)
{
	SO_NODE_INIT_CLASS(MleAntiRotate,SoTransformation,"Transformation");
}

// This function implements the behavior of this node on traversal, which
// is to undo any rotations in the current transformation matrix.
void
MleAntiRotate::doAction(SoAction *action)
{
	// Get the current matrix to this point.
	SoState *state = action->getState();
	SbMatrix objToScreen =
		SoModelMatrixElement::get(state)*
		SoViewingMatrixElement::get(state);
	
	// Factor this matrix to get the pure rotation.
	SbMatrix r,u,p;
	SbVec3f s,t;
	objToScreen.factor(r,s,u,t,p);

	if ( s[0] != 0.0 && s[1] != 0.0 && s[2] != 0.0 )
	{
		// Invalidate the cache.
		SoCacheElement::invalidate(state);

		// Undo the scaling.
		//   Note that because we may have shearing, this requires
		//   rotating back to a coordinate scaling basis.
		SoModelMatrixElement::rotateBy(state,this,r.transpose());
		SoModelMatrixElement::scaleBy(state,this,
			SbVec3f(1.0/s[0],1.0/s[1],1.0/s[2]));
		SoModelMatrixElement::rotateBy(state,this,r);

		// Undo the rotation.
		SoModelMatrixElement::rotateBy(state,this,u.transpose());

		// Put back the scaling.
		SoModelMatrixElement::rotateBy(state,this,r.transpose());
		SoModelMatrixElement::scaleBy(state,this,s);
		SoModelMatrixElement::rotateBy(state,this,r);
	}

}

void
MleAntiRotate::callback(SoCallbackAction *action)
{
	MleAntiRotate::doAction(action);
}

void
MleAntiRotate::GLRender(SoGLRenderAction *action)
{
	MleAntiRotate::doAction(action);
}

void
MleAntiRotate::getMatrix(SoGetMatrixAction *action)
{
	MleAntiRotate::doAction(action);
}

void
MleAntiRotate::pick(SoPickAction *action)
{
	MleAntiRotate::doAction(action);
}
