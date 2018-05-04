/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file Nudger.cxx
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 * @date Jan 24, 2006
 *
 * This file implements a class that allows objects to be "nudged" a little
 * bit in any screen space direction.
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

// Include Inventor header files
#include <Inventor/manips/SoTransformManip.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>

// Include Magic Lantern header files.
#include <mle/mlAssert.h>

//#include "internal/i18n.h"

#include "Nudger.h"
#include <mle/MleActor.h>
#include <math/transfrm.h>

// Shorthand
//#define SCENE_MGR CoManagerManager::getSceneManager()

// mvo
#undef ASSERT
#define ASSERT	MLE_ASSERT

/////////////////////////////////////////////////////////////////////////////
//
// This structure stores everything that is needed to nudge an
// object. One of these is allocated for each object being nudged.
//
/////////////////////////////////////////////////////////////////////////////

struct NudgerObjectInfo {
    SoTransform	*xf;		// Object's transform node to change
    SbVec3f	worldCenter;	// World-space center of object's bbox
    SbVec3f	screenCenter;	// Screen-space center of object's bbox

    // Vectors along which to translate to nudge 1 pixel in each of
    // the 3 directions (only 2 used if orthographic)
    SbVec3f	translationDir[3];
};

/////////////////////////////////////////////////////////////////////////////
//
// Constructor takes the selection context in which to work.
//
/////////////////////////////////////////////////////////////////////////////

Nudger::Nudger()
{
//    ASSERT(context != NULL);
//    selectionContext = context;

    // This records the last undo item we created. If we are about to
    // perform an operation and this is still the last item in the
    // undo stack, we don't need to start a new operation. This allows
    // multiple nudges to be compressed into a single undoable operation.
//    lastUndoItem = NULL;

    curObject = -1;
    
    // mvo additions for firewalker - from CoSceneManager

    if (actionViewportRegion == NULL)
	actionViewportRegion = new SbViewportRegion;
}

/////////////////////////////////////////////////////////////////////////////
//
// Destructor.
//
/////////////////////////////////////////////////////////////////////////////

Nudger::~Nudger()
{
    for (int i = 0; i < infos.getLength(); i++)
	delete (struct NudgerObjectInfo *) infos[i];
}

/////////////////////////////////////////////////////////////////////////////
//
// Nudges all selected objects by the given number of pixels in
// the given direction (X=0, Y=1, or Z=2). numPixels can be
// negative.
//
/////////////////////////////////////////////////////////////////////////////

void
Nudger::nudge(SoPath *selection, MleActor *actor, 
	      const SbViewVolume &viewVol, const SbViewportRegion &vpReg,
	      int direction, int numPixels)
{
    // make sure path stays around for us
    selection->ref();
    
    // Set up the view
    setView(viewVol, vpReg);

    // Set up undo information 
    int numSelected = 1;
    prepareForNudge(numSelected);

    // Do this for each selected object
    for (int i = 0; i < numSelected; i++)
	nudgeObject(selection, actor, direction, numPixels);

//    SCENE_MGR->setDirty();

    selection->unref();
}

/////////////////////////////////////////////////////////////////////////////
//
// Sets the view volume and viewport information to use for
// subsequent nudges.
//
/////////////////////////////////////////////////////////////////////////////

void
Nudger::setView(const SbViewVolume &vol, const SbViewportRegion &vpReg)
{
    viewVol = vol;
    pixelSize = 1.0 / vpReg.getViewportSizePixels()[0];

    // Set the viewport region for subsequent matrix inquiries
    setActionViewportRegion(vpReg);
}

/////////////////////////////////////////////////////////////////////////////
//
// Begins a nudge operation. This should be called before nudging
// any objects with nudge() to allow undo to be set up. The number
// of objects that will be nudged is passed in.
//
/////////////////////////////////////////////////////////////////////////////

