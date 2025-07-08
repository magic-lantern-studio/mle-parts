/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file antirot.cxx
 * @ingroup MlParts
 *
 * This file implements an Inventor node that undoes any rotation applied
 * in the current transformation matrix during scene graph traversal.
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
