/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dgeorwc.h
 * @ingroup MleParts
 *
 * This file implements the web carrier for a 3D Geometry Registry
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

#include "mle/mlErrno.h"

#include "mle/brendapi.h"
#include "mle/scalarbr.h"
#include "mle/br3drole.h"
#include "mle/mlLoad.h"
#include "math/transfrm.h"
#include "mle/3dgeomrm.h"

#include "mle/3dgeomrc.h"
#include "mle/3dgeorwc.h"
#include "mle/3dgeorwm.h"

void**
Mle3dGeometryRegistryWebCarrier::set(MleRole* role, 
				  MediaRef geometryRegistry,
				  int& geometryRegistrySize)
// Reads in a list of geometry from a geometry registry media ref and sets up
// the BRender scene graph with the geometry nodes.
{ 
	geometryRegistrySize = 0;

	br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
	if (NULL == root)
	{
#ifdef MLE_REHEARSAL
		fprintf(stderr,
			"Mle3dGeometryRegistryWebCarrier::Error--no geometry on role.\n");
#endif /* MLE_REHEARSAL */
		mlSetErrno( MLERR_ROLE_NO_ROOT );
		return NULL;
	}

	if (MLE_NO_MEDIA == geometryRegistry)
	{ 
#ifdef MLE_REHEARSAL
		fprintf(stderr,
			"Mle3dGeometryRegistryWebCarrier::Error--geometry registry property has no value.\n");
		fprintf(stderr,
			"Mle3dGeometryRegistryWebCarrier::Warning--this role has no geometry.\n");
#endif /* MLE_REHEARSAL */
		mlSetErrno( MLEERR_ROLE_NO_MEDIA );
		return NULL;
	}
	
	// Load the geometry registry media reference.
	Mle3dGeometryRegistryWebMediaRef *geometryMedia = 
		(Mle3dGeometryRegistryWebMediaRef*) mlLoadMediaRef(geometryRegistry,NULL);

	if (NULL == geometryMedia)
	{
#ifdef MLE_REHEARSAL
		fprintf(stderr,
			"Mle3dGeometryRegistryWebCarrier::Error--no media reference for geometry registry.\n");
#endif /* MLE_REHEARSAL */
		mlSetErrno( MLERR_ROLE_MEDIA_LOAD_FAILED );
		return NULL;
	}

	// Load state for issuing read() calls later.
	// XXX Watch out! In most callbacks we pass the this ptr
	// for an object of the type of the apply() class.  But in 
	// this case, since there\'s no real prop-role, we pass the
	// media ref.  We\'ll also need the real role, so pass it
	// as client data.
	geometryMedia->setCallback( (void*(*)(void*,void*))
				    Mle3dGeometryRegistryWebCarrier::apply, 
				    (void*) geometryMedia,
				    (void*) role );

	// Cast geometryMedia to MleMediaRef, which handles its own IO.
	// Return success or failure of the read.

	return (void**) geometryMedia->read();
}

// Function prototypes for hidden things.
extern void ml3dGeometryRegistryRoleRemoveGeometry(br_actor* parent);
extern br_matrix34** ml3dGeometryRegistryRoleAddGeometry(br_actor* parent,
	unsigned int geometryRegistrySize, br_model** geometryRegistryList);

// WHOA XXX - We don\'t get a pointer to this struct, instead to 
// something else.  This is pretty bogus.  I guess because of how the
// prop-del is set up.

void
Mle3dGeometryRegistryWebCarrier::apply( MediaRef* ref, MleRole* role )
{
	// The media ref contains the data we want to use.

	Mle3dGeometryRegistryWebMediaRef* geometryMedia = 
		(Mle3dGeometryRegistryWebMediaRef*) ref;

	// Make sure it\'s all there.
	if (NULL != geometryMedia->getList() &&
	    NULL != geometryMedia->getSize())
	{

		// Remove the current set of geometry in this node, 
		// if any.
		br_actor* root = (br_actor*)(Mle3dRole::cast(role)->getRoot());
		if (NULL != root)
		{
			ml3dGeometryRegistryRoleRemoveGeometry(root); 
		
			// Set up the scene graph with the models 
			// from the geometry registry
			// reg->transformList = ...
			(void) ml3dGeometryRegistryRoleAddGeometry
				(root, 
				 geometryMedia->getSize(), 
				 geometryMedia->getList()
				 );
		}
	}

	// So now we hold transformList, which is what our parent wants
	// to get.
	// We have no callback to process, because we don\'t store one.
	// XXX Why Not?
}
