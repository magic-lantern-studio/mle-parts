/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file iv3dset.h
 * @ingroup MleParts
 *
 * This file contains the class for a 3D Set targeting the Inventor
 * platform.
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

#ifndef __MLE_IV3DSET_H_
#define __MLE_IV3DSET_H_

// Include Magic Lantern Runtime header files.
#include "mle/MleSet.h"
#include "mle/Mle3dSet.h"

class SbVec2s;
class SoSeparator;
class SoCamera;
class SoSwitch;
class SoPath;
class SoRotation;
class SoFieldSensor;

class MlVector3;
class MlRotation;
class MlTransform;

// XXX - major hack: for now both this class and the brender equivalent
// look like the same class. atk/Player.cxx includes br3dset.h and
// casts 3d sets to that flavor of Mle3dSet. For that reason, the
// two classes MUST HAVE THE SAME VIRTUAL TABLES. This will be fixed
// after beta (bug #374766), but for now beware!

class MLE3DSET_API Mle3dSet : public MleSet
{
    MLE_SET_HEADER(Mle3dSet);

  public:

    Mle3dSet(void);
    
    virtual void attach(MleRole *parent,MleRole *child);

    // Camera position.
    virtual int setCameraTransform(MlTransform *t);

    virtual void getCameraTransform(MlTransform *t);

    virtual int setCameraPosition(const MlVector3 *position);

    virtual void getCameraPosition(MlVector3 *position);

    virtual int setCameraOrientation(const MlRotation *orientation);

    virtual void getCameraOrientation(MlRotation *orientation);

    virtual int setCameraNearClipping(MlScalar nearPlane);

    virtual MlScalar getCameraNearClipping(void);

    virtual int setCameraFarClipping(MlScalar farPlane);

    virtual MlScalar getCameraFarClipping(void);

    virtual int setCameraAspect(MlScalar aspect);

    virtual MlScalar getCameraAspect(void);
        
#ifdef MLE_REHEARSAL

    // Getting attributes of the stage.  These methods should be abstract.
    virtual const char** getFunctions();

    virtual const char** getFunctionAttributes(char* functionName);

    // Geometry queries.
    //virtual void getBounds(MlScalar min[3],MlScalar max[3]);

    virtual void projectScreenCoordinates(int x,int y,MlScalar *coord);

    virtual void intersectScreenCoordinates(int x, int y, MlScalar *coord);

#endif /* MLE_REHEARSAL */

    //// /////// everything above here must be same as br3set //// //////
    //// ///////  (see note at top of class definition XXX)   //// //////

    // Set-specific api.
#ifdef MLE_REHEARSAL

    virtual SoPath *getPickPath(const SbVec2s &point);

#endif /* MLE_REHEARSAL */

    SoSeparator *getRoot(void) const { return m_root; }

    SoCamera *getCamera(void) const;

    SoSwitch *getCameraSwitch(void) const { return m_cameraSwitch; }

  protected:

	/** Root of the set scene graph. */
    SoSeparator *m_root;
	/** Switch over ortho and perspective cameras. */
    SoSwitch *m_cameraSwitch;

    void getStageSize(int *width, int *height);

#ifdef MLE_REHEARSAL

    SoRotation *m_lightRot;

    static void cameraCB(Mle3dSet *set, SoFieldSensor *);

#endif /* MLE_REHEARSAL */

};

#endif /* __MLE_IV3DSET_H_ */
