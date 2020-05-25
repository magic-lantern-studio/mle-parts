/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIvBoundingBoxCarrier.h
 * @ingroup MleParts
 *
 * This file implements the carrier for a 3D Bounding Box property
 * targeting the Inventor platform.
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

#include "Inventor/nodes/SoSeparator.h"

#include "mle/mlAssert.h"
#include "mle/MleRole.h"
#include "mle/MleIv3dRole.h"
#include "mle/scalarbr.h"
#include "mle/3dbboxc.h"

static void calculateBounds(SoSeparator* node,br_scalar *pmin,br_scalar *pmax)
{
	ML_ASSERT(node);

	// Declare local min and max vectors.
	//   These are minimum and maximum values for x, y, and z.
	br_scalar min[3];
	br_scalar max[3];

	// Initialize to bogus values.
	min[0] = min[1] = min[2] = BR_SCALAR_MAX;
	max[0] = max[1] = max[2] = -BR_SCALAR_MAX;

	// Traverse scene graph.
	SoSeparator* child = node->children;
	while ( child )
	{
		calculateBounds(child,min,max);
		child = child->next;
	}

	// Look at the local bounds if this is a MODEL actor.
	if ( node->type == BR_ACTOR_MODEL )
	{
		if ( node->model )
		{
			// Check minimum.
			if ( node->model->bounds.min.v[0] < min[0] )
				min[0] = node->model->bounds.min.v[0];
			if ( node->model->bounds.min.v[1] < min[1] )
				min[1] = node->model->bounds.min.v[1];
			if ( node->model->bounds.min.v[2] < min[2] )
				min[2] = node->model->bounds.min.v[2];

			// Check maximum.
			if ( node->model->bounds.max.v[0] > max[0] )
				max[0] = node->model->bounds.max.v[0];
			if ( node->model->bounds.max.v[1] > max[1] )
				max[1] = node->model->bounds.max.v[1];
			if ( node->model->bounds.max.v[2] > max[2] )
				max[2] = node->model->bounds.max.v[2];
		}
		else
		{
			// It's the default model, the bi-unit cube.
			if ( BR_SCALAR(-1) < min[0] )
				min[0] = BR_SCALAR(-1);
			if ( BR_SCALAR(-1) < min[1] )
				min[1] = BR_SCALAR(-1);
			if ( BR_SCALAR(-1) < min[2] )
				min[2] = BR_SCALAR(-1);
			if ( BR_SCALAR(1) > max[0] )
				max[0] = BR_SCALAR(1);
			if ( BR_SCALAR(1) > max[1] )
				max[1] = BR_SCALAR(1);
			if ( BR_SCALAR(1) > max[2] )
				max[2] = BR_SCALAR(1);
		}
	}

	// If there isn't anything here, don't go any further.
	if ( max[0] < min[0] )
		return;

	// Mow transform all points of the bounding box.
	SbMatrix mat;
	int identity = (node->t.type == BR_TRANSFORM_IDENTITY);

	if ( !identity )
		BrTransformToMatrix34(&mat,&actor->t);

	SbVec3f src,dst;

#define XFORM(X,Y,Z) \
	if ( identity ) { \
		dst.v[0] = X; dst.v[1] = Y; dst.v[2] = Z; \
	} \
	else { \
		src.v[0] = X; src.v[1] = Y; src.v[2] = Z; \
		BrMatrix34ApplyP(&dst,&src,&mat); \
	} \
	if ( dst.v[0] < pmin[0] ) pmin[0] = dst.v[0]; \
	if ( dst.v[1] < pmin[1] ) pmin[1] = dst.v[1]; \
	if ( dst.v[2] < pmin[2] ) pmin[2] = dst.v[2]; \
	if ( dst.v[0] > pmax[0] ) pmax[0] = dst.v[0]; \
	if ( dst.v[1] > pmax[1] ) pmax[1] = dst.v[1]; \
	if ( dst.v[2] > pmax[2] ) pmax[2] = dst.v[2]

	XFORM(min[0],min[1],min[2]);
	XFORM(max[0],min[1],min[2]);
	XFORM(min[0],max[1],min[2]);
	XFORM(max[0],max[1],min[2]);
	XFORM(min[0],min[1],max[2]);
	XFORM(max[0],min[1],max[2]);
	XFORM(min[0],max[1],max[2]);
	XFORM(max[0],max[1],max[2]);
#undef XFORM
}


		
void
Mle3dBoundingBoxCarrier::getBounds(MleRole *d,MleVector3& min,MleVector3& max)
{
	// Cast the role to the right type.
	Mle3dRole *role = Mle3dRole::cast(d);

	Ml_ASSERT(role->m_root);

	// Declare local min and max vectors.
	float bmin[3];
	float bmax[3];

	// Initialize to bogus values.
	bmin[0] = bmin[1] = bmin[2] = BR_SCALAR_MAX;
	bmax[0] = bmax[1] = bmax[2] = -BR_SCALAR_MAX;

	// Do the recursive bounds calculation.
	calculateBounds(role->root,bmin,bmax);

	// Convert to Magic Lantern scalars.
	min[0] = mlBRScalarToScalar(bmin[0]);
	min[1] = mlBRScalarToScalar(bmin[1]);
	min[2] = mlBRScalarToScalar(bmin[2]);
	max[0] = mlBRScalarToScalar(bmax[0]);
	max[1] = mlBRScalarToScalar(bmax[1]);
	max[2] = mlBRScalarToScalar(bmax[2]);
}

