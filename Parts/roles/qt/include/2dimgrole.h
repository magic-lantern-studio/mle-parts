/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 2dimgrole.h
 * @ingroup MleParts
 *
 * This file defines the class for a 2D Image Role targeting
 * the Qt platform.
 *
 * @author Mark S. Millard
 * @date Jul 8, 2017
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2017 Wizzer Works
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

#ifndef __MLE_2DIMGROLE_H_
#define __MLE_2DIMGROLE_H_

// Include system header files.

// Include Qt header files.
#include <QPixmap>

// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "math/vector.h"
#include "mle/MleMediaRef.h"
#include "mle/2drole.h"

// Include Magic Lantern Qt header files.
#include "qtrole_global.h"

// Declare external classes.
class MleActor;


/**
 * This class defines the interface for a 2D Image Role targeting
 * the Qt platform.
 */
class MLEQTROLESHARED_EXPORT Mle2dImgRole : public Mle2dRole
{

    MLE_ROLE_HEADER(Mle2dImgRole);

  public:

    /**
     * A constructor that sets the associated Actor.
     *
     * @param actor A pointer to the actor to associate with this Role.
     */
    Mle2dImgRole(MleActor *actor);

    /**
     * The destructor.
     */
    virtual ~Mle2dImgRole(void);

    /**
     * Update the position where the image is drawn.  The position is
     * in screen coordinates and lower left is the image origin.
     *
     * @param pos The position of the image, in screen coordinates.
     */
    void screenLocation(MlVector2 &pos);

    /**
     * This method will read in a single pixelmap image.  It can also be used to
     * read in a different pixelmap image during runtime.
     *
     * @param img The Media Reference to load.
     */
    void load(MlMediaRef img);

    /**
     * @brief Load an image from a Media Reference.
     *
     * @param img The Media Reference to load.
     */
    void load(MleMediaRef *img);

    /**
     * The display of the image can be toggled on or off.
     *
     * @param state If <b>TRUE</b>, then the image will be displayed.
     * If <b>FALSE</b> then the image will be hidden.
     */
    void display(int state);

    /**
     * The render() function of the 2D Set calls this function to update
     * this Role every cycle.
     *
     * @param data A pointer to the QPainter context to draw.
     */
    virtual void draw(void *data);

  protected:

    /** Image origin (lower left) in screen coordinates. */
    MlVector2 screenPosition;
    /** The display state, TRUE for on, FALSE for off. */
    int displayState;
    /** A pointer to the Qt surface. */
    QPixmap m_surface;

  private:

    // Remember media reference to reload on Qt reset.
    MlMediaRef m_imgMediaRef;
};


#endif /* __MLE_2DIMGROLE_H_ */
