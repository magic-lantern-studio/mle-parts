/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file CubeDragger.h
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

#ifndef  __CUBEDRAGGER_H_
#define  __CUBEDRAGGER_H_

//#include <Inventor/draggers/SoGestureDragger.h>
#include <Inventor/draggers/SoTransformerDragger.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFVec3f.h>

class SbPlaneProjector;
class SbLineProjector;
class SoFieldSensor;
class SoSensor;

//class CubeDragger : public SoGestureDragger
class CubeDragger : public SoTransformerDragger
{

    SO_KIT_HEADER(CubeDragger);

    SO_KIT_CATALOG_ENTRY_HEADER(overallStyle);
    SO_KIT_CATALOG_ENTRY_HEADER(surroundScale);
    SO_KIT_CATALOG_ENTRY_HEADER(mainSep);
    SO_KIT_CATALOG_ENTRY_HEADER(mainSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(inactiveSep);
    SO_KIT_CATALOG_ENTRY_HEADER(activeRootSep);
    SO_KIT_CATALOG_ENTRY_HEADER(activeSep);
    SO_KIT_CATALOG_ENTRY_HEADER(activeRotation);
    SO_KIT_CATALOG_ENTRY_HEADER(active);
    SO_KIT_CATALOG_ENTRY_HEADER(axisFeedbackTransform);
    SO_KIT_CATALOG_ENTRY_HEADER(axisFeedbackSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(singleAxisFeedback);
    SO_KIT_CATALOG_ENTRY_HEADER(doubleAxisFeedback);
    SO_KIT_CATALOG_ENTRY_HEADER(crosshairFeedbackSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(crosshairFeedback);

  public:
    // This is used to return the side being dragged for translation
    enum Side {
	FRONT, BACK,
	LEFT, RIGHT,
	TOP, BOTTOM,
	NONE
    };

    // Constructor
    CubeDragger();

    // Fields. Note that even though the dragger does only
    // translation, it needs all fields so that undo will work
    // correctly.
    SoSFVec3f		translation;
    SoSFVec3f		scaleFactor;
    SoSFRotation	rotation;

  public:
    // This returns the side being dragged during translation. It
    // makes sense to call this only from a dragger callback.
    Side		getCurrentSide() const { return currentSide; }

  SoINTERNAL public:
    static void		initClass();	// initialize the class

  private:
    // Character string from which the shared geometry is read
    static const char	geomBuffer[];

    Side		currentSide;
    Side		restartSide;
    int			currentDir;   // Records current axis being used
    SoFieldSensor	*translationSensor, *rotationSensor, *scaleSensor;
    SbPlaneProjector	*planeProj;
    SbLineProjector	*lineProj;
    SbVec3f		worldRestartPt;
    SbBool		altDown, ctlDown, shftDown;
    SbBool		constraining;

    // Destructor
    virtual ~CubeDragger();

    // Prepare for dragging, drag, finish dragging
    void		dragStart();
    void		drag();
    void		dragFinish();

    // Handles a value changed callback
    void		valueChanged();

    // Handles a change to a field
    void		fieldChanged();

    // Handles a meta key press or release
    void		handleMetaKey();

    // Other dragger stuff
    virtual SbBool	setUpConnections(SbBool onOff,
					 SbBool doItAlways = FALSE);
    virtual void	setDefaultOnNonWritingFields();

    // Sets currentSide based on what was picked
    void		setSideFromPick();

    // Shows the correct feedback based on the current state
    void		showFeedback();

    // Sets up default part geometry
    void		setAllDefaultParts();

    // Callback invokers:
    static void		startCB(void *, SoDragger *d)
	{ ((CubeDragger *) d)->dragStart(); }
    static void		motionCB(void *, SoDragger *d)
	{ ((CubeDragger *) d)->drag(); }
    static void		finishCB(void *, SoDragger *d)
	{ ((CubeDragger *) d)->dragFinish(); }
    static void		valueChangedCB(void *, SoDragger *d)
	{ ((CubeDragger *) d)->valueChanged(); }
    static void		fieldSensorCB(void *userData, SoSensor *)
	{ ((CubeDragger *) userData)->fieldChanged(); }
    static void		metaKeyChangeCB(void *, SoDragger *d)
	{ ((CubeDragger *) d)->handleMetaKey(); }
};

#endif /* __CUBEDRAGGER_H_ */
