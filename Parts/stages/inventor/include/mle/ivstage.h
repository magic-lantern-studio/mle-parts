/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file ivstage.h
 * @ingroup MlParts
 *
 * This file defines the Inventor Stage.
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

#ifndef __MLE_IVSTAGE_H_
#define __MLE_IVSTAGE_H_

#include "mle/MleStage.h"

// Include Inventor header files.
#include <Inventor/SbLinear.h>
#if defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Inventor/Win/viewers/SoWinFullViewer.h>
#endif /* WIN32 */
#if defined(__linux__) || defined(__APPLE__)
#if defined(MLE_SOQT)
#include <QtGlobal>
#include <QEvent>
#include <Inventor/Qt/viewers/SoQtFullViewer.h>
#elif defined(MLE_SOGTK)
#include <Inventor/Gtk/viewers/SoGtkFullViewer.h>
#else
// Default to SoXt
#include <Inventor/Xt/viewers/SoXtFullViewer.h>
#endif /* Qt */
#endif /* __linux__ */

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

#include "mle/MleScheduler.h"
#include "mle/MleEvent.h"

#include "math/vector.h"

#include "mle/MleIvStage.h"

#if defined(MLE_REHEARSAL)
#if defined(__linux__) || defined(__APPLE__)
#if defined(MLE_SOQT)
class SoQtRenderArea;
#elif defined(MLE_SOGTK)
class SoGtkRenderArea;
#else
class SoXtRenderArea;
#endif /* Qt */
#endif /* __linux__ */
#if defined(WIN32)
class SoWinRenderArea;
#endif /* WIN32 */
#endif /* MLE_REHEARSAL */

class SoSelection;
class SoSeparator;
class SoAction;
class SoCamera;
class SoOrthographicCamera;
class SoPerspectiveCamera;
class SoEventCallback;
class SoGroup;
class SoSwitch;
class SoTranslation;
class SoDragger;
class SoTransformer2Dragger;
class SoTransformer2Manip;
class SoTransformerManip;
class SoCube;
class SoNodeSensor;
class SoNode;
class SoPath;
class SoPickedPoint;

class Target;
class Source;

class SoFullViewer;
class MleSnapper;

class MleSet;
class MleRole;
class MleSchederItem;
class Nudger;

#if defined(MLE_SOQT)
// Global definition for Magic Lantern Qt events.
#define QT_RESIZE_EVENT 0x0000
#define QT_CLOSE_EVENT  0x0001
#define QT_EXPOSE_EVENT 0x0002
#define QT_MOUSE_EVENT  0x0003
#define QT_PAINT_EVENT  0x0004
#endif /* MLE_SOQT */


class MLE_IVSTAGE_API MleIvStage : public MleStage
{
    MLE_STAGE_HEADER(MleIvStage);

  public:

    static void getProperty(MleObject *object, const char *name, unsigned char **value); // value is an output parameter.
    static void setProperty(MleObject *object, const char *name, unsigned char *value); // value is an input parameter.

    // Property declarations.
    MLE_STAGE_PROPERTY(MlVector2,size,getSizeProperty,setSizeProperty)

    // constructor and destructor
    MleIvStage(void);
    virtual ~MleIvStage(void);
    
#if defined(MLE_REHEARSAL)

    virtual void init(void);

    virtual void resolveEdit(const char *property);

#if defined(__linux__)
#if 0
    // X event handling: we call SoDB::doSelect() so that Inventor
    // animation, etc, will run even when no X events are being generated.
    virtual int doSelect(int nfds,
    fd_set *readfds,
    fd_set *writefds,
    fd_set *exceptfds,
    struct timeval *userTimeOut);
#endif /* 0 */
#endif /* __linux__ */
    
    // Editing.
    virtual void setEditing(int flag);
    virtual void edit(void);
    
    // Configuration.
#if defined(__linux__) || defined(__APPLE__)
#if ! defined(MLE_SOQT) || ! defined(MLE_SOGTK)
    virtual int getFD();
#endif
#endif /* __linux__ */

    virtual int setSize(int width,int height);

