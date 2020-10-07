/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file SpotMarker.cxx
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 *
 * This file implements a class representing a marker displayed for snapping
 * operations.
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

// Defining this to be non-zero causes the SpotMarker scene graph to
// be read from "SpotMarker.iv" in the current directory, rather from
// the buffer created by ivToIncludeFile. This feature can be used to
// more quickly test changes to the scene graph, since there is no
// need to recompile first.
#define READ_FROM_FILE 0

#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/details/SoConeDetail.h>
#include <Inventor/details/SoCubeDetail.h>
#include <Inventor/details/SoCylinderDetail.h>
#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/details/SoLineDetail.h>
#include <Inventor/draggers/SoDragger.h>
#include <Inventor/draggers/SoTransformerDragger.h>
//#include <Inventor/draggers/SoTransformer2Dragger.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/nodekits/SoSeparatorKit.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoPickStyle.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoVertexShape.h>

#if 0
//mvo
#include <Vrml/VrmlCone.h>
#include <Vrml/VrmlCube.h>
#include <Vrml/VrmlCylinder.h>
#include <Vrml/VrmlSphere.h>
#endif

//mvo
//#include <CosmoCreate/CoAssert.h>
#include <mle/mlAssert.h>
#define ASSERT MLE_ASSERT

//mvo
//#include "CtBase.h"

#include "CubeDragger.h"
#include "MultiSearchAction.h"
#include "SpotMarker.h"

#if ! READ_FROM_FILE
// This contains the definition of sceneBuffer:
#include "SpotMarkerGraph.h"
#endif

/////////////////////////////////////////////////////////////////////////////
//
// Constructor takes flag indicating whether the Control key needs
// to be down to place the marker and a prefix string used to
// access nodes in the read-in scene graph.
//
/////////////////////////////////////////////////////////////////////////////

SpotMarker::SpotMarker(SbBool controlKeyFlag, const char *prefix)
{
    displayCB = NULL;

    needsControlKey = controlKeyFlag;
    prefixString    = prefix;

    tolerance = 0.1;

    bboxAction   = NULL;
    searchAction = NULL;

    displayed = FALSE;
    scaleEnabled = FALSE;
    isActive  = FALSE;

    curCamera = NULL;
    curAspectRatio = 1.0;

    haveViewVol = FALSE;

    root = NULL;

    // Initialize these to match the position and orientation of the
    // marker graph
    location.setValue(0.0, 0.0, 0.0);
    normal.setValue(0.0, 1.0, 0.0);
    edgeLength = 0.0;
}

/////////////////////////////////////////////////////////////////////////////
//
// Destructor.
//
/////////////////////////////////////////////////////////////////////////////

SpotMarker::~SpotMarker()
{
    if (root != NULL)
	root->unref();

    if (curCamera != NULL)
	curCamera->unref();

    if (bboxAction != NULL)
	delete bboxAction;

    if (searchAction != NULL)
	delete searchAction;
}

/////////////////////////////////////////////////////////////////////////////
//
// Returns the scene graph that displays the marker and interacts
// with it. The root of this graph remains until this instance is
// destroyed.
//
/////////////////////////////////////////////////////////////////////////////

SoSeparator *
SpotMarker::getSceneGraph()
{
    if (root == NULL)
	buildSceneGraph();
    return root;
}

/////////////////////////////////////////////////////////////////////////////
//
// Builds the scene graph.
//
/////////////////////////////////////////////////////////////////////////////

void
SpotMarker::buildSceneGraph()
{
    ////////////////////////////////////////////
    //
    // Read the scene graph:
    //

    SoInput in;
#if READ_FROM_FILE
    if (! in.openFile("SpotMarker.iv")) {
	fprintf(stderr, "Can't open SpotMarker.iv\n");
	exit(1);
    }
#else
    in.setBuffer((void *) sceneBuffer, sizeof(sceneBuffer));
#endif
    SoSeparator *readRoot = SoDB::readAll(&in);
    if (readRoot == NULL) {
	fprintf(stderr, "Bad read of SpotMarker.iv\n");
	exit(1);
    }
    readRoot->ref();

    ////////////////////////////////////////////
    //
    // Find important nodes
    //

#define FIND_NODE(name,class) (class *) findNode(name, class::getClassTypeId())

    SoEventCallback *ecb;

    root	   = FIND_NODE("root",		SoSeparator);
    root->ref();
    ecb		   = FIND_NODE("eventCallback",	SoEventCallback);
    displaySwitch  = FIND_NODE("displaySwitch",	SoSwitch);
    pointSwitch	   = FIND_NODE("pointSwitch",	SoSwitch);
    normalSwitch   = FIND_NODE("normalSwitch",	SoSwitch);
    pickStyle	   = FIND_NODE("pickStyle",	SoPickStyle);
    transform	   = FIND_NODE("transform",	SoTransform);
    edgeSwitch	   = FIND_NODE("edgeSwitch",	SoSwitch);
    edgeCoords     = FIND_NODE("edgeCoords",	SoCoordinate3);
    end1Transform  = FIND_NODE("end1Transform",	SoTransform);
    end2Transform  = FIND_NODE("end2Transform",	SoTransform);

#undef FIND_NODE

    ////////////////////////////////////////////
    //
    // Set up the nodes
    //

    // Set up the event callback node to handle middle mouse presses
    ecb->addEventCallback(SoMouseButtonEvent::getClassTypeId(), mouseCB, this);

    readRoot->unref();
}

/////////////////////////////////////////////////////////////////////////////
//
// Finds a node in the read-in scene graph. Adds prefix first.
//
/////////////////////////////////////////////////////////////////////////////

