/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file SpotMarker.h
 * @ingroup MlParts
 *
 * This file defines a class representing a marker displayed for snapping
 * operations.
 */

 // COPYRIGHT_BEGIN
 //
 // The MIT License (MIT)
 //
 // Copyright (c) 2017-2025 Wizzer Works
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


#ifndef  __SPOTMARKER_H_
#define  __SPOTMARKER_H_

// Include Inventor header files.
#include <Inventor/SbBox.h>
#include <Inventor/events/SoMouseButtonEvent.h>

// Declare external classes.
class MultiSearchAction;
class SoCamera;
class SoCoordinate3;
class SoEventCallback;
class SoConeDetail;
class SoCubeDetail;
class SoCylinderDetail;
class SoFaceDetail;
class SoLineDetail;
class SoPickStyle;
class SoPickedPoint;
class SoSeparator;
class SoSwitch;
class SoTransform;

/**
 * Typedef for callback that gets invoked when the display state of
 * the marker is changed via a mouse action.
 */
typedef void SpotMarkerDisplayCB(void *userData);


/**
 * Base class representing a marker displayed for snapping operations.
 * It allows interactive placement of the marker object using the 
 * mouse. The Target and Source classes are derived from this.
 */
class SpotMarker
{

  public:
    // Constructor takes flag indicating whether the Control key needs
    // to be down to place the marker and a prefix string used to
    // access nodes in the read-in scene graph.
    SpotMarker(SbBool needsControlKey, const char *prefix);
    ~SpotMarker();

    // Returns the scene graph that displays the marker and interacts
    // with it. The root of this graph remains until this instance is
    // destroyed.
    SoSeparator *	getSceneGraph();

    // Sets/returns the marker snapping tolerance. If a point is
    // picked on an object within this tolerance of an important point
    // (such as a corner of a bbox), the marker is snapped to that
    // important point. The tolerance is expressed as a fraction of
    // the object size, which is the average dimension. The default is
    // 0.1 of the size.
    void		setSnappingTolerance(float frac) { tolerance = frac; }
    float		getSnappingTolerance() const	 { return tolerance; }

    // Sets the default position of the marker based on the given
    // scene bounding box
    void		setDefaultPosition(const SbBox3f &sceneBox);

    // Sets the camera to use to compute the size
    void		setCamera(SoCamera *camera);

    // Updates the size of the marker to be reasonable, given the
    // current view volume
    void		updateSize();

    // Returns the current view volume used to view the marker
    const SbViewVolume &getViewVolume()
	{ computeViewVolume(); return curViewVol; }

    // Sets a callback to invoke whenever the display state of the
    // marker changes as the result of a user mouse action
    void		setDisplayCallback(SpotMarkerDisplayCB *cb,
					   void *userData = NULL)
	{ displayCB = cb; displayCBData = userData; }

    // Turns display of the marker on or off, or returns status
    void		setDisplay(SbBool flag, SbBool invokeCallback = FALSE);
    SbBool		isDisplayed() const	{ return displayed; }

    // Sets/returns whether scale snapping is enabled
    void		enableScaleSnapping(SbBool flag);
    SbBool		isScaleSnappingEnabled() const { return scaleEnabled; }

    // Returns the current marker point, normal, and length. The
    // returned length is 0.0 if scale snapping is not enabled or the
    // marker is not at an edge.
    const SbVec3f &	getPoint() const	{ return location; }
    const SbVec3f &	getNormal() const	{ return normal; }
    float		getLength() const	{ return edgeLength; }

    // Returns the two end points of the edge, if scale snapping is
    // enabled
    const SbVec3f &	getEdgeEnd1() const	{ return edgeEnd1; }
    const SbVec3f &	getEdgeEnd2() const	{ return edgeEnd2; }

    // This is called when the state of snapping changes. It updates
    // the look of the marker based on the current state.
    void		updateActiveParts(SbBool translating,
					  SbBool rotating,
					  SbBool scaling);

  protected:
    SpotMarkerDisplayCB	*displayCB;	// Callback for display changes
    void		*displayCBData;	// Data for callback
    SbBool		needsControlKey;
    const char *	prefixString;
    SoCamera		*curCamera;
    float		curAspectRatio;
    SbViewVolume	curViewVol;
    SbBool		haveViewVol;
    float		tolerance;
    SbBool		displayed;
    SbBool		scaleEnabled;	// Scale snapping enabled
    SbBool		isActive;	// Actively handling events
    float		curDist;	// Current distance from viewVol
    SoMouseButtonEvent::Button	whichButton;	// Which button used to drag

