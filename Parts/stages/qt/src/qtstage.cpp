/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file qtstage.cpp
 * @ingroup MleParts
 *
 * This file implements the Qt Stage.
 *
 * @author Mark S. Millard
 * @date May 24, 2017
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
#include <QGuiApplication>
#include <QSurfaceFormat>
#include <QThread>
#include <qdebug.h>

// Include Magic Lantern Runtime Engine header files.
#include "mle/mlMalloc.h"
#include "mle/MleDirector.h"
#include "mle/MleScheduler.h"

// Include Qt Stage header files.
#include "qtstage.h"


// Global declaration for Magic Lantern Qt events.
QTSTAGESHARED_EXPORT MleEvent QT_RESIZE_EVENT = 0x0000;
QTSTAGESHARED_EXPORT MleEvent QT_CLOSE_EVENT  = 0x0001;
QTSTAGESHARED_EXPORT MleEvent QT_EXPOSE_EVENT = 0x0002;
QTSTAGESHARED_EXPORT MleEvent QT_MOUSE_EVENT  = 0x0003;
QTSTAGESHARED_EXPORT MleEvent QT_PAINT_EVENT  = 0x0004;

MLE_STAGE_SOURCE(QtStage, MleStage)

QtStage::QtStage(void)
    : m_screen(NULL), m_window(NULL), m_offscreenBuffer(NULL)
{
    size[0] = 640; // Default width.
    size[1] = 480; // Default height;
}

QtStage::~QtStage(void)
{
    if (m_window) delete m_window;
    if (m_offscreenBuffer) delete m_offscreenBuffer;
}

void
QtStage::init(void)
{
    // Declare local variables
    MleCallbackId cbId;

    QSurfaceFormat format;
    format.setDepthBufferSize(16);
    format.setSamples(4);
    QSurfaceFormat::setDefaultFormat(format);

    // Create a new main window.
    m_window = new QtStageWindow(this);
    m_window->init();

    // Insert resize callback into event dispatch manager.
    MleEvent sizeEvent = QT_RESIZE_EVENT;
    cbId = g_theTitle->m_theEventMgr->installEventCB(
        sizeEvent, QtStage::resizeEventCB, (void *)this);

    // Insert close callback into event dispatch manager.
    MleEvent closeEvent = QT_CLOSE_EVENT;
    cbId = g_theTitle->m_theEventMgr->installEventCB(
        closeEvent, QtStage::closeEventCB, (void *)this);

    // Insert stage into the scheduler.
    g_theTitle->m_theScheduler->insertFunc(
        PHASE_STAGE, QtStage::render, this, this);
}

void
QtStage::setName(const QString &name)
{
    m_window->setWindowTitle(name);
}

void
QtStage::getSize(int *width,int *height)
{
    *width = size[0];
    *height = size[1];
}

MleSchedulerItem *
QtStage::addSet(QTSTAGE_RENDER_CALLBACK cb,MleSet *set)
{
    // Declare local variables.
    MleSchedulerItem *item;

    // Just pass this function on to the scheduler.
    item = g_theTitle->m_theScheduler->insertFunc(
        PHASE_SET, (MleSchedulerFunc)cb, set, this);

    return item;
}

void
QtStage::render(void *stage)
{
    QtStage *qtStage = (QtStage *)stage;

    //qDebug() << "QtStage: rendering the stage.";

    // Schedule a repaint event.
    qtStage->m_window->repaint();
}

void
QtStage::resize(int x, int y, int width, int height)
{
    size[0] = width;
    size[1] = height;

    qDebug() << "QtStage: resizing to width:" << size[0] << ", height:" << size[1];

    // Create a new offscreen buffer.
    if (m_offscreenBuffer) delete m_offscreenBuffer;
    m_offscreenBuffer = new QPixmap(width, height);

    // Clear the offscreen buffer.
    QPainter painter(m_offscreenBuffer);
    QRectF bounds(x, y, width, height);
    painter.fillRect(bounds, Qt::black);
}

