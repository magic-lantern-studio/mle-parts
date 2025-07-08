/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file Nudger.h
 * @ingroup MlParts
 *
 * This file defines a class that allows objects to be "nudged" a little
 * bit in any screen space direction.
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
