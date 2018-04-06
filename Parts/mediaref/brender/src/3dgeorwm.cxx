/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 3dgeorwm.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Geometry Media Reference
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

#include <mle/MleMediaRef.h>
#include <mle/refucvt.h>
#include <mle/3dgeorwm.h>

MLE_MEDIAREF_SOURCE(Mle3dGeometryRegistryWebMediaRef,Mle3dGeometryRegistryMediaRef);

Mle3dGeometryRegistryWebMediaRef::Mle3dGeometryRegistryWebMediaRef(void)
{
	// Replace the old ref converter with one that understands URLs
	delRefConverter();
	setRefConverter(new MleReferenceUrlConverter);
}

// Mle3dGeometryRegistryWebMediaRef::~Mle3dGeometryRegistryWebMediaRef(void)
// Unnecessary, since we\'d do nothing but let our parent\'s destructor
// be invoked, which is what the language will do anyhow.


void
Mle3dGeometryRegistryWebMediaRef::setCallback( void*(*CB)(void*, void*), 
    void *c, void *data)
{
	callback = CB;
	client = c;
	clientData = data;
}

void*
Mle3dGeometryRegistryWebMediaRef::read(void)
// Gets all the models from the member buffer and parents them all to 
// the actor which is returned.
// Allows for non-blocking I/O.
{
	char *buffer = mediaReferences->buffer;

	// Set up for failure mode
	setList(NULL);

	// read BRender Model Registry from external reference
	if (NULL == buffer) {
#ifdef MLE_REHEARSAL
		fprintf(stderr, "Mle3dGeometryRegistryWebMediaRef::Error--no location for referenced media.\n");
#endif /* MLE_REHEARSAL */
		return NULL;
	}

	// Cast to our type so we can really use it.
	MleReferenceUrlConverter *ruc = (MleReferenceUrlConverter *)refConvert;

	// Invoke the converter to prepare the local file
	ruc->setReference(buffer);

	// Load state for issuing read() calls later.
	ruc->setCallback( (void*(*)(void*,void*))Mle3dGeometryRegistryWebMediaRef::apply, this, NULL );

	return (void*) ruc->read();
}

void*
Mle3dGeometryRegistryWebMediaRef::apply(Mle3dGeometryRegistryWebMediaRef *mref,
				       void *)
{
	MleReferenceUrlConverter *refConvert = (MleReferenceUrlConverter *)mref->refConvert;

	// Means we downloaded successfully to buffer
	if (TRUE == refConvert->isLoaded())
	{
		char *filename = refConvert->getFilename();
		unsigned int i;
		mref->setList(Mle3dGeometryRegistryMediaRef::loadModelRegistry(filename, i));
		mref->setSize(i);
		refConvert->unlinkFile();
	}
	else {
		mref->setList(NULL);
		mref->setSize(0);
	}

#ifdef MLE_REHEARSAL
	if (!(mref->getList() && mref->getSize()))
		fprintf(stderr,"Mle3dGeometryRegistryMediaRef::Warning--no geometry found in geometry registry.\n");
#endif /* MLE_REHEARSAL */
	
	if (NULL != mref->callback && NULL != mref->client)
	{
		return (*mref->callback)( mref->client, mref->clientData );
	}
	else
	{
		return NULL;
	}
}
