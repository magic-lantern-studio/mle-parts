/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file iv3dset.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Set targeting the Inventor
 * platform.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2018-2025 Wizzer Works
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
#include <Inventor/SbLinear.h>
#include <Inventor/SbBox.h>
#include <Inventor/SoPath.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTransformSeparator.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/sensors/SoFieldSensor.h>

// Include Magic Lantern header files.
#include "mle/mlAssert.h"
#include "mle/mlMalloc.h"

#include "math/scalar.h"
#include "math/vector.h"
#include "math/rotation.h"
#include "math/transfrm.h"

#include "mle/MleStageFuncs.h"
#include "mle/iv3dset.h"
#include "mle/MleIv3dRole.h"
#include "mle/ivstage.h"

MLE_SET_SOURCE(Mle3dSet,MleSet);

#ifdef MLE_DIGITAL_WORKPRINT
void
Mle3dSet::initClass(void)
{
	mleRegisterSetClass(Mle3dSet,MleSet);

	// Set properties go here.
}
#endif /* MLE_DIGITAL_WORKPRINT */

Mle3dSet::Mle3dSet(void)
{
	// Create the Set root.
	m_root = new SoSeparator;
	m_root->ref();

	// Create the Set camera.
	//   There are two possible cameras, one orthographic and one
	//   perspective
	m_cameraSwitch = new SoSwitch;
	m_root->addChild(m_cameraSwitch);

	// Create the perspective camera.
	SoPerspectiveCamera *perspectiveCamera = new SoPerspectiveCamera;
	m_cameraSwitch->addChild(perspectiveCamera);
	perspectiveCamera->position.setValue(0,0,3);

	// Create the orthographic camera.
	SoOrthographicCamera *orthographicCamera = new SoOrthographicCamera;
	m_cameraSwitch->addChild(orthographicCamera);
	orthographicCamera->position.setValue(0,0,3);

	// Use the perspective camera by default.
	m_cameraSwitch->whichChild.setValue(0);
	
	// XXX temporary

#ifdef MLE_DIGITAL_WORKPRINT
	// Create a headlight.
	SoTransformSeparator *lightSep = new SoTransformSeparator;
	m_root->addChild(lightSep);
	m_lightRot = new SoRotation;
	SoLight *light = new SoDirectionalLight;
	lightSep->addChild(m_lightRot);
	lightSep->addChild(light);
	m_lightRot->rotation.setValue(getCamera()->orientation.getValue());
	
	// Add a sensor on camera so headlight points same dir as camera.
	SoFieldSensor *sensor = 
	    new SoFieldSensor((SoSensorCB *) cameraCB, this);
	sensor->attach(&getCamera()->orientation);
	sensor->setPriority(0);
#endif /* MLE_DIGITAL_WORKPRINT */

	// Attach the Set scene graph to the stage
	//   This is a stage-specific protocol, not a base class feature
    //MleIvStage *stage = MleIvStage::cast(MleStage::g_theStage);
    //stage->addSet(NULL, this);
}

void
Mle3dSet::init()
{
    // Attach the Set scene graph to the stage
    //   This is a stage-specific protocol, not a base class feature
    MleStage *stage = MleStage::g_theStage;
    //MleIvStage *ivstage = MleIvStage::cast(MleStage::g_theStage);
    MleIvStage *ivstage = MleIvStage::cast(stage);
    ivstage->addSet(NULL, this);
}

void
Mle3dSet::attach(MleRole *p,MleRole *c)
{
	// Make sure we really have a child.
	MLE_ASSERT(c);

	// Cast pointers to Inventor roles.
	Mle3dRole *parent = p ? Mle3dRole::cast(p) : NULL;
	Mle3dRole *child  = Mle3dRole::cast(c);

	// See if we are attaching to another role or to the set.
	if ( parent )
	{
		// It's a role.
		parent->addChild(child);
#ifdef MLE_REHEARSAL
		child->setIvParent(parent->getRoot());
#endif /* MLE_REHEARSAL */
	}
	else
	{
		// It's the set.
		m_root->addChild(child->getRoot());
#ifdef MLE_REHEARSAL
		child->setIvParent(m_root);
#endif /* MLE_REHEARSAL */
	}
}