SoNode *
SpotMarker::findNode(const char *name, const SoType &type)
{
    SbString fullName = prefixString;
    fullName += "_";
    fullName += name;

    SoNode *node = SoNode::getByName(fullName);

#if defined(FW_DEBUG)
    if (node == NULL) {
	fprintf(stderr, "Couldn't find node \"%s\"\n", fullName);
	exit(1);
    }
    else if (! node->isOfType(type)) {
	fprintf(stderr, "Node \"%s\" is the wrong type\n", fullName);
	exit(1);
    }
#endif /* FW_DEBUG */

    return node;
}

/////////////////////////////////////////////////////////////////////////////
//
// Sets the default position of the marker based on the given scene
// bounding box.
//
/////////////////////////////////////////////////////////////////////////////

void
SpotMarker::setDefaultPosition(const SbBox3f &sceneBox)
{
    if (root == NULL)
	buildSceneGraph();

    // Start from scratch
    transform->setToDefaults();

    SbVec3f center;
    if (sceneBox.isEmpty())
	center.setValue(0.0, 0.0, 0.0);
    else {
	// Set the transformation of the marker to be centered on
	// the bbox in x and y and pointing along +z in the front
	center = sceneBox.getCenter();
	center[2] = sceneBox.getMax()[2];
	transform->translation = center;
    }
    transform->rotation.setValue(SbVec3f(1.0, 0.0, 0.0), M_PI/2.0);

    // Initialize location and normal
    location = center;
    normal.setValue(0.0, 0.0, 1.0);
}

/////////////////////////////////////////////////////////////////////////////
//
// Sets the camera to use to compute the size
//
/////////////////////////////////////////////////////////////////////////////

void
SpotMarker::setCamera(SoCamera *camera)
{
    if (curCamera != camera) {
	if (curCamera != NULL)
	    curCamera->unref();
	curCamera = camera;
	if (curCamera != NULL)
	    curCamera->ref();

	haveViewVol = FALSE;
    }
}

/////////////////////////////////////////////////////////////////////////////
//
// Updates the size of the marker to be reasonable, given the current
// view volume.
//
/////////////////////////////////////////////////////////////////////////////

void
SpotMarker::updateSize()
{
    if (isDisplayed()) {

	// Update the view volume
	haveViewVol = FALSE;
	computeViewVolume();

	// Get the current location and size of the SpotMarker
	SbVec3f spotMarkerLoc  = transform->translation.getValue();
	float   spotMarkerSize = transform->scaleFactor.getValue()[0];

	// Compute a size that will make it occupy 1/20th of the window
	float newSize = curViewVol.getWorldToScreenScale(spotMarkerLoc, .05);

	// Set the size if it is different enough from the previous size
	if (newSize < .75 * spotMarkerSize || newSize > 1.333 * spotMarkerSize)
	    transform->scaleFactor.setValue(newSize, newSize, newSize);
    }
}

/////////////////////////////////////////////////////////////////////////////
//
// Turns display of the marker on or off.
//
/////////////////////////////////////////////////////////////////////////////

void
SpotMarker::setDisplay(SbBool flag, SbBool invokeCallback)
{
    if (displayed != flag) {
	if (root == NULL)
	    buildSceneGraph();

	displaySwitch->whichChild = (flag ? SO_SWITCH_ALL : SO_SWITCH_NONE);
	displayed = flag;

	if (invokeCallback && displayCB != NULL)
	    (*displayCB)(displayCBData);
    }
}

/////////////////////////////////////////////////////////////////////////////
//
// Sets whether scale snapping is enabled.
//
/////////////////////////////////////////////////////////////////////////////

void
SpotMarker::enableScaleSnapping(SbBool flag)
{
    if (scaleEnabled != flag) {
	if (root == NULL)
	    buildSceneGraph();

	scaleEnabled = flag;

	// Remove the feedback, if it's active
	edgeSwitch->whichChild = SO_SWITCH_NONE;
    }
}


/////////////////////////////////////////////////////////////////////////////
//
// This is called when the state of snapping changes. It updates
// the look of the marker based on the current state.
//
/////////////////////////////////////////////////////////////////////////////

void
SpotMarker::updateActiveParts(SbBool translating, SbBool rotating,
			      SbBool scaling)
{
    // Set the switches accordingly
    int child;

    child = translating ? 1 : 0;
    if (pointSwitch->whichChild.getValue() != child)
	pointSwitch->whichChild = child;

    child = rotating ? 1 : 0;
    if (normalSwitch->whichChild.getValue() != child)
	normalSwitch->whichChild = child;

    // Change the scale snapping only if it is active
    int edgeChild = edgeSwitch->whichChild.getValue();
    if (edgeChild >= 0) {
	child = scaling ? 1 : 0;
	if (edgeChild != child)
	    edgeSwitch->whichChild = child;
    }
}

/////////////////////////////////////////////////////////////////////////////
//
// Processes a mouse event.
//
/////////////////////////////////////////////////////////////////////////////

