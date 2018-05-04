/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file Nudger.h
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 * @date Jan 24, 2006
 *
 * This file defines a class that allows objects to be "nudged" a little
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

#ifndef  __NUDGER_H_
#define  __NUDGER_H_

// Include Inventor header files.
#include <Inventor/SbLinear.h>
#include <Inventor/SbPList.h>
#include <Inventor/SbViewportRegion.h>

//class CoSelectionContext;
//class CoSelector;
//class SoUndoItem;

//class SoPath;

class MleActor;

class Nudger
{

  public:
    // Constructor takes the selection context in which to work
    Nudger();
    ~Nudger();

    // Nudges all selected objects by the given number of pixels in
    // the given direction (X=0, Y=1, or Z=2). numPixels can be
    // negative.
    void		nudge(SoPath *selection, 
			      MleActor *actor, 
			      const SbViewVolume &viewVol,
			      const SbViewportRegion &vpReg,
			      int direction, int numPixels);

    // This clears the current nudging state, so that the next nudging
    // operation will begin a new set. By default, consecutive nudges
    // are compressed into one operation for undo/redo. This divides
    // them up.
//    void		resetUndoState();

  private:
//    CoSelectionContext	*selectionContext;
    SbViewVolume	viewVol;
    float		pixelSize;
//    SoUndoItem		*lastUndoItem;
    SbBool		needToSetUp;
    int			numObjects;	// Number of objects being nudged
    int			curObject;	// Which object being nudged
    SbPList		infos;		// Pointers to NudgerObjectInfos

    // mvo - from CoSceneManager
    static SbViewportRegion	    *actionViewportRegion;
    static SoGetBoundingBoxAction  *bboxAction;
    static SoGetMatrixAction	    *matrixAction;

    // Sets the view volume and viewport information to use for
    // subsequent nudges
    void		setView(const SbViewVolume &vol,
				const SbViewportRegion &vpReg);

    // Begins a nudge operation. This should be called before nudging
    // any objects with nudge() to allow undo to be set up. The number
    // of objects that will be nudged is passed in.
    void		prepareForNudge(int numObjects);

    // Allocates numObjects NudgerObjectInfos structures
    void		allocateInfos();

    // Nudges the given selected object
    void		nudgeObject(SoPath *selection, MleActor *actor,
				    int direction, int numPixels);

    // Sets up for nudging in a perspective or orthographic view
    void		setUpPerspective(SoPath *selection);
    void		setUpOrthographic();
    
    // mvo - stuff to replace CoSceneManager calls
    SbXfBox3f getXfBoundingBox(SoPath *selection, MleActor *actor);
    SbBox3f getBoundingBox(SoPath *selection, MleActor *actor);
    SbMatrix &getMatrix(SoPath *path);
    static void setActionViewportRegion(const SbViewportRegion &vpReg);

};

#endif /* __NUDGER_H_ */
