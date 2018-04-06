/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file qt2dset.h
 * @ingroup MleParts
 *
 * This file defines the class for a 2D Set targeting
 * the Qt platform.
 *
 * @author Mark S. Millard
 * @date Jul 11, 2017
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

#ifndef __MLE_QT2DSET_H_
#define __MLE_QT2DSET_H_

// Include Qt header files.
#include <QPixmap>
#include <QRect>

// Include Magic Lantern Math header files.
#include "math/scalar.h"
#include "math/vector.h"

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleSet.h"
#include "mle/MleScheduler.h"
#include "mle/MleEventDispatcher.h"

// Include Magic Lantern Qt Stage header files.
#include "mle/qt/qtstage.h"

#include "qtset_global.h"

class MleRole;
class Mle2dRole;

// Type used for viewBoundsCallback routines.
typedef void (*Mle2dSetViewBoundsCallback)(Mle2dRole *);

/**
 * @brief This class defines the Qt 2D Set.
 */
class MLEQTSETSHARED_EXPORT Mle2dSet: public MleSet
{
    MLE_SET_HEADER(Mle2dSet);

  public:

    /**
     * The default constructor.
     */
    Mle2dSet(void);

    /**
     * The desctructor.
     */
    virtual ~Mle2dSet(void);

    /**
     * Initialize the Set.
     */
    virtual void init(void);

    // Attach and detach a Role to the 2dSet's scene graph (currently
    // implemented as a linked-list).

    /**
     * Attach a Role to the Set.  If <i>curRole</i> is <b>NULL<b>, then
     * <i>newRole</i> is attached to the end of the list. Otherwise it will
     * be attached following the <i>curRole</i>.
     *
     * @param curRole The role to attach the new one too.
     * @param newRole The role being attached to the Set.
     */
    virtual void attach(MleRole *curRole, MleRole *newRole);

    /**
     * Detach the specified Role from the Set.
     *
     * @param curRole The Role to detach from the Set.
     */
    virtual void detach(MleRole *curRole);

    /**
     * This method is used to traverse the scene graph and render each
     * 2D role.
     *
     * @param set The Set to render.
     *
     * @see attach()
     */
    static void render(MleSet *set);

    /**
     * This method is used to set the scheduler id for the render method
     * (so that it can be removed during destruction).
     *
     * @param id The scheduler id for the render method.
     */
    inline void setRenderCBId(MleSchedulerItem *id)
    {
        m_renderCBId = id;
    }

    /**
     * This method is used to set the viewBoundsCallback member variable.
     * The viewBoundsCallback is a function which will be called for each
     * Role in the 2dSet's scene graph as the scene graph is
     * rendered.
     */
    inline Mle2dSetViewBoundsCallback setViewBoundsCallback(
        Mle2dSetViewBoundsCallback callback)
    {
        Mle2dSetViewBoundsCallback old_cb = m_viewBoundsCallback;
        m_viewBoundsCallback = callback;
        return(old_cb);
    }

    /**
     * This method is used to get the width of the Set.
     *
     * @param The width, in pixels, is returned.
     */
    inline unsigned int getWidth(void)
    {
        return m_offscreenBuffer->width();
    }

    /**
     * This method is used to get the height of the Set.
     *
     * @param The height, in pixels, is returned.
     */
    inline unsigned int getHeight(void)
    {
        return m_offscreenBuffer->height();
    }

    /**
     * This method is used to get the Qt surface used
     * to render the Set.
     */
    inline QPixmap *getOffscreenBuffer(void)
    {
        return m_offscreenBuffer;
    }

    /**
     * This method changes the size of the offscreen buffer.
     *
     * @param width The new width of the buffer.
     * @param height The new height of the buffer.
     */
    MlBoolean resize(int width, int height);

    /**
     * This method is a callback for responding to a Resize event.
     *
     * @param event The Magic Lantern event.
     * @param callData Data associated with the event.
     * @param clienData Data associated with the callback.
     *
     * @return If the callback is succussful, then 1 will be returned.
     * Otherwise, 0 will be returned.
     */
    static int resizeEventCB(MleEvent event,void *callData,void *clientData);

    /**
     * This method is a callback for responding to a Paint event.
     *
     * @param event The Magic Lantern event.
     * @param callData Data associated with the event.
     * @param clienData Data associated with the callback.
     *
     * @return If the callback is succussful, then 1 will be returned.
     * Otherwise, 0 will be returned.
     */
    static int paintEventCB(MleEvent event,void *callData,void *clientData);

    // The properties exposed in the DWP/DPP.

    /**
     * The position of the Set.  It contains x and y position in screen coordinates,
     * where (0, 0) corresponds to the lower left corner of the window.
     */
    MlVector2 position;

    /**
     * The size of the Set. It contains width and height in screen coordinates.
     */
    MlVector2 size;

  protected:
    /**
     * The headRole is the first Role drawn.  This makes the
     * headRole always appear in the bottom layer among all the
     * Roles attached to this Set.  Similiarly, the last Role
     * in this list always appear in the top layer.
     */
    Mle2dRole *m_headRole;

    /** The callback used to calculate the boundary of the view. */
    Mle2dSetViewBoundsCallback m_viewBoundsCallback;

    // All management of screen clears and dirty rectangles is being
    // done in the Set (probably will change).

    #define MAX_DIRTY_RECTANGLES 16

    friend void ViewBoundsCallback(Mle2dRole *role);

    /** Table of dirty rectangles. */
    QRect m_dirtyRectangles[MAX_DIRTY_RECTANGLES];
    /** The last dirty rectangle in the table. */
    int m_lastDirtyRectangle;

    /** Union of rectangles that were drawn during this frame. */
    QRect m_renderBounds;

    /** Union of rectangles that were cleared during this frame. */
    QRect m_clearBounds;

    /** The Set's offscreen buffer to render to. */
    QPixmap *m_offscreenBuffer;

  private:
    MleSchedulerItem *m_renderCBId;     // Render callback id (scheduler).
    MleCallbackId m_resizeCBId;         // Resize callback id (event dispatcher).
    MleCallbackId m_paintCBId;          // Paint callback id (event dispatcher).

};

#endif /* __MLE_QT2DSET_H_ */