void
SpotMarker::processMouse(SoEventCallback *ecb)
{
    // The camera better have been set
    ASSERT(curCamera != NULL);

    //
    // If the left mouse button is pressed on the marker geometry, or
    // the middle mouse button is pressed anywhere, the user is
    // positioning the SpotMarker.
    //

    const SoEvent *event = ecb->getEvent();
    SoHandleEventAction *hea = ecb->getAction(); 

    if (event->isOfType(SoMouseButtonEvent::getClassTypeId())) {

	// Save the current viewport region aspect ratio so we can
	// update the view volume correctly
	const SbViewportRegion &vpReg =
	    SoViewportRegionElement::get(hea->getState());
	curAspectRatio = vpReg.getViewportAspectRatio();

	const SoMouseButtonEvent *mev = (const SoMouseButtonEvent *) event;

	// If the marker is not currently being dragged around
	if (! isActive) {

	    const SoPickedPoint *pp = NULL;
	    SbBool onDragger;

	    // If the left mouse button is on the marker or the middle
	    // mouse button was pressed, start dragging
	    if (SO_MOUSE_PRESS_EVENT(mev, BUTTON2) &&
		// (wasCtrlDown doesn't always return TRUE as non-zero)
		((mev->wasCtrlDown() != FALSE) == needsControlKey)) {

		pp = getPickedPoint(ecb, onDragger);

		isActive = TRUE;
		whichButton = SoMouseButtonEvent::BUTTON2;

		// Make sure the pick is not on the marker itself
		if (pp != NULL && pp->getPath()->containsNode(root)) {
		    // While the button is pressed, we make the SpotMarker
		    // unpickable, so it doesn't interfere with picking the
		    // objects in the scene.
		    pickStyle->style = SoPickStyle::UNPICKABLE;

		    // Force a re-pick
		    hea->setPickRoot(hea->getPickRoot());
		    pp = getPickedPoint(ecb, onDragger);
		}

		// If over background, turn marker off
		if (pp == NULL)
		    setDisplay(FALSE, TRUE);
	    }
	    else if (isDisplayed() && SO_MOUSE_PRESS_EVENT(mev, BUTTON1)) {
		pp = getPickedPoint(ecb, onDragger);

		if (pp != NULL && pp->getPath()->containsNode(root)) {
		    isActive = TRUE;
		    whichButton = SoMouseButtonEvent::BUTTON1;

		    // Set pp to NULL so we don't try to use the pick
		    // on the marker itself
		    pp = NULL;

		    // While the button is pressed, we make the SpotMarker
		    // unpickable, so it doesn't interfere with picking the
		    // objects in the scene.
		    pickStyle->style = SoPickStyle::UNPICKABLE;
		}
	    }

	    if (isActive) {
		// While the button is pressed, we make the SpotMarker
		// unpickable, so it doesn't interfere with picking the
		// objects in the scene.
		pickStyle->style = SoPickStyle::UNPICKABLE;

		// Initialize curDist so we know that it hasn't been set
		curDist = -1.0;

		if (pp != NULL) {
		    const SbViewportRegion &vpReg = hea->getViewportRegion();
		    setLocation(pp, event->getNormalizedPosition(vpReg),
				whichButton == SoMouseButtonEvent::BUTTON2,
				mev->wasShiftDown(), onDragger);
		}

		ecb->addEventCallback(SoLocation2Event::getClassTypeId(),
				      mouseCB, this);

		// Make sure we are the correct size
		updateSize();

		ecb->setHandled();
	    }
	}

	// If the active button was released, stop
	else if (SoMouseButtonEvent::isButtonReleaseEvent(mev, whichButton)) {
	    ecb->removeEventCallback(SoLocation2Event::getClassTypeId(),
				     mouseCB, this);

	    // Make the marker pickable again
	    pickStyle->setToDefaults();

	    // Make sure our size is ok
	    updateSize();

	    updateActiveParts(FALSE, FALSE, FALSE);
	    isActive = FALSE;

	    ecb->setHandled();
	}
    }

    // If the mouse moved while the middle button is down, move the SpotMarker
    else {
	ASSERT(isActive);
	ASSERT(event->isOfType(SoLocation2Event::getClassTypeId()));

	const SbViewportRegion &vpReg = hea->getViewportRegion();
	SbBool onDragger;
	const SoPickedPoint *pp = getPickedPoint(ecb, onDragger);
	setLocation(pp, event->getNormalizedPosition(vpReg),
		    whichButton == SoMouseButtonEvent::BUTTON2,
		    event->wasShiftDown(), onDragger);

	ecb->setHandled();
    }
}

/////////////////////////////////////////////////////////////////////////////
//
// Computes and stores the view volume based on the current camera
//
/////////////////////////////////////////////////////////////////////////////

void
SpotMarker::computeViewVolume()
{
    if (! haveViewVol) {
	ASSERT(curCamera != NULL);
	curViewVol = curCamera->getViewVolume(curAspectRatio);
	haveViewVol = TRUE;
    }
}

/////////////////////////////////////////////////////////////////////////////
//
// Sets the location of the marker based on the given picked
// point. If the point is NULL and removeIfNoHit is TRUE, this
// removes the current SpotMarker. If doSnap is TRUE, the marker is
// snapped to important points. If onDragger is TRUE, snapping is
// done very carefully to avoid extra work.
//
/////////////////////////////////////////////////////////////////////////////

void
SpotMarker::setLocation(const SoPickedPoint *pp, const SbVec2f &mousePos,
			SbBool removeIfNoHit, SbBool doSnap, SbBool onDragger)
{
    // If there is no picked object and we want to remove the SpotMarker,
    // remove it
    if (pp == NULL && removeIfNoHit)
	setDisplay(FALSE, TRUE);

    // Otherwise, we need to find a new location for the SpotMarker
    else {
	// We need to determine the location, orientation, and
	// size of the marker in world space. That is the space we use
	// to set the transform of the SpotMarker. This location may snap
	// to important points, if doSnap is TRUE.
	getLocation(pp, mousePos, doSnap, onDragger);

	// Transform the marker - this may flip the normal
	setRotation(normal);
	transform->translation = location;

	// If we are at an edge, set up the scale edge feedback (if
	// scale snapping is enabled)
	if (isScaleSnappingEnabled() && atEdge) {

	    // Add a scale constraint
	    SbVec3f edgeVec = (edgeEnd2 - edgeEnd1);
	    edgeLength = edgeVec.normalize();

	    // Set the endpoints of the edge
	    edgeCoords->point.set1Value(0, edgeEnd1);
	    edgeCoords->point.set1Value(1, edgeEnd2);

	    // Set up the transformations on the end markers
	    end1Transform->translation = edgeEnd1;
	    end2Transform->translation = edgeEnd2;

	    SbVec3f yAxis(0.0, 1.0, 0.0);
	    end1Transform->rotation = SbRotation(-yAxis, edgeVec);
	    end2Transform->rotation = SbRotation( yAxis, edgeVec);

	    edgeSwitch->whichChild = 0;
	}
	else {
	    edgeEnd1.setValue(0.0, 0.0, 0.0);
	    edgeEnd2.setValue(0.0, 0.0, 0.0);
	    edgeLength = 0.0;
	    edgeSwitch->whichChild = SO_SWITCH_NONE;
	}

	// Let the derived classes do extra work
	locationChanged();

	// Make sure we are displayed
	if (! isDisplayed())
	    setDisplay(TRUE, TRUE);
    }
}

