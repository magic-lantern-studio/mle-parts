/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 2drole.h
 * @ingroup MleParts
 *
 * This file contains the class for a 2D Role.
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

#ifndef __MLE_2DROLE_H_
#define __MLE_2DROLE_H_

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleRole.h"
#include "mle/Mle2dRole.h"

// Include Magic Lantern Math header files.
#include "math/scalar.h"

// External declarations.
class MleActor;
class Mle2dSet;


/**
 * @brief The 2D Role base class.
 *
 * The 2D Role base class is used for subclassing to create a
 * Role that can be attached to the Mle2dSet.  All the 2D
 * Roles must initialize their bounding box and provide a draw
 * function for rendering during runtime.
 *
 * @see Mle2dSet
 */

class MLE_2DROLE_API Mle2dRole : public MleRole 
{

    MLE_ROLE_HEADER(Mle2dRole);

  public:

    // @brief Constructor for a 2D Role base class.
    //
    // @param A pointer to the associated Actor.
    Mle2dRole(MleActor *actor);

    // @brief Destructor for a 2D Role base class.
    virtual ~Mle2dRole(void);

    // @brief Get the bounding box of the Role.
    //
    // The bounding box is specified in pixels
    // relative to the corresponding Set's coordinate system,
    // (0, 0) corresponds to the lower left corner (origin of the
    // corresponding Set).
    //
    // @param min The x and y coordinate value of the lower left-hand
    // corner of the Role's extent (min[0], min[1]);
    // @param max The x and y coordinate value of the upper, right-hand
    // corner of the Role's extent (max[0], max[1]).
    virtual void getBounds(MlScalar min[2], MlScalar max[2]);
 
    // @brief Set the bounding box of the Role.
    //
    // The bounding box is specified in pixels
    // relative to the corresponding Set's coordinate system,
    // (0, 0) corresponds to the lower left corner (origin of the
    // corresponding Set).
    //
    // @param min The x and y coordinate value of the lower left-hand
    // corner of the Role's extent (min[0], min[1]);
    // @param max The x and y coordinate value of the upper, right-hand
    // corner of the Role's extent (max[0], max[1]).
    virtual void setBounds(MlScalar min[2], MlScalar max[2]);

    // @brief Draw the 2D Role.
    //
    // This function is called by the render() function in Mle2dSet
    // during the stage phase to draw the Role on the Qt platform.
    //
    // @param data A pointer to the data to be drawn.
    virtual void draw(void *data) = 0;

    /**
     * Override operator new.
     *
     * @param tSize The size, in bytes, to allocate.
     */
    void* operator new(size_t tSize);

    /**
     * Override operator new array.
     *
     * @param tSize The size, in bytes, to allocate.
     */
    void* operator new[](size_t tSize);

    /**
     * Override operator delete.
     *
     * @param p A pointer to the memory to delete.
     */
    void operator delete(void* p);

    /**
     * Override operator delete array.
     *
     * @param p A pointer to the memory to delete.
     */
    void operator delete[](void* p);

  protected:

    friend class Mle2dSet;

    Mle2dRole		*m_prev;
    Mle2dRole		*m_next;
    MlScalar		m_bBoxMin[2];
    MlScalar		m_bBoxMax[2];
};


#endif /* __MLE_2DROLE_H_ */
