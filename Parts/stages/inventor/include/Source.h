/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file Source.h
 * @ingroup MlParts
 *
 * This file defines a class that displays the source for snapping operations.
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