void
Nudger::prepareForNudge(int _numObjects)
{
//    SoUndoBuffer *undoBuffer = CoManagerManager::getUndoBuffer();

    // Begin an undo operation unless the previous one was also a nudge
//    if (lastUndoItem == NULL || lastUndoItem != undoBuffer->getLastItem()) {
//	undoBuffer->startOperation(i18n::getMsg("nudgeMsg"));
	needToSetUp = TRUE;
	numObjects = _numObjects;

	// Make sure we have enough info structures
	allocateInfos();
//    }
//    else {
//	ASSERT(numObjects == _numObjects);
//	needToSetUp = FALSE;
//    }

    // Start with the first object
    curObject = 0;
}

/////////////////////////////////////////////////////////////////////////////
//
// Allocates numObjects NudgerObjectInfos structures.
//
/////////////////////////////////////////////////////////////////////////////

void
Nudger::allocateInfos()
{
    int curNumInfos = infos.getLength();

    // Allocate and store these from the end toward the
    // beginning. That way the list will grow only once.
    for (int i = numObjects; i >= curNumInfos; i--)
	infos[i] = new NudgerObjectInfo;
}

/////////////////////////////////////////////////////////////////////////////
//
// Nudges the given selected object.
//
/////////////////////////////////////////////////////////////////////////////

void
Nudger::nudgeObject(SoPath *selection, MleActor *actor, 
		    int direction, int numPixels)
{
    ASSERT(selection != NULL);
    ASSERT(curObject >= 0 && curObject < numObjects);

    SbBool isOrtho =
	(viewVol.getProjectionType() == SbViewVolume::ORTHOGRAPHIC);

    // Nudging in Z is useless in an orthographic view
    ASSERT(! (isOrtho && direction == 2));

    // mvo - get object pointed to by path
    SoTransformManip *selectedManip = (SoTransformManip *) selection->getTail();
    MLE_ASSERT(selectedManip->isOfType(SoTransformManip::getClassTypeId()));

    NudgerObjectInfo *info = (struct NudgerObjectInfo *) infos[curObject];

    // If this isn't another in a succession of nudges, set up
    if (needToSetUp) {
	// Get the transformation of the object. Since only selected
	// objects can be nudged, we know that this is actually a manip of
	// some type. If it isn't, we can't do anything with it.
	info->xf = selectedManip;

	if (! info->xf->isOfType(SoTransformManip::getClassTypeId()))
	    info->xf = NULL;

	else {
	    // Get the center of the world-space bounding box of the object
	    // and also project it into screen space
	    SbBox3f bbox = getBoundingBox(selection, actor);
	    info->worldCenter = bbox.getCenter();

	    viewVol.projectToScreen(info->worldCenter, info->screenCenter);
//info->screenCenter[0] = info->screenCenter[1] = info->screenCenter[2] = 0.5;

	    // Set up an undo operation. For maximum efficiency and
	    // correctness, use a TransformUndo item.
//	    SoTransformManip *xfManip = (SoTransformManip *) info->xf;
//	    lastUndoItem = new TransformUndo(xfManip);
//	    CoManagerManager::getUndoBuffer()->saveItem(lastUndoItem);

	    // Do other setup based on the view type
	    if (isOrtho)
		setUpOrthographic();
	    else
		setUpPerspective(selection);
	}
    }

    // "Forward" and "back" are reversed because of the depth values
    if (direction == 2)
	numPixels = -numPixels;

    // Translate the object
    if (info->xf != NULL) {
	info->xf->translation = (info->xf->translation.getValue() +
			     numPixels * info->translationDir[direction]);
    }

    // Get ready for the next object
    curObject++;
}

/////////////////////////////////////////////////////////////////////////////
//
// Sets up for nudging in a perspective view.
//
/////////////////////////////////////////////////////////////////////////////

