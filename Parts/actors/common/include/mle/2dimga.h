/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 2dimga.h
 * @ingroup MleParts
 *
 * This file defines the class for a 2D Image Actor.
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

#ifndef __MLE_2DIMGA_H_
#define __MLE_2DIMGA_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "math/vector.h"
#include "mle/MleActor.h"
#include "mle/MleMediaRef.h"
#include "mle/Mle2dImageActor.h"

/**
 * @brief The 2D image actor.
 *
 * The 2D image actor is used to control the corresponding role to display
 * a specified 2D image media.  This actor provides functional interfaces
 * for positional changes, loading of image media, and visibility control of
 * the image media.  It also supports image media that contains transparency
 * using alpha channel on the SGI platform and single transparent color
 * in the color palette on the PC platform.  This actor currently does not
 * support image media scaling.
 *
 * One example of using this actor is to draw a "Head Up Display".  The
 * actual drawing is done in the corresponding role, Mle2dImgRole.
 * See its man page for more detail.
 *
 * @see Mle2dImgRole, Mle2dSet
 */
class MLE_2DIMAGEACTOR_API Mle2dImgActor : public MleActor 
{
    MLE_ACTOR_HEADER(Mle2dImgActor);

  public:

    static void getProperty(MleObject *object, const char *name, unsigned char **value); // value is an output parameter.
    static void setProperty(MleObject *object, const char *name, unsigned char *value); // value is an input parameter.

    // Property declarations.
    MLE_ACTOR_PROPERTY(MlVector2, position, getPositionProperty, setPositionProperty)
    MLE_ACTOR_PROPERTY(MlMediaRef, image, getImageProperty, setImageProperty)
    MLE_ACTOR_PROPERTY(int, displayState, getDisplayStateProperty, setDisplayStateProperty)

    // Construct a 2D image actor.
    Mle2dImgActor(void);

    // Destruct a 2D image actor.
    virtual ~Mle2dImgActor(void);

    // Initialize the actor and push initial property values to the role.
    virtual void init(void);

#ifdef MLE_REHEARSAL
    // Tools can change property values directly in actor memory.  This
    // function can be used to propagate the changes to its role
    // to make its states consistent.
    virtual void resolveEdit(const char *property);
#endif  /* MLE_REHEARSAL */

    // Set the current position.
    virtual void setPosition(MlVector2 &pos);

    // Set the current image.
    virtual void setImage(MlMediaRef img);

    // Set the current image from the specified Media Reference object.
    virtual void setImage(MleMediaRef *img);

    // Set the display state of the actor, set visible with TRUE or invisible
    // with FALSE.
    virtual void setVisible(int state);

    // Get the display state of the actor.  Return TRUE when actor is
    // visible, FALSE when actor is not displayed.
    virtual int getVisible(void);

    // Set the image property when not processing a Digital Playprint.
    void setImageProperty(MleMediaRef *img)
    { m_mref = img; image = MLE_NO_MEDIA; }

#if 0
    // Beginning of properties.

    // <b>position</b> property contains x and y positions relative to
    // the corresponding set's coordinate system.  (0, 0) corresponds
    // to the lower left corner (origin of the corresponding set).
    MlVector2 position;

    // <b>image</b> property contains media reference to the image media.
    MlMediaRef image;

    // <b>displayState</b> property contains the display state, TRUE for
    // visible, FALSE for invisible.
    int displayState;

    // Set the position property without pushing it to the associated role.
    void setPositionPoperty(MlVector2 &pos)
    { position = pos; }

    // Set the image property.
    void setImageProperty(MlMediaRef img)
    { image = img; m_mref = NULL; }

    // Set the image property when not processing a Digital Playprint.
    void setImageProperty(MleMediaRef *img)
    { m_mref = img; image = MLE_NO_MEDIA; }

    // Set the displayState property.
    void setDisplayStateProperty(int state)
    { displayState = state; }
#endif

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

    // A pointer to a Media Reference; used when not processing from a
    // Digital Playprint. If this reference is used, then the image property
    // should be set to MLE_NO_MEDIA.
    MleMediaRef *m_mref;

};


#endif /* __MLE_2DIMGACTOR_H_ */