void*
Mle3dSet::operator new(size_t tSize)
{
    void* p = mlMalloc(tSize);
    return p;
}

void*
Mle3dSet::operator new[](size_t tSize)
{
    void* p = mlMalloc(tSize);
    return p;
}

void
Mle3dSet::operator delete(void* p)
{
    mlFree(p);
}

void
Mle3dSet::operator delete[](void* p)
{
    mlFree(p);
}

//////////////////////////////////////////////////////////////////////
// camera api

SoCamera *
Mle3dSet::getCamera(void) const
{
	return (SoCamera *)m_cameraSwitch->getChild(
		m_cameraSwitch->whichChild.getValue());
}

int Mle3dSet :: setCameraTransform(MlTransform *t)
{
    // Set position.
    MlVector3 translation;
    t->getTranslation(translation);
    setCameraPosition(&translation);
    
    // Factor out pure rotation piece from transform.
    MlTransform junkRot, rot, junkProj;
    MlVector3 junkScale, junkTranslation;
    if (!t->factor(junkRot, junkScale, rot, junkTranslation, junkProj))
    {
		printf("WARNING: Mle3dSet(iv)::setCameraTransform: can't factor\n");
		return -1;
    }

    MlRotation orient(rot);
    setCameraOrientation(&orient);

#ifdef MLE_REHEARSAL
    // Notify stage so it can sync its camera.
    MleIvStage *stage = MleIvStage::cast(MleStage::g_theStage);
    stage->setCameraMoved(this, getCamera());
#endif /* MLE_REHEARSAL */

    return 0;
}


void Mle3dSet :: getCameraTransform(MlTransform *t)
{
    t->identity();
    
    // Get orientation.
    MlRotation rotation;
    getCameraOrientation(&rotation);
    rotation.getValue(*t);

    // Add translation to matrix.
    MlVector3 translation;
    getCameraPosition(&translation);
    t->setTranslationOnly(translation);
    
    // Ignore scaling.
}

int
Mle3dSet::setCameraPosition(const MlVector3 *position)
{
    MLE_ASSERT(position);

	// Get the camera.
	SoCamera *camera = getCamera();

	camera->position.setValue(
		mlScalarToFloat((*position)[0]),
		mlScalarToFloat((*position)[1]),
		mlScalarToFloat((*position)[2]));

#ifdef MLE_REHEARSAL
    // Notify stage so it can sync its camera.
    MleIvStage *stage = MleIvStage::cast(MleStage::g_theStage);
    stage->setCameraMoved(this, camera);
#endif /* MLE_REHEARSAL */
	return 0;
}

void
Mle3dSet::getCameraPosition(MlVector3 *position)
{
    MLE_ASSERT(position);

	// Get the camera.
	SoCamera *camera = getCamera();

    for (int i = 0; i < 3; i++)
		(*position)[i] = mlFloatToScalar(camera->position.getValue()[i]);
//    *(SbVec3f *)position = camera->position.getValue();
}

int
Mle3dSet::setCameraOrientation(const MlRotation *orientation)
{
    const MlRotation &orient = *orientation;
    
	// Get the camera.
	SoCamera *camera = getCamera();

    // Set the orientation by quaternion.
    camera->orientation.setValue(
	mlScalarToFloat(orient[0]),
	mlScalarToFloat(orient[1]),
	mlScalarToFloat(orient[2]),
	mlScalarToFloat(orient[3]));

#ifdef MLE_REHEARSAL
    // Notify stage so it can sync its camera.
    MleIvStage *stage = MleIvStage::cast(MleStage::g_theStage);
    stage->setCameraMoved(this, camera);
#endif /* MLE_REHEARSAL */

    // Any input is allowable, so return success.
    return 0;
}

