/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file br3dset.h
 * @ingroup MleParts
 *
 * This file contains the class for a 3D Set targeting the BRender
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

#ifndef __MLE_BR3DSET_H_
#define __MLE_BR3DSET_H_

// Include system header files.
#include <limits.h>

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleSet.h"

// Include Magic Lantern Math header files.
#include "math/vector.h"

// Include BRender target header files.
#ifdef WIN32
#include "mle/brendapi.h"
#include "mle/brstage.h"

#define MAX_DIRTY_RECTANGLES 16
#endif /* WIN32 */

struct br_actor;
class MlVector3;
class MlRotation;
class MlTransform;


// XXX major hack: for now both this class and the brender equivalent
// look like the same class. atk/Player.c++ includes br3dset.h and
// casts 3d Sets to that flavor of Mle3dSet. For that reason, the
// two classes MUST HAVE THE SAME VIRTUAL TABLES. This will be fixed
// after beta (bug #374766), but for now beware!

/**
 * @brief This class defines the BRender 2D Set.
 */
class Mle3dSet : public MleSet
{
    MLE_SET_HEADER(Mle3dSet);

  public:

    Mle3dSet(void);
    
    virtual void attach(MleRole *parent,MleRole *child);
    
    // Camera controls.

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

    // Functions available to the authoring tools.
    virtual const char** getFunctions();

    virtual const char** getFunctionAttributes(char *functionName);

    // Geometry queries.
    virtual void getBounds(MlScalar min[3],MlScalar max[3]);

    virtual void projectScreenCoordinates(int x,int y,MlScalar *coord);

    virtual void intersectScreenCoordinates(int x, int y, MlScalar *coord);

#endif /* MLE_REHEARSAL */
    
    ////// ///// Everything above here must be same as iv3dset //// //////
    ////// ///// (see note at top of class definition XXX)     //// //////
    
    // Set-specific API.

    static void render(MleSet *set);

	// Camera access.

    virtual int setCamera(MleActor *newCamera);
    
    // This routine returns the camera that the Set manages.
    br_actor *getCamera(void) const { return m_camera; }

#ifdef MLE_REHEARSAL

    // Picking.
    MleRole *pickRole(int x,int y);

    // Global render mode support for brender stage.
    virtual void setRenderMode(char *mode);

    virtual char *getRenderMode();

#endif /* MLE_REHEARSAL */
    
    // The properties exposed in the DWP.

    // position - contains x and y position in screen coordinate,
    // (0, 0) corresponds to the lower left corner of the window.
    MlVector2 position;

	// size - contains width and height in screen coordinate.
    MlVector2 size;
    
  protected:

    br_actor *m_root;      // Root of the scene graph.
    br_actor *m_camera;    // Camera associated with the scene graph.

#ifdef WIN32

    // All management of screen clears and dirty rectangles is being
    // done in the Set (probably will change).
    
    friend void BR_CALLBACK ViewBoundsCallback(
					       br_actor *actor,
					       br_model *model,
					       br_material *material,
					       void *render_data,
					       br_uint_8 style,
					       br_matrix4 *model_to_screen,
					       br_int_32 *bounds);
    
    // Table of dirty rectangles.
    brwin_dirty_rect m_dirtyRectangles[MAX_DIRTY_RECTANGLES];
    int m_lastDirtyRectangle;
    
    // Union of rectangles that were drawn during this frame.
    brwin_dirty_rect m_renderBounds;
    
    // Union of rectangles that were cleared during this frame.
    brwin_dirty_rect m_clearBounds;

#endif /* WIN32 */
};

#endif /* __MLE_BR3DSET_H_ */
