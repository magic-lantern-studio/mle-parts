/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file Source.h
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 * @date Jan 24, 2006
 *
 * This file defines a class that displays the source for snapping operations.
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

#ifndef  __SOURCE_H_
#define  __SOURCE_H_

#include "SpotMarker.h"

class SoMatrixTransform;

/**
 * @brief Source is a class that displays the source for snapping operations. It also
 * allows interactive placement of the source object using the middle
 * mouse button.
 */
class Source : public SpotMarker
{
  public:

    Source();
    ~Source();

    // Initializes, changes, or finishes change to positioning matrix
    // based on manipulation.
    void startMatrixChange(const SbMatrix &xf);
    void changeMatrix(const SbMatrix &deltaMatrix);
    void finishMatrixChange();

  private:

    // Important nodes:
    SoMatrixTransform *matrixTransform;

    // This saves the base matrix for changes passed to changeMatrix.
    SbMatrix matrixChangeBase;

    // Builds the scene graph.
    virtual void buildSceneGraph();

    // Called when the location changes. Prepares the matrixTransform.
    virtual void locationChanged();
};

#endif /* __SOURCE_H_ */