void
Nudger::setUpPerspective(SoPath *selection)
{
    NudgerObjectInfo *info = (struct NudgerObjectInfo *) infos[curObject];

    // Compute the cumulative transformation matrix for the path
    // to the object, NOT including the xf transform node
    const SbMatrix &objToWorld =
	getMatrix(selection);
//	getMatrix(selection->getSelectedPath());

    // Compute the transformation matrix for the transform node itself
    SbMatrix xfMat, xfInv;
    info->xf->getRotationSpaceMatrix(xfMat, xfInv);

    // Combine these
    SbMatrix totalMat = xfMat * objToWorld;

    // Now we want to find which of the object-space principal axes is
    // closest to the desired screen-space direction.

    // Project the points 1 unit along each of the object-space
    // principal axes (after being transformed into world space)
    // into screen space. Then compute the screen-space distances
    // from the center to the axis points
    SbVec3f objectDirections[3];
    SbVec3f worldDirections[3];
    SbVec3f worldPoints[3];
    SbVec3f screenDistances[3];
	int dim;
    for (dim = 0; dim < 3; dim++) {

	// Compute the world-space axis direction
	objectDirections[dim].setValue(0.0, 0.0, 0.0);
	objectDirections[dim][dim] = 1.0;
	totalMat.multDirMatrix(objectDirections[dim], worldDirections[dim]);
	worldDirections[dim].normalize();

	// Compute the world space point along the axis
	worldPoints[dim] = info->worldCenter + worldDirections[dim];

	// Project to screen space and find the distance
	viewVol.projectToScreen(worldPoints[dim], worldPoints[dim]);
	screenDistances[dim] = worldPoints[dim] - info->screenCenter;
    }

    // For each of the three directions, see which axis changes the
    // most in that direction
    SbBool foundDim[3];
    int maxDim[3];
	int i;
    for (i = 0; i < 3; i++)
	foundDim[i] = FALSE;
    for (i = 0; i < 3; i++) {
	float maxDist = 0.0;
	maxDim[i] = -1;
	for (dim = 0; dim < 3; dim++) {

	    // Don't reuse one we've already found
	    if (foundDim[dim])
		continue;

	    float dist = screenDistances[dim][i];
	    if (dist < 0.0)
		dist = -dist;
	    if (dist > maxDist) {
		maxDim[i] = dim;
		maxDist = dist;
	    }
	}
	ASSERT(maxDim[i] >= 0);
	foundDim[maxDim[i]] = TRUE;
    }

    // They should all be different
    ASSERT(maxDim[0] != maxDim[1] &&
	   maxDim[0] != maxDim[2] &&
	   maxDim[1] != maxDim[2]);

    // Compute the translation direction for each direction and the
    // translation amount to move 1 pixel in each
    SbVec3f transAmount;
    for (i = 0; i < 3; i++) {
	xfMat.multDirMatrix(objectDirections[maxDim[i]],
			    info->translationDir[i]);
	info->translationDir[i].normalize();

	// Because SbViewVolume::projectToScreen() returns z values
	// that vary non-linearly, we can't just base the translation
	// amount on the z distance. So for front-to-back nudging, we 
	// use the average of the left-right and top-bottom amounts,
	// with the appropriate sign.
	if (i < 2)
	    transAmount[i] = pixelSize / screenDistances[maxDim[i]][i];
	else {
	    transAmount[i] =
		(fabs(transAmount[0]) + fabs(transAmount[1])) / 2.0;
	    if (screenDistances[maxDim[i]][i] < 0.0)
		transAmount[i] = -transAmount[i];
	}

	info->translationDir[i] *= transAmount[i];
    }
}

/////////////////////////////////////////////////////////////////////////////
//
// Sets up for nudging in an orthographic view.
//
/////////////////////////////////////////////////////////////////////////////