/////////////////////////////////////////////////////////////////////////////
//
// Called when the location changes. Does nothing by default.
//
/////////////////////////////////////////////////////////////////////////////

void
SpotMarker::locationChanged()
{
}

/////////////////////////////////////////////////////////////////////////////
//
// Stores the attachment of the marker for the given picked point. If
// doSnap is TRUE, this checks to see if the pick is near an important
// point and moves it.
//
/////////////////////////////////////////////////////////////////////////////

void
SpotMarker::getLocation(const SoPickedPoint *pp, const SbVec2f &mousePos,
			SbBool doSnap, SbBool onDragger)
{
    // If the pick didn't hit anything, set up a default SpotMarker
    // attachment
    if (pp == NULL) {

	// Make sure the view volume is up to date
	computeViewVolume();

	// Compute the distance to the viewpoint if not already done
	if (curDist < 0.0) {
	    SbVec3f v = (transform->translation.getValue() - 
			 curViewVol.getProjectionPoint());
	    curDist = fabs(v.dot(curViewVol.getProjectionDirection()));
	}
	location = curViewVol.getPlanePoint(curDist, mousePos);
	normal   = -curViewVol.getProjectionDirection();
	atEdge   = FALSE;
	return;
    }

    // See if we hit a construction tool
#if 0
//mvo: ignore this for now - FW has no construction tools
    CtBase *tool = NULL;
    for (int i = 0; i < pp->getPath()->getLength(); i++) {
	if (pp->getPath()->getNode(i)->isOfType(CtBase::getClassTypeId())) {
	    tool = (CtBase *) pp->getPath()->getNode(i);
	    break;
	}
    }

    // If we hit a construction tool, let it do its own snapping
    if (tool != NULL)
	tool->snap(pp, location, normal, atEdge, edgeEnd1, edgeEnd2);

    // Otherwise, use the point as is and snap if necessary
    else 
#endif
    {

	// Get the world-space point and normal
	location = pp->getPoint();
	normal   = pp->getNormal();

	// Assume we are not at an edge
	atEdge = FALSE;

	SbBool isSnapped;

	// Snapping requires extra work
	if (doSnap) {

	    // Save the space conversion matrix and the object-space point
	    // of the pick intersection
	    objectToWorld = pp->getObjectToWorld();
	    objLocation   = pp->getObjectPoint();

	    // Compute the average size and principal planes
	    computeSizeAndPlanes(pp);

	    // Try to snap to the bounding box, if the manipulator is attached
	    if (onDragger)
		isSnapped = snapToBBox();

	    // If no manipulator, see if the point should snap to an
	    // important point on the intersected object
	    else if (pp->getDetail() != NULL) {
		const SoDetail *detail = pp->getDetail();
		SoType type = detail->getTypeId();

		// See if the intersection is near a vertex or edge of
		// a vertex-based shape
		if (type.isDerivedFrom(SoFaceDetail::getClassTypeId()))
		    isSnapped = snapToFace(pp, (SoFaceDetail *) detail);
		else if (type.isDerivedFrom(SoLineDetail::getClassTypeId()))
		    isSnapped = snapToEdge(pp, (SoLineDetail *) detail);

		// Also check primitive shape types
#if 0
//mvo
		else if (type.isDerivedFrom(SoConeDetail::getClassTypeId()))
		    isSnapped = snapToCone(pp, (SoConeDetail *) detail);
		else if (type.isDerivedFrom(SoCubeDetail::getClassTypeId()))
		    isSnapped = snapToCube(pp, (SoCubeDetail *) detail);
		else if (type.isDerivedFrom(SoCylinderDetail::
					    getClassTypeId()))
		    isSnapped = snapToCylinder(pp, (SoCylinderDetail *)detail);
#endif
	    }

	    // Check shapes that have no details
	    else {
#if 0
//mvo
		SoNode *tail = ((SoFullPath *) pp->getPath())->getTail();
		if (tail->isOfType(VrmlSphere::getClassTypeId()))
		    isSnapped = snapToSphere(pp);

		else
#endif
		    isSnapped = FALSE;
	    }
	}
	else
	    isSnapped = FALSE;

	// If the marker was snapped, indicate so by turning on the
	// active version
	if (isSnapped)
	    updateActiveParts(TRUE, TRUE, atEdge);
	else
	    updateActiveParts(FALSE, FALSE, FALSE);
    }
}

/////////////////////////////////////////////////////////////////////////////
//
// Computes and stores the correct rotation in the SpotMarker
// transform based on the given normal vector and the current view
// volume. The rotation keeps the marker normal pointed toward the
// viewer at all times. Note: this may flip the normal.
//
/////////////////////////////////////////////////////////////////////////////

