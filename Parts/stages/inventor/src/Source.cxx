/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file Source.cxx
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 *
 * This file implements a class that displays the source for snapping operations.
 */

/// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2017-2020 Wizzer Works
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

// Include Inventor header files.
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoMatrixTransform.h>
#include <Inventor/nodes/SoTransform.h>

// Include Magic Lantern Rehearsal Player header files.
#include "Source.h"

/////////////////////////////////////////////////////////////////////////////
//
// Constructor.
//
/////////////////////////////////////////////////////////////////////////////

Source::Source() : SpotMarker(TRUE, "SRC")
{
}

/////////////////////////////////////////////////////////////////////////////
//
// Destructor.
//
/////////////////////////////////////////////////////////////////////////////

Source::~Source()
{
}

/////////////////////////////////////////////////////////////////////////////
//
// Builds the scene graph. Does the usual stuff, then accesses extra
// needed nodes.
//
/////////////////////////////////////////////////////////////////////////////

void
Source::buildSceneGraph()
{
    // Do the normal stuff
    SpotMarker::buildSceneGraph();

#define FIND_NODE(name,class) (class *) findNode(name, class::getClassTypeId())

    matrixTransform = FIND_NODE("matrixTransform", SoMatrixTransform);

#undef FIND_NODE
}

/////////////////////////////////////////////////////////////////////////////
//
// Initializes change to positioning matrix. This is passed the matrix
// to convert from the space in which the delta matrix is applied to
// world space.
//
/////////////////////////////////////////////////////////////////////////////

void
Source::startMatrixChange(const SbMatrix &xf)
{
    matrixChangeBase = xf;
}

/////////////////////////////////////////////////////////////////////////////
//
// Changes positioning matrix based on manipulation.
//
/////////////////////////////////////////////////////////////////////////////

void
Source::changeMatrix(const SbMatrix &deltaMatrix)
{
    matrixTransform->matrix = deltaMatrix * matrixChangeBase;
}

/////////////////////////////////////////////////////////////////////////////
//
// Finishes change to positioning matrix.
//
/////////////////////////////////////////////////////////////////////////////

void
Source::finishMatrixChange()
{
    // Apply the positioning matrix to the source point, normal, and
    // edge endpoints, and update the scene graph
    const SbMatrix &m = matrixTransform->matrix.getValue();
    m.multVecMatrix(location, location);
    m.inverse().transpose().multDirMatrix(normal, normal);
    normal.normalize();

    transform->rotation = SbRotation(SbVec3f(0.0, 1.0, 0.0), normal);
    transform->translation = location;

    if (isScaleSnappingEnabled() && atEdge) {
	m.multVecMatrix(edgeEnd1, edgeEnd1);
	m.multVecMatrix(edgeEnd2, edgeEnd2);
	SbVec3f edgeVec = (edgeEnd2 - edgeEnd1);
	edgeLength = edgeVec.normalize();
	edgeCoords->point.set1Value(0, edgeEnd1);
	edgeCoords->point.set1Value(1, edgeEnd2);
	end1Transform->translation = edgeEnd1;
	end2Transform->translation = edgeEnd2;

	SbVec3f yAxis(0.0, 1.0, 0.0);
	end1Transform->rotation = SbRotation(-yAxis, edgeVec);
	end2Transform->rotation = SbRotation( yAxis, edgeVec);
    }

    // Clear out the matrix transform, if not default
    matrixTransform->setToDefaults();
}

/////////////////////////////////////////////////////////////////////////////
//
// Called when the location changes. Prepares the matrixTransform.
//
/////////////////////////////////////////////////////////////////////////////

void
Source::locationChanged()
{
    if (! matrixTransform->matrix.isDefault())
	matrixTransform->setToDefaults();
}