    virtual void getSize(int *width,int *height);

#if defined(__linux__) || defined(__APPLE__)
#if defined(MLE_SOQT)
    virtual QWidget *getWindow(void);
#elif defined(MLE_SOGTK)
    virtual GtkWidget *getWindow(void);
#else
    virtual Window getWindow(void);
    virtual Display* getDisplay(void);
#endif /* Qt */
#endif /* __linux__ */
#if defined(WIN32)
    virtual HWND getWindow();
#endif /* WIN32 */

    virtual void setOffscreen(int flag);
    
    // Rendering.
    //   render() forces a redraw.
    virtual void render(void);
    
    // Manipulation.
    virtual void activateManipulator(MleActor *actor,int invokeCallback=0);

    virtual void deactivateManipulator(MleActor *actor,int invokeCallback=0);

    // Getting attributes of the stage.
    virtual const char** getFunctions();
    virtual const char** getFunctionAttributes(char* functionName);

    // Control over IV viewers.
    virtual int setViewer(const char* viewerName);
    virtual const char* getViewer();

    // Control over edit modes.
    virtual int setEditMode(char* editMode);
    virtual char* getEditMode();

    // Sets rendering mode for entire stage.
    void setRenderMode(char *renderMode);
    const char * getRenderMode() const;

    // Snapping enabling.
    virtual int hasSnappingTarget();
    virtual void setSnapping(int flag);
#if 0
    virtual int getSnapping();
#endif /* 0 */
    
    // Sets/returns whether scale snapping is enabled.
    // Todo: note not in funcs list / stage base class
    void enableScaleSnapping(MlBoolean flag);

    MlBoolean isScaleSnappingEnabled() const;

    // Nudges currently-selected object in the given direction by
    // the given number of pixels. direction is 0,1,or 2 for x, y, or z
    virtual void nudge(int direction, int numPixels);
    
    // Rearranging the drawing order of the sets in this stage.

    // pushSet() - Pushes the set one layer down.  If success, 0 is
    //               returned; otherwise, -1 is returned.  If the set is
    //               already at the bottom, no action is taken and -1 is
    //               returned.
    virtual int pushSet(MleSet *set);

    // pushSetToBottom() - Same as pushSet() except the set is pushed
    //                     to the bottom layer.  If success, 0 is returned;
    //                     otherwise, -1 is returned.  If the set is
    //                     already at the bottom, no action is taken and -1
    //                     is returned.
    virtual int pushSetToBottom(MleSet *f);

    // popSet() - Pops the set one layer up.  If success, 0 is returned;
    //            otherwise, -1 is returned.  If the set is already at
    //            the top, no action is taken and -1 is returned.
    virtual int popSet(MleSet *f);

    // popSetToTop() - Same as popSet() except the set is popped to the
    //                 top layer.  If success, 0 is returned; otherwise, -1
    //                 is returned.  If the set is already at the top, no
    //                 action is taken and -1 is returned.
    virtual int popSetToTop(MleSet *f);

    // Background colors.
    virtual void setBgndColor(float* color);

    virtual void getBgndColor(float* color); 

    // Horizon grid.
    virtual void setHorizonGrid(int onOff);

    virtual int getHorizonGrid(); 

    virtual void openPrefsDialog();

    virtual void viewAll();

    virtual void showDecoration(int onOff);

    // Moves all currently-selected objects so the master selection
    // ends up at the snapping target. If startUndo is TRUE, this
    // starts an undo operation.
    void moveToTarget();

    // Recalculates the clipping planes if auto clipping planes enabled.
    // Should be called anytime an actor property changes that could
    // effect actor transform.
    virtual void recalcAutoClipPlanes();

#endif /* MLE_REHEARSAL */
    
    // Stage-specific API.
    // This is new API (not inherited) for this particular stage type.

    /**
     * @brief Register a new Set with this Stage.
     *
     * @param set A pointer to the Set to register.
     *
     * @return A pointer to a MleSchedulerItem for the scheduled
     * render callaback is returned.
     */
    virtual MleSchedulerItem *addSet(void (*render)(MleSet *),MleSet *set);

