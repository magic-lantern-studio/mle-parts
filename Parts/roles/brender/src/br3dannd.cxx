/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file br3dammd.cxx
 * @ingroup MleParts
 *
 * This file implements a 3D Annotation Role
 * targeting the BRender platform.
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

#include <stdio.h>

#include "mle/brendapi.h"

#include "mle/br3dannd.h"
#include "mle/3dannoc.h"
#include "mle/mlAssert.h"
#include "mle/scalarbr.h"

#define MAXVERTS 512
#define MAXFACES 256

MLE_ROLE_SOURCE(MleBr3dAnnotationRole,Mle3dRole);

void
Mle3dAnnotationCarrier::clear(MleRole *d,void *context)
{
	// Cast the role to the right type
	MleBr3dAnnotationRole *role = MleBr3dAnnotationRole::cast(d);

	br_actor *actor = (br_actor *)role->dict.find(context);
	if ( actor )
	{
		// Remove stuff:
		//   a model with no geometry is a problem
		//   so define some degenerate model.
		actor->model->nvertices = 1;
		actor->model->nfaces = 1;

		actor->model->faces[0].vertices[0] = 0;
		actor->model->faces[0].vertices[1] = 0;
		actor->model->faces[0].vertices[2] = 0;

		// update the model
		BrModelUpdate(actor->model,BR_MODU_ALL);
	}
}

void
Mle3dAnnotationCarrier::drawLine(MleRole *d, void *context, const MleVector3& from,
  const MleVector3& to, float red, float grn, float blu)
{
	// Cast the role to the right type.
	MleBr3dAnnotationRole *role = MleBr3dAnnotationRole::cast(d);

	br_actor *actor = (br_actor *)role->dict.find(context);
	if ( actor == NULL )
	{
		// Make a new context.
		actor = BrActorAllocate(BR_ACTOR_MODEL, role);
		BrActorAdd(role->root,actor);

		// Add it to the dictionary.
		role->dict.set(context,actor);

		// Add the model.
		actor->model = BrModelAllocate("annomo",MAXVERTS,MAXFACES);
		actor->model->flags = BR_MODF_KEEP_ORIGINAL | BR_MODF_DONT_WELD;

		// A model with no geometry is a problem
		// so define some degenerate model.
		actor->model->nvertices = 1;
		actor->model->nfaces = 1;

		actor->model->faces[0].vertices[0] = 0;
		actor->model->faces[0].vertices[1] = 0;
		actor->model->faces[0].vertices[2] = 0;
		actor->model->vertices[0].p.v[0] = BR_SCALAR(0);
		actor->model->vertices[0].p.v[1] = BR_SCALAR(0);
		actor->model->vertices[0].p.v[2] = BR_SCALAR(0);

		BrModelAdd(actor->model);

		// Add the material
		actor->material = BrMa.terialAllocate("annoma");
		actor->material->flags = BR_MATF_PRELIT | BR_MATF_SMOOTH;
		BrMaterialAdd(actor->material);

		// Render in wireframe.
		actor->render_style = BR_RSTYLE_EDGES;
	}

	br_model *model = actor->model;
	ML_ASSERT(model->nvertices < MAXVERTS);
	ML_ASSERT(model->nfaces < MAXFACES);

	// Add a face.
	//   BRender only renders triangles, so to create a line segment,
	//   we must make a degenerate triangle and render edges.
	model->faces[model->nfaces].vertices[0] = model->nvertices;
	model->faces[model->nfaces].vertices[1] = model->nvertices + 1;
	model->faces[model->nfaces].vertices[2] = model->nvertices;

	model->nfaces++;

	// Add the two vertices.
	model->vertices[model->nvertices].p.v[0] = mlScalarToBRScalar(from[0]);
	model->vertices[model->nvertices].p.v[1] = mlScalarToBRScalar(from[1]);
	model->vertices[model->nvertices].p.v[2] = mlScalarToBRScalar(from[2]);
	model->vertices[model->nvertices].red = (int)255*red;
	model->vertices[model->nvertices].grn = (int)255*grn;
	model->vertices[model->nvertices].blu = (int)255*blu;

	model->nvertices++;

	model->vertices[model->nvertices].p.v[0] = mlScalarToBRScalar(to[0]);
	model->vertices[model->nvertices].p.v[1] = mlScalarToBRScalar(to[1]);
	model->vertices[model->nvertices].p.v[2] = mlScalarToBRScalar(to[2]);
	model->vertices[model->nvertices].red = (int)255*red;
	model->vertices[model->nvertices].grn = (int)255*grn;
	model->vertices[model->nvertices].blu = (int)255*blu;

	model->nvertices++;

	// Update the model.
	BrModelUpdate(actor->model,BR_MODU_ALL);
}

