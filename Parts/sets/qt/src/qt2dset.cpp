/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file qt2dset.cpp
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

// Include Qt header files.
#include <qdebug.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleDirector.h"

// Include Qt target header files.
#include "mle/2drole.h"
#include "qt2dset.h"

MLE_SET_SOURCE(Mle2dSet, MleSet)


Mle2dSet::Mle2dSet(void)
  : m_offscreenBuffer(NULL)
{
    QtStage *stage;
    MleSchedulerItem *id;

    // Initialize default property values.
    position.setValue(ML_SCALAR(0), ML_SCALAR(0));
    size.setValue(ML_SCALAR(640), ML_SCALAR(480));

    // Initialize member variables.
    m_headRole = NULL;
    m_renderCBId = NULL;
    m_resizeCBId = NULL;
    m_paintCBId = NULL;

    m_viewBoundsCallback = NULL;
    m_lastDirtyRectangle = 0;

    // Add the Set to the Stage.  The stage is in control of the set rendering.
    // No render() function is passed to the scheduler for periodic Set
    // rendering.  The stage adds this set's render() function
    // into the Set phase of the scheduler.
    stage = QtStage::cast(MleStage::g_theStage);
    id = stage->addSet((void (*)(MleSet *)) render, this);
    setRenderCBId(id);

    // Initialize offscreen buffer.

    // Create the offscreen buffer using the resize event callback.
    Mle2dSet::resizeEventCB(QT_RESIZE_EVENT,NULL,(void *)this);
}

Mle2dSet::~Mle2dSet(void)
{
    // Unschedule the set render() function for the Qt stage only.
    if (m_renderCBId)
        g_theTitle->m_theScheduler->remove(m_renderCBId);

    // Uninstall resize event callback.
    if (m_resizeCBId)
        g_theTitle->m_theEventMgr->uninstallEventCB(QT_RESIZE_EVENT,m_resizeCBId);

    // Uninstall patin event callback.
    if (m_paintCBId)
        g_theTitle->m_theEventMgr->uninstallEventCB(QT_PAINT_EVENT,m_paintCBId);

    // Delete all attached Roles.  The destructor of each 2D Role will
    // call this set's detach() function and update the headrole pointer.
    // So when the headrole is NULL, all the attached Roles are
    // deleted.
    while(m_headRole)
        delete m_headRole;
}

void
Mle2dSet::init()
{
    // Insert resize callback into event dispatch manager.
    m_resizeCBId = g_theTitle->m_theEventMgr->installEventCB(
        QT_RESIZE_EVENT,Mle2dSet::resizeEventCB,(void *)this);

    // Bump priority of dispatched callback.
    g_theTitle->m_theEventMgr->changeEventCBPriority(QT_RESIZE_EVENT,m_resizeCBId,
        MLE_RESIZE_SET_PRIORITY);

    // Insert paint callback into event dispath manager.
    m_paintCBId = g_theTitle->m_theEventMgr->installEventCB(
        QT_PAINT_EVENT,Mle2dSet::paintEventCB,(void *)this);
}

void
Mle2dSet::attach(MleRole *curR, MleRole *newR)
{
    Mle2dRole *currentRole, *newRole;

    MLE_ASSERT(newR);
    newRole = Mle2dRole::cast(newR);

    if (curR)
    {
        // Attach after "curR" Role.
        currentRole = Mle2dRole::cast(curR);

        newRole->m_next = currentRole->m_next;
        newRole->m_prev = currentRole;
        currentRole->m_next = newRole;
        if (newRole->m_next)
            newRole->m_next->m_prev = newRole;
    } else
    {
        // Attach to the end of the list.
        if (m_headRole)
        {
            for (currentRole = m_headRole; currentRole->m_next; )
                currentRole = currentRole->m_next;
            currentRole->m_next = newRole;
            newRole->m_prev = currentRole;
            newRole->m_next = NULL;
        } else
        {
            m_headRole = newRole;
            newRole->m_next = NULL;
            newRole->m_prev = NULL;
        }
    }
}

void
Mle2dSet::detach(MleRole *curR)
{
    Mle2dRole *role;

    MLE_ASSERT(curR);
    role = Mle2dRole::cast(curR);

    if (role == m_headRole)
    {
        if (m_headRole->m_next)
            m_headRole->m_next->m_prev = NULL;
        m_headRole = m_headRole->m_next;
    } else
    {
        role->m_prev->m_next = role->m_next;
        if (role->m_next)
            role->m_next->m_prev = role->m_prev;
    }
}


// I would be interested in finding out if the ViewBoundsCallback and the
// g_curSet variable could be made into private static data members of the
// class - not important, but wraps things up a bit neater than using
// friend functions.

void ViewBoundsCallback(Mle2dRole *);

// Keep a copy of the current set for use by the bounds callback.
static Mle2dSet *g_curSet;

void
Mle2dSet::render(MleSet *set)
{
    //qDebug() << "Mle2dSet: rendering the 2D set.";
    Mle2dSet *the2dSet = (Mle2dSet *)set;

    Mle2dRole *next = the2dSet->m_headRole;
    while (next)
    {
        // Render the attached Role.
        next->draw(the2dSet->m_offscreenBuffer);
        next = next->m_next;
    }

    // Render the Set into the Stage.
    QtStage *theStage = QtStage::cast(MleStage::g_theStage);
    QPixmap *stageBuffer = theStage->getOffscreenBuffer();
    QPainter painter(stageBuffer);
    int x = the2dSet->position[0];
    int y = the2dSet->position[1];
    int width = the2dSet->getWidth();
    int height = the2dSet->getHeight();
    QPixmap *setBuffer = the2dSet->getOffscreenBuffer();
    painter.drawPixmap(x, y, width, height, *setBuffer);
}