    /**
     * @brief Called by a Set when it's camera moves.
     *
     * @param set The associated Set.
     * @param camera The Inventor camera.
     */
    void setCameraMoved(MleSet *set, SoCamera *camera);

#ifdef MLE_SOQT
    /**
     * @brief Set the shell parent widget.
     *
     * @param parent The widget to set as the pareent of the stage.
     */
    void setShellParent(QWidget *parent)
    { m_shellParent = parent; }
#endif /* MLE_SOQT */

    /**
     * @brief Initializes platform-specific data for the stage.
     *
     * @return A pointer to the initialized platform-specific data structure
     * is returned.
     */
    void *initPlatform(void);

#ifdef MLE_SOQT
    /**
     * @brief Exit the stage.
     *
     * @param event A pointer to the QtEvent generating an exit event.
     */
    static void doExit(QEvent *event);
#endif /* MLE_SOQT */

    // Beginning of properties.

    // size property contains width and height in pixels.
    //MlVector2 size;

  protected:

    // Our viewer is a full inventor viewer for rehearsal, but for
    // runtime is just a render area.
#if defined(MLE_REHEARSAL)
#if defined(__linux__) || defined(__APPLE__)
#if defined(MLE_SOQT)
    SoQtFullViewer *m_viewer;
    SoQtFullViewer *m_examVwr, *m_flyVwr, *m_planeVwr;
#elif defined(MLE_SOGTK)
    SoGtkFullViewer *m_viewer;
    SoGtkFullViewer *m_examVwr, *m_flyVwr, *m_planeVwr;
#else
    // MleFullViewer  *m_walkVwr;
    // MleFullViewer *m_viewer;
    // MleFullViewer *m_examVwr, *m_flyVwr, *m_planeVwr;
    SoXtFullViewer *m_viewer;
    SoXtFullViewer *m_examVwr, *m_flyVwr, *m_planeVwr;
#endif /* Qt */
#endif /* __linux__ */
#if defined(WIN32)
    SoWinFullViewer *m_viewer;
    SoWinFullViewer *m_examVwr, *m_flyVwr, *m_planeVwr;
#endif /* WIN32 */
#else /* MLE_REHEARSAL */
#if defined(__linux__) || defined(__APPLE__)
#if defined(MLE_SOQT)
    SoQtRenderArea *m_viewer;
#elif defined(MLE_SOGTK)
    SoGtkRenderArea *m_viewer;
#else
    SoXtRenderArea *m_viewer;
#endif
#endif /* __linux__ */
#if defined(WIN32)
    SoWinRenderArea *m_viewer;
#endif /* WIN32 */
#endif /* ! MLE_REHEARSAL */

    // Root of the scene graph.
    SoSelection *m_root;
    SoSeparator *m_sets;

    // This is a local struct definition to bundle information for the
    // Inventor callback node.  It contains information about the
    // set, so the set rendering function can be called during
    // Inventor scene graph traversal.  In this Inventor stage, only
    // 2D set is added as Inventor callback node.
    typedef struct _SetInfo {
        void (*func)(MleSet *);
        MleSet *set;
    } SetInfo;

#if defined(__linux__) || defined(__APPLE__)
#if defined(MLE_SOQT)
    // The shell's parent widget for the Qt platform.
    QWidget *m_shellParent;
#elif defined(MLE_SOGTK)
    // The shell's parent widget for Gtk platform.
    GtkWidget *m_shellParent;
#else
    // The shell's parent widget for Xt platform.
    Widget m_shellParent;
#endif /* Qt */
#endif /* __linux__ */

#if defined(MLE_REHEARSAL)
#if defined(WIN32)
    HWND m_shellParent;
#endif /* WIN32 */
    
    SoSeparator *m_toolRoot;
    MleSnapper *m_snapper;
    Target *m_target;
    Source *m_source;

    SoEventCallback *eventCallback;

    SoSwitch *m_manipSwitch;
    SoGroup *m_manipGroup;
    SoTransformer2Manip *m_manip;
    SoTransformerManip *m_transManip;
    SoPath *m_manipPath;
    SoTranslation *m_manipHiddenCubeTranslation;
    SoCube *m_manipHiddenCube;
    SbMatrix m_manipNorm;
    
    // Switch to turn horizon grid on/off
    SoSwitch *m_gridSwitch;

    SoSwitch *m_cameraSwitch;
    SoPerspectiveCamera *m_perspectCamera;
    SoOrthographicCamera *m_orthoCamera;
    SoNodeSensor *m_cameraSensor;
    
