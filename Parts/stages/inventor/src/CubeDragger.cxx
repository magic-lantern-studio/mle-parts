/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file CubeDragger.cxx
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 * @date Jan 24, 2006
 *
 * The CubeDragger is a translation-only version of the
 * SoTransformerDragger. It has the same look and feel as that dragger if you
 * forget about all the rotation and scale stuff.
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
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/events/SoEvent.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/nodes/SoLocateHighlight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/projectors/SbLineProjector.h>
#include <Inventor/projectors/SbPlaneProjector.h>
#include <Inventor/sensors/SoFieldSensor.h>

// Include Magic Lantern Rehearsal Player header files.
#include "CubeDragger.h"
#include "CubeDraggerGeom.h"

SO_KIT_SOURCE(CubeDragger);

/////////////////////////////////////////////////////////////////////////////
//
// Initializes the class.
//
/////////////////////////////////////////////////////////////////////////////

void
CubeDragger::initClass()
{
    //SO_KIT_INIT_CLASS(CubeDragger, SoGestureDragger, "GestureDragger");
	SO_KIT_INIT_CLASS(CubeDragger, SoTransformerDragger, "GestureDragger");
}

/////////////////////////////////////////////////////////////////////////////
//
// Constructor.
//
/////////////////////////////////////////////////////////////////////////////