void
Mle3dSet::getCameraOrientation(MlRotation *orientation)
{
	// Get the camera.
	SoCamera *camera = getCamera();

    const SbRotation &rot = camera->orientation.getValue();
    float f[4];
    rot.getValue(f[0], f[1], f[2], f[3]);
    for (int i = 0; i < 4; i++)
		(*orientation)[i] = mlFloatToScalar(f[i]);
}

int
Mle3dSet::setCameraNearClipping(MlScalar nearPlane)
{
	// Get the camera.
	SoCamera *camera = getCamera();

	camera->nearDistance.setValue(mlScalarToFloat(nearPlane));

#ifdef MLE_REHEARSAL
    // Notify stage so it can sync its camera.
    MleIvStage *stage = MleIvStage::cast(MleStage::g_theStage);
    stage->setCameraMoved(this, camera);
#endif /* MLE_REHEARSAL */
    
	return 0;	// This means success.
}

MlScalar
Mle3dSet::getCameraNearClipping(void)
{
	// Get the camera.
	SoCamera *camera = getCamera();

	return mlFloatToScalar(camera->nearDistance.getValue());
}

int
Mle3dSet::setCameraFarClipping(MlScalar farPlane)
{
	// Get the camera.
	SoCamera *camera = getCamera();

	camera->farDistance.setValue(mlScalarToFloat(farPlane));

#ifdef MLE_REHEARSAL
    // Notify stage so it can sync its camera.
    MleIvStage *stage = MleIvStage::cast(MleStage::g_theStage);
    stage->setCameraMoved(this, camera);
#endif /* MLE_REHEARSAL */

	return 0;	// This means success.
}

MlScalar
Mle3dSet::getCameraFarClipping(void)
{
	// Get the camera.
	SoCamera *camera = getCamera();

	return mlFloatToScalar(camera->farDistance.getValue());
}

int
Mle3dSet::setCameraAspect(MlScalar aspect)
{
	// Get the camera.
	SoCamera *camera = getCamera();

	// Special case zero to mean set with the stage aspect ratio.
	if ( aspect == ML_SCALAR_ZERO )
	{
		// Get the stage size.
		int width,height;
		getStageSize(&width, &height);

	    // Overwrite the aspect.
	    aspect = mlDiv(mlLongToScalar(width),mlLongToScalar(height));
	}

    camera->aspectRatio.setValue(mlScalarToFloat(aspect));

#ifdef MLE_REHEARSAL
    // Notify stage so it can sync its camera.
    MleIvStage *stage = MleIvStage::cast(MleStage::g_theStage);
    stage->setCameraMoved(this, camera);
#endif /* MLE_REHEARSAL */

    return 0;	// This means success.
}

MlScalar
Mle3dSet::getCameraAspect(void)
{
	// Get the camera.
	SoCamera *camera = getCamera();

	return mlFloatToScalar(camera->aspectRatio.getValue());
}

#ifdef MLE_DIGITAL_WORKPRINT
// Camera rotation sensor callback.
// static
void Mle3dSet::cameraCB(Mle3dSet *set, SoFieldSensor *)
{
    set->m_lightRot->rotation.setValue(
	set->getCamera()->orientation.getValue());
}
#endif /* MLE_DIGITAL_WORKPRINT */

// Convenience function.
void
Mle3dSet :: getStageSize(int *width, int *height)
{
#ifdef MLE_DIGITAL_WORKPRINT
    MleStage::g_theStage->getSize(width, height);
#else
    // XXX - quick fix for build.
    *width = 640;
    *height = 480;
#endif /* MLE_DIGITAL_WORKPRINT */
}

#ifdef MLE_REHEARSAL

//////////////////////////////////////////////////////////////////////
//
// Sending set functional info to the tools.
//

const char** Mle3dSet::getFunctions()
{
    static char *funcs[] = 
    {
		NULL
    };
    
    return (const char**)funcs;
}