void
Nudger::setUpOrthographic()
{
    NudgerObjectInfo *info = (struct NudgerObjectInfo *) infos[curObject];

    // Find the screen-space points 1 pixel away in both valid
    // directions
    for (int i = 0; i < 2; i++) {

	// Find the screen space point where we want the center to end up
	SbVec2f screenEnd(info->screenCenter[0], info->screenCenter[1]);
	screenEnd[i] += pixelSize;

	// Find the world-space points that corresponds to those points
	SbVec3f vecToCenter = info->worldCenter - viewVol.getProjectionPoint();
	float distToCenter = vecToCenter.dot(viewVol.getProjectionDirection());
	SbVec3f newCenter = viewVol.getPlanePoint(distToCenter, screenEnd);

	info->translationDir[i] = newCenter - info->worldCenter;
    }
}

/////////////////////////////////////////////////////////////////////////////
//
// This clears the current nudging state, so that the next nudging
// operation will begin a new set. By default, consecutive nudges
// are compressed into one operation for undo/redo. This divides
// them up.
//
/////////////////////////////////////////////////////////////////////////////

#if 0
void
Nudger::resetUndoState()
{
    lastUndoItem = NULL;
}
#endif

//////////////////////////////////////////////////////////////////////
////////////////     mvo additions for firewalker  ///////////////////
//////////////////////////////////////////////////////////////////////

// these functions are in CoSceneManager in Cosmo Create tree

SbViewportRegion	*Nudger::actionViewportRegion = NULL;
SoGetBoundingBoxAction	*Nudger::bboxAction   = NULL;
SoGetMatrixAction	*Nudger::matrixAction = NULL;

// static
void Nudger::setActionViewportRegion(const SbViewportRegion &vpReg)
{
    *actionViewportRegion = vpReg;

    if (bboxAction == NULL)
	bboxAction = new SoGetBoundingBoxAction(vpReg);
    else
	bboxAction->setViewportRegion(vpReg);

    if (matrixAction == NULL)
	matrixAction = new SoGetMatrixAction(vpReg);
    else
	matrixAction->setViewportRegion(vpReg);
}

/////////////////////////////////////////////////////////////////////////////
//
// Returns the bounding box of the given selected object. NOTE:
// the returned bbox does NOT include the manipulator's handles
// and stuff. 
//
/////////////////////////////////////////////////////////////////////////////

SbXfBox3f
Nudger::getXfBoundingBox(SoPath *selection, MleActor *actor)
//Nudger::getXfBoundingBox(CoSelector *selection)
{
    MLE_ASSERT(selection);
    
    // Initialize action if not already done
    if (bboxAction == NULL)
	bboxAction = new SoGetBoundingBoxAction(*actionViewportRegion);

    // We want to compute the bbox of the selected object, including
    // its transformation. Unfortunately, we can't just compute the
    // bbox of the whole object, because that would include the
    // manip's handles. So we need to do this in two steps: compute
    // the bbox of the object without the manip, and then transform
    // that bbox by the transformation.

    SoNode *obj = selection->getTail();
    ASSERT(obj != NULL);
    bboxAction->apply(obj);
    SbXfBox3f &xfbox = bboxAction->getXfBoundingBox();

    // mvo - we need to get actor's transform here
    MlTransform t;
    actor->getTransform(t);
    SbMatrix m;
    for (int i = 0; i < 4; i++)
    {
	for (int j = 0; j < 3; j++)
	    m[i][j] = mlScalarToFloat(t[i][j]);

	// inventor matrices have a 4th column
	m[i][3] = 0.0;
    }
    m[3][3] = 1.0;

//    SbMatrix &m = getMatrix(selection->getTransform());
    xfbox.transform(m);
    
    return xfbox;
}

//////////////////////////////////////////////////////////////////////
// mvo - getting selection and actor is hokey, but need to be able
// to get actor's xform...

SbBox3f
Nudger::getBoundingBox(SoPath *selection, MleActor *actor)
{
    return getXfBoundingBox(selection, actor).project();
}

//////////////////////////////////////////////////////////////////////

SbMatrix &
Nudger::getMatrix(SoPath *path)
{
    // Initialize action if not already done
    if (matrixAction == NULL)
	matrixAction = new SoGetMatrixAction(*actionViewportRegion);

    matrixAction->apply(path);

    return matrixAction->getMatrix();
}