void
SpotMarker::setRotation(SbVec3f &normal)
{
    // Make sure the view volume is up to date
    computeViewVolume();

    // Flip the normal if it faces away from the viewer
    if (normal.dot(curViewVol.getProjectionDirection()) > 0.0)
	normal.negate();

    transform->rotation = SbRotation(SbVec3f(0.0, 1.0, 0.0), normal);
}

/////////////////////////////////////////////////////////////////////////////
//
// Computes the average object size and bounding box planes for a
// picked object.
//
/////////////////////////////////////////////////////////////////////////////

void
SpotMarker::computeSizeAndPlanes(const SoPickedPoint *pp)
{
    // First, we need to find the bounding box in object space; the
    // transformation to world space is likely to make it too large.
    // To find the bbox in object space, we use the path from the root
    // of the picked object down to this leaf shape node to reset the
    // bbox action when applying it to the picked object.

    // ??? Should use a real vpreg?
    if (bboxAction == NULL)
	bboxAction = new SoGetBoundingBoxAction(SbViewportRegion(100,100));

    bboxAction->setResetPath(pp->getPath());
    bboxAction->apply(pp->getPath());
    SbBox3f bbox = bboxAction->getBoundingBox();

    // Compute the average size of the object to use for snapping
    // comparisons
    SbVec3f sizes = bbox.getMax() - bbox.getMin();
    averageObjectSize = (sizes[0] + sizes[1] + sizes[2]) / 3.0;

    // Set the principal planes of the object-space box
    planes[0] = bbox.getMin();
    planes[1] = bbox.getCenter();
    planes[2] = bbox.getMax();
}

/////////////////////////////////////////////////////////////////////////////
//
// Snaps the current attachment to an important point of the bounding
// box.
//
/////////////////////////////////////////////////////////////////////////////

