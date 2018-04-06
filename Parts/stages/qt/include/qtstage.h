/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file qtstage.h
 * @ingroup MleParts
 *
 * This file defines the Qt Stage.
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

#ifndef __MLE_QTSTAGE_H_
#define __MLE_QTSTAGE_H_

// Include Qt header files.
#include <QScreen>
#include <QPainter>

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleStage.h"
#include "mle/MleScheduler.h"
#include "mle/MleEventDispatcher.h"
#include "mle/MlePtrArray.h"

// Include Magic Lantern Math header files.
#include "math/vector.h"

// Include QtStage header files.
#include "qtstage_global.h"
#include "qtstagewindow.h"
#include "qtrenderer.h"

class MleSet;
typedef void (*QTSTAGE_RENDER_CALLBACK)(MleSet *);

/**
 * @brief This class implements a Qt Stage for the OpenGL platform.
 */
class QTSTAGESHARED_EXPORT QtStage : public MleStage
{
     MLE_STAGE_HEADER(QtStage);

  public:
    /**
     * @brief Qt Stage constructor.
     *
     * Default constructor.
     */
    QtStage(void);

    /**
     * @brief Qt Stage destructor.
     */
    virtual ~QtStage(void);

    /**
     * This method installs the event and scheduler callbacks.
     */
    virtual void init(void);

    /**
     * @brief Set the name of the Stage.
     *
     * @param name A reference to a QString containing the title of
     * the stage.
     */
    void setName(const QString &name);

    /**
     * @brief Add a Set to this Stage.
     *
     * Registers a new Set with this stage
     * and returns a pointer to the scheduled render item.
     *
     * @param cb A callback for rendering the stage.
     * @param set The MleSet to add.
     *
     * @return A pointer to the scheduled render item is returned.
     */
    virtual MleSchedulerItem *addSet(QTSTAGE_RENDER_CALLBACK cb,MleSet *set);

    /**
     * This method is an accessor function to retrieve the size of the stage.
     *
     * @param width The width of the Stage is returned.
     * @param height The height of the Stage is returned.
     */
    virtual void getSize(int *width,int *height);

    /**
     * This method changes the size of the window managed by the stage.
     *
     * @param x The new x origin of the window.
     * @param y The new y origin of the window.
     * @param width The new width of the window.
     * @param height The new height of the window.
     */
    void resize(int x, int y, int width, int height);

    /**
     * This method renders the stage.
     * It is a callback to be used during the PHASE_STAGE phase
     * of the execution cycle.
     *
     * @param stage A pointer to the stage.
     */
    static void render(void *stage);

    /**
     * This callback is used to capture the size message. It is called
     * via the event dispatch manager.
     *
     * @param event The Magic Lantern event.
     * @param callData Call data for the event callback.
     * @param clientData Client data for the event callback
     */
    static int resizeEventCB(MleEvent event,void *callData,void *clientData);

    /**
     * This callback is used to capture the close message. It is called
     * via the event dispatch manager.
     *
     * @param event The Magic Lantern event.
     * @param callData Call data for the event callback.
     * @param clientData Client data for the event callback
     */
    static int closeEventCB(MleEvent event,void *callData,void *clientData);

    /**
     * This callback is used to capture the expose message. It is called
     * via the event dispatch manager.
     *
     * @param event The Magic Lantern event.
     * @param callData Call data for the event callback.
     * @param clientData Client data for the event callback
     */
    static int exposeEventCB(MleEvent event,void *callData,void *clientData);

    /**
     * This callback is used to capture the mouse message. It is called
     * via the event dispatch manager.
     *
     * @param event The Magic Lantern event.
     * @param callData Call data for the event callback.
     * @param clientData Client data for the event callback
     */
    static int mouseEventCB(MleEvent event,void *callData,void *clientData);

    /**
     * This callback is used to capture the paint message. It is called
     * via the event dispatch manager.
     *
     * @param event The Magic Lantern event.
     * @param callData Call data for the event callback.
     * @param clientData Client data for the event callback
     */
    static int paintEventCB(MleEvent event,void *callData,void *clientData);

    void setScreen(QScreen *screen)
    { m_screen = screen; }

    QScreen *getScreen(void)
    { return m_screen; }

    QPixmap *getOffscreenBuffer(void)
    { return m_offscreenBuffer; }

    /**
     * @brief Make the stage visible.
     */
    void show(void);

    /**
     * @brief Exit the stage.
     *
     * @param event A pointer to the QtEvent generating an exit event.
     */
    static void exit(QEvent *event);

    /**
     * @brief Handle the Qt Show event.
     *
     * @param event A pointer to the QShowEvent.
     */
    static void processShowEvent(QEvent *event);

    /**
     * @brief Handle the Qt Resize event.
     *
     * @param event A pointer to the QResizeEvent.
     */
    static void processResizeEvent(QEvent *event);

    /**
     * @brief Handle the Qt Mouse event.
     *
     * @param event A pointer to the QMouseEvent.
     */
    static void processMouseEvent(QEvent *event);

    /**
     * @brief Handle the Qt Paint event.
     *
     * @param event A pointer to the QPaintEvent.
     */
    static void processPaintEvent(QEvent *event);

    /** Flag indicating that the stage will exit. */
    static MlBoolean g_doExit;

    /** "size" property contains width and height in pixels. */
    MlVector2 size;

  private:
    //int m_width; int m_height;  // Size of stage.

    QScreen *m_screen;
    QtStageWindow *m_window;
    QPixmap *m_offscreenBuffer;
};

#endif // __MLE_QTSTAGE_H_