CubeDragger::CubeDragger()
{
    SO_KIT_CONSTRUCTOR(CubeDragger);

    SO_KIT_ADD_CATALOG_ENTRY(surroundScale, SoSurroundScale, 
			     TRUE, topSeparator, geomSeparator,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(overallStyle, SoGroup, TRUE,
			     topSeparator, NULL, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(mainSep, SoSeparator, TRUE,
			     topSeparator, NULL, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(mainSwitch, SoSwitch, TRUE,
			     mainSep, NULL, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(inactiveSep, SoSeparator, TRUE,
			     mainSwitch, NULL, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(activeRootSep, SoSeparator, FALSE,
			     mainSwitch, NULL, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(activeSep, SoSeparator, FALSE,
			     activeRootSep, NULL, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(activeRotation, SoRotation, TRUE,
			     activeSep, NULL, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(active, SoSeparator, TRUE,
			     activeSep, NULL, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(axisFeedbackTransform, SoTransform, TRUE,
			     activeRootSep, NUKK, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(axisFeedbackSwitch, SoSwitch, TRUE,
			     activeRootSep, NULL, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(singleAxisFeedback, SoSeparator, TRUE,
			     axisFeedbackSwitch, NULL, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(doubleAxisFeedback, SoSeparator, TRUE,
			     axisFeedbackSwitch, NULL, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(crosshairFeedbackSwitch, SoSwitch, TRUE,
			     activeRootSep, NULL, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(crosshairFeedback, SoSeparator, TRUE,
			     crosshairFeedbackSwitch, NULL, TRUE);

    // read geometry for shared parts
    if (SO_KIT_IS_FIRST_INSTANCE())
	readDefaultParts("cubeDragger.iv", (const char *)geomBuffer, sizeof(geomBuffer));

    SO_KIT_ADD_FIELD(scaleFactor, (1.0, 1.0, 1.0));
    SO_KIT_ADD_FIELD(rotation,    (SbRotation::identity()));
    SO_KIT_ADD_FIELD(translation, (0.0, 0.0, 0.0));

    SO_KIT_INIT_INSTANCE();

    currentSide = restartSide = NONE;
    planeProj = new SbPlaneProjector;
    lineProj  = new SbLineProjector;

    setAllDefaultParts();

    // Add the callbacks
    addStartCallback(&startCB);
    addMotionCallback(&motionCB);
    addFinishCallback(&finishCB);
    addOtherEventCallback(&metaKeyChangeCB);
    addValueChangedCallback(&valueChangedCB);

    // Updates the motionMatrix when any field is set.
    translationSensor = new SoFieldSensor(&fieldSensorCB, this);
    rotationSensor    = new SoFieldSensor(&fieldSensorCB, this);
    scaleSensor       = new SoFieldSensor(&fieldSensorCB, this);
    translationSensor->setPriority(0);
    rotationSensor->setPriority(0);
    scaleSensor->setPriority(0);

    setUpConnections(TRUE, TRUE);

    // Empirical testing shows better results with this many pixels
    // before selecting gesture
    setMinGesture(15);
}

/////////////////////////////////////////////////////////////////////////////
//
// Sets up default part geometry.
//
/////////////////////////////////////////////////////////////////////////////

void
CubeDragger::setAllDefaultParts()
{
    enableNotify(FALSE);

    setAnyPartAsDefault("overallStyle",		"cubeOverallStyle");
    setAnyPartAsDefault("inactiveSep", 		"cubeInactiveSep");
    setPartAsDefault("active",			"cubeActive");
    setPartAsDefault("singleAxisFeedback",	"cubeSingleAxisFeedback");
    setPartAsDefault("doubleAxisFeedback",	"cubeDoubleAxisFeedback");
    setPartAsDefault("crosshairFeedback",	"cubeCrosshairFeedback");

    showFeedback();

    // Set the Locate Highlight colors if the resource is there:
    SoNode *hln = SoNode::getByName("cubeLocateMaterial");
    if (hln && hln->isOfType(SoMaterial::getClassTypeId())) {
	SoMaterial *hlm = (SoMaterial *) hln;
	SbColor emc = hlm->emissiveColor[0];
	SbColor dfd = hlm->diffuseColor[0];

	// The children of the inactive separator are the highlight
	// groups
	SoSeparator *sep = (SoSeparator *) inactiveSep.getValue();
	for (int i = 0; i < sep->getNumChildren(); i++) {
	    SoNode *node = sep->getChild(i);
	    if (node->isOfType(SoLocateHighlight::getClassTypeId())) {
		SoLocateHighlight *lhg = (SoLocateHighlight *) node;
		lhg->enableNotify(FALSE);
		lhg->color = emc;
		lhg->style  = SoLocateHighlight::EMISSIVE_DIFFUSE;
		lhg->enableNotify(TRUE);
	    }
	}
    }

    enableNotify(TRUE);
    touch();
}

/////////////////////////////////////////////////////////////////////////////
//
// Destructor.
//
/////////////////////////////////////////////////////////////////////////////

CubeDragger::~CubeDragger()
{
    delete planeProj;
    delete lineProj;

    if (translationSensor != NULL)
	delete translationSensor; 
    if (rotationSensor != NULL)
	delete rotationSensor; 
    if (scaleSensor != NULL)
	delete scaleSensor; 
}

/////////////////////////////////////////////////////////////////////////////
//
// Sets up or breaks connections in dragger.
//
/////////////////////////////////////////////////////////////////////////////

SbBool
CubeDragger::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (! doItAlways && connectionsSetUp == onOff)
	return onOff;

    if (onOff) {
	// We connect AFTER base class.
	//SoGestureDragger::setUpConnections(onOff, FALSE);
	SoTransformerDragger::setUpConnections(onOff, FALSE);

	// Call the sensor CBs to make things are up-to-date.
	fieldSensorCB(this, NULL);

	// Connect the field sensor
	if (translationSensor->getAttachedField() != &translation)
	    translationSensor->attach(&translation);
	if (rotationSensor->getAttachedField() != &rotation)
	    rotationSensor->attach(&rotation);
	if (scaleSensor->getAttachedField() != &scaleFactor)
	    scaleSensor->attach(&scaleFactor);
    }
    else {
	// Disconnect the field sensor
	if (translationSensor->getAttachedField() != NULL)
	    translationSensor->detach();
	if (rotationSensor->getAttachedField() != NULL)
	    rotationSensor->detach();
	if (scaleSensor->getAttachedField() != NULL)
	    scaleSensor->detach();

	// Then do base class stuff
	//SoGestureDragger::setUpConnections(onOff, FALSE);
	SoTransformerDragger::setUpConnections(onOff, FALSE);
    }

    connectionsSetUp = onOff;

    return ! onOff;
}

/////////////////////////////////////////////////////////////////////////////
//
// Prepares for dragging.
//
/////////////////////////////////////////////////////////////////////////////

void
CubeDragger::dragStart()
{
    // Invalidate surroundScale if it exists.
    SoSurroundScale *ss = (SoSurroundScale *) surroundScale.getValue();
    if (ss != NULL)
	ss->invalidate();

    // Make a note of which modifier keys are down.
    altDown  = getEvent()->wasAltDown();
    ctlDown  = getEvent()->wasCtrlDown();
    shftDown = getEvent()->wasShiftDown();

    constraining = shftDown;

    // Determine what side we are dragging. We can re-use the side
    // stored in restartSide if we are starting a new gesture from the
    // metaKey callback. Otherwise, we need to use the pick info to
    // find the side.
    currentSide = restartSide;
    if (currentSide == NONE)
	setSideFromPick();

    // Reset direction for gesture-selected constrained motion
    // But if we are restarting and also constraining to a direction,
    // leave it alone:
    if (restartSide == NONE || ! constraining)
	currentDir = -1;

    // Show the correct feedback
    showFeedback();

    // This is the point we'll use if a metaKey callback makes us re-start.
    worldRestartPt = getWorldStartingPoint();

    // Initialize the translation
    if (currentSide != NONE) {
	// Calculations will be in world space, so that gesture based
	// constraints will be evenly weighted in all 3 directions instead
	// of skewed by scale of object.
	SbVec3f planePt = getWorldStartingPoint();

	SbVec3f planeNormal;
	switch (currentSide) {
	  case FRONT:
	  case BACK:
	    planeNormal.setValue(0.0, 0.0, 1.0);
	    break;
	  case LEFT:
	  case RIGHT:
	    planeNormal.setValue(1.0, 0.0, 0.0);
	    break;
	  case TOP:
	  case BOTTOM:
	    planeNormal.setValue(0.0, 1.0, 0.0);
	    break;
	}
	planeNormal = getBoxDirInWorldSpace(planeNormal);
	planeNormal.normalize();

	// For calculating motion within the plane.
	planeProj->setPlane(SbPlane(planeNormal, planePt));
	planeProj->setWorkingSpace(SbMatrix::identity());

	// If alt key is down, we also need a projector perpendicular to plane.
	if (altDown) {
	    lineProj->setLine(SbLine(planePt, planePt + planeNormal));
	    lineProj->setWorkingSpace(SbMatrix::identity());
	}
    }
}

/////////////////////////////////////////////////////////////////////////////
//
// Sets currentSide based on what was picked.
//
/////////////////////////////////////////////////////////////////////////////

void
CubeDragger::setSideFromPick()
{
    const SoFullPath *pickPath = (const SoFullPath *) getPickPath();

    // Leave the currentSide as NONE if there's no pick on us
    if (pickPath == NULL || ! pickPath->containsNode(this))
	return;

    // Since we know that the 6 children of the inactive separator are
    // the 6 faces (in the correct order), we just have to look at the
    // correct index in the picked path. Start at the bottom.
    SoNode *inactSep = inactiveSep.getValue();
	int i;
    for (i = pickPath->getLength() - 1; i >= 0; i--)
	if (pickPath->getNode(i) == inactSep)
	    break;
    if (i < 0) {
	fprintf(stderr, "CubeDragger: couldn't find inactive separator\n");
	return;
    }

    currentSide = (Side) pickPath->getIndex(i + 1);
}

/////////////////////////////////////////////////////////////////////////////
//
// Drags.
//
/////////////////////////////////////////////////////////////////////////////

void
CubeDragger::drag()
{
    if (currentSide == NONE)
	return;

    // All calculations are in world space:
    SbVec3f newHit;

    if (! altDown) {
	// Calculate intersection with plane projector.
	// Don't bother with setWorkingSpace. For world space, always
	// identity()
	planeProj->setViewVolume(getViewVolume());
	newHit = planeProj->project(getNormalizedLocaterPosition());

	// If we need to start a new gesture, we'll carry on from this pt.
	worldRestartPt = newHit;
    }
    else {
	// Move perpendicular to the face

	// Calculate intersection with line projector.
	lineProj->setViewVolume(getViewVolume());
	newHit = lineProj->project(getNormalizedLocaterPosition());

	// Move the projection plane up to the height of the new hit.
	const SbPlane &oldPlane = planeProj->getPlane();
	const SbVec3f &oldNorm  = oldPlane.getNormal();
	planeProj->setPlane(SbPlane(oldNorm, newHit));

	// Intersect with the new plane to find the new worldRestartPt
	// If we need to start a new gesture, we'll carry on from this pt.
	worldRestartPt = planeProj->project(getNormalizedLocaterPosition());
    }

    // find the difference between current and beginning intersections.
    SbVec3f worldMotion = newHit - getWorldStartingPoint();

    // If not constraining or we are doing perpendicular motion, clear
    // the 1D translation direction
    if (! constraining || altDown)
	currentDir = -1;

    // Of we are constraining, compute the 1D translation direction
    // (if necessary) based on which direction got the maximum locater
    // motion
    else {
	if (currentDir == -1) {
	    // If we are not ready to pick a direction yet, don't move
	    if (! isAdequateConstraintMotion())
		return;

	    SbBool xf = TRUE, yf = TRUE, zf = TRUE;
	    switch (currentSide) {
	      case FRONT:
	      case BACK:
		zf = FALSE;
		break;
	      case LEFT: 
	      case RIGHT:
		xf = FALSE;
		break;
	      case TOP:
	      case BOTTOM: 
		yf = FALSE;
		break;
	    }
	    currentDir = getMouseGestureDirection(xf, yf, zf);
	    showFeedback();
	}

	// Project 'workSpaceMotion' onto the preferred axis.
	SbVec3f dirVec(0.0, 0.0, 0.0);
	dirVec[currentDir] = 1.0;
	dirVec = getBoxDirInWorldSpace(dirVec);
	dirVec.normalize();

	worldMotion = dirVec * dirVec.dot(worldMotion);
    }

    // Append this to the startMotionMatrix, which we saved at the beginning
    // of the drag, to find the current motion matrix.
    // We need to send our matrix that converts from world space to localSpace.
    SbMatrix wldToLcl = getWorldToLocalMatrix();
    setMotionMatrix(appendTranslation(getStartMotionMatrix(), worldMotion,
				      &wldToLcl));
}

/////////////////////////////////////////////////////////////////////////////
//
// Handles a meta key press or release.
//
/////////////////////////////////////////////////////////////////////////////

void
CubeDragger::handleMetaKey()
{
    SoHandleEventAction *ha = getHandleEventAction();

    // Only do this if we are grabbing events
    if (ha->getGrabber() != this)
	return;

    // We only want key press or release events.
    const SoEvent *event = getEvent();
    if (! SO_KEY_PRESS_EVENT(event, ANY) && ! SO_KEY_RELEASE_EVENT(event, ANY))
	return;

    // Is the key constrain, modify, or control?
    const SoKeyboardEvent *ke = (const SoKeyboardEvent *) event;
    SoKeyboardEvent::Key key = ke->getKey();

    if (key == SoKeyboardEvent::LEFT_ALT   ||
	key == SoKeyboardEvent::RIGHT_ALT  || 
	key == SoKeyboardEvent::LEFT_SHIFT ||
	key == SoKeyboardEvent::RIGHT_SHIFT) {

	// We want to end the old gesture and start a new one.

	// [A] Release the grabber. This ends the gesture and calls all 
	//     finishCallbacks (on parent dragger, too, if we're registered)
	//     Remember the side so you can restart with it after releasing.
	Side savedSide = currentSide;
	ha->releaseGrabber();

	// [B] Set the starting point to be our saved worldRestartPoint
	restartSide = savedSide;
	setStartingPoint(worldRestartPt);

	// [C] Become the grabber again. This begins a new gesture and
	//     calls all startCallbacks (parent dragger, too).  Info
	//     like viewVolume, viewportRegion, handleEventAction, and
	//     tempPathToThis is still valid.
	ha->setGrabber(this);

	// [D] set handled
	ha->setHandled();
    }
}

/////////////////////////////////////////////////////////////////////////////
//
// Finishes dragging.
//
/////////////////////////////////////////////////////////////////////////////

void
CubeDragger::dragFinish()
{
    currentSide = restartSide = NONE;
    showFeedback();

    // Invalidate surroundScale if it exists.
    SoSurroundScale *ss = (SoSurroundScale *) surroundScale.getValue();
    if (ss != NULL)
	ss->invalidate();
}

/////////////////////////////////////////////////////////////////////////////
//
// Shows the correct feedback based on the current side.
//
/////////////////////////////////////////////////////////////////////////////

void
CubeDragger::showFeedback()
{
    // Inactive? Show the default version of the cube
    if (currentSide == NONE) {
	setSwitchValue(mainSwitch.getValue(), 0);
	return;
    }

    // Turn on the active version
    setSwitchValue(mainSwitch.getValue(), 1);

    // Set up the feedback transformations
    SoRotation *r = (SoRotation *) activeRotation.getValue();
    if (r == NULL) {
	r = new SoRotation;
	setAnyPart("activeRotation", r);
    }

    // Rotate the highlighted side from the front to the correct side
    SbRotation rot;
    switch (currentSide) {
      case FRONT:
	rot = SbRotation::identity();
	break;
      case BACK:
	rot.setValue(SbVec3f(0.0, 1.0, 0.0), M_PI);
	break;
      case LEFT:
	rot.setValue(SbVec3f(0.0, 1.0, 0.0), -M_PI/2.0);
	break;
      case RIGHT:
	rot.setValue(SbVec3f(0.0, 1.0, 0.0), M_PI/2.0);
	break;
      case TOP:
	rot.setValue(SbVec3f(1.0, 0.0, 0.0), -M_PI/2.0);
	break;
      case BOTTOM:
	rot.setValue(SbVec3f(1.0, 0.0, 0.0), M_PI/2.0);
	break;
    }
    r->rotation = rot;

    // If constraining, show the correct single axis
    if (altDown || currentDir >= 0 && currentDir <= 2) {
	setSwitchValue(axisFeedbackSwitch.getValue(), 0);

	// We also need to rotate the default vertical axis to the
	// correct orientation
	SbVec3f startAxis(0.0, 1.0, 0.0);
	SbVec3f rotatedAxis;
	rot.multVec(startAxis, rotatedAxis);

	int dir;
	if (altDown) {
	    switch (currentSide) {
	      case FRONT:
	      case BACK:
		dir = 2;
		break;
	      case LEFT:
	      case RIGHT:
		dir = 0;
		break;
	      case TOP:
	      case BOTTOM:
		dir = 1;
		break;
	    }

	    // Also turn on the crosshairs
	    setSwitchValue(crosshairFeedbackSwitch.getValue(), 0);
	}
	else {
	    dir = currentDir;
	    // Turn off the crosshairs
	    setSwitchValue(crosshairFeedbackSwitch.getValue(), SO_SWITCH_NONE);
	}

	SbVec3f desiredAxis(0.0, 0.0, 0.0);
	desiredAxis[dir] = 1.0;

	rot *= SbRotation(rotatedAxis, desiredAxis);
    }

    // Otherwise, show the double axis
    else
	setSwitchValue(axisFeedbackSwitch.getValue(), 1);

    // Translate the axes to the cursor location and rotate them to
    // the correct side
    SoTransform *xf = (SoTransform *) axisFeedbackTransform.getValue();
    if (xf == NULL) {
	xf = new SoTransform;
	setAnyPart("axisFeedbackTransform", xf);
    }
    xf->translation = getWorldPointInBoxSpace(getWorldStartingPoint());
    xf->rotation = rot;
}

/////////////////////////////////////////////////////////////////////////////
//
// Handles a value changed callback.
//
/////////////////////////////////////////////////////////////////////////////

void
CubeDragger::valueChanged()
{
    SbMatrix motMat = getMotionMatrix();

    SbVec3f trans, scale;
    SbRotation rot, scaleOrient;
    getTransformFast(motMat, trans, rot, scale, scaleOrient);

    // Disconnect the field sensors
    translationSensor->detach();
    rotationSensor->detach();
    scaleSensor->detach();

    if (translation.getValue() != trans)
	translation = trans;
    if (rotation.getValue() != rot)
	rotation = rot;
    if (scaleFactor.getValue() != scale)
	scaleFactor = scale;

    // Reconnect the field sensors
    translationSensor->attach(&translation);
    rotationSensor->attach(&rotation);
    scaleSensor->attach(&scaleFactor);
}

/////////////////////////////////////////////////////////////////////////////
//
// Handles a change to a field.
//
/////////////////////////////////////////////////////////////////////////////

void
CubeDragger::fieldChanged()
{
    // Incorporate the new field value into the matrix 
    SbMatrix motMat = getMotionMatrix();
    workFieldsIntoTransform(motMat);
    setMotionMatrix(motMat);
}

/////////////////////////////////////////////////////////////////////////////
//
// Sets default on fields we don't want to write out.
//
/////////////////////////////////////////////////////////////////////////////

void
CubeDragger::setDefaultOnNonWritingFields()
{
    surroundScale.setDefault(TRUE);

    // Call the base class...
    //SoGestureDragger::setDefaultOnNonWritingFields();
	SoTransformerDragger::setDefaultOnNonWritingFields();
}
