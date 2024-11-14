/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file iv3dset.h
 * @ingroup MleParts
 *
 * This file contains the class for a 3D Set targeting the Inventor
 * platform.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2018-2024 Wizzer Works
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

class MLE_3DSET_API Mle3dSet : public MleSet
{
    MLE_SET_HEADER(Mle3dSet);

  public:

    Mle3dSet(void);

    virtual void init(void);
    
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

#ifdef MLE_DIGITAL_WORKPRINT

    SoRotation *m_lightRot;

    static void cameraCB(Mle3dSet *set, SoFieldSensor *);

#endif /* MLE_DIGITAL_WORKPRINT */

};

#endif /* __MLE_IV3DSET_H_ */