    Nudger *m_nudger;
    
    MleSet *m_activeSet;
    MleRole *m_activeRole;
    
    int m_offscreen;

    // A list contains all sets sorted based on drawing order (first one
    // on the list is drawn last).
    typedef struct _SetDrawOrder {
        SetInfo *setInfo;
        _SetDrawOrder *prev;
        _SetDrawOrder *next;
    } SetDrawOrder;

    SetDrawOrder *m_setDrawOrder;

    // Get current camera based on switch.
    SoCamera * getCamera();

    // SetActiveRole() is called from the event callback to
    // identify the active role.  A manip is activated and
    // configured.
    void setActiveRole(MleSet *set,MleRole *role, int cbFlag=1);
    void setActiveSet(MleSet *set);

    // Sets point of interest for navigation when in some iv viewers.
    void setPointOfInterest(MlVector3 pos);

    //
    // Add set to the beginning of the set drawing order list.
    //
    void addSetDrawOrder(MleIvStage::SetInfo *setInfo);

    // Sets up snapping for the current view.
    void initSnapping();

    // Updates the target and source.
    void updateTargetAndSource();
    
    // Sets/returns whether 2D grid is enabled.
    void enableGrid(MlBoolean flag);
    MlBoolean isGridEnabled() const;

    // Fixes edit mode - may have changed from seek back to nav.
    void fixEditMode();

    // notifyCB() is the function registered to be called from
    //   the deletion notifier.
    static void notifyCB(void *,MleIvStage *stage);
    
#endif /* MLE_REHEARSAL */
    
    // Update() is the function the stage registers with the scheduler.
    static void update(MleIvStage *stage);

    /**
     * This callback is the Inventor callback node
     * used to invoke a set.  Only the 2D set uses this in
     * this stage.
     */
    static void setCB(void *data,SoAction *action);

    // eventHandler() is the callback for Inventor event handling.
#if defined(__linux__) || defined(__APPLE__)
#if defined(MLE_SOQT)
    static int eventHandler(MleIvStage *stage,QEvent *event);

    /**
     * This callback is used to capture the close message. It is called
     * via the event dispatch manager.
     *
     * @param event The Magic Lantern event.
     * @param callData Call data for the event callback.
     * @param clientData Client data for the event callback
     */
    static int closeEventCB(MleEvent event,void *callData,void *clientData);
#elif defined(MLE_SOGTK)
    static int eventHandler(MleIvStage *stage,GdkEvent *event);
#else
    static int eventHandler(MleIvStage *stage,XAnyEvent *event);
#endif /* Qt */
#endif /* __linux__ */
#if defined(WIN32)
    static int eventHandler(MleIvStage *stage,MSG *event);
#endif /* WIN32 */
    
#if defined(MLE_REHEARSAL)
    // eventCB() is the callback for the Inventor event callback
    //   node use to get events that draggers don't get.
    static void eventCB(MleIvStage *stage,SoEventCallback *callback);
    void eventCallbackProc(SoEventCallback *callback);

    // Callbacks for the Inventor manip's dragger node.
    static void startDraggerCB(MleIvStage *stage,SoDragger *dragger);
    void startDraggerCallback(MlBoolean generateCallback);
    static void draggerCB(MleIvStage *stage,SoDragger *dragger);
    void draggerCallback(MlBoolean generateCallback);
    void transDraggerCallback(MlBoolean generateCallback);
    static void finishDraggerCB(MleIvStage *stage,SoDragger *dragger);
    void finishDraggerCallback(MlBoolean generateCallback);

    static void cameraCB(MleIvStage *stage,SoNodeSensor *sensor);
    
    void enableSelectionCallbacks(MlBoolean onOff);

    static SoPath * pickFilterCB(MleIvStage *stage,const SoPickedPoint *pick);
    static void selectCB(MleIvStage *stage,SoPath *path);
    static void deselectCB(MleIvStage *stage,SoPath *path);
    
    // Horizon grid iv buffer.
    static unsigned const char g_gridBuffer[];

#endif /* MLE_REHEARSAL */
};

#endif /* __MLE_IVSTAGE_H_ */