int
QtStage::resizeEventCB(MleEvent event,void *callData,void *clientData)
{
    // Declare local variables.
    QtStage *theStage = (QtStage *)clientData;
    MleResizeEvent *eventData = (MleResizeEvent *)callData;

    qDebug() << "QtStage: resizeEventCB received Resize event.";

    // Rebuild the offscreen buffer.
    theStage->resize(0, 0, eventData->m_width, eventData->m_height);

    return 0;
}

int
QtStage::closeEventCB(MleEvent event,void *callData,void *clientData)
{
    // Declare local variables.
    QtStage *theStage = (QtStage *)clientData;
    //QtCallData *eventData = (QtCallData *)callData;

    // Process the close event.
    QtStage::g_doExit = ML_TRUE;

    return 0;
}

int
QtStage::exposeEventCB(MleEvent event,void *callData,void *clientData)
{
    // Declare local variables.
    QtStage *theStage = (QtStage *)clientData;
    //QtCallData *eventData = (QtCallData *)callData;

    // Process the expose event.
    //qDebug() << "QtStage: exposeEventCB received Expose event.";

    return 0;
}

int
QtStage::mouseEventCB(MleEvent event,void *callData,void *clientData)
{
    // Declare local variables.
    QtStage *theStage = (QtStage *)clientData;
    //QtCallData *eventData = (QtCallData *)callData;

    // Process the expose event.
    //qDebug() << "QtStage: mouseEventCB received Mouse event.";

    return 0;
}

int
QtStage::paintEventCB(MleEvent event,void *callData,void *clientData)
{
    // Declare local variables.
    QtStage *theStage = (QtStage *)clientData;
    //QtCallData *eventData = (QtCallData *)callData;

    // Process the expose event.
    //qDebug() << "QtStage: paintEventCB received Paint event.";

    return 0;
}

void
QtStage::show()
{
    m_window->show();
}

MlBoolean QtStage::g_doExit = ML_FALSE;

void
QtStage::exit(QEvent *event)
{
    //qDebug() << "QtStage: exit received Qt Close event.";
    // Dispatch Magic Lantern close event.
    g_theTitle->m_theEventMgr->dispatchEvent(QT_CLOSE_EVENT, NULL);
}

void
QtStage::processShowEvent(QEvent *event)
{
    //qDebug() << "QtStage: processShowEvent received Qt Show event.";
    // Dispatch Magic Lantern expose event.
    g_theTitle->m_theEventMgr->dispatchEvent(QT_EXPOSE_EVENT, NULL);
}

void
QtStage::processResizeEvent(QEvent *event)
{
    qDebug() << "QtStage: processResizeEvent received Qt Resize event.";

    // Translate Qt event into a Magic Lantern event.
    QResizeEvent *resizeEvent = (QResizeEvent *) event;
    QSize size = resizeEvent->size();
    MleResizeEvent mleEvent;
    mleEvent.m_width = size.width();
    mleEvent.m_height = size.height();

    // Dispatch Magic Lantern resize event.
    g_theTitle->m_theEventMgr->dispatchEvent(QT_RESIZE_EVENT, &mleEvent);
}

void
QtStage::processMouseEvent(QEvent *event)
{
    // Translate Qt event into a Magic Lantern event.
    QMouseEvent *mouseEvent = (QMouseEvent *) event;
    if (mouseEvent->type() == QEvent::MouseButtonPress) {
        //qDebug() << "QtStage: processMouseEvent received Qt Mouse Press event.";
    } else if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        //qDebug() << "QtStage: processMouseEvent received Qt Mouse Release event.";
    } else if (mouseEvent->type() == QEvent::MouseMove) {
        //qDebug() << "QtStage: processMouseEvent received Qt Mouse Move event.";
    } else {
        //qDebug() << "QtStage: processMouseEvent received Unknown Qt Mouse event.";
    }

    // Dispatch Magic Lantern mouse event.
    g_theTitle->m_theEventMgr->dispatchEvent(QT_MOUSE_EVENT, NULL);
}

void
QtStage::processPaintEvent(QEvent *event)
{
    //qDebug() << "QtStage: processPaintEvent received Qt Paint event.";

    QPaintEvent *paintEvent = (QPaintEvent *) event;
    // May need to define and forward region to paint.

    // Dispatch Magic Lantern Paint event.
    g_theTitle->m_theEventMgr->dispatchEvent(QT_PAINT_EVENT, NULL);
}
