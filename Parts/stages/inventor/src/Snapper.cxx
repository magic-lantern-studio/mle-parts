/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file Snapper.cxx
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 *
 * This file implements a class that constrains a manipulator to "snap"
 * during manipulation to a given alignment constraint.
 */

// COPYRIGHT_BEGIN
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
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/draggers/SoTransformer2Dragger.h>
#include <Inventor/events/SoEvent.h>
#include <Inventor/manips/SoTransformer2Manip.h>

// Include Magic Lantern header files.
//#include <CosmoCreate/CoAssert.h>
#include <mle/mlAssert.h>
#define ASSERT	MLE_ASSERT

// Include Magic Lantern Rehearsal Player header files.
#include "CubeDragger.h"
#include "CubeManip.h"
#include "Snapper.h"
#include "Source.h"
#include "Target.h"

//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//
// The SnapperManipInfo class stores information about a manipulator
// that is being snapped. It is passed around through callbacks.
//
/////////////////////////////////////////////////////////////////////////////

class SnapperManipInfo
{
 public:
    Snapper		*snapper;
    SoTransformManip	*manip;
    SoFullPath		*pathToManip;
    SoFullPath		*pathToManipParent;
    SoDragger		*dragger;
    SbMatrix		preMotionMatrix;
    SbBool		isTransformer2Manip;

    // These hold the field values from the dragger. These are changed
    // when snapping occurs.
    SbVec3f		scaleFactor;
    SbRotation		rotation;
    SbVec3f		translation;
    SbVec3f		center;

    SnapperManipInfo(Snapper *snapper, SoPath *pathToManip);
    ~SnapperManipInfo();

    // Temporarily disables/re-enables value changed callback
    void		enableValueChangedCallback(SbBool which);

  private:
    // Keep track of whether value changed is enabled.
    SbBool              valueChangeEnabled;
    // Static callbacks
    static void	dragStartCB(void *userData, SoDragger *)
	{ SnapperManipInfo *info = (SnapperManipInfo *) userData;
	  info->snapper->initSnap(info); }
    static void	valueChangedCB(void *userData, SoDragger *)
	{ SnapperManipInfo *info = (SnapperManipInfo *) userData;
	  info->snapper->snap(info); }
    static void	dragFinishCB(void *userData, SoDragger *)
	{ SnapperManipInfo *info = (SnapperManipInfo *) userData;
	  info->snapper->finishSnap(info); }
};

/////////////////////////////////////////////////////////////////////////////
//
// Constructor.
//
/////////////////////////////////////////////////////////////////////////////