    // SpotMarker attachment info:
    SbVec3f	location;	// Location of marker
    SbVec3f	normal;		// Normal vector at that point
    SbBool	atEdge;		// True if marker is near an important edge
    SbVec3f	objLocation;	// Location in object space
    SbMatrix	objectToWorld;	// Object-space to world-space conversion
    SbVec3f	planes[3];	// Planes of bounding box in object space
    float	averageObjectSize; // Average of 3 dimensional sizes of object
    SbVec3f	edgeEnd1;	// One end of edge
    SbVec3f	edgeEnd2;	// Other end of edge
    float	edgeLength;	// Length of edge

    // Important nodes:
    SoSeparator		*root;
    SoSwitch		*displaySwitch;	// Turns display on or off
    SoSwitch		*pointSwitch;	// Switches to active version
    SoSwitch		*normalSwitch;	// Switches to active version
    SoPickStyle		*pickStyle;	// Disables picking
    SoTransform		*transform;	// Places marker in scene
    SoSwitch		*edgeSwitch;	// Turns scale edge on or off
    SoCoordinate3	*edgeCoords;	// Coordinates of scale edge
    SoTransform		*end1Transform, *end2Transform; // Transform ends

    // Actions:
    SoGetBoundingBoxAction *bboxAction;	  // For computing object sizes
    MultiSearchAction	   *searchAction; // For searching for coordinates

    // This contains the data for reading the scene graph
    static unsigned const char sceneBuffer[];

    // Builds the scene graph
    virtual void	buildSceneGraph();

    // Finds a node in the read-in scene graph. Adds prefix first.
    SoNode *		findNode(const char *name, const SoType &type);

    // Processes a mouse event
    void		processMouse(SoEventCallback *ecb);

    // Computes and stores the view volume based on the current camera
    void		computeViewVolume();

    // Sets the location of the marker based on the given picked
    // point. If the point is NULL and removeIfNoHit is TRUE, this
    // removes the current marker. If doSnap is TRUE, the marker is
    // snapped to important points. If onDragger is TRUE, snapping is
    // done very carefully to avoid extra work.
    void		setLocation(const SoPickedPoint *pp,
				    const SbVec2f &mousePos,
				    SbBool removeIfNoHit,
				    SbBool doSnap, SbBool onDragger);

    // Called when the location changes. Does nothing by default.
    virtual void	locationChanged();

    // Stores the attachment of the marker for the given picked
    // point. If doSnap is TRUE, this checks to see if the pick is
    // near an important point and moves it.
    void		getLocation(const SoPickedPoint *pp,
				    const SbVec2f &mousePos,
				    SbBool doSnap, SbBool onDragger);

    // Computes and stores the correct rotation in the marker
    // transform based on the given normal vector and the current view
    // volume. The rotation keeps the marker normal pointed toward the
    // viewer at all times. Note: this may flip the normal.
    void		setRotation(SbVec3f &normal);

    // Computes the average object size and bounding box planes for a
    // picked object
    void		computeSizeAndPlanes(const SoPickedPoint *pp);

    // Snaps the current attachment to an important point of the
    // bounding box
    SbBool		snapToBBox();

    // Snaps to the face or edge of a vertex-based shape represented
    // by the given SoFaceDetail or SoLineDetail
    SbBool		snapToFace(const SoPickedPoint *pp,
				   const SoFaceDetail *fd);
    SbBool		snapToEdge(const SoPickedPoint *pp,
				   const SoLineDetail *ld);

    // Snaps to an important point of a primitive shape
    SbBool		snapToCone(const SoPickedPoint *pp,
				   SoConeDetail *cd);
    SbBool		snapToCube(const SoPickedPoint *pp,
				   SoCubeDetail *cd);
    SbBool		snapToCylinder(const SoPickedPoint *pp,
				       SoCylinderDetail *cd);
    SbBool		snapToSphere(const SoPickedPoint *pp);

    // These are used to snap to a part of a primitive shape
    SbBool		snapToDisc(float &x, float &z, float radius);
    SbBool		snapToHeight(float &y, float height);

    // Moves the spot location to the given object space point
    void		moveToPoint(const SbVec3f &objPoint);

    // Finds coordinates affecting the tail of the path in the given
    // picked point
    const SbVec3f *	findCoordinates(const SoPickedPoint *pp);

    // Returns the picked point to use to place the marker. This
    // returns TRUE in onDragger if a dragger was hit.
    const SoPickedPoint * getPickedPoint(SoEventCallback *ecb,
					 SbBool &onDragger);

    // Returns TRUE if the given picked point is on a dragger. onBox
    // is set to TRUE if the pick is on one of the translation planes
    // of the dragger, which form the main box.
    SbBool		isOnDragger(const SoPickedPoint *pp, SbBool &onBox);

    // Event callback:
    static void		mouseCB(void *userData, SoEventCallback *ecb) 
	{ ((SpotMarker *) userData)->processMouse(ecb); }
};

#endif /* __SPOTMARKER_H_ */