const char** Mle3dSet::getFunctionAttributes(char* functionName)
{
    static char *renderFuncs[] =
    {
        (char *)RENDER_AS_IS,
        (char *)RENDER_HIDDEN_LINE,
        (char *)RENDER_NO_TEXTURE,
        (char *)RENDER_LOWRES,
        (char *)RENDER_WIREFRAME,
        (char *)RENDER_POINTS,
        (char *)RENDER_BBOX,
		NULL
    };

    if (!strcmp(functionName, SET_ATTR_RENDER_MODES))
    {
		return (const char **)renderFuncs;
    }
    
    printf("ERROR Mle3dSet::getFunctionAttributes: unknown func name '%s'\n", 
	functionName);
    return NULL;
}

#endif	/* MLE_REHEARSAL */

#ifdef MLE_REHEARSAL

// Returns the point of intersection of a ray cast into the screen
// and geometry in the scene, from a given screen x,y position.

void 
Mle3dSet::intersectScreenCoordinates(int x, int y, MlScalar* coord)
{
    MLE_ASSERT(m_root);

    // Get the stage.
    MleIvStage *stage = MleIvStage::cast(MleStage::g_theStage);

    // Call inventor to do the intersection.

    int width, height;
    getStageSize(&width, &height);
    SoRayPickAction *pickAction = 
	new SoRayPickAction(SbViewportRegion(width, height));
    // Invert height because iv screen coords start in lower left.
    SbVec2s point(x, height - y);
    pickAction->setPoint(point);
    pickAction->apply(m_root);

    const SoPickedPoint *pickedPoint = pickAction->getPickedPoint();
    if (pickedPoint != NULL)
    {
		// Set the point of intersection.
		const SbVec3f &vec = pickedPoint->getPoint();
		coord[0] = mlFloatToScalar(vec[0]);
		coord[1] = mlFloatToScalar(vec[1]);
		coord[2] = mlFloatToScalar(vec[2]);
    }
    else
    {
		coord[0] = coord[1] = coord[2] = ML_SCALAR_ZERO;
    }
}

#endif	/* MLE_REHEARSAL */

#ifdef MLE_REHEARSAL

void 
Mle3dSet::projectScreenCoordinates(int x,int y,MlScalar* coord)
{
    // Get the stage width and height.
    int width,height;
    MleStage::g_theStage->getSize(&width,&height);

	// Get the camera.
	SoCamera *camera = getCamera();

    // For some reason the getViewVolume() call needs to have
    // the current screen aspect ratio passed in, rather than
    // using the camera's settings (if no param passed).
    float aspectRatio = (float)width/(float)height;
    SbViewVolume viewVol = camera->getViewVolume(aspectRatio);
    SbPlane plane = viewVol.getPlane(20.0);

    // Get normalized device coords.
    SbVec2f point;
    point[0] = (float)(x)/(float)width;
    point[1] = (float)(height - y)/(float)height;

    SbLine line;
    viewVol.projectPointToLine(point, line);
    SbVec3f intersection;
    if (plane.intersect(line, intersection))
    {
		for (int i = 0; i < 3; i++)
			coord[i] = mlFloatToScalar(intersection[i]);
    }
    else
    {
		printf("NOTE Mle3dSet(iv)::projectScreenCoordinates: no intersection\n");
		coord[0] = coord[1] = coord[2] = ML_SCALAR_ZERO;
    }
}

SoPath *
Mle3dSet::getPickPath(const SbVec2s &point)
{
    int width, height;
    SoPath *path, *tmpPath;
    SoPickedPoint *pickPoint;
    SoRayPickAction *pickAction;

    MLE_ASSERT(m_root);

    //
    // Call inventor to do the intersection.
    //
    getStageSize(&width, &height);
    pickAction = new SoRayPickAction(SbViewportRegion(width, height));
    pickAction->setPoint(point);
    pickAction->apply(m_root);

    pickPoint = pickAction->getPickedPoint();
    if (pickPoint)
	{
		tmpPath = pickPoint->getPath();
		if(tmpPath)
			path = tmpPath->copy();
		else
			path = NULL;
    }
    else
		path = NULL;

    delete pickAction;

    return path;
}

#endif	/* MLE_REHEARSAL */
