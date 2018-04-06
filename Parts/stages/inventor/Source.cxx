/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file Source.cxx
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 * @date Jan 24, 2006
 *
 * This file implements a class that displays the source for snapping operations.
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