SbBool
SpotMarker::snapToBBox()
{
    // See if the object-space point is near a corner or center point
    // of the bbox. Do this by comparing each coordinate (separately)
    // with those of the principal planes.
    SbBool numSnapped = 0, numSnappedToEnd = 0, snappedPlane[3];

    // For each of the 3 dimensions...
    for (int dim = 0; dim < 3; dim++) {

	// First, compute the minimum distance to a bbox plane
	float minDist = 1e12;
	int   whichPlane;
	for (int i = 0; i < 3; i++) {
	    float dist = fabs(planes[i][dim] - objLocation[dim]);
	    if (dist < minDist) {
		minDist = dist;
		whichPlane = i;
	    }
	}

	// If this distance is close enough, move the point to the
	// corresponding plane
	if (minDist < tolerance * averageObjectSize) {
	    objLocation[dim] = planes[whichPlane][dim];
	    numSnapped++;
	    snappedPlane[dim] = whichPlane;
	    if (whichPlane != 1)
		numSnappedToEnd++;
	}
	else
	    snappedPlane[dim] = -1;
    }

    // If we snapped the object-space point...
    if (numSnapped > 0) {

	// Compute a new world-space point
	moveToPoint(objLocation);

	// If the point is at an edge, we want to return the
	// world-space ends of that edge. The point is at an edge
	// if it snapped to exactly 2 min or max planes (the
	// center plane doesn't count).
	if (numSnappedToEnd == 2) {

	    atEdge = TRUE;

	    // Find the object-space endpoints of the edge
	    SbVec3f objEnd1, objEnd2;
	    for (int i = 0; i < 3; i++) {

		// If this is the one dimension not snapped to an
		// end plane:
		if (snappedPlane[i] != 0 && snappedPlane[i] != 2) {
		    objEnd1[i] = planes[0][i];
		    objEnd2[i] = planes[2][i];
		}

		// If this is one of the two dimensions snapped to
		// an end plane:
		else {
		    objEnd1[i] = planes[snappedPlane[i]][i];
		    objEnd2[i] = planes[snappedPlane[i]][i];
		}
	    }

	    // Transform the ends into world space
	    objectToWorld.multVecMatrix(objEnd1, edgeEnd1);
	    objectToWorld.multVecMatrix(objEnd2, edgeEnd2);
	}

	return TRUE;
    }
    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
// Snaps to the face of a vertex-based shape represented by the given
// SoFaceDetail.
//
/////////////////////////////////////////////////////////////////////////////

SbBool
SpotMarker::snapToFace(const SoPickedPoint *pp, const SoFaceDetail *fd)
{
    const SbVec3f *coords = findCoordinates(pp);

    // Find the closest edge
    int numVerts = fd->getNumPoints();
    float closestEdgeDist = 1e12;
    SbVec3f closestEdge1, closestEdge2;
    SbVec3f closestEdgePoint;
    SbVec3f prevVert = coords[fd->getPoint(numVerts-1)->getCoordinateIndex()];
	int i;
    for (i = 0; i < numVerts; i++) {
	SbVec3f vert = coords[fd->getPoint(i)->getCoordinateIndex()];

	SbVec3f closestPt =
	    SbLine(prevVert, vert).getClosestPoint(objLocation);
	float edgeDist = (closestPt - objLocation).length();
	if (edgeDist < closestEdgeDist) {
	    closestEdgeDist  = edgeDist;
	    closestEdge1     = prevVert;
	    closestEdge2     = vert;
	    closestEdgePoint = closestPt;
	}

	prevVert = vert;
    }

    // Attach to the closest edge if we are close enough
    if (closestEdgeDist < tolerance * averageObjectSize) {
	atEdge = TRUE;
	objectToWorld.multVecMatrix(closestEdge1, edgeEnd1);
	objectToWorld.multVecMatrix(closestEdge2, edgeEnd2);

	// If close enough to either of the endpoints or the center
	// point of the edge, snap to it
	SbVec3f point[3];
	float dist[3];

	point[0] = closestEdge1;
	point[1] = closestEdge2;
	point[2] = (closestEdge1 + closestEdge2) / 2.0f;

	float minDist = 1e12;
	int minPoint = -1;
	for (i = 0; i < 3; i++) {
	    dist[i] = (point[i] - objLocation).length();
	    if (dist[i] < minDist) {
		minPoint = i;
		minDist = dist[i];
	    }
	}
	ASSERT(minPoint >= 0);
	if (minDist < tolerance * averageObjectSize)
	    moveToPoint(point[minPoint]);

	// Otherwise, just snap to the edge
	else
	    moveToPoint(closestEdgePoint);

	return TRUE;
    }

    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
// Snaps to the edge of a vertex-based shape represented by the given
// SoLineDetail.
//
/////////////////////////////////////////////////////////////////////////////

SbBool
SpotMarker::snapToEdge(const SoPickedPoint *pp, const SoLineDetail *ld)
{
    const SbVec3f *coords = findCoordinates(pp);
    SbVec3f p0 = coords[ld->getPoint0()->getCoordinateIndex()];
    SbVec3f p1 = coords[ld->getPoint1()->getCoordinateIndex()];

    atEdge = TRUE;
    objectToWorld.multVecMatrix(p0, edgeEnd1);
    objectToWorld.multVecMatrix(p1, edgeEnd2);

    // Move to a vertex if we are close enough
    float dist0 = (p0 - objLocation).length();
    float dist1 = (p1 - objLocation).length();
    float dist = (dist1 < dist0 ? dist1 : dist0);
    if (dist < tolerance * averageObjectSize) {
	location = (dist1 < dist0 ? edgeEnd2 : edgeEnd1);
	return TRUE;
    }
    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
// Snaps to an important point of a cone.
//
/////////////////////////////////////////////////////////////////////////////

#if 0
//mvo

SbBool
SpotMarker::snapToCone(const SoPickedPoint *pp, SoConeDetail *cd)
{
    SbBool isSnapped = FALSE;

    // Get the intersected cone and its height and bottom radius
    VrmlCone *cone = (VrmlCone *) ((SoFullPath *) pp->getPath())->getTail();
    ASSERT(cone->isOfType(VrmlCone::getClassTypeId()));
    float height = cone->height.getValue() / 2.0;
    float radius = cone->bottomRadius.getValue();

    // Get the object-space intersection point
    SbVec3f objPoint = pp->getObjectPoint();

    // See which part was intersected
    switch (cd->getPart()) {
      case VrmlCone::SIDES:
	// See if the intersection is close enough to the top,
	// center, or bottom to snap it there
	if (snapToHeight(objPoint[1], height)) {
	    // Adjust the x and z values accordingly
	    if (objPoint[1] > 0.0) {
		// Move to apex
		objPoint[0] = objPoint[2] = 0.0;
	    }
	    else if (objPoint[1] < 0.0) {
		// Move to base
		float scale = radius / sqrtf(objPoint[0]*objPoint[0] +
					     objPoint[2]*objPoint[2]);
		objPoint[0] *= scale;
		objPoint[2] *= scale;
	    }
	    else {
		// Move to center
		float scale = 0.5 * radius / sqrtf(objPoint[0]*objPoint[0] +
						   objPoint[2]*objPoint[2]);
		objPoint[0] *= scale;
		objPoint[2] *= scale;
	    }
	    moveToPoint(objPoint);
	    isSnapped = TRUE;
	}
	break;

      case VrmlCone::BOTTOM:
	// See if the intersection is close enough to the center or
	// edges of the bottom circular face
	if (snapToDisc(objPoint[0], objPoint[2], radius)) {
	    moveToPoint(objPoint);
	    isSnapped = TRUE;
	}
	break;

      default:
	// Don't know what to do with this
	break;
    }

    return isSnapped;
}

/////////////////////////////////////////////////////////////////////////////
//
// Snaps to an important point of a cube.
//
/////////////////////////////////////////////////////////////////////////////

SbBool
SpotMarker::snapToCube(const SoPickedPoint *pp, SoCubeDetail *cd)
{
    // Get the intersected cube and its sizes
    VrmlCube *cube = (VrmlCube *) ((SoFullPath *) pp->getPath())->getTail();
    ASSERT(cube->isOfType(VrmlCube::getClassTypeId()));
    SbVec3f size(cube->width.getValue() / 2.0,
		 cube->height.getValue() / 2.0,
		 cube->depth.getValue() / 2.0);

    // Get the object-space intersection point
    SbVec3f objPoint = pp->getObjectPoint();

    // We need to check for snapping in the 2 dimensions of the picked
    // face
    int dim1, dim2;

    // See which part (face) was intersected
    switch (cd->getPart()) {
      case 0: // Front
      case 1: // Back
	dim1 = 0;
	dim2 = 1;
	break;
      case 2: // Left
      case 3: // Right
	dim1 = 1;
	dim2 = 2;
	break;
      case 4: // Top
      case 5: // Bottom
	dim1 = 0;
	dim2 = 2;
	break;

      default:
	// Don't know what to do with this
	return FALSE;
    }

    // Make sure to check both dimensions!
    SbBool snapped1 = snapToHeight(objPoint[dim1], size[dim1]);
    SbBool snapped2 = snapToHeight(objPoint[dim2], size[dim2]);
    if (snapped1 || snapped2) {
	moveToPoint(objPoint);
	return TRUE;
    }
    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
// Snaps to an important point of a cylinder.
//
/////////////////////////////////////////////////////////////////////////////

SbBool
SpotMarker::snapToCylinder(const SoPickedPoint *pp, SoCylinderDetail *cd)
{
    SbBool isSnapped = FALSE;

    // Get the intersected cylinder
    VrmlCylinder *cyl = (VrmlCylinder *)
	((SoFullPath *) pp->getPath())->getTail();
    ASSERT(cyl->isOfType(VrmlCylinder::getClassTypeId()));

    // Get the object-space intersection point
    SbVec3f objPoint = pp->getObjectPoint();

    // See which part was intersected
    switch (cd->getPart()) {
      case VrmlCylinder::SIDES:
	// See if the intersection is close enough to the top,
	// center, or bottom to snap it there
	if (snapToHeight(objPoint[1], cyl->height.getValue() / 2.0)) {
	    moveToPoint(objPoint);
	    isSnapped = TRUE;
	}
	break;

      case VrmlCylinder::TOP:
      case VrmlCylinder::BOTTOM:
	// See if the intersection is close enough to the center or
	// edges of the top or bottom circular face
	if (snapToDisc(objPoint[0], objPoint[2], cyl->radius.getValue())) {
	    moveToPoint(objPoint);
	    isSnapped = TRUE;
	}
	break;

      default:
	// Don't know what to do with this
	break;
    }
    return isSnapped;
}

/////////////////////////////////////////////////////////////////////////////
//
// Snaps to an important point of a sphere.
//
/////////////////////////////////////////////////////////////////////////////

SbBool
SpotMarker::snapToSphere(const SoPickedPoint *pp)
{
    // Get the intersected sphere and its height and bottom radius
    VrmlSphere *sphere =
	(VrmlSphere *) ((SoFullPath *) pp->getPath())->getTail();
    ASSERT(sphere->isOfType(VrmlSphere::getClassTypeId()));
    float radius = sphere->radius.getValue();

    // Get the object-space intersection point
    SbVec3f objPoint = pp->getObjectPoint();

    float tol = radius * tolerance;

    // See if the point is close to a pole. To find the closest pole,
    // all we have to do is find the coordinate with the largest
    // absolute value.
    int   dim = -1;
    float val = -1.0;
    for (int i = 0; i < 3; i++) {
	float v = fabs(objPoint[i]);
	if (v > val) {
	    val = v;
	    dim = i;
	}
    }
    ASSERT(dim >= 0);

    // Now we need to see if the point is close enough to the pole. If
    // so, move there.
    if (val > radius - tol) {
	SbBool isNegative = (objPoint[dim] < 0.0);

	// Move the point
	objPoint.setValue(0.0, 0.0, 0.0);
	objPoint[dim] = (isNegative ? -radius : radius);
	moveToPoint(objPoint);

	// Also move the normal
	SbVec3f norm(0.0, 0.0, 0.0);
	norm[dim] = (isNegative ? -1.0 : 1.0);
	objectToWorld.inverse().transpose().multDirMatrix(norm, normal);
	normal.normalize();

	return TRUE;
    }

    // If it's not close to a pole, see if it is close enough to any of the 3
    // equators. To find the closest equator, find the coordinate with
    // the smallest absolute value.
    dim = -1;
    val = 2.0 * radius;
    for (i = 0; i < 3; i++) {
	float v = fabs(objPoint[i]);
	if (v < val) {
	    val = v;
	    dim = i;
	}
    }
    ASSERT(dim >= 0);

	// See if the point is close enough to the pole. If so, move there.
    if (val < tol) {
	// Move the point
	objPoint[dim] = 0.0;
	objPoint *= radius / objPoint.length();
	moveToPoint(objPoint);

	// Also move the normal
	objectToWorld.inverse().transpose().multDirMatrix(objPoint, normal);
	normal.normalize();

	return TRUE;
    }

    return FALSE;
}

//mvo
#endif	// #if 0

/////////////////////////////////////////////////////////////////////////////
//
// If the given (x,z) point is close enough to the center or edge of
// the disk with the given radius centered at (0,0), this snaps the x
// and z values and returns TRUE.
//
/////////////////////////////////////////////////////////////////////////////

SbBool
SpotMarker::snapToDisc(float &x, float &z, float radius)
{
    // See if the intersection is close enough to the center or
    // edges of the disc
    float tol = radius * tolerance;
    float dist = sqrtf(x*x + z*z);
    ASSERT(dist >= 0.0 && dist <= radius);

    // Closer to center
    if (dist < radius / 2.0) {
	if (dist < tol) {
	    x = z = 0.0;
	    return TRUE;
	}
	// Closer to edge
    }
    else {
	if (radius - dist < tol) {
	    x *= radius / dist;
	    z *= radius / dist;
	    return TRUE;
	}
    }
    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
// If the given y value is close enough to 0 or the positive or
// negative height value, this snaps the y value and returns TRUE.
//
/////////////////////////////////////////////////////////////////////////////

SbBool
SpotMarker::snapToHeight(float &y, float height)
{
    float tol = 2.0 * height * tolerance;
    ASSERT(y <= height && y >= -height);

    // If closest to top
    if (y > height / 2.0) {
	if (height - y < tol) {
	    y = height;
	    return TRUE;
	}
    }
    // If closest to bottom
    else if (y < -height / 2.0) {
	if (y - -height < tol) {
	    y = -height;
	    return TRUE;
	}
    }
    // If closest to center
    else {
	if (y < tol / 2.0 && y > -tol / 2.0) {
	    y = 0.0;
	    return TRUE;
	}
    }
    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
// Moves the spot location to the given object space point
//
/////////////////////////////////////////////////////////////////////////////

void
SpotMarker::moveToPoint(const SbVec3f &objPoint)
{
    objectToWorld.multVecMatrix(objPoint, location);
}
    
/////////////////////////////////////////////////////////////////////////////
//
// Finds coordinates affecting the tail of the path in the given
// picked point.
//
/////////////////////////////////////////////////////////////////////////////

const SbVec3f *
SpotMarker::findCoordinates(const SoPickedPoint *pp)
{
    // If the tail of the path is an indexed shape with a vertex
    // property node, use the coordinates from it
    SoNode *tail = ((const SoFullPath *) pp->getPath())->getTail();
    if (tail->isOfType(SoVertexShape::getClassTypeId())) {
	SoVertexProperty *vp = (SoVertexProperty *)
	    ((SoVertexShape *) tail)->vertexProperty.getValue();
	if (vp != NULL && vp->vertex.getNum() > 0)
	    return vp->vertex.getValues(0);
    }

    // Otherwise, search for coordinate and SoVertexProperty nodes
    if (searchAction == NULL) {
	searchAction = new MultiSearchAction;
	SoTypeList coordTypes;
	coordTypes.append(SoCoordinate3::getClassTypeId());
	coordTypes.append(SoVertexProperty::getClassTypeId());
	searchAction->setInterest(SoSearchAction::ALL);
	searchAction->setTypeList(coordTypes);
    }

    searchAction->apply(pp->getPath());

    // There has to be at least one or we couldn't have picked the shape
    ASSERT(searchAction->getPaths().getLength() > 0);

    // Use the last one that has coordinates in it
    for (int i = searchAction->getPaths().getLength() - 1; i >= 0; i--) {
	tail = ((const SoFullPath *) searchAction->getPaths()[i])->getTail();
	if (tail->isOfType(SoCoordinate3::getClassTypeId())) {
	    SoCoordinate3 *coords = (SoCoordinate3 *) tail;
	    return coords->point.getValues(0);
	}
	else {
	    ASSERT(tail->isOfType(SoVertexProperty::getClassTypeId()));
	    SoVertexProperty *vp = (SoVertexProperty *) tail;
	    if (vp->vertex.getNum() > 0)
		return vp->vertex.getValues(0);
	}
    }

    // We should never get here
    ASSERT(FALSE);
    return NULL;
}

/////////////////////////////////////////////////////////////////////////////
//
// Returns the picked point to use to place the SpotMarker. This
// returns TRUE in onDragger if a dragger surrounding an object was
// hit.
//
/////////////////////////////////////////////////////////////////////////////

const SoPickedPoint *
SpotMarker::getPickedPoint(SoEventCallback *ecb, SbBool &onDragger)
{
    const SoPickedPoint *pp = ecb->getPickedPoint();

    // If no intersection, return it as is
    if (pp == NULL) {
	onDragger = FALSE;
	return NULL;
    }

    // If we didn't hit a dragger, return it as is. If it hit the
    // "box" part of the dragger (translation planes), we can use the
    // intersection as is, also.
    SbBool onBox;
    onDragger = isOnDragger(pp, onBox);
    if (! onDragger || onBox)
	return pp;

    // If it hit another part of the dragger, we need to look at the
    // rest of the intersections. Ask for all intersections from the action
    const SoPickedPointList &pps = ecb->getAction()->getPickedPointList();

    // Find the first one that did not hit a dragger or hit the box of
    // the dragger
    for (int i = 0; i < pps.getLength(); i++) {
	onDragger = isOnDragger(pps[i], onBox);
	if (! onDragger || onBox)
	    return pps[i];
    }

    // If we get here, only bad parts of draggers were hit. Return NULL.
    return NULL;
}

/////////////////////////////////////////////////////////////////////////////
//
// Returns TRUE if the given picked point is on a dragger. onBox
// is set to TRUE if the pick is on one of the translation planes
// of the dragger, which form the main box.
//
/////////////////////////////////////////////////////////////////////////////

SbBool
SpotMarker::isOnDragger(const SoPickedPoint *pp, SbBool &onBox)
{
    SbBool onDragger = FALSE;

    const SoFullPath *path = (const SoFullPath *) pp->getPath();

    for (int i = 0; i < path->getLength(); i++) {

	SoNode *node = path->getNode(i);
	SoType type  = node->getTypeId();

	if (type.isDerivedFrom(SoDragger::getClassTypeId())) {

	    // Any pick on a CubeDragger is just fine
	    if (type.isDerivedFrom(CubeDragger::getClassTypeId()))
		onDragger = onBox = TRUE;

#if 0
	    // There may be other draggers in the scene. Make sure
	    // this is an SoTransformer2Dragger
	    else if (type.isDerivedFrom(SoTransformer2Dragger::
					getClassTypeId())) {

		onDragger = TRUE;

		SoTransformer2Dragger *d = (SoTransformer2Dragger *) node;
#else
		// There may be other draggers in the scene. Make sure
	    // this is an SoTransformerDragger
	    else if (type.isDerivedFrom(SoTransformerDragger::
					getClassTypeId())) {

		onDragger = TRUE;

		SoTransformerDragger *d = (SoTransformerDragger *) node;
#endif

		// See if the pick is on a translation part
		onBox =
		    (path->containsNode(d->getPart("translator1", FALSE)) ||
		     path->containsNode(d->getPart("translator2", FALSE)) ||
		     path->containsNode(d->getPart("translator3", FALSE)) ||
		     path->containsNode(d->getPart("translator4", FALSE)) ||
		     path->containsNode(d->getPart("translator5", FALSE)) ||
		     path->containsNode(d->getPart("translator6", FALSE)));

		// If we are near an edge of the manipulator box part,
		// we may have intersected a side that faces away from
		// the camera. (Sorting of intersection points is a
		// little tricky at exact edges.) Reject any point if
		// the normal faces away from the viewer.
		if (onBox) {
		    // Make sure the view volume is up to date
		    computeViewVolume();
		    const SbVec3f &norm = pp->getNormal();
		    if (norm.dot(curViewVol.getProjectionDirection()) >= 0.0)
			onBox = FALSE;
		}
	    }
	}
    }

    return onDragger;
}