MlBoolean
Mle2dSet::resize(int width, int height)
{
    QtStage *theStage = QtStage::cast(MleStage::g_theStage);

    // Remove previous offscreen buffer.
    if (m_offscreenBuffer) delete m_offscreenBuffer;

    qDebug() << "Mle2dSet: resizing offscreen buffer to width:" << width << ", height:" << height;

    // Create the new offscreen buffer for the Set.
    m_offscreenBuffer = new QPixmap(width,height);

    // Clear the offscreen buffer.
    QPainter painter(m_offscreenBuffer);
    QRectF bounds(0, 0, width, height);
    painter.fillRect(bounds, Qt::black); // Todo: this color should probably obtained from Set property.

    return TRUE;
}

int
Mle2dSet::resizeEventCB(MleEvent event,void *callData,void *clientData)
{
    // Declare local variables.
    MlBoolean status = FALSE;

    // Dereference the Stage and Set.
    Mle2dSet *set = (Mle2dSet *)clientData;
    QtStage *theStage = QtStage::cast(MleStage::g_theStage);

    // We don't want to resize while the Stage is resetting the device.
    //if (theStage->isResetting())
    //    return status;

    // Retrieve width and height of the stage.
    // XXX --  This is not really correct; the width and height should be
    //         obtained from 2D set properties.
    int width, height;
    theStage->getSize(&width,&height);

    // Resize the set. Assuming the Set is tracking the size of the Stage.
    // Todo: we may not want to do this, and make the Set size independent
    // of the Stage.
    status = set->resize(width, height);

    return status;
}

int
Mle2dSet::paintEventCB(MleEvent event,void *callData,void *clientData)
{
    // Declare local variables.
    MlBoolean status = FALSE;

    //qDebug() << "Mle2dSet: paintEventCB received Paint event.";

    // Dereference the Stage and Set.
    Mle2dSet *set = (Mle2dSet *)clientData;
    QtStage *theStage = QtStage::cast(MleStage::g_theStage);

    // We don't want to paint while the Stage is resetting the device.
    //if (theStage->isResetting())
    //    return status;

    status = TRUE;
    return status;
}

void
ViewBoundsCallback(Mle2dRole *role)
{
    // Declare local variables.
    QRect *dp = g_curSet->m_dirtyRectangles;
    MlScalar x,y;
    MlScalar width,height;
    MlScalar min[2], max[2];
    QtStage *stage = QtStage::cast(MleStage::g_theStage);
    int xoffset = 0;
    int yoffset = 0;

    role->getBounds(min, max);
    x = min[0] - ML_SCALAR(xoffset);
    y = min[1] - ML_SCALAR(yoffset);
    width = max[0] - min[0] + ML_SCALAR_ONE;
    height = max[1] - min[1] + ML_SCALAR_ONE;

    // Accumulate total bounding rectangle.
    if (x < ML_SCALAR(g_curSet->m_renderBounds.left()))
        g_curSet->m_renderBounds.setLeft(mlScalarToLong(x));
    if (y < ML_SCALAR(g_curSet->m_renderBounds.bottom()))
        g_curSet->m_renderBounds.setBottom(mlScalarToLong(y));
    if (x + width > ML_SCALAR(g_curSet->m_renderBounds.right() + 1))
        g_curSet->m_renderBounds.setRight(
            mlScalarToLong(x) + mlScalarToLong(width) - 1);
    if (y + height > ML_SCALAR(g_curSet->m_renderBounds.top() + 1))
        g_curSet->m_renderBounds.setTop(
            mlScalarToLong(y) + mlScalarToLong(height) - 1);

    // Store into dirty rectangle list; if list is full, merge current
    // into last dirty rectangle.
    if (g_curSet->m_lastDirtyRectangle >= MAX_DIRTY_RECTANGLES)
    {
        dp = g_curSet->m_dirtyRectangles + g_curSet->m_lastDirtyRectangle - 1;
        if (x < ML_SCALAR(dp->left()))
            dp->setLeft(mlScalarToLong(x));
        if (y < ML_SCALAR(dp->bottom()))
            dp->setBottom(mlScalarToLong(y));
        if (x + width > ML_SCALAR(dp->right() + 1))
            dp->setRight(mlScalarToLong(x) + mlScalarToLong(width) - 1);
        if (y + height > ML_SCALAR(dp->top() + 1))
            dp->setTop(mlScalarToLong(y) + mlScalarToLong(height) - 1);
    } else
    {
        dp = g_curSet->m_dirtyRectangles + g_curSet->m_lastDirtyRectangle;
        dp->setLeft(mlScalarToLong(x));
        dp->setBottom(mlScalarToLong(y));
        dp->setRight(mlScalarToLong(x) + mlScalarToLong(width) - 1);
        dp->setTop(mlScalarToLong(y) + mlScalarToLong(height) - 1);

        g_curSet->m_lastDirtyRectangle++;
    }
}
