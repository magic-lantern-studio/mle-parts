/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file CubeDragger.h
 * @ingroup MlParts
 *
 * The CubeDragger is a translation-only version of the
 * SoTransformerDragger. It has the same look and feel as that dragger if you
 * forget about all the rotation and scale stuff.
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
    static const unsigned char geomBuffer[];

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