SnapperManipInfo::SnapperManipInfo(Snapper *sn, SoPath *pathToNewManip)
{
    // Store pointer to snapper
    snapper = sn;

    // Store path to manipulator
    pathToManip = (SoFullPath *) pathToNewManip;
    pathToManip->ref();

    SoNode *tail = pathToManip->getTail();

    ASSERT(tail->isOfType(SoTransformManip::getClassTypeId()));

    // Store pointer to manipulator
    manip = (SoTransformManip *) tail;
    manip->ref();

    // The manipulator is either an SoTransformer2Manip or a CubeManip
    isTransformer2Manip = manip->isOfType(SoTransformer2Manip::getClassTypeId());
    ASSERT(isTransformer2Manip || manip->isOfType(CubeManip::getClassTypeId()));

    // Compute path to manipulator's parent
    pathToManipParent = (SoFullPath *)
	pathToManip->copy(0, pathToManip->getLength() - 1);
    pathToManipParent->ref();

    // Store pointer to manipulators dragger and add callbacks
    dragger = manip->getDragger();
    dragger->addStartCallback(dragStartCB, this);
    dragger->addFinishCallback(dragFinishCB, this);

    valueChangeEnabled = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
// Destructor.
//
/////////////////////////////////////////////////////////////////////////////

SnapperManipInfo::~SnapperManipInfo()
{
    // Remove callbacks, just to be sure
    dragger->removeStartCallback(dragStartCB, this);
    dragger->removeFinishCallback(dragFinishCB, this);

    // To remove value changed, use our method which remembers if it's on
    // or not:
    enableValueChangedCallback(FALSE);

    manip->unref();
    pathToManip->unref();
    pathToManipParent->unref();
}

/////////////////////////////////////////////////////////////////////////////
//
// Temporarily disables/re-enables value changed callback.
//
/////////////////////////////////////////////////////////////////////////////

void
SnapperManipInfo::enableValueChangedCallback(SbBool which)
{
    if (which == valueChangeEnabled)
	return;

    if (which)
	dragger->addValueChangedCallback(valueChangedCB, this);
    else
	dragger->removeValueChangedCallback(valueChangedCB, this);

    valueChangeEnabled = which;
}

//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//
// Constructor.
//
/////////////////////////////////////////////////////////////////////////////

Snapper::Snapper()
{
    setRotationTolerance(10.0);
    setTranslationTolerance(0.02);
    setScaleTolerance(0.5);

    target = NULL;
    source = NULL;

    gangManips = TRUE;
    snapState = 0;
    motionType = NOT_MOVING;

    bboxAction   = NULL;
    matrixAction = NULL;
}

/////////////////////////////////////////////////////////////////////////////
//
// Destructor.
//
/////////////////////////////////////////////////////////////////////////////

Snapper::~Snapper()
{
    for (int i = 0; i < infos.getLength(); i++)
	delete (SnapperManipInfo *) infos[i];

    if (bboxAction != NULL)
	delete bboxAction;
    if (matrixAction != NULL)
	delete matrixAction;
}

/////////////////////////////////////////////////////////////////////////////
//
// Enables or disables snapping.
//
/////////////////////////////////////////////////////////////////////////////

void
Snapper::enable(SbBool flag)
{
    if (target->isDisplayed() != flag) {
	// Change the display status of the target and source. Note
	// that when enabling snapping, the target is always
	// displayed, but the source remains off until the user places
	// it again.
	if (flag)
	    target->setDisplay(TRUE);
	else {
	    target->setDisplay(FALSE);
	    source->setDisplay(FALSE);
	}
    }
}

/////////////////////////////////////////////////////////////////////////////
//
// Returns the current state of snapping.
//
/////////////////////////////////////////////////////////////////////////////

SbBool
Snapper::isEnabled() const
{
    return target->isDisplayed();
}

/////////////////////////////////////////////////////////////////////////////
//
// Enables or disables scale snapping.
//
/////////////////////////////////////////////////////////////////////////////

void
Snapper::enableScaleSnapping(SbBool flag)
{
    if (target->isScaleSnappingEnabled() != flag) {
	target->enableScaleSnapping(flag);
	source->enableScaleSnapping(flag);
    }
}

/////////////////////////////////////////////////////////////////////////////
//
// Returns the current state of scale snapping.
//
/////////////////////////////////////////////////////////////////////////////

SbBool
Snapper::isScaleSnappingEnabled() const
{
    return target->isScaleSnappingEnabled();
}

/////////////////////////////////////////////////////////////////////////////
//
// This adds a manipulator to constrain. The manipulator must be
// derived from SoTransformManip and must be the last node in the
// given path.
//
/////////////////////////////////////////////////////////////////////////////

void
Snapper::addManip(SoPath *pathToNewManip)
{
    SnapperManipInfo *info = new SnapperManipInfo(this, pathToNewManip);
    infos.append(info);
}

/////////////////////////////////////////////////////////////////////////////
//
// This removes a manipulator.
//
/////////////////////////////////////////////////////////////////////////////

void
Snapper::removeManip(SoTransformManip *manip)
{
    // Find the info containing this manip and remove it
    SnapperManipInfo *info;
    for (int i = 0; i < infos.getLength(); i++) {
	info = (SnapperManipInfo *) infos[i];
	if (info->manip == manip) {
	    infos.remove(i);
	    break;
	}		
    }

    ASSERT(info->manip == manip);
    delete info;
}

/////////////////////////////////////////////////////////////////////////////
//
// Sets the rotation-snapping tolerance, represented as a number of
// degrees.
//
/////////////////////////////////////////////////////////////////////////////

void
Snapper::setRotationTolerance(float numDegrees)
{
    // Convert to radians
    float radians = numDegrees * M_PI / 180.0;

    // A negative tolerance is the same as a positive one
    if (radians < 0.0)
	radians = -radians;

    // If the tolerance is greater than PI/2 (90 degrees), it will
    // snap on the other side, anyway. So clamp to PI/2.
    if (radians > M_PI / 2.0)
	radians = M_PI / 2.0;

    // The tolerance we use later is the cosine of the angle. Take the
    // cosine here so the comparison is valid. We want the tolerance
    // to be 0 when the angle is 0 (cosine is 1).
    rotTolerance = 1.0 - cos(radians);
}

/////////////////////////////////////////////////////////////////////////////
//
// Returns the rotation tolerance, in degrees.
//
/////////////////////////////////////////////////////////////////////////////

float
Snapper::getRotationTolerance() const
{
    return acos(1.0 - rotTolerance) * 180.0 / M_PI;
}

/////////////////////////////////////////////////////////////////////////////
//
// Initializes snapping, setting up targets and stuff.
//
/////////////////////////////////////////////////////////////////////////////

void
Snapper::initSnap(SnapperManipInfo *info)
{
    // Get the target plane, point, and length from the Target
    ASSERT(target != NULL);

    // Assume nothing
    motionType = FIRST_MOTION;

    // Initialize actions if not already done
    if (bboxAction == NULL) {
	bboxAction   = new SoGetBoundingBoxAction(SbViewportRegion(100,100));
	matrixAction = new SoGetMatrixAction(SbViewportRegion(100,100));
    }

    // Compute the matrices that transform world space to dragger
    // space and vice-versa. This should include any sibling nodes to
    // the left of the manip. Also compute the matrix for transforming
    // normals from world space to dragger space.
    SoFullPath *pathForMatrix;
    if (info->pathToManip->getIndexFromTail(0) > 0) {
	pathForMatrix = (SoFullPath *) info->pathToManipParent->copy();
	pathForMatrix->append(info->pathToManip->getIndexFromTail(0) - 1);
    }
    else
	pathForMatrix = info->pathToManipParent;
    pathForMatrix->ref();
    matrixAction->apply(pathForMatrix);
    SbMatrix draggerToWorldP = matrixAction->getMatrix();
    SbMatrix worldToDraggerP = matrixAction->getInverse();
    SbMatrix worldToDraggerN = draggerToWorldP.transpose();
    pathForMatrix->unref();

    if (target->isDisplayed())
	initSnappingStuff(info, worldToDraggerP, worldToDraggerN,
			  draggerToWorldP);

    if (gangManips || source->isDisplayed()) {
	matrixAction->apply(info->pathToManip);
	origMatrixInv = matrixAction->getInverse();

	if (gangManips) {
	    for (int i = 0; i < infos.getLength(); i++) {
		SnapperManipInfo *otherInfo = (SnapperManipInfo *) infos[i];
		if (otherInfo != info) {
		    matrixAction->apply(otherInfo->pathToManip);
		    otherInfo->preMotionMatrix = matrixAction->getMatrix();
		}
	    }
	}

	// Tell the source to prepare for motion. Give it the matrix
	// to convert from dragger space to world space so it can
	// transform itself properly later on.
	if (source->isDisplayed())
	    source->startMatrixChange(draggerToWorldP);
    }

    // Save the current values of the dragger's translation and
    // ScaleFactor fields, so we can tell later which dimensions are
    // changing.
    if (info->isTransformer2Manip) {
	SoTransformer2Dragger *dragger = (SoTransformer2Dragger *) info->dragger;
	startScaleFactor = dragger->scaleFactor.getValue();
	startTranslation = dragger->translation.getValue();
	startCenter      = dragger->center.getValue();
    }
    else {
	ASSERT(info->dragger->isOfType(CubeDragger::getClassTypeId()));
	CubeDragger *dragger = (CubeDragger *) info->dragger;
	startTranslation = dragger->translation.getValue();
    }

    // From now on, we want to track changes to the dragger's values
    info->enableValueChangedCallback(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//
// This does the snapping, based on some change to the manipulator.
//
/////////////////////////////////////////////////////////////////////////////

void
Snapper::snap(SnapperManipInfo *info)
{
    // This may be called when a value in the manip is changed for
    // some other reason. If so, don't try to snap.
    if (motionType == NOT_MOVING)
	return;

    // Save the current snap state and assume nothing is snapped
    int saveSnapState = snapState;
    snapState = 0;

    // Save the current values of the dragger's fields. Snapping will
    // modify these values if necessary. When we are done, we will set
    // the dragger's matrix from these fields if anything snapped.
    if (info->isTransformer2Manip) {
	SoTransformer2Dragger *dragger =
	    (SoTransformer2Dragger *) info->dragger;
	info->scaleFactor = dragger->scaleFactor.getValue();
	info->rotation    = dragger->rotation.getValue();
	info->translation = dragger->translation.getValue();
	info->center      = dragger->center.getValue();
    }
    else {
	ASSERT(info->dragger->isOfType(CubeDragger::getClassTypeId()));
	CubeDragger *dragger = (CubeDragger *) info->dragger;
	info->scaleFactor = dragger->scaleFactor.getValue();
	info->rotation    = dragger->rotation.getValue();
	info->translation = dragger->translation.getValue();
	info->center.setValue(0.0, 0.0, 0.0);
    }

    // See what type of motion is occurring, if this is the first time
    if (motionType == FIRST_MOTION)
	getManipMotion(info);

    if (isEnabled()) {
	// Rotation
	if (motionType == ROTATING || motionType == UNKNOWN) {

	    // If the source is active, use just the object-space
	    // source normal
	    if (source->isDisplayed()) {
		if (snapRotation(info, objectSourceNormal))
		    snapState |= ROTATE_ANY;
	    }

	    // Otherwise, compare all 3 principal axes with the target normal
	    else {
		SbVec3f normal(0.0, 0.0, 0.0);
		for (int i = 0; i < 3; i++) {
		    normal[i] = 1.0;
		    if (snapRotation(info, normal))
			snapState |= (ROTATE_X << i);
		    normal[i] = 0.0;
		}
	    }
	}

	// There is no need to snap translation in dimensions that are
	// not changing. In fact, it is annoying to users if snapping
	// occurs in other dimensions during constrained motion. The
	// moveInDimension flags indicate whether motion is allowed
	// in each of the 3 dimensions.

	// Translation
	if (motionType == TRANSLATING || motionType == UNKNOWN) {
	    for (int i = 0; i < 3; i++) {
		if (moveInDimension[i]) {
		    int which;
		    if (snapTranslation(info, i, which))
			snapState |= (TRANSLATE_X_MIN << (3 * i + which));
		}
	    }
	}

	// Translation of the rotation/scaling center
	if (motionType == CENTERING || motionType == UNKNOWN) {
	    for (int i = 0; i < 3; i++) {
		if (moveInDimension[i]) {
		    int which;
		    if (snapCenter(info, i, which))
			snapState |= (TRANSLATE_X_MIN << (3 * i + which));
		}
	    }
	}

	// Scale: Compare all 3 principal axes with all target normals
	if ((motionType == SCALING || motionType == UNKNOWN) &&
	    isScaleSnappingEnabled()) {
	    SbBool isUniform = ! info->dragger->getEvent()->wasShiftDown();
	    for (int i = 0; i < 3; i++) {
		if (moveInDimension[i]) {
		    if (snapScale(info, i, isUniform)) {
			snapState |= (SCALE_X << i);

			// Stop if we snapped to one dimension and
			// this is uniform scaling. Also stop if the
			// source edge snapped to the target edge.
			if (isUniform || (source->isDisplayed() &&
					  source->getLength() > 0.0))
			    break;
		    }
		}
	    }
	}
    }

    // If anything snapped, update the matrix in the dragger
    if (snapState != 0) {

	// Different course of action for changing center than other fields:
	if (motionType != CENTERING) {
	    // Compute the cumulative matrix
	    SbMatrix newDraggerMatrix;
	    computeNewMatrix(info, newDraggerMatrix,
			     ! (snapState & (ROTATE_ANY | SCALE_ANY)));

	    // Disable our callback and set the dragger's matrix
	    info->enableValueChangedCallback(FALSE);
	    info->dragger->setMotionMatrix(newDraggerMatrix);
	    info->enableValueChangedCallback(TRUE);
	}
	else {
	    // All we need to do is set the center field to the snapped value.
	    // The rest should just work.
	    SoDragger *d = info->dragger;
		ASSERT(d->isOfType(SoTransformer2Dragger::getClassTypeId()));
	    info->enableValueChangedCallback(FALSE);
	    ((SoTransformer2Dragger *)d)->center = info->center;
	    info->enableValueChangedCallback(TRUE);
	}
    }

    // Transform all other manips and the source, if necessary
    transform(info);

    // Update the target display if the state changed
    if (snapState != saveSnapState)
	target->updateActiveParts(snapState & TRANSLATE_ANY,
				  snapState & ROTATE_ANY,
				  snapState & SCALE_ANY);
}

/////////////////////////////////////////////////////////////////////////////
//
// This cleans up after the drag has completed.
//
/////////////////////////////////////////////////////////////////////////////

void
Snapper::finishSnap(SnapperManipInfo *info)
{
    motionType = NOT_MOVING;

    // Stop caring about changes to the dragger
    info->enableValueChangedCallback(FALSE);

    // Tell the source that motion is finished
    if (source->isDisplayed())
	source->finishMatrixChange();

    // Since the snapper is no longer active, change the state and
    // update the target
    if (snapState != 0) {
	snapState = 0;
	target->updateActiveParts(FALSE, FALSE, FALSE);
    }
}

/////////////////////////////////////////////////////////////////////////////
//
// Initializes member variables when snapping is enabled.
//
/////////////////////////////////////////////////////////////////////////////

void
Snapper::initSnappingStuff(SnapperManipInfo *info,
			   const SbMatrix &worldToDraggerP,
			   const SbMatrix &worldToDraggerN,
			   const SbMatrix &draggerToWorldP)
{
    ASSERT(target->isDisplayed());

    // Compute the dragger-space target point and normal
    worldToDraggerP.multVecMatrix(target->getPoint(), targetPoint);
    worldToDraggerN.multDirMatrix(target->getNormal(), targetNormal);
    targetNormal.normalize();

    // To compute the target length for scaling, start with the
    // vectors of the correct length along each of the world space
    // axes. Transform the origin and the 3 endpoints into dragger
    // space and compute the new lengths.
    float len = target->getLength();
    if (len > 0.0) {
	SbVec3f origin, endpoints[3];
	worldToDraggerP.multVecMatrix(SbVec3f(0.0, 0.0, 0.0), origin);
	worldToDraggerP.multVecMatrix(SbVec3f(len, 0.0, 0.0), endpoints[0]);
	worldToDraggerP.multVecMatrix(SbVec3f(0.0, len, 0.0), endpoints[1]);
	worldToDraggerP.multVecMatrix(SbVec3f(0.0, 0.0, len), endpoints[2]);
	for (int d = 0; d < 3; d++)
	    targetLengths[d] = (endpoints[d] - origin).length();
    }
    else
	targetLengths.setValue(0.0, 0.0, 0.0);

    if (source->isDisplayed()) {

	// If the source is active, transform the source point and
	// normal from world space to object space and store them for
	// later use. To transform the point, we need the product of
	// the worldToDraggerP and the inverse of the manip's
	// transform, which then brings us to object space. The normal
	// matrix is similar.
	matrixAction->apply(info->manip);
	SbMatrix objectToWorldP = matrixAction->getMatrix() * draggerToWorldP;
	SbMatrix worldToObjectP = worldToDraggerP * matrixAction->getInverse();
	SbMatrix worldToObjectN = objectToWorldP.transpose();
	worldToObjectP.multVecMatrix(source->getPoint(),  objectSourcePoint);
	worldToObjectN.multDirMatrix(source->getNormal(), objectSourceNormal);
	objectSourceNormal.normalize();

	// If doing scale snapping, we also need to transform the
	// endpoints of the scale edge to object space, since that is
	// where the scale factors are easiest to deal with.
	if (isScaleSnappingEnabled() && len > 0.0) {
	    SbVec3f end1, end2;
	    worldToObjectP.multVecMatrix(source->getEdgeEnd1(), end1);
	    worldToObjectP.multVecMatrix(source->getEdgeEnd2(), end2);
	    draggerSourceEdge = end2 - end1;

	    // And compute the length of the the target edge in dragger space
	    worldToDraggerP.multVecMatrix(target->getEdgeEnd1(), end1);
	    worldToDraggerP.multVecMatrix(target->getEdgeEnd2(), end2);
	    draggerTargetLength = (end2 - end1).length();
	}
    }

    // If the source is not active, snapping uses the bounding box of
    // the manipulated object. Set up boxPoints to contain the min,
    // center, and max points of the manipulated object in that
    // object's local space.
    else {
	// Compute the bounding box of the manipulated object. To compute
	// the bbox in the correct space, we apply the action to the path
	// from the root to the manip, but reset everything after the
	// manip itself, so its transformation does not get applied.
	bboxAction->setResetPath(info->pathToManip, FALSE);
	bboxAction->apply(info->pathToManipParent);
	SbBox3f objectBox = bboxAction->getBoundingBox();

	// Compute the min, max, and center points
	boxPoints[0] = objectBox.getMin();
	boxPoints[1] = objectBox.getCenter();
	boxPoints[2] = objectBox.getMax();
    }
}

/////////////////////////////////////////////////////////////////////////////
//
// Tests if rotation is close to the given object space normal. If so,
// it snaps the rotation and returns TRUE.
//
/////////////////////////////////////////////////////////////////////////////

SbBool
Snapper::snapRotation(SnapperManipInfo *info, const SbVec3f &normal)
{
    // Get the matrix that transforms normals from object space into
    // manip space (the same as dragger space)
    matrixAction->apply(info->manip);
    SbMatrix objectToManip = matrixAction->getInverse().transpose();

    // Transform the box normal into the manip's space
    SbVec3f manipNormal;
    objectToManip.multDirMatrix(normal, manipNormal);
    manipNormal.normalize();

    // The target normal is already in the manip's space. See if
    // the normals are close to each other.
    float diff = manipNormal.dot(targetNormal);
    float adiff = fabs(diff);

    if (1.0 - adiff <= rotTolerance) {

	if (adiff < 1.0) { 

	    // Compute the manip-space rotation that will bring the
	    // normal in line with the target normal
	    SbRotation deltaRot;
	    if (diff < 0.0)
		deltaRot.setValue(-manipNormal, targetNormal);
	    else
		deltaRot.setValue(manipNormal, targetNormal);

	    // Change the rotation
	    info->rotation *= deltaRot;
	}
	return TRUE;
    }

    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
// Tests if translation is close to the plane perpendicular to the
// given axis dimension (0 = X, 1 = Y, 2 = Z). If so, it snaps the
// translation and returns TRUE. It also returns min, center, or
// max as 0, 1, 2 in "which".
//
/////////////////////////////////////////////////////////////////////////////

SbBool
Snapper::snapTranslation(SnapperManipInfo *info, int dimension, int &which)
{
    // Get the matrices that transform into the manip's space and back
    matrixAction->apply(info->manip);
    const SbMatrix &objectToManip = matrixAction->getMatrix();
    const SbMatrix &manipToObject = matrixAction->getInverse();

    // Get the normal to the planes we are testing
    SbVec3f normal(0.0, 0.0, 0.0);
    normal[dimension] = 1.0;

    // Transform the plane normal into world space and normalize it
    SbVec3f worldNormal;
    manipToObject.transpose().multDirMatrix(normal, worldNormal);
    worldNormal.normalize();

    // If the Source is active, set up only 1 box point, using the
    // source point. Otherwise, we look at all 3 (min, center, max).
    SbVec3f worldBoxPoints[3];
    int numPointsToUse, pointToUseForDistance;
    if (source->isDisplayed()) {
	numPointsToUse = 1;
	pointToUseForDistance = 0;
	objectToManip.multVecMatrix(objectSourcePoint, worldBoxPoints[0]);
    }
    else {
	numPointsToUse = 3;
	pointToUseForDistance = 1;
	for (int i = 0; i < 3; i++)
	    objectToManip.multVecMatrix(boxPoints[i], worldBoxPoints[i]);
    }

    // Compute the world-space snapping distance based on the current
    // translation tolerance and view volume
    const SbViewVolume &viewVol = target->getViewVolume();
    float worldSnapDistance =
	viewVol.getWorldToScreenScale(worldBoxPoints[pointToUseForDistance],
				      transTolerance);

    // In world-space, compute the signed distances of the target
    // point from the three planes to test, and see which of the
    // three has the smallest absolute value.
    SbVec3f worldDists;
    float   minWorldDist;
    float   absMinWorldDist = 1e12;
    for (int i = 0; i < numPointsToUse; i++) {
	worldDists[i] = (targetPoint - worldBoxPoints[i]).dot(worldNormal);
	float absWorldDist = fabs(worldDists[i]);
	if (absWorldDist < absMinWorldDist) {
	    which = i;
	    minWorldDist    = worldDists[i];
	    absMinWorldDist = absWorldDist;
	}
    }

    // See if the smallest distance is within the world-space
    // snapping distance
    if (absMinWorldDist < worldSnapDistance) {

	// Compute the new translation value by moving the
	// correct distance along the world-space normal
	if (absMinWorldDist > 0.0)
	    info->translation += minWorldDist * worldNormal;

	return TRUE;
    }

    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
// Tests if center is close to the plane perpendicular to the
// given axis dimension (0 = X, 1 = Y, 2 = Z). If so, it snaps the
// center and returns TRUE. It also returns min, center, or
// max as 0, 1, 2 in "which".
//
/////////////////////////////////////////////////////////////////////////////

SbBool
Snapper::snapCenter(SnapperManipInfo *info, int dimension, int &which)
{
    ASSERT(info->dragger->isOfType(SoTransformer2Dragger::getClassTypeId()));
    SoTransformer2Dragger *dragger = (SoTransformer2Dragger *) info->dragger;

    // Get the matrices that transform into the manip's space and back
    matrixAction->apply(info->manip);
    const SbMatrix &objectToManip = matrixAction->getMatrix();
    const SbMatrix &manipToObject = matrixAction->getInverse();

    // Get the normal to the planes we are testing
    SbVec3f normal(0.0, 0.0, 0.0);
    normal[dimension] = 1.0;

    // Transform the plane normal into world space and normalize it
    SbVec3f worldNormal;
    manipToObject.transpose().multDirMatrix(normal, worldNormal);
    worldNormal.normalize();

    // If the Source is active, set up only 1 box point, using the
    // source point. Otherwise, we look at the 3 box points, but in this 
    // case our box is centered around the 'center' field and not the 
    // geometric center, so we do extra calculations...
    SbVec3f worldBoxPoints[3];
    int numPointsToUse, pointToUseForDistance;
    if (source->isDisplayed()) {
	numPointsToUse = 1;
	pointToUseForDistance = 0;
	objectToManip.multVecMatrix(objectSourcePoint, worldBoxPoints[0]);
    }
    else {
	numPointsToUse = 3;
	pointToUseForDistance = 1;
	// Before converting to world coordinates, we need to change the 
	// box points to center around our 'center' field...
	SbVec3f addForHighVec = boxPoints[2] - boxPoints[1];
	SbVec3f subForLowVec = boxPoints[1] - boxPoints[0];
	boxPoints[0] = dragger->center.getValue() - subForLowVec;
	boxPoints[1] = dragger->center.getValue();
	boxPoints[2] = dragger->center.getValue() + addForHighVec;
	for (int i = 0; i < 3; i++)
	    objectToManip.multVecMatrix(boxPoints[i], worldBoxPoints[i]);
    }

    // Compute the world-space snapping distance based on the current
    // translation tolerance (we'll use this for centering as well) 
    // and view volume
    const SbViewVolume &viewVol = target->getViewVolume();
    float worldSnapDistance =
	viewVol.getWorldToScreenScale(worldBoxPoints[pointToUseForDistance],
				      transTolerance);

    // In world-space, compute the signed distances of the target
    // point from the three planes to test, and see which of the
    // three has the smallest absolute value.
    SbVec3f worldDists;
    float   minWorldDist;
    float   absMinWorldDist = 1e12;
    for (int i = 0; i < numPointsToUse; i++) {
	worldDists[i] = (targetPoint - worldBoxPoints[i]).dot(worldNormal);
	float absWorldDist = fabs(worldDists[i]);
	if (absWorldDist < absMinWorldDist) {
	    which = i;
	    minWorldDist    = worldDists[i];
	    absMinWorldDist = absWorldDist;
	}
    }

    // See if the smallest distance is within the world-space
    // snapping distance
    if (absMinWorldDist < worldSnapDistance) {

	// Compute the new center value by moving the
	// correct distance along the world-space normal
	if (absMinWorldDist > 0.0)
	    info->center += minWorldDist * worldNormal;

	return TRUE;
    }

    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
// Tests if scaling in the given dimension is close to the target
// scale length. If so, it snaps the scaling and returns TRUE.
//
/////////////////////////////////////////////////////////////////////////////

SbBool
Snapper::snapScale(SnapperManipInfo *info, int dimension, SbBool isUniform)
{
    // Compute the size of the feature being scaled. If the source is
    // active and has an edge, this is that edge. Otherwise, this is
    // the side of the manip box in the appropriate dimension. Also
    // set the target length.
    float currentLength, targetLength;
    if (source->isDisplayed() && source->getLength() > 0.0) {
	// Scale the edge by the current scale of the manipulated object,
	// then compute its length
	SbVec3f scaledEdge;
	for (int i = 0; i < 3; i++)
	    scaledEdge[i] = info->scaleFactor[i] * draggerSourceEdge[i];
	currentLength = scaledEdge.length();
	targetLength = draggerTargetLength;
    }
    else {
	// Scale the box size by the current scale of the manipulated
	// object
	currentLength = info->scaleFactor[dimension] *
	    (boxPoints[2][dimension] - boxPoints[0][dimension]);
	targetLength = targetLengths[dimension];
    }

    // Can't do anything with non-positive lengths
    if (currentLength <= 0.0 || targetLength <= 0.0)
	return FALSE;

    // Compute the difference in the lengths as a fraction of the
    // target size
    float sizeDiff = currentLength - targetLength;
    if (sizeDiff < 0.0)
	sizeDiff = -sizeDiff;
    float scaleFrac = sizeDiff / targetLength;

    // See if this is close enough
    if (scaleFrac <= scaleTolerance) {

	// Change the scale factor in the manip. If this is a
	// uniform scale, change all three appropriately
	float scaleFac = targetLength / currentLength;
	if (isUniform)
	    info->scaleFactor *= scaleFac;
	else
	    info->scaleFactor[dimension] *= scaleFac;
	return TRUE;
    }

    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
// Snaps the manipulator with the given index to meet and align
// with the first target point and plane.
//
/////////////////////////////////////////////////////////////////////////////

void
Snapper::snapNow(int whichManip)
{
    ASSERT(whichManip >= 0 && whichManip < infos.getLength());
    SnapperManipInfo *info = (SnapperManipInfo *) infos[whichManip];

    // Pretend we are doing interactive snapping to set things up
    initSnap(info);

    // If the Source is active, we use its world-space point and
    // normal for snapping
    if (source->isDisplayed()) {
	rotateSnapNow(info, source->getNormal());
	transform(info);

	// Make sure the rotation gets applied to the source's
	// location before doing the translation
	finishSnap(info);
	initSnap(info);

	translateSnapNow(info, source->getPoint());
	transform(info);
    }
    else {
	SbVec3f objSourcePoint, objSourceNormal;
	computeSnapSide(info, objSourceNormal, objSourcePoint);

	// Transform the normal into world space, and normalize it
	SbVec3f worldSourceNormal;
	matrixAction->apply(info->manip);
	SbMatrix normalMatrix = matrixAction->getInverse().transpose();
	normalMatrix.multDirMatrix(objSourceNormal, worldSourceNormal);
	worldSourceNormal.normalize();

	// Snap the rotation
	rotateSnapNow(info, worldSourceNormal);
	transform(info);

	// Transform the source point into world-space. We have to do this
	// after the rotation is done so that the rotation affects this.
	SbVec3f worldSourcePoint;
	matrixAction->apply(info->manip);
	const SbMatrix &pointMatrix = matrixAction->getMatrix();
	pointMatrix.multVecMatrix(objSourcePoint, worldSourcePoint);

	// Snap the translation
	translateSnapNow(info, worldSourcePoint);
	transform(info);
    }

    finishSnap(info);
}

/////////////////////////////////////////////////////////////////////////////
//
// Computes the object-space normal and center point of the side of the
// dragger box to use for snap-now.
//
/////////////////////////////////////////////////////////////////////////////

void
Snapper::computeSnapSide(SnapperManipInfo *info,
			 SbVec3f &boxNormal, SbVec3f &boxCenter)
{
    // Figure out which of the 6 outer planes of the manip is most
    // opposite the target plane normal by looking for the most
    // negative dot product.
    matrixAction->apply(info->manip);
    SbMatrix normalMatrix = matrixAction->getInverse().transpose();
    SbVec3f normal, worldNormal;
    int closestDim = -1, closestSign = 0.0;
    float minDot = 1e12;

    for (int dim = 0; dim < 3; dim++) {
	normal.setValue(0.0, 0.0, 0.0);

	// Try minimum plane in this dimension first
	normal[dim] = -1.0;
	normalMatrix.multDirMatrix(normal, worldNormal);
	worldNormal.normalize();
	float dot = worldNormal.dot(targetNormal);
	if (dot < minDot) {
	    closestDim  = dim;
	    closestSign = -1.0;
	    minDot = dot;
	}

	// Now try maximum plane in this dimension
	normal[dim] = 1.0;
	normalMatrix.multDirMatrix(normal, worldNormal);
	worldNormal.normalize();
	dot = worldNormal.dot(targetNormal);
	if (dot < minDot) {
	    closestDim  = dim;
	    closestSign = 1.0;
	    minDot = dot;
	}
    }

    ASSERT(closestDim >= 0 && closestSign != 0.0);
    // We should have found at least one negative dot product
    ASSERT(minDot < 0.0);

    // Get the normal to the desired manip plane
    boxNormal.setValue(0.0, 0.0, 0.0);
    boxNormal[closestDim] = closestSign;

    // Get the center point of the chosen bounding box plane
    boxCenter = boxPoints[1];	// Center of box
    boxCenter[closestDim] = boxPoints[closestSign < 0.0 ? 0 : 2][closestDim];
}

/////////////////////////////////////////////////////////////////////////////
//
// Rotates the given manipulator so that it aligns with the first
// target plane normal.
//
/////////////////////////////////////////////////////////////////////////////

void
Snapper::rotateSnapNow(SnapperManipInfo *info, const SbVec3f &startNormal)
{
    // Compute the rotation that will bring the box normal in line
    // with the target normal and change the rotation in the manip
    SbRotation curRot = info->manip->rotation.getValue();
    SbRotation deltaRot(-startNormal, targetNormal);
    info->enableValueChangedCallback(FALSE);
    info->manip->rotation = curRot * deltaRot;
    info->enableValueChangedCallback(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//
// Translates the given manipulator to center the correct face on the
// first target point.
//
/////////////////////////////////////////////////////////////////////////////

void
Snapper::translateSnapNow(SnapperManipInfo *info, const SbVec3f &startPoint)
{
    // Compute the world-space translation vector to snap to the
    // target point
    SbVec3f snapVec = targetPoint - startPoint;

    // Compute the new translation value by moving the
    // correct distance along the world-space normal
    SbVec3f curTrans = info->manip->translation.getValue();
    SbVec3f newTrans = curTrans + snapVec;

    // Change the translation in the manip
    info->enableValueChangedCallback(FALSE);
    info->manip->translation = newTrans;
    info->enableValueChangedCallback(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//
// Transforms all ganged manips and the source by the given info's
// motion matrix.
//
/////////////////////////////////////////////////////////////////////////////

void
Snapper::transform(SnapperManipInfo *info)
{
    // If all other manipulators should be affected, change them
    SbMatrix deltaMat = origMatrixInv * info->dragger->getMotionMatrix();
    if (gangManips) {
	for (int i = 0; i < infos.getLength(); i++) {
	    SnapperManipInfo *otherInfo = (SnapperManipInfo *) infos[i];
	    if (otherInfo != info) {
		SbMatrix newMatrix = otherInfo->preMotionMatrix * deltaMat;
		otherInfo->manip->setMatrix(newMatrix);
	    }
	}
    }

    // Move the source
    if (source->isDisplayed())
	source->changeMatrix(deltaMat);
}

/////////////////////////////////////////////////////////////////////////////
//
// This computes the type of motion the manipulator is undergoing.
//
/////////////////////////////////////////////////////////////////////////////

void
Snapper::getManipMotion(SnapperManipInfo *info)
{
    ManipMotion motion;

    // If this is an SoTransformer2Manip find out what motion is
    // occurring. For translation, scaling and center, set the moveInDimension
    // flags based on which dimensions are allowed.
    moveInDimension[0] = moveInDimension[1] = moveInDimension[2] = FALSE;

//??? BUG:
//??? Selection of moveInDimension is wrong in case where shift is down.
//??? The motion needs to be projected on the x/y/z axes of the 
//??? in dragger space,, which may be rotated,  not in the space before 
//??? the dragger.

#define SET_TRANS_MOVE(D1, D2, D3)					      \
    if (info->dragger->getEvent()->wasCtrlDown())			      \
	moveInDimension[D1] = TRUE;					      \
    else if (info->dragger->getEvent()->wasShiftDown()) {		      \
	if (fabs(info->translation[D2] - startTranslation[D2]) >	      \
	    fabs(info->translation[D3] - startTranslation[D3]))		      \
	    moveInDimension[D2] = TRUE;					      \
	else								      \
	    moveInDimension[D3] = TRUE;					      \
    }									      \
    else								      \
	moveInDimension[D2] = moveInDimension[D3] = TRUE

#define SET_CENTER_MOVE(D1, D2, D3)					      \
    if (info->dragger->getEvent()->wasShiftDown()) {		              \
	if (fabs(info->center[D2] - startCenter[D2]) >	                      \
	    fabs(info->center[D3] - startCenter[D3]))		              \
	    moveInDimension[D2] = TRUE;					      \
	else								      \
	    moveInDimension[D3] = TRUE;					      \
    }									      \
    else								      \
	moveInDimension[D2] = moveInDimension[D3] = TRUE

    if (info->isTransformer2Manip) {

	switch (((SoTransformer2Dragger *) info->dragger)->getCurrentState()) {
	  case SoTransformer2Dragger::RIT_X_ROTATE:
	  case SoTransformer2Dragger::LFT_X_ROTATE:
	    if (!info->dragger->getEvent()->wasCtrlDown())
		motion = ROTATING;
	    else {
		motion = CENTERING;
	        SET_CENTER_MOVE(0, 1, 2);
	    }
	    break;
	  case SoTransformer2Dragger::TOP_Y_ROTATE:
	  case SoTransformer2Dragger::BOT_Y_ROTATE:
	    if (!info->dragger->getEvent()->wasCtrlDown())
		motion = ROTATING;
	    else {
		motion = CENTERING;
	        SET_CENTER_MOVE(1, 0, 2);
	    }
	    break;
	  case SoTransformer2Dragger::FNT_Z_ROTATE:
	  case SoTransformer2Dragger::BAK_Z_ROTATE:
	    if (!info->dragger->getEvent()->wasCtrlDown())
		motion = ROTATING;
	    else {
		motion = CENTERING;
	        SET_CENTER_MOVE(2, 0, 1);
	    }
	    break;

	  case SoTransformer2Dragger::PX_PY_PZ_3D_SCALE:
	  case SoTransformer2Dragger::PX_PY_NZ_3D_SCALE:
	  case SoTransformer2Dragger::PX_NY_PZ_3D_SCALE:
	  case SoTransformer2Dragger::PX_NY_NZ_3D_SCALE:
	  case SoTransformer2Dragger::NX_PY_PZ_3D_SCALE:
	  case SoTransformer2Dragger::NX_PY_NZ_3D_SCALE:
	  case SoTransformer2Dragger::NX_NY_PZ_3D_SCALE:
	  case SoTransformer2Dragger::NX_NY_NZ_3D_SCALE:
	    motion = SCALING;
	    // If uniform scale, allow snapping in all 3 dimensions
	    if (! info->dragger->getEvent()->wasShiftDown())
		moveInDimension[0] = moveInDimension[1] =
		    moveInDimension[2] = TRUE;
	    // Otherwise, compute the dimension with the greatest
	    // change in scale
	    else {
		int maxDim;
		float maxChange = -1.0;
		for (int i = 0; i < 3; i++) {
		    float d = fabs(info->scaleFactor[i] - startScaleFactor[i]);
		    if (d > maxChange) {
			maxChange = d;
			maxDim = i;
		    }
		}
		moveInDimension[maxDim] = TRUE;
	    }
	    break;

	  case SoTransformer2Dragger::RIT_TRANSLATE:
	  case SoTransformer2Dragger::LFT_TRANSLATE:
	    motion = TRANSLATING;
	    SET_TRANS_MOVE(0, 1, 2);
	    break;

	  case SoTransformer2Dragger::TOP_TRANSLATE:
	  case SoTransformer2Dragger::BOT_TRANSLATE:
	    motion = TRANSLATING;
	    SET_TRANS_MOVE(1, 0, 2);
	    break;

	  case SoTransformer2Dragger::FNT_TRANSLATE:
	  case SoTransformer2Dragger::BAK_TRANSLATE:
	    motion = TRANSLATING;
	    SET_TRANS_MOVE(2, 0, 1);
	    break;

	  default:
	    motion = UNKNOWN;
	    break;
	}
    }

    // For a CubeManip, the only motion is translation
    else {
	ASSERT(info->dragger->isOfType(CubeDragger::getClassTypeId()));
	motion = TRANSLATING;
	switch (((CubeDragger *) info->dragger)->getCurrentSide()) {
	  case CubeDragger::FRONT:
	  case CubeDragger::BACK:
	    SET_TRANS_MOVE(2, 0, 1);
	    break;

	  case CubeDragger::LEFT:
	  case CubeDragger::RIGHT:
	    SET_TRANS_MOVE(0, 1, 2);
	    break;

	  case CubeDragger::TOP:
	  case CubeDragger::BOTTOM:
	    SET_TRANS_MOVE(1, 0, 2);
	    break;

	  default:
	    // Better not be NONE!
	    ASSERT(FALSE);
	    break;
	}
    }

    motionType = motion;

#undef SET_TRANS_MOVE
}

/////////////////////////////////////////////////////////////////////////////
//
// Computes the cumulative matrix from the changed fields.
//
/////////////////////////////////////////////////////////////////////////////

void
Snapper::computeNewMatrix(SnapperManipInfo *info, SbMatrix &matrix,
			  SbBool isJustTranslation)
{
    // Combine the scale, rotation, and translation into a matrix
    SbMatrix tmp;
    matrix.setTranslate(-info->center);
    tmp.setScale(info->scaleFactor);
    matrix.multRight(tmp);
    tmp.setRotate(info->rotation);
    matrix.multRight(tmp);
    tmp.setTranslate(info->center);
    matrix.multRight(tmp);
    tmp.setTranslate(info->translation);
    matrix.multRight(tmp);

    // Don't need to do this if just translating
    if (! isJustTranslation) {
	// If we snapped rotation or scale, we may have moved the
	// transformation center. To compensate, we have to adjust the
	// translation so that the center doesn't move. First, compute
	// the starting center point.
	SbMatrix draggerMatrix = info->dragger->getMotionMatrix();
	SbVec3f origCenter = getTransformCenter(info);
	SbVec3f startCenter;
	draggerMatrix.multVecMatrix(origCenter, startCenter);

	// Find the new center point
	SbVec3f endCenter;
	matrix.multVecMatrix(origCenter, endCenter);

	// Adjust the translation to compensate
	tmp.setTranslate(startCenter - endCenter);
	matrix.multRight(tmp);
    }
}

/////////////////////////////////////////////////////////////////////////////
//
// This computes the center of transformation from the given info.
//
/////////////////////////////////////////////////////////////////////////////

SbVec3f
Snapper::getTransformCenter(SnapperManipInfo *info)
{
    ASSERT(info->isTransformer2Manip);
    //SoTransformer2Dragger *dragger = (SoTransformer2Dragger *) info->dragger;
	SoTransformerDragger *dragger = (SoTransformerDragger *) info->dragger;

    // Get the center in box space
    SbVec3f boxCenter = dragger->getInteractiveCenterInBoxSpace();

    // Convert that to the dragger's local space - this is the space
    // we want
    SbMatrix boxSpaceToLocal, localSpaceToBox;
    dragger->getPartToLocalMatrix("surroundScale",
				  boxSpaceToLocal, localSpaceToBox);
    SbVec3f localCenter;
    boxSpaceToLocal.multVecMatrix(boxCenter, localCenter);

    return localCenter;
}
