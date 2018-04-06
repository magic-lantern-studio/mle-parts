/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file ivstage.cxx
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 * @date Jan 24, 2006
 *
 * This file implements the Inventor Stage.
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

//  XXX a lot of code is duplicated from brender - should consider
//  moving into a common base class sometime after MLE 1.0.

#if defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#if defined(__sgi) || defined(__linux__)
#include <X11/IntrinsicP.h>
#endif

#include <Inventor/SoPickedPoint.h>
//#include <Inventor/SoNewManips.h>

#if defined(_sgi) || defined(__linux__)
//#include "mle/MleWalkViewer.h"
//#include "mle/MleExaminerViewer.h"
//#include "mle/MlePlaneViewer.h"
//#include "mle/MleFlyViewer.h"
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/Xt/viewers/SoXtPlaneViewer.h>
#include <Inventor/Xt/viewers/SoXtFlyViewer.h>
#endif
#if defined(WIN32)
#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/Win/viewers/SoWinPlaneViewer.h>
#include <Inventor/Win/viewers/SoWinFlyViewer.h>
#endif

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/draggers/SoTransformerDragger.h>
#include <Inventor/draggers/SoTransformer2Dragger.h>
#include <Inventor/manips/SoTransformer2Manip.h>
#include <Inventor/manips/SoTransformerManip.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/nodes/SoPickStyle.h>
#include <Inventor/sensors/SoNodeSensor.h>

#include "mle/mlAssert.h"
#include "mle/mlMalloc.h"

#include "math/transfrm.h"

#include "mle/MleDirector.h"
#include "mle/MleScheduler.h"
#include "mle/MleSet.h"

#if defined(__sgi)
#include "mle/sgiplatform.h"
#endif
#if defined(__linux__)
//#include "mle/ivplatform.h"
#include "mle/mlPlatformData.h"
#endif
#if defined(WIN32)
//#include "mle/ivplatform.h"
#include "mle/mlPlatformData.h"
#include "mle/MleEventDispatcher.h"
#endif
#include "mle/MleStageFuncs.h"
#include "mle/2drole.h"

#include "Snapper.h"
#include "Source.h"
#include "Target.h"
#include "Nudger.h"

#include "mle/ivstage.h"
#include "mle/iv3dset.h"
#include "mle/iv2dset.h"
#include "mle/MleIv3dRole.h"

// Defining this to be non-zero causes the GridGraph scene graph to
// be read from "grid.iv" in the current directory, rather from
// the buffer created by ivToIncludeFile. This feature can be used to
// more quickly test changes to the scene graph, since there is no
// need to recompile first.
#define READ_FROM_FILE 0

#if ! READ_FROM_FILE
// This contains the definition of sceneBuffer:
#include "GridGraph.h"
#endif


MLE_STAGE_SOURCE(MleIvStage,MleStage);


#ifdef MLE_REHEARSAL
void
MleIvStage::initClass(void)
{
    mleRegisterStageClass(MleIvStage, MleStage);
    mleRegisterStageMember(MleIvStage, size, MlVector2);
}

void
MleIvStage::resolveEdit(const char *property)
{
}

#if defined(WIN32)
// This routine is used by the popup window that parents the
// stage offscreen.
LRESULT CALLBACK PopupWindowProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
#endif
#endif /* MLE_REHEARSAL */


MleIvStage::MleIvStage(void)
{

// This is a little trickery to put rehearsal stage construction into an
//   init() procedure, and still allow the same file to be compiled without
//   rehearsal with stage construction in the constructor.  The reason is
//   that we need to create the stage to talk to it (configure it), but
//   some configuration options (offscreen, size, etc.) can only be applied
//   at creation.  This is the type of thing that goes into constructors,
//   but we don't want to clutter up our runtime interface with rehearsal
//   parameters.
#ifdef MLE_REHEARSAL
	m_offscreen = 0;
	m_setDrawOrder = NULL;
}

void
MleIvStage::init(void)
{
#endif /* MLE_REHEARSAL initialization trickery */

#ifdef MLE_REHEARSAL
	// Initialize variables.
	m_activeRole = NULL;
	m_activeSet = NULL;
#endif

#if defined(__sgi) || defined(__linux__)
	// Initialize Inventor and Xt.
	Widget mainWindow = SoXt::init("Magic Lantern");
#endif
#if defined(WIN32)
	// Initializes SoWin library (and implicitly also the Coin
    // library). Returns a top-level / shell window to use.
	HWND mainWindow = (HWND)g_theTitle->m_platformData;
	if (mainWindow == NULL)
	{
        mainWindow = SoWin::init("Magic Lantern");
	} else
	{
		SoWin::init(mainWindow);
	}
#endif

	// Put the window offscreen if asked.
#if defined(__sgi) || defined(__linux__)
	Widget parent = mainWindow;
#endif
#if defined(WIN32)
	HWND parent = mainWindow;
#endif
#ifdef MLE_REHEARSAL
	if ( m_offscreen )
	{
#if defined(__sgi) || defined(__linux__)
		// Create the popup widget.
		parent = XtCreatePopupShell("popup",
			transientShellWidgetClass,
			mainWindow,NULL,0);

		// Move it offscreen.
		XtVaSetValues(parent,
			XtNoverrideRedirect,TRUE,
			XtNx,-1024,
			XtNy,-1024,
			NULL);

		// Show the popup window.
		XtPopup(parent,XtGrabNone);
		XtMoveWidget(parent,-1024,-1024);
		
		//m_shellParent = parent;
#endif /* __sgi */
#if defined(WIN32)
		WNDCLASSEX wndclass;
		HINSTANCE hInstance = NULL;
		static char szAppName[] = "PopupWindow";

		wndclass.cbSize = sizeof(wndclass);
		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc = PopupWindowProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInstance;
		wndclass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = szAppName;
		wndclass.hIconSm = LoadIcon(NULL,IDI_APPLICATION);

		RegisterClassEx(&wndclass);

		parent = CreateWindow(
			szAppName,			// window class name
			"Auteur Player",	// window caption
			WS_OVERLAPPED,		// window style
			CW_USEDEFAULT,		// initial x position
			CW_USEDEFAULT,		// initial y position
			CW_USEDEFAULT,		// initial x size
			CW_USEDEFAULT,		// initial y size
			NULL,				// parent window handle
			NULL,				// window menu handle
			hInstance,			// program instance handle
			NULL);				// creation parameters


		SetParent(mainWindow,parent);
		SetWindowPos(parent,HWND_NOTOPMOST,-1024,-1024,0,0,SWP_NOSIZE | SWP_NOZORDER);
#endif
	}
	m_shellParent = parent;

	// Create the main window and rendering area.
#if defined(__sgi) || defined(__linux__)
	//m_examVwr = new MleExaminerViewer(parent);
	//m_flyVwr = new MleFlyViewer(parent);
	//m_planeVwr = new MlePlaneViewer(parent);
	//m_walkVwr = new MleWalkViewer(parent);
	m_examVwr = new SoXtExaminerViewer(parent);
	//m_flyVwr = new SoXtFlyViewer(parent);
	//m_planeVwr = new SoXtPlaneViewer(parent);
#endif
#if defined(WIN32)
	m_examVwr = new SoWinExaminerViewer(parent);
	//m_flyVwr = new SoWinFlyViewer(parent);
	//m_planeVwr = new SoWinPlaneViewer(parent);
#endif
	m_viewer = NULL;
    
#else	// #ifdef MLE_REHEARSAL
	// Not at rehearsal - no true iv viewer.
#if defined(__sgi) || defined(__linux__)
	m_viewer = new SoXtRenderArea(parent);
#endif
#if defined(WIN32)
	m_viewer = new SoWinRenderArea(parent);
#endif
	m_viewer->setAutoRedraw(FALSE);
#endif	// #ifdef MLE_REHEARSAL

	// Configure viewer.
#ifdef MLE_REHEARSAL
	setViewer(STAGE_VIEWER_EXAMINER);
#endif
	m_viewer->setTitle("Magic Lantern - Inventor Stage");
	m_viewer->setSize(SbVec2s(640,480));
#if defined(__sgi) || defined(__linux__)
	m_viewer->setEventCallback((SoXtRenderAreaEventCB *)eventHandler,this);
#endif
#if defined(WIN32)
	m_viewer->setEventCallback((SoWinRenderAreaEventCB *)eventHandler,this);
#endif
	m_viewer->show();

	// Create the root of the scene graph.
	m_root = new SoSelection;
	m_root->ref();

// XXX try this sometime, may give better performance
// m_root->renderCaching.setValue(SoSeparator::OFF);

	// Set up selection.
	m_root->policy.setValue(SoSelection::SINGLE);

	// Attach set root.
	m_sets = new SoSeparator;
	m_root->addChild(m_sets);

// XXX try this sometime, may give better performance
// m_sets->renderCaching.setValue(SoSeparator::OFF);

#ifdef MLE_REHEARSAL
	// Add a headlight node - this serves to light the source/target
    m_root->addChild(new SoDirectionalLight);

	// Create cameras.
	//   You might wonder why we have a camera here when all the
	//   sets have their own camera.  The reason is that we need
	//   to display manipulators.  It is not easy (perhaps)
	//   impossible) to know where to insert manip geometry into
	//   the set scene graph in a way that it won't interfere with
	//   the roles' assumption of the graph topology.  So we
	//   pull the manip geometry outside the set.  In order to
	//   have it display correctly, we need to give it its own camera.
	m_cameraSwitch = new SoSwitch;
	m_perspectCamera = new SoPerspectiveCamera;
	m_orthoCamera = new SoOrthographicCamera;

	//
	// Attach cameras, switch on the perspective camera for the viewer.
	//
	m_root->addChild(m_cameraSwitch);
	m_cameraSwitch->addChild(m_perspectCamera);
	m_cameraSwitch->addChild(m_orthoCamera);
	m_cameraSwitch->whichChild.setValue(0);

	// Create the camera sensor.
	m_cameraSensor = new SoNodeSensor;
	m_cameraSensor->setFunction((SoSensorCB *)cameraCB);
	m_cameraSensor->setData(this);
	m_cameraSensor->setPriority(0);

	// Be sure new xform2manip/dragger classes.
	// are init'd (SoNewManips seems to do this for CosmoCreate3d)
#if defined(__sgi) || defined(__linux__)
	//SoNewManips::init();
#endif
	// Note: Coin3D library modifications initializes the new manips.

	// Create a Target, Source, and Snapper
	m_target = new Target;
	m_target->setSnappingTolerance(0.1);
	m_source = new Source;
	m_source->setSnappingTolerance(0.1);
	m_snapper = new MleSnapper;
	m_snapper->setTarget(m_target);
	m_snapper->setSource(m_source);

	// Create a separator to hold the target and source markers and
	// construction tools. Add the target and source marker scene
	// graphs. These have to be before the selection node so they can
	// get all the events properly.
	m_toolRoot = new SoSeparator;
	m_root->addChild(m_toolRoot);
	m_toolRoot->addChild(m_target->getSceneGraph());
	m_toolRoot->addChild(m_source->getSceneGraph());

	// Add the horizon grid to the toolRoot.
	SoInput in;
#if READ_FROM_FILE
	if (! in.openFile("grid.iv")) {
	    fprintf(stderr, "Can't open grid.iv\n");
	    exit(1);
	}
#else
	in.setBuffer((void *) g_gridBuffer, sizeof(g_gridBuffer));
#endif
	SoSeparator *gridRoot = SoDB::readAll(&in);
	if (gridRoot == NULL) {
	    fprintf(stderr, "Bad read of grid.iv\n");
	    exit(1);
	}
	// Make grid unpickable.
	SoPickStyle *gridPickStyle = new SoPickStyle;
	gridPickStyle->style.setValue(SoPickStyle::UNPICKABLE);
	gridRoot->insertChild(gridPickStyle, 0);

	// Add grid to switch inside tool root.
	m_gridSwitch = new SoSwitch;
	m_gridSwitch->addChild(gridRoot);
	m_gridSwitch->whichChild.setValue(SO_SWITCH_NONE);
	m_toolRoot->addChild(m_gridSwitch);

	// Create manip for 3D actors.
	// Attach manip under group node under manip switch.
	// XXX should possibly put this under m_toolRoot as well.
	m_manipSwitch = new SoSwitch;
	m_manipGroup = new SoGroup;
	m_manip = new SoTransformer2Manip;
	m_manip->getDragger()->addStartCallback((SoDraggerCB *)startDraggerCB,
					      this);
	m_manip->getDragger()->addMotionCallback((SoDraggerCB *)draggerCB,
					       this);
	m_manip->getDragger()->addFinishCallback((SoDraggerCB *)finishDraggerCB,
					       this);
	m_manipGroup->addChild(m_manip);

	// Add a hidden box after manip - the box mimics the bbox
	// of the actual object being manip'd, which is elsewhere in
	// the tree. This is needed for the snapper->snapNow() code
	// to work properly.
	SoDrawStyle *ds = new SoDrawStyle;
	ds->style.setValue(SoDrawStyleElement::INVISIBLE);
	m_manipHiddenCubeTranslation = new SoTranslation;
	m_manipHiddenCube = new SoCube;
	m_manipGroup->addChild(ds);
	m_manipGroup->addChild(m_manipHiddenCubeTranslation);
	m_manipGroup->addChild(m_manipHiddenCube);

	//
	// Create manip for 2D actors.  Turn off unused parts of the
	// dragger.  Note that for the dragger, part rotator5 cannot be
	// turned off; otherwise, the dragger will not perform translation.
	// To work around this, reposition the rotator5 to (0,0,30) so
	// user cannot see it during editing.
	// 
	m_transManip = new SoTransformerManip;
	SoTransformerDragger *transDragger = (SoTransformerDragger *)
					     m_transManip->getDragger();
	transDragger->addStartCallback((SoDraggerCB *) startDraggerCB, this);
	transDragger->addMotionCallback((SoDraggerCB *) draggerCB, this);
	transDragger->addFinishCallback((SoDraggerCB *) finishDraggerCB, this);
	transDragger->setPart("translator1", new SoSeparator);
	transDragger->setPart("translator2", new SoSeparator);
	transDragger->setPart("translator3", new SoSeparator);
	transDragger->setPart("translator4", new SoSeparator);
	transDragger->setPart("translator6", new SoSeparator);
	transDragger->setPart("rotator1", new SoSeparator);
	transDragger->setPart("rotator2", new SoSeparator);
	transDragger->setPart("rotator3", new SoSeparator);
	transDragger->setPart("rotator4", new SoSeparator);
	SoSeparator *rot5 = (SoSeparator *) transDragger->getPart("rotator5",
								  FALSE);
	MLE_ASSERT(rot5);
	SoGroup *rot5Gp2 = (SoGroup *) rot5->getChild(1);
	MLE_ASSERT(rot5Gp2);
	SoTranslation *rot5Gp2Trans = (SoTranslation *) rot5Gp2->getChild(2);
	rot5Gp2Trans->translation.setValue(0.0f, 0.0f, 30.0f);
	transDragger->setPart("rotator6", new SoSeparator);
	transDragger->setPart("scale1", new SoSeparator);
	transDragger->setPart("scale2", new SoSeparator);
	transDragger->setPart("scale3", new SoSeparator);
	transDragger->setPart("scale4", new SoSeparator);
	transDragger->setPart("scale5", new SoSeparator);
	transDragger->setPart("scale6", new SoSeparator);
	transDragger->setPart("scale7", new SoSeparator);
	transDragger->setPart("scale8", new SoSeparator);

	//
	// Attach manips.
	//
	m_root->addChild(m_manipSwitch);
	m_manipSwitch->addChild(m_manipGroup);
	m_manipSwitch->addChild(m_transManip);
	
	// Calculate the path to the manip, for snapper's use.
	m_manipSwitch->whichChild.setValue(0);
	SoSearchAction search;
	search.setType(SoTransformer2Manip::getClassTypeId());
	search.apply(m_root);
	m_manipPath = search.getPath()->copy();
//XXX need to ref this?
	m_manipPath->ref();
	MLE_ASSERT(m_manipPath);
	m_manipSwitch->whichChild.setValue(SO_SWITCH_NONE);
	
	// Add event callback to scoop up non-manipulator events.
	eventCallback = new SoEventCallback;
	m_root->addChild(eventCallback);
	
	// Create a nudger.
    m_nudger = new Nudger;
#endif

	// Done setting up, so attach it to the viewer.
	m_viewer->setSceneGraph(m_root);

#ifdef MLE_REHEARSAL

	// Init snapping.
	initSnapping();

	// Realize all windows if not offscreen.
	if ( ! m_offscreen )
#endif
#if defined(__sgi) || defined(__linux__)
		SoXt::show(mainWindow);
#endif
#if defined(WIN32)
		SoWin::show(mainWindow);
#endif

	// Initialize platform data.
	g_theTitle->m_platformData = initPlatform();

#if defined(__sgi) || defined(__linux__)
	// Flush the Xt queue.
	// Get the application context for the stage.
	XtAppContext appContext = SoXt::getAppContext();

	// Check for events and dispatch them if found.
	while ( XtAppPending(appContext) )
		XtAppProcessEvent(appContext,XtIMAll);
#endif
#if defined(WIN32)
	MSG msg;

	// Flush the message queue.
	while ( PeekMessage(&msg, mainWindow, 0, 0, PM_REMOVE) > 0 )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
#endif

	// Schedule the rendering function.
	g_theTitle->m_theScheduler->insertFunc(
		PHASE_STAGE,(MleSchedulerFunc)update,this,this);
}

MleIvStage::~MleIvStage(void)
{
#ifdef MLE_REHEARSAL
	MleIvStage::SetDrawOrder *tmpPtr;

	while (m_setDrawOrder) {
		tmpPtr = m_setDrawOrder;
		m_setDrawOrder = m_setDrawOrder->next;
		delete tmpPtr;
	}

	// Note we're not bothering to kill off sub-objects - shouldn't
	// be a problem since stage exists for entire process.
#endif
}

// initPlatform
//   Creates and initializes platform-specific data.
void*
MleIvStage::initPlatform(void)
{
#if defined(__sgi)
	MleSGIPlatformData *data = (MleSGIPlatformData *)mlMalloc(sizeof(MleSGIPlatformData));
#endif
#if defined(__linux__)
	//MleIvPlatformData *data = (MleIvPlatformData *)mlMalloc(sizeof(MleIvPlatformData));
	MleIvPlatformData *data = new MleIvPlatformData();
#endif
#if defined(WIN32)
    //MleIvPlatformData *data = (MleIvPlatformData *)mlMalloc(sizeof(MleIvPlatformData));
	MleIvPlatformData *data = new MleIvPlatformData();
#endif

    // Provide users with the render area widget.
    data->m_widget = m_viewer->getOverlayWidget();
#if defined(WIN32)
	data->m_widget = m_viewer->getGLWidget();
#endif
    MLE_ASSERT(data->m_widget);

#if defined(__sgi) || defined(__linux__)
    // Provide users with the application context.
    data->m_appContext = SoXt::getAppContext();
    MLE_ASSERT(data->appContext);
#endif

    // Focus management.
    data->m_focusEventHandlerRefCount = 0;

    // Device management.
    data->setKeyboardManager(MLE_INPUT_DEVICE_MANAGER_NOT_INSTANTIATED);
    data->setMouseManager(MLE_INPUT_DEVICE_MANAGER_NOT_INSTANTIATED);
    data->setJoystickManager(MLE_INPUT_DEVICE_MANAGER_NOT_INSTANTIATED);

    return((void *) data);
}

#if defined(__sgi) || defined(__linux__)
#ifdef MLE_REHEARSAL
#if 0
// X event handling: we call SoDB::doSelect() so that inventor
// animation, etc, will run even when no X events are being generated.
int MleIvStage :: doSelect(int nfds, 
  fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
  struct timeval *userTimeOut)
{
#if 0
    //XXX help! mvo 5/7/96 7pm
    // doing this causes event queue to unjam w/o an enterNotify event;
    // mouse motion, kbd input, or expose events will do it
    XtAppContext appContext = SoXt::getAppContext();
//XXX mvo 5/8/96 2:15pm try only certain events
    if (XtAppPending(appContext) & ~XtIMTimer)
    {
printf("doSelect: events pending\n");
	if (readfds)
	    FD_ZERO(readfds);
    
	printf("ivstage doSelect - xpending\n");
	FD_SET(getFD(), readfds);
    
	XtAppProcessEvent(appContext, ~XtIMTimer);
#if 0
	// XXX mvo 5/8/96 1:50pm do exactly what iv mainloop does
	// result -> when we do this, events never finish getting processed
	// and viewer never displays black (initial expose event)
        XEvent event;
	SoXt::nextEvent(appContext, &event);
	if (SoXt::dispatchEvent(&event))
	    printf("doSelect: event handled\n");
	else
	    printf("doSelect: events not handled\n");
#endif	// #if 0 (inner)

	return 0;
    }

printf("doSelect: no events pending - call doSelect()\n");

#endif	// #if 0 (outer)

    // mvo 5/8/96: temporary workaround - call doSelect for exam vwr
    // since it has no side effects, but call select directly for
    // other viewers since doSelect screws up their event streams
    if (m_viewer == m_examVwr)
        return SoDB::doSelect(nfds, readfds, writefds, exceptfds, userTimeOut);

    return select(nfds, readfds, writefds, exceptfds, userTimeOut);
}
#endif
#endif	// MLE_REHEARSAL doSelect()
#endif  // __sgi

// This adds a set to the stage.
MleSchedulerItem *
MleIvStage::addSet(void (*render)(MleSet *),MleSet *set)
{
    MLE_ASSERT(set);

	MleIvStage::SetInfo *finfo = new MleIvStage::SetInfo;

    if (set->isa("Mle3dSet")) {
        m_sets->addChild(Mle3dSet::cast(set)->getRoot());

		finfo->func = NULL;
		finfo->set = set;
	}
	else if(set->isa("Mle2dSet")) {
		// Create a new callback node.
		SoCallback *callback = new SoCallback;
		m_sets->addChild(callback);

		finfo->func = render;
		finfo->set = set;

		// Set the callback.
		callback->setCallback(setCB,finfo);
	}

#ifdef MLE_REHEARSAL
    // Reset snapping each time a set is added.
    initSnapping();

    // If this is the first set, make it the active set.
    if ( m_activeSet == NULL )
        setActiveSet(set);

	//
	// Add to the beginning of the set drawing order list.  Note the
	// first one on the list is drawn last.
	//
	addSetDrawOrder(finfo);
#endif

    return NULL;
}

// This is the set callback.
//   This function is called from the SoCallback node that corresponds
//   to a set.  In the Inventor stage, only 2D sets are represented by
//   SoCallback node.  Inventor is given control of rendering, and 2D set
//   rendering is invoked through this callback.
void
MleIvStage::setCB(void *data,SoAction *action)
{
	// Cast the data to the set info.
	MleIvStage::SetInfo *finfo = (MleIvStage::SetInfo *)data;

	if ( action->isOfType(SoGLRenderAction::getClassTypeId()) )
	{
		// Have the set do rendering by invoking the callback.
		(*finfo->func)(finfo->set);
	}
#ifdef MLE_REHEARSAL
	else if ( action->isOfType(SoGetBoundingBoxAction::getClassTypeId()) &&
		finfo->set == MleIvStage::cast(g_theStage)->m_activeSet )
	{
		// Only return the bounding box for the active set.
		//   This is used for camera adjustment, so it is only
		//   applicable for the set that the viewer is attached to.
		if (finfo->set->isa("Mle2dSet")) {
			Mle2dSet *set = Mle2dSet::cast(finfo->set);

			//
			// Get The bounding box.
			//
			MlScalar min[3], max[3];
			set->getBounds(min, max);

			//
			// Apply to the action.
			//
			SbBox3f extend(mlScalarToFloat(min[0]),
				       mlScalarToFloat(min[1]),
				       mlScalarToFloat(min[2]),
				       mlScalarToFloat(max[0]),
				       mlScalarToFloat(max[1]),
				       mlScalarToFloat(max[2]));
			((SoGetBoundingBoxAction *) action)->extendBy(extend);
		}
	}
#endif /* MLE_REHEARSAL bounding box action */
}

#if defined(__sgi) || defined(__linux__)
// This function is the Inventor event handler.  It is Installed on the
//   render area to pick up X events before the render area processes them.
//   It shold return TRUE if the event is not to be passed on to the
//   render area.
int
MleIvStage::eventHandler(MleIvStage *stage,XAnyEvent *event)
{
#ifdef MLE_REHEARSAL
	// Get stage size.
	int width,height;
	stage->getSize(&width,&height);

	if ( stage->getEditing() )
	{
	    // Check for double click.
	    if ( event->type == ButtonRelease )
	    {
			XButtonEvent *be = (XButtonEvent *)event;
			if ( be->button == Button1 && 
				MleStage::checkForDoubleClick(be) )
			{
				if ( stage->m_activeRole )
				{
					// Find mask value.
					if ( be->state & Mod1Mask )
					{
						// Call the launch callback.
						if ( stage->launchCB )
							(*stage->launchCB)(
								stage->m_activeRole->getActor(),
								stage->m_launchClientData);
					}
					else 
					{
						// Call the open callback.
						if ( stage->openCB )
							(*stage->openCB)(
							stage->m_activeRole->getActor(),
							stage->m_openClientData);
					}
				}
			}
	    }
	    if ( event->type == ButtonPress )
	    {
			XButtonEvent *be = (XButtonEvent *)event;
			if ( be->button == Button3 )
			{
				printf("Right mouse button pressed.\n");
				if (stage->rightMouseCB)
				{
					printf("Right mouse button sent.\n");
					XUngrabPointer(stage->getDisplay(), CurrentTime);
					(*stage->rightMouseCB)((XEvent*) event,
										  stage->m_rightMouseClientData);
				}
				return TRUE;
			}
	    }

	    // Normally let event go through (rehearsal editing time).
	    return FALSE;
	}
#endif /* MLE_REHEARSAL */

	// Can deliver events to the player here.

	// Don't pass this event to the viewer.
	return TRUE;
}
#endif

#if defined(WIN32)
// This function is the Inventor event handler.  It is Installed on the
//   render area to pick up Windows messages before the render area processes them.
//   It shold return TRUE if the event is not to be passed on to the
//   render area.
int
MleIvStage::eventHandler(MleIvStage *stage,MSG *msg)
{
#ifdef MLE_REHEARSAL
	// Get stage size.
	int width,height;
	stage->getSize(&width,&height);

	if ( stage->getEditing() )
	{
	    // Check for double click.
	    if ( msg->message == WM_LBUTTONUP )
	    {
			if ( MleStage::checkForDoubleClick(msg) )
			{
				if ( stage->m_activeRole )
				{
					// Find mask value.
					if ( msg->wParam & MK_SHIFT )
					{
						// Call the launch callback.
						if ( stage->launchCB )
							(*stage->launchCB)(
								stage->m_activeRole->getActor(),
								stage->m_launchClientData);
					}
					else 
					{
						// Call the open callback.
						if ( stage->openCB )
							(*stage->openCB)(
							stage->m_activeRole->getActor(),
							stage->m_openClientData);
					}
				}
			}
	    }
	    if ( msg->message == WM_RBUTTONDOWN )
	    {
			printf("Right mouse button pressed.\n");
			if (stage->rightMouseCB)
			{
				printf("Right mouse button sent.\n");
				(*stage->rightMouseCB)((MSG*) msg,
					stage->m_rightMouseClientData);
			}
			return TRUE;
	    }

	    // Normally let event go through (rehearsal editing time).
	    return FALSE;
	}
#endif /* MLE_REHEARSAL */

	// Can deliver events to the player here.
	Win32CallData data;
    MleEvent event = msg->message;
    int status;
  
    data.hwnd = msg->hwnd;
    data.uMsg = msg->message;
    data.wParam = msg->wParam;
    data.lParam = msg->lParam;
    status = g_theTitle->m_theEventMgr->dispatchEvent(event,(void *) &data);

	// Don't pass this event to the viewer.
	return TRUE;
}
#endif

void
MleIvStage::update(MleIvStage * stage)
{
	// Draw the scene graph.
	stage->m_viewer->render();

#if defined(__sgi) || defined(__linux__)
	// Get the application context for the stage.
	XtAppContext appContext = SoXt::getAppContext();

	// Check for events and dispatch them if found.
	while ( XtAppPending(appContext) )
		XtAppProcessEvent(appContext,XtIMAll);
#endif
#if defined(WIN32)
	MSG msg;
	HWND window = SoWin::getTopLevelWidget();

	// Flush the message queue.
	while ( PeekMessage(&msg, window, 0, 0, PM_REMOVE) > 0 )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
#endif
}

void
MleIvStage:: getProperty(MleObject *object, const char *name, unsigned char **value)
{
    //cout << "Getting MleIvStage property " << name << "." << endl;
    if (strcmp("size",name) == 0)
    {
        MlVector2 property = ((MleIvStage *)object)->getSizeProperty();
        *((MlVector2 *)value) = property;
    } else
    {
        // TBD: log warning.
        //cout << "***** ERROR: Unknown MleIvStage property: " << name << endl;
    }
}

void
MleIvStage::setProperty(MleObject *object, const char *name, unsigned char *value)
{
    //cout << "Setting MleIvStage property " << name << "." << endl;
    if (strcmp("size",name) == 0)
    {
        ((MleIvStage *)object)->setSizeProperty(*((MlVector2 *)value));
    } else {
        // TBD: log warning.
        //cout << "***** ERROR: Unknown MleIvStage property: " << name << endl;
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// everything below this point is for rehearsal only
#ifdef MLE_REHEARSAL

void
MleIvStage::edit(void)
{
	MLE_ASSERT(m_editMode);

#if defined(__sgi)
	// Get the application context for the stage.
	XtAppContext appContext = SoXt::getAppContext();

	// Check for events and dispatch them if found.
	while ( XtAppPending(appContext) )
		XtAppProcessEvent(appContext,XtIMAll);
#endif
#if defined(WIN32)
	MSG msg;
	HWND window = SoWin::getTopLevelWidget();

	// Flush the message queue.
	while ( PeekMessage(&msg, window, 0, 0, PM_REMOVE) > 0 )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
#endif
}

#if defined(__sgi) || defined(__linux__)
int 
MleIvStage::getFD()
{
    return (ConnectionNumber(XtDisplay(m_viewer->getWidget())));
}
#endif

int
MleIvStage::setSize(int width,int height)
{
	m_viewer->setSize(SbVec2s(width,height));

	return 0;	// Signifies success.
}

void
MleIvStage::getSize(int *width,int *height)
{
	SbVec2s size = m_viewer->getSize();

	*width = size[0];
	*height = size[1];
}

void
MleIvStage::setOffscreen(int flag)
{
	m_offscreen = flag;
}

void
MleIvStage::render(void)
{
	m_viewer->render();
}

void
MleIvStage::activateManipulator(MleActor *actor,int invokeCallback)
{
    MLE_ASSERT(actor);
    MLE_ASSERT(m_editMode != 0);

    MleRole *role = actor->getRole();
    if ( role && role->m_set )
    {
		enableSelectionCallbacks(0);

		m_root->deselectAll();
		setActiveRole(m_activeSet, NULL, invokeCallback);

		if (role->m_set->isa("Mle3dSet"))
		{
			m_root->select(Mle3dRole::cast(role)->getRoot());
			setActiveRole(role->m_set, role, invokeCallback);
		}
		else if (role->m_set->isa("Mle2dSet"))
			setActiveRole(role->m_set, role, invokeCallback);

		enableSelectionCallbacks(1);
    }
}

void
MleIvStage::deactivateManipulator(MleActor *actor,int invokeCallback)
{
    MLE_ASSERT(actor);
    MLE_ASSERT(m_editMode != 0);

    MleRole *role = actor->getRole();
    if ( role && role == m_activeRole )
    {
		enableSelectionCallbacks(0);

		m_root->deselectAll();
		setActiveRole(m_activeSet, NULL, invokeCallback);

		enableSelectionCallbacks(1);
    }
}

#if defined(__sgi) || defined(__linux__)
Window
MleIvStage::getWindow(void)
{
	return XtWindow(m_shellParent);
}

Display*
MleIvStage::getDisplay(void)
{
	return XtDisplay(m_viewer->getWidget());
}
#endif
#if defined(WIN32)
HWND
MleIvStage::getWindow(void)
{
	return m_shellParent;
}
#endif

//////////////////////////////////////////////////////////////////////
//
// Getting attributes of the stage.
//

const char** MleIvStage::getFunctions()
{
    static char *funcs[] =
    {
		STAGE_MOVE_TO_TARGET, 
		STAGE_SNAPPING_TARGET, 
		STAGE_BGND_COLOR, 
		STAGE_VIEW_ALL, 
		NULL
    };
    
    return (const char **)funcs;
}

const char** MleIvStage::getFunctionAttributes(char* functionName)
{
    if (!strcmp(functionName, STAGE_ATTR_EDIT_MODES))
    {
		static char *funcs[] =
		{
			STAGE_ATTR_EDIT_MODE_PLAY, 
			STAGE_ATTR_EDIT_MODE_PICK, 
			STAGE_ATTR_EDIT_MODE_SEEK, 
			NULL
		};
    
		return (const char **)funcs;
	}

	if (!strcmp(functionName, STAGE_ATTR_VIEWERS))
	{
		static char *funcs[] =
		{
			STAGE_VIEWER_EXAMINER, 
			STAGE_VIEWER_FLY, 
			STAGE_VIEWER_WALK, 
			STAGE_VIEWER_PLANE, 
			NULL
		};

		return (const char **)funcs;
	}

	if (!strcmp(functionName, STAGE_ATTR_GLOBAL_RENDER_MODES))
	{
		static char *renderFuncs[] =
		{
			RENDER_AS_IS, 
			RENDER_HIDDEN_LINE, 
			RENDER_NO_TEXTURE, 
			RENDER_LOWRES, 
			RENDER_WIREFRAME, 
//XXX mvo 4/24/96: render points crashes when there's a selected item...
// filed as a bug which we'll look at after we get onto 6.2 (b/c 6.2
// may fix this problem)
//			RENDER_POINTS, 
			RENDER_BBOX, 
	    NULL
		};
    
		return (const char **)renderFuncs;
    }

    printf("WARNING MleIvStage::getFunctionAttributes: unknown func '%s'\n", 
	functionName);
    return NULL;
}

//////////////////////////////////////////////////////////////////////
//
// Control over IV viewers
//
// this routine has been copied to sgiivstage.c++ -- these two copies
// should be kept in sync.
//


int MleIvStage::setViewer(char* viewerName)
{
#if defined(__sgi) || defined(__linux__)
    //MleFullViewer *oldViewer = m_viewer;
    SoXtFullViewer *oldViewer = m_viewer;
#endif
#if defined(WIN32)
	SoWinFullViewer *oldViewer = m_viewer;
#endif

    if (!strcmp(viewerName, STAGE_VIEWER_FLY))
		m_viewer = m_flyVwr;
    else if (!strcmp(viewerName, STAGE_VIEWER_PLANE))
		m_viewer = m_planeVwr;
#if defined(__sgi) || defined(__linux__)
    //else if (!strcmp(viewerName, STAGE_VIEWER_WALK))
	//	m_viewer = m_walkVwr;
#endif
    else    // STAGE_VIEWER_EXAMINER
		m_viewer = m_examVwr;

    m_viewer->setDecoration(FALSE);
    m_viewer->setPopupMenuEnabled(FALSE);
    m_viewer->setViewing(FALSE);
    m_viewer->setAutoClipping(FALSE);

    // Only do this extra init stuff if this is not the first
    // viewer, because the first viewer's init gets this stuff
    // done already in the init() method.
    if (oldViewer)
    {
		oldViewer->setEventCallback(NULL);
		oldViewer->setSceneGraph(NULL);

		// Copy some values from previous viewer.
		m_viewer->setTitle(oldViewer->getTitle());
		m_viewer->setSize(oldViewer->getSize());
		m_viewer->setDecoration(oldViewer->isDecoration());
		m_viewer->setAutoClipping(oldViewer->isAutoClipping());
		m_viewer->setBackgroundColor(oldViewer->getBackgroundColor());
		m_viewer->setHeadlight( oldViewer->isHeadlight() );
		m_viewer->setBufferingType( oldViewer->getBufferingType() );
		m_viewer->setViewing( oldViewer->isViewing() );
		m_viewer->setSeekTime( oldViewer->getSeekTime() );	

#if defined(__sgi) || defined(__linux__)
		m_viewer->setEventCallback((SoXtRenderAreaEventCB *)eventHandler,this);
#endif
#if defined(WIN32)
		m_viewer->setEventCallback((SoWinRenderAreaEventCB *)eventHandler,this);
#endif

		m_viewer->show();
		oldViewer->hide();

#if defined(__sgi) || defined(__linux__)
		m_viewer->setDrawStyle(SoXtViewer::STILL, 
				 oldViewer->getDrawStyle(SoXtViewer::STILL));
		m_viewer->setDrawStyle( 
				SoXtViewer::INTERACTIVE,
				oldViewer->getDrawStyle(SoXtViewer::INTERACTIVE) );
#endif
#if defined(WIN32)
		m_viewer->setDrawStyle(SoWinViewer::STILL, 
				 oldViewer->getDrawStyle(SoWinViewer::STILL));
		m_viewer->setDrawStyle( 
				SoWinViewer::INTERACTIVE,
				oldViewer->getDrawStyle(SoWinViewer::INTERACTIVE) );
#endif

		m_viewer->setSceneGraph(m_root);

#if defined(__sgi) || defined(__linux__)
		// Flush the Xt queue.
		// Get the application context for the stage.
		XtAppContext appContext = SoXt::getAppContext();
    
		// Check for events and dispatch them if found.
		while ( XtAppPending(appContext) )
			XtAppProcessEvent(appContext,XtIMAll);
#endif
#if defined(WIN32)
		MSG msg;
		HWND window = SoWin::getTopLevelWidget();

		// Flush the message queue.
		while ( PeekMessage(&msg, window, 0, 0, PM_REMOVE) > 0 )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
#endif

		// Sync up the camera by setting the active set.
		setActiveSet(m_activeSet);
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////

char* MleIvStage::getViewer()
{
    // XXX should optimize these strings by defining them
    // as static const in the class - eventually will need to move
    // into resources.
    if (m_viewer == m_flyVwr)
		return STAGE_VIEWER_FLY;
    if (m_viewer == m_planeVwr)
		return STAGE_VIEWER_PLANE;
#if defined(__sgi)
    if (m_viewer == m_walkVwr)
		return STAGE_VIEWER_WALK;
#endif
    if (m_viewer == m_examVwr)
		return STAGE_VIEWER_EXAMINER;
	
    return NULL;
}

//////////////////////////////////////////////////////////////////////

void MleIvStage::setBgndColor(float* callerColor)
{
    SbColor color(callerColor);
    m_viewer->setBackgroundColor(color);
}

//////////////////////////////////////////////////////////////////////

void MleIvStage::getBgndColor(float* color)
{
    m_viewer->getBackgroundColor().getValue(
	color[0], color[1], color[2]);
}

//////////////////////////////////////////////////////////////////////
// Horizon grid
void MleIvStage::setHorizonGrid(int onOff)
{
    m_gridSwitch->whichChild.setValue(
	onOff ? SO_SWITCH_ALL : SO_SWITCH_NONE);
}

//////////////////////////////////////////////////////////////////////
// returns on/off value
int MleIvStage::getHorizonGrid()
{
    return (m_gridSwitch->whichChild.getValue() == SO_SWITCH_ALL);
}

//////////////////////////////////////////////////////////////////////

void MleIvStage::openPrefsDialog()
{
#if defined(__sgi)
    m_viewer->openPrefSheet();
#endif
}

//////////////////////////////////////////////////////////////////////

void MleIvStage::viewAll()
{
    m_viewer->viewAll();
}

//////////////////////////////////////////////////////////////////////

void MleIvStage::showDecoration(int onOff)
{
    m_viewer->setDecoration(onOff);
}

//////////////////////////////////////////////////////////////////////
//
// Control over edit modes
//

int MleIvStage::setEditMode(char* editMode)
{
    if (!strcmp(editMode, STAGE_ATTR_EDIT_MODE_PLAY))
    {
		setEditing(0);
    }
    else if (!strcmp(editMode, STAGE_ATTR_EDIT_MODE_PICK))
    {
		setEditing(1);
    }
    else if (!strcmp(editMode, STAGE_ATTR_EDIT_MODE_NAV))
    {
		setEditing(2);
    }
    else if (!strcmp(editMode, STAGE_ATTR_EDIT_MODE_SEEK))
    {
		setEditing(3);
    }
    else
    {
		printf("ivStage:setEditMode: unknown mode '%s'\n", editMode);
    }
    return 0;
}

char* MleIvStage::getEditMode()
{
    fixEditMode();
    return "";
}

//////////////////////////////////////////////////////////////////////

void
MleIvStage::setEditing(int mode)
{
	// Mode may have changed from seek back to nav.
	fixEditMode();

	if ( mode == m_editMode )
		return;
	
	// Undo old mode.
	switch( m_editMode )
	{
	    case 0:	// Run mode.
		break;
	    case 1:	// Pick mode.
			// Remove the event callback.
			eventCallback->removeEventCallback(
				SoMouseButtonEvent::getClassTypeId(),
				(SoEventCallbackCB *)eventCB,this);

			// Remove the selection callbacks.
			enableSelectionCallbacks(0);

			// Unset the active role.
			//   We do this because if we are leaving editing mode
			//   the role may be deleted as a result of execution.
			m_root->deselectAll();
			setActiveRole(m_activeSet,NULL);

			// Remove the camera node sensor.
			m_cameraSensor->detach();
			break;
	    default:	// nav (2) and seek (3) modes
			// Remove the event callback.
			eventCallback->removeEventCallback(
				SoMouseButtonEvent::getClassTypeId(),
				(SoEventCallbackCB *)eventCB,this);

			// Remove the camera node sensor.
			m_cameraSensor->detach();
			break;
	}

	// Call superclass method.
	MleStage::setEditing(mode);

	// Enter new mode.
	switch ( mode )
	{
	    case 0:	// Run mode.
		break;
	    case 1:	// Pick mode.
			// Add the event callbacks.
			eventCallback->addEventCallback(
				SoMouseButtonEvent::getClassTypeId(),
				(SoEventCallbackCB *)eventCB,this);

			// Sync up the camera by setting the active set.
			setActiveSet(m_activeSet);

			// Add the selection callbacks.
			enableSelectionCallbacks(1);

			// Attach node sensor to the camera.
			m_cameraSensor->attach(m_cameraSwitch->whichChild.getValue() ?
				(SoCamera *)m_orthoCamera : (SoCamera *)m_perspectCamera);

			// Touch the sets separator to invalidate bounding box cache.
			if (m_activeSet && m_activeSet->isa("Mle2dSet"))
				m_sets->touch();

			// Set picking mode.
			m_viewer->setViewing(FALSE);
			break;
	    default:	// nav (2) and seek (3) modes.
			// Add the event callbacks.
			eventCallback->addEventCallback(
				SoMouseButtonEvent::getClassTypeId(),
				(SoEventCallbackCB *)eventCB,this);

			// Sync up the camera by setting the active set.
			setActiveSet(m_activeSet);

			// Attach node sensor to the camera.
			m_cameraSensor->attach(m_cameraSwitch->whichChild.getValue() ?
				(SoCamera *)m_orthoCamera : (SoCamera *)m_perspectCamera);

			// Touch the sets separator to invalidate bounding box cache.
			if (m_activeSet && m_activeSet->isa("Mle2dSet"))
				m_sets->touch();

			// Set viewing mode.
			m_viewer->setViewing(TRUE);
			
			// Seek mode only: set into seek temporary mode.
			if (mode == 3)
				m_viewer->setSeekMode(TRUE);

			break;
	}
}

//
// Fixes edit mode - may have changed from seek back to nav.
//

void MleIvStage::fixEditMode()
{
    // XXX need to expunge these numbers!
    if ( m_editMode == 3 && !m_viewer->isSeekMode())
	m_editMode = 2;
}


// This sets the active role, attaching a manip
void
MleIvStage::setActiveRole(MleSet *set,MleRole *role,int cbFlag)
{
	// Do nothing if this is already the active role.
	if ( role == m_activeRole )
		return;
	
	// Remove any leftover selection on NULL role.
	//   This prevents a bug that happens on a scene change.  The
	//   activeRole is appropriately unset as a result of a
	//   monitor call.  However, the selection node retains the
	//   truncated path as the active selection.  When something
	//   is selected next, this becomes the second part of a multiple
	//   selection when only a single selection is expected.  To
	//   prevent this, make sure the selection is cleared when the
	//   active role is cleared.
	if ( role == NULL )
		m_root->deselectAll();

	// Unset current active role.
	if ( m_activeRole )
	{
		// NOTE! The activeRole may have already been
		// deleted (if we're called by notifyCB),
		// so accessing it aside from just its pointer
		// could be dangerous.
		//
		// getActor() access here is slightly dangerous
		// if activeRole has been deleted.
		// It works because the notifier 
		// removal doesn't have to work - it's being 
		// removed right after it is executed anyway.
		// The way for it to fail is if the memory
		// had been changed to some existing actor
		// in the notifier...not terribly likely.
		// See pv issu(e) #409013. --mvo 8/1/96

		// Remove the callback for deletion.
		MleMonitor::g_deleteNotifier.removeCallback(
			m_activeRole->getActor(),
			(MleNotifierFunc)notifyCB,this);

		// Turn off the manip.
		m_manipSwitch->whichChild.setValue(SO_SWITCH_NONE);

		// Remove manipulator from snapper.
		//  We do this by checking whether snapper had any
		//  manip set on it, since at this point activeRole
		//  could be destroyed. This can happen if the stage
		//  has a selected actor, and the user does a scene
		//  change from SE or other tool - the actor is destroyed
		//  and we're called from the destroy notifyCB. isa()
		//  on the deleg would be unsafe here.
		if (m_snapper->getNumManips() > 0) 
		{
		    MLE_ASSERT(m_snapper != NULL);
		    m_snapper->removeManip(m_manip);
		}

		// Notify interested parties.
		if ( cbFlag && unpickCB )
			(*unpickCB)(m_activeRole->getActor(),m_unpickClientData);
	}
	
	m_activeRole = role;

	// Set up new active role.
	if ( m_activeRole )
	{
		// Log a callback for deletion.
		MleMonitor::g_deleteNotifier.addCallback(
			m_activeRole->getActor(),
			(MleNotifierFunc)notifyCB,this);

		// Make sure the camera points to this set.
		setActiveSet(set);

		if ( m_activeRole->isa("Mle3dRole") )
		{
			Mle3dRole *ivd =
				Mle3dRole::cast(m_activeRole);

			// Find out what the actor thinks the transform is.
			MleActor *actor = m_activeRole->getActor();
			MlTransform atrans;
			actor->getTransform(atrans);

			// Convert to an Inventor matrix.
			SbMatrix actorMat(
					  mlScalarToFloat(atrans[0][0]),
					  mlScalarToFloat(atrans[0][1]),
					  mlScalarToFloat(atrans[0][2]),
					  0,
					  mlScalarToFloat(atrans[1][0]),
					  mlScalarToFloat(atrans[1][1]),
					  mlScalarToFloat(atrans[1][2]),
					  0,
					  mlScalarToFloat(atrans[2][0]),
					  mlScalarToFloat(atrans[2][1]),
					  mlScalarToFloat(atrans[2][2]),
					  0,
					  mlScalarToFloat(atrans[3][0]),
					  mlScalarToFloat(atrans[3][1]),
					  mlScalarToFloat(atrans[3][2]),
					  1);

			// Set the hidden cube to be correct
			// size/pos to match geometry.
			SoGetBoundingBoxAction bbaction(
				SbViewportRegion(640,480));
			bbaction.apply(ivd->getRoot());
			SbXfBox3f geomBbox = bbaction.getXfBoundingBox();
			
			// Remove the actor's transform, so we just get
			// the geometry's bbox (actor xform is included
			// already in manip xform).
			geomBbox.setTransform(
			    geomBbox.getTransform() * actorMat.inverse());

			// get an axis-aligned box, so that the bbox
			// is big enough to enclose entire deleg geometry.
			SbBox3f geomFinalBox = geomBbox.project();

			// Set cube translation node and cube size
			// from bbox.
			m_manipHiddenCubeTranslation->translation.setValue(
			    geomFinalBox.getCenter());
			float width, height, depth;
			geomFinalBox.getSize(width, height, depth);
			m_manipHiddenCube->width.setValue(width);
			m_manipHiddenCube->height.setValue(height);
			m_manipHiddenCube->depth.setValue(depth);
			
			// Force manip to rescale its box.
			// Note this is highly dependent on the
			// internals of SoTransformer2Manip. Necessary
			// because otherwise manip geom. doesn't resize
			// correctly when switching between objects.
			SoDragger *dragger = m_manip->getDragger();
			SoSurroundScale *ss =
			    (SoSurroundScale *) 
			    dragger->getPart("surroundScale", FALSE);
			ss->invalidate();

			// Turn on the manip
			int mIdx = m_manipSwitch->findChild(m_manipGroup);
			m_manipSwitch->whichChild.setValue(mIdx);

			// Inform the Snapper of the new manipulator.
			MLE_ASSERT(m_snapper != NULL);
			m_snapper->addManip(m_manipPath->copy());

			// Sync the manip and the geometry.
			SoPath *path = m_root->getPath(0);
			MLE_ASSERT(path);
			while ( Mle3dRole::g_pickRegistry.find(
					path->getTail()) == NULL )
			{
			    MLE_ASSERT(path->getLength() > 0);
			    path->truncate(path->getLength() - 1);
			}

			// Get the bounding box via a matrix action.
			//   This is a "transformed" bounding box, which
			//   returns the base bounding box not including
			//   the actor transform or geom transforms.
			SoGetMatrixAction matrixAction(
				SbViewportRegion(640,480));
			matrixAction.apply(path);
			SbMatrix xform = matrixAction.getMatrix();
			
			// Append local xform.
			xform *= actorMat;

			// Factor transform into manip fields.
			SbVec3f translation;
			SbRotation rotation;
			SbVec3f scaleFactor;
			SbRotation scaleOrientation;
			xform.getTransform(translation,rotation,
				scaleFactor,scaleOrientation);
			
			// Update the manip.
			m_manip->translation.setValue(translation);
			m_manip->rotation.setValue(rotation);
			m_manip->scaleFactor.setValue(scaleFactor);

			// Set the correction matrix to not include
			// actor transform.
			m_manipNorm = xform*actorMat.inverse();

			// Set picked obj as point of interest.
			MlVector3 poi(mlFloatToScalar(translation[0]),
				      mlFloatToScalar(translation[1]), 
				      mlFloatToScalar(translation[2]));
			setPointOfInterest(poi);
		}
		else if (m_activeRole->isa("Mle2dRole"))
		{
			Mle2dRole *ivd = Mle2dRole::cast(m_activeRole);

			//
			// Turn on the manip.
			//
			int tMIdx = m_manipSwitch->findChild(m_transManip);
			m_manipSwitch->whichChild.setValue(tMIdx);

			//
			// Get the bounding box of the role.
			//
			MlScalar min[2], max[2];
			ivd->getBounds(min, max);
			MLE_ASSERT(min[0] <= max[0]);
			MLE_ASSERT(min[1] <= max[1]);

			//
			// Store manip geometry into manipNorm (without
			// transformation).
			//
			float scaleX, scaleY;
			scaleX = mlScalarToFloat(max[0] - min[0] +
						 ML_SCALAR_ONE) / 2.0f;
			scaleY = mlScalarToFloat(max[1] - min[1] +
						 ML_SCALAR_ONE) / 2.0f;
			SbVec3f scale(scaleX, scaleY, 1.0f);
			m_manipNorm.makeIdentity();
			m_manipNorm.setScale(scale);

			//
			// Get the manip transformation (only translation
			// in the case of 2D role).
			//
			SbVec3f trans(mlScalarToFloat(max[0] + min[0]) / 2.0f,
				      mlScalarToFloat(max[1] + min[1]) / 2.0f,
				      0.0f);

			//
			// Update the dragger.
			//
			m_transManip->translation.setValue(trans);
			m_transManip->rotation.setValue(SbRotation::identity());
			m_transManip->scaleFactor.setValue(scale);
		}


		// Notify interested parties.
		if ( cbFlag && pickCB )
			(*pickCB)(m_activeRole->getActor(),m_pickClientData);
	}
}

// Sets point of interest for navigation when in some iv viewers.
void
MleIvStage::setPointOfInterest(MlVector3 pos)
{
    // If we're using examiner viewer, set focal distance
    // of camera to be dist to actor selected.
    if (m_viewer == m_examVwr)
    {
		SoCamera *cam = (SoCamera*) m_cameraSwitch->getChild(
			m_cameraSwitch->whichChild.getValue());
		SbVec3f cameraPosIv = cam->position.getValue();
		MlVector3 cameraPos(mlFloatToScalar(cameraPosIv[0]),
					mlFloatToScalar(cameraPosIv[1]),
					mlFloatToScalar(cameraPosIv[2]));
		MlVector3 distVec = pos - cameraPos;
		cam->focalDistance.setValue(mlScalarToFloat(distVec.length()));
    }
}

void
MleIvStage::setActiveSet(MleSet *set)
{
	// Set the persistent variable.
	m_activeSet = set;

	if ( m_activeSet == NULL )
		return;

	if ( m_activeSet->isa("Mle3dSet") )
	{
		// XXX The way this should really work is to use the
		//   camera API on the set.  It's not done here
		//   because right now the Mle3dSet doesn't have
		//   the complete camera API.

		// We have to make sure the camera sensor is detached before
		//   we update the stage camera from the set camera.  This
		//   is because the sensor has priority 0, which is effected
		//   immediately when the camera is changed.  This will go
		//   off and update the set camera, which trashes the values
		//   we are trying to preserve
		SoNode *sensorNode = m_cameraSensor->getAttachedNode();
		m_cameraSensor->detach();

		// Sync the cameras.
		SoCamera *fcamera =
			Mle3dSet::cast(m_activeSet)->getCamera();
		if ( fcamera->isOfType(SoPerspectiveCamera::getClassTypeId()) )
		{
			// Set the switch to perspective.
			m_cameraSwitch->whichChild.setValue(0);

			// Attach the viewer to the perspective camera.
			m_viewer->setCamera(m_perspectCamera);

			m_perspectCamera->copyFieldValues(fcamera);
		}
		else
		{
			// Set the switch to orthographic.
			m_cameraSwitch->whichChild.setValue(1);

			// Attach the viewer to the orthographic camera.
			m_viewer->setCamera(m_orthoCamera);

			m_orthoCamera->copyFieldValues(fcamera);
		}

		// Now reestablish the cameraSensor if it was attach
		if ( sensorNode )
			m_cameraSensor->attach(sensorNode);
	}
	else if (m_activeSet->isa("Mle2dSet")) {
		//
		// Use the orthographic camera.
		//
		int oCIdx = m_cameraSwitch->findChild(m_orthoCamera);
		m_cameraSwitch->whichChild.setValue(oCIdx);

		//
		// XXX When set properties are used for 2D set bounding
		//     box, use the following call
		//
		//     MlScalar min[3], max[3];
		//     ((Mle2dSet *) activeSet)->getBounds(min, max);
		//
		//     For now use the size of the whole stage window.
		//
		int width, height;
		getSize(&width, &height);

		//
		// Start updating the stage Inventor orthographic camera
		// parameters.
		//

		//
		// Set the camera position at the middle of the set in
		// xy and 11.0 in z.
		//
		// XXX Use these when set properties are used for 2D set
		//     bounding box.
		//
		//     pos[0] = mlScalarToFloat(max[0] - min[0]) / 2.0f;
		//     pos[1] = mlScalarToFloat(max[1] - min[1]) / 2.0f;
		//
		float pos[3];
		pos[0] = width / 2.0f;
		pos[1] = height / 2.0f;
		pos[2] = 11.0f;
		m_orthoCamera->position.setValue(pos[0], pos[1], pos[2]);

		//
		// Set the camera orientation with zero rotation looking
		// down the z axis.
		//
		m_orthoCamera->orientation.setValue(SbRotation::identity());

		//
		// Set the camera distance to the near and far clipping planes
		// at 1.0 and 21.0 (or 10.0 and -10.0 in z).
		//
		float nearDist = 1.0f;
		float farDist = 21.0f;
		m_orthoCamera->nearDistance.setValue(nearDist);
		m_orthoCamera->farDistance.setValue(farDist);

		//
		// Set the camera focal distance at 11.0 (or 0.0 in z).
		//
		m_orthoCamera->focalDistance.setValue(11.0f);

		//
		// Set the camera aspect ratio to be 640 by 480 and the
		// height of the camera viewing volume.
		//
		m_orthoCamera->aspectRatio.setValue(640.0f / 480.0f);
		m_orthoCamera->height.setValue(height);
	}
}

void
MleIvStage::addSetDrawOrder(MleIvStage::SetInfo *setInfo)
{
	//
	// Add the set to the beginning of the list.
	//
	if (m_setDrawOrder)
	{
		MleIvStage::SetDrawOrder *tmpPtr = m_setDrawOrder;

		tmpPtr->prev = new MleIvStage::SetDrawOrder;
		tmpPtr->prev->setInfo = setInfo;
		tmpPtr->prev->prev = NULL;
		tmpPtr->prev->next = tmpPtr;
		m_setDrawOrder = tmpPtr->prev;
	}
	else {
		m_setDrawOrder = new MleIvStage::SetDrawOrder;
		m_setDrawOrder->setInfo = setInfo;
		m_setDrawOrder->prev = NULL;
		m_setDrawOrder->next = NULL;
	}
}

// Get current camera based on switch.
SoCamera * MleIvStage::getCamera()
{
    MLE_ASSERT(m_cameraSwitch->whichChild.getValue()>=0);
    return (SoCamera*) m_cameraSwitch->getChild(
			m_cameraSwitch->whichChild.getValue());
}

// Called by a set when it's camera moves.
void MleIvStage::setCameraMoved(MleSet *set, SoCamera *cam)
{
    // Only track set's camera if this set is the active one.
    if (set == m_activeSet)
    {
		// Re-set the active set to force stage cam to sync up.
		setActiveSet(set);
    }
}

void
MleIvStage::notifyCB(void *,MleIvStage *stage)
{
	// We're being told that the actor that owns the active role
	//   has been deleted.  Set the active role to NULL so we
	//   aren't referring to it any more.
	stage->setActiveRole(stage->m_activeSet,NULL);
}

//////////////////////////////////////////////////////////////////////

void
MleIvStage::eventCB(MleIvStage *stage,SoEventCallback *callback)
{
    stage->eventCallbackProc(callback);
}

void
MleIvStage::eventCallbackProc(SoEventCallback *callback)
{
    //
    // Get the event.
    //
    const SoEvent *event = callback->getEvent();

    if(SoMouseButtonEvent::isButtonPressEvent(event,
	SoMouseButtonEvent::BUTTON1)) {

	//
	// Find the window position.
	//
	const SbVec2s& position = event->getPosition();
	int width,height;
	getSize(&width,&height);

	enableSelectionCallbacks(0);

	//
	// Deselect active role if there's any.
	//
	if (m_activeRole)
	{
	    m_root->deselectAll();
	    setActiveRole(m_activeSet, NULL);
	}

	//
	// Loop through the available sets starting from the
	// layer at the top on down.
	//
	MleRole *role = NULL;
	MleSet *set;
	MleIvStage::SetDrawOrder *setOrder;

	for (setOrder = m_setDrawOrder; setOrder;
	    setOrder = setOrder->next)
	{

	    set = setOrder->setInfo->set;

	    //
	    // Switch on the set type.
	    //
	    if (set->isa("Mle3dSet"))
		{
			//
			// Get the path for the node picked.
			//
			SoPath *path = Mle3dSet::cast(set)->getPickPath(position);

			//
			// Make sure the selection is not on the dragger itself.
			//
			if (path && path->getTail() != m_manip)
			{
				//
				// Search for a registered node, looking back from
				// the tail.
				//
				for (int i = 0; i < path->getLength(); i++)
				{
					//
					// Look in the dictionary to find a role.
					//
					role = (MleRole *)
						   Mle3dRole::g_pickRegistry.find(
							   path->getNodeFromTail(i));

					//
					// Stop if we find one.
					//
					if(role)
						break;
				}

				if (role)
				{
					//
					// Select the role.
					//
					m_root->select(Mle3dRole::cast(role)->
							 getRoot());
					setActiveRole(set, role);

					//
					// Break out of the for loop which loops
					// through all the sets.
					//
					break;
				}
			}
	    }
	    else if(set->isa("Mle2dSet")) {
		//
		// Select the role if any.
		//
		role = Mle2dSet::cast(set)->pickRole(
			       position[0], position[1]);
		if(role) {
		    setActiveRole(set, role);

		    //
		    // Break out of the for loop which loops through
		    // all the sets.
		    //
		    break;
		}
	    }
	}

	enableSelectionCallbacks(1);
    }
}

//////////////////////////////////////////////////////////////////////
// static
void
MleIvStage::startDraggerCB(MleIvStage *stage,SoDragger *)
{
    stage->startDraggerCallback(TRUE);
}

void MleIvStage::startDraggerCallback(MlBoolean generateCallback)
{
    MLE_ASSERT(m_activeRole);

    MleActor *actor = m_activeRole->getActor();
    MLE_ASSERT(actor);

    // Notify interested parties.
    if ( startManipCB && generateCallback )
	    (*startManipCB)(actor, m_startManipClientData);
}

// static
void
MleIvStage::draggerCB(MleIvStage *stage, SoDragger *dragger)
{
    MLE_ASSERT(stage->m_activeRole);

    if (stage->m_activeRole->isa("Mle3dRole"))
		stage->draggerCallback(TRUE);
    else if(stage->m_activeRole->isa("Mle2dRole"))
		stage->transDraggerCallback(TRUE);
}

void MleIvStage::draggerCallback(MlBoolean generateCallback)
{
    MLE_ASSERT(m_activeRole);

    MleActor *actor = m_activeRole->getActor();
    MLE_ASSERT(actor);

    // Construct the manip matrix.
    SbMatrix dmat;
    dmat.setTransform(
	    m_manip->translation.getValue(),
	    m_manip->rotation.getValue(),
	    m_manip->scaleFactor.getValue());
    
    // Extract changes from the norm here.
    SbMatrix delta = m_manipNorm.inverse()*dmat;

    // Build the appropriate matrix.
    MlTransform t;
    t[0][0] = mlFloatToScalar(delta[0][0]);
    t[0][1] = mlFloatToScalar(delta[0][1]);
    t[0][2] = mlFloatToScalar(delta[0][2]);
    t[1][0] = mlFloatToScalar(delta[1][0]);
    t[1][1] = mlFloatToScalar(delta[1][1]);
    t[1][2] = mlFloatToScalar(delta[1][2]);
    t[2][0] = mlFloatToScalar(delta[2][0]);
    t[2][1] = mlFloatToScalar(delta[2][1]);
    t[2][2] = mlFloatToScalar(delta[2][2]);
    t[3][0] = mlFloatToScalar(delta[3][0]);
    t[3][1] = mlFloatToScalar(delta[3][1]);
    t[3][2] = mlFloatToScalar(delta[3][2]);

    // Set the actor transformation.
    actor->setTransform(t);

    // Resolve the edit.
    actor->resolveEdit();

    // Now get the actor transformation.
    //   This may not be the same as the transform set because the
    //   actor may not accept all (or any) kinds of transforms
    actor->getTransform(t);

    delta[0][0] = mlScalarToFloat(t[0][0]);
    delta[0][1] = mlScalarToFloat(t[0][1]);
    delta[0][2] = mlScalarToFloat(t[0][2]);
    delta[1][0] = mlScalarToFloat(t[1][0]);
    delta[1][1] = mlScalarToFloat(t[1][1]);
    delta[1][2] = mlScalarToFloat(t[1][2]);
    delta[2][0] = mlScalarToFloat(t[2][0]);
    delta[2][1] = mlScalarToFloat(t[2][1]);
    delta[2][2] = mlScalarToFloat(t[2][2]);
    delta[3][0] = mlScalarToFloat(t[3][0]);
    delta[3][1] = mlScalarToFloat(t[3][1]);
    delta[3][2] = mlScalarToFloat(t[3][2]);

    // Correct the transformation for actor constraints.
    SbVec3f translation;
    SbRotation rotation;
    SbVec3f scaleFactor;
    SbRotation soRotation;
    dmat = m_manipNorm*delta;
    dmat.getTransform(translation,rotation,scaleFactor,soRotation);

    m_manip->translation.setValue(translation);
    m_manip->rotation.setValue(rotation);
    m_manip->scaleFactor.setValue(scaleFactor);
    
    //
    // Notify interested parties.
    //
    if (manipCB && generateCallback)
	(*manipCB)(actor, m_manipClientData);
}

void
MleIvStage::transDraggerCallback(MlBoolean generateCallback)
{
    MLE_ASSERT(m_activeRole);

    MleActor *actor = m_activeRole->getActor();
    MLE_ASSERT(actor);

    //
    // Get the changes.
    //
    SbVec3f pos = m_transManip->translation.getValue();

    //
    // Get the role bounding box for offset calculation.
    //
    Mle2dRole *role = (Mle2dRole *) (actor->getRole());
    MlScalar min[2], max[2];
    role->getBounds(min, max);
    MLE_ASSERT(min[0] <= max[0]);
    MLE_ASSERT(min[1] <= max[1]);
    float offsetX, offsetY;
    offsetX = mlScalarToFloat(max[0] - min[0]) / 2.0f;
    offsetY = mlScalarToFloat(max[1] - min[1]) / 2.0f;

    //
    // Build the appropriate matrix for the actor.  Note we want the
    // lower left of the actor rather than the center which the manip
    // gives.
    //
    pos[0] = pos[0] - offsetX;
    pos[1] = pos[1] - offsetY;
    MlTransform t;
    t[0][0] = ML_SCALAR_ONE;
    t[0][1] = ML_SCALAR_ZERO;
    t[0][2] = ML_SCALAR_ZERO;
    t[1][0] = ML_SCALAR_ZERO;
    t[1][1] = ML_SCALAR_ONE;
    t[1][2] = ML_SCALAR_ZERO;
    t[2][0] = ML_SCALAR_ZERO;
    t[2][1] = ML_SCALAR_ZERO;
    t[2][2] = ML_SCALAR_ONE;
    t[3][0] = mlFloatToScalar(pos[0]);
    t[3][1] = mlFloatToScalar(pos[1]);
    t[3][2] = ML_SCALAR_ZERO;

    //
    // Update the actor position to be in sync with the manip.
    //
    actor->setTransform(t);

    //
    // Propagate the change to the role side.
    //
    actor->resolveEdit();

    //
    // Get the actor transformation; this may not be the same as the
    // transform set above because the actor may not accept all
    // (or any) kinds of transform.
    //
    actor->getTransform(t);

    //
    // Note the dragger position if based on the center of the role
    // rather than lower left corner of the role.
    //
    pos[0] = mlScalarToFloat(t[3][0]) + offsetX;
    pos[1] = mlScalarToFloat(t[3][1]) + offsetY;
    pos[2] = mlScalarToFloat(t[3][2]);

    //
    // Correct manip transformation based on actor constraints.
    //
    m_transManip->translation.setValue(pos);

    //
    // Touch the set separator to invalidate the bounding box.
    //
    m_sets->touch();

    //
    // Notify interested parties.
    //
    if (manipCB && generateCallback)
		(*manipCB)(actor, m_manipClientData);
}

// static
void
MleIvStage::finishDraggerCB(MleIvStage *stage,SoDragger *)
{
    stage->finishDraggerCallback(TRUE);
}

void MleIvStage::finishDraggerCallback(MlBoolean generateCallback)
{
    MLE_ASSERT(m_activeRole);

    MleActor *actor = m_activeRole->getActor();
    MLE_ASSERT(actor);

    // Set manip'd obj as point of interest.
    MlTransform t;
    actor->getTransform(t);
    MlVector3 pos = t[3];
    setPointOfInterest(pos);

    // Notify interested parties.
    if (finishManipCB && generateCallback)
	    (*finishManipCB)(actor, m_finishManipClientData);
}

//////////////////////////////////////////////////////////////////////

void
MleIvStage::cameraCB(MleIvStage *stage,SoNodeSensor *)
{
	// mvo 5/23/96: focalDistance often starts outside
	// of the range of near to far clip planes. This seems to
	// be due to inventor not wanting to muck with focalDistance
	// in its core system (but apps like SceneViewer let iv set
	// up and handle a lot more, causing them to not have this
	// problem).  So, we always
	// ensure it's within that range. This causes correct dolly
	// calibration and center of rotation. One side effect is that
	// sometimes the focalDistance will venture outside the near
	// to far range, and be popped back to (near+far)/2. This can
	// have a jarring effect during rotations of worlds that are
	// fairly flat (thin in one dimension). Filed as bug #376694.
	// It also causes the beneficial effect of allowing the 
	// examiner viewer to dolly in indefinitely.

	SoCamera*cam = stage->getCamera();
	if (cam->nearDistance.getValue() > cam->focalDistance.getValue() ||
	    cam->farDistance.getValue() < cam->focalDistance.getValue())
	{
	    // recalc focal distance - avoid a recursive cam callback
	    SoNode *attachedNode = stage->m_cameraSensor->getAttachedNode();
	    stage->m_cameraSensor->detach();
	    
	    cam->focalDistance.setValue(
	       (cam->nearDistance.getValue() + cam->farDistance.getValue())/2.0);
    
	    // now reestablish the cameraSensor if it was attached
	    if ( attachedNode )
		    stage->m_cameraSensor->attach(attachedNode);
        }


	if ( stage->m_activeSet == NULL )
		return;
	
	if ( stage->m_activeSet->isa("Mle3dSet") )
	{
		// XXX we should do this by calling the camera API on the
		//   set, but it isn't fully implemented yet
		SoCamera *fcamera = 
			Mle3dSet::cast(stage->m_activeSet)->getCamera();
		
		if ( fcamera->isOfType(SoPerspectiveCamera::getClassTypeId()) )
			fcamera->copyFieldValues(stage->m_perspectCamera);
		else
			fcamera->copyFieldValues(stage->m_orthoCamera);
	}
}

// static
SoPath * 
MleIvStage :: pickFilterCB(MleIvStage *stage,const SoPickedPoint *pick)
{
    // Filter out picks on the dragger geom - since the dragger is not
    // in the geom tree, picks on it cause selected geom to get deselected
    // so we ignore picks on the dragger.
    if (pick->getPath()->getTail() == stage->m_manip)
	return (new SoPath);

    return (pick->getPath());
}

void
MleIvStage::selectCB(MleIvStage *stage,SoPath *path)
{
    MLE_ASSERT(stage);
    MLE_ASSERT(path);

    // If the selection is on the dragger itself, then simply ignore it.
    if (path->getTail() == stage->m_manip)
	return;

    // Search for a registered node, looking back from the tail.
    MleRole *role = NULL;
    for ( int i = 0; i < path->getLength(); i++ )
    {
		// Look in the dictionary to find a role.
		role = (MleRole *)Mle3dRole::g_pickRegistry.find(
			path->getNodeFromTail(i));
		
		// Stop if we find one.
		if ( role )
			break;
		}
    
    // Make this the active role.
    //   NULL is okay, because that just sets no active role.
    stage->setActiveRole(stage->m_activeSet,role);
}

void
MleIvStage::deselectCB(MleIvStage *stage,SoPath *)
{
    // XXX should look up the right set for role
    stage->setActiveRole(stage->m_activeSet,NULL);
}

void MleIvStage::enableSelectionCallbacks(MlBoolean onOff)
{
    if (onOff)
    {
		m_root->setPickFilterCallback((SoSelectionPickCB *)pickFilterCB,
			this);
    }
    else
    {
		m_root->setPickFilterCallback(NULL, NULL);
    }
}

// Sets rendering mode for entire stage
void MleIvStage::setRenderMode(char *renderMode)
{
#if defined(__sgi) || defined(__linux__)
    SoXtViewer::DrawStyle style;
    if (!strcmp(renderMode, RENDER_AS_IS))
    {
		style = SoXtViewer::VIEW_AS_IS;
    }
    else if (!strcmp(renderMode, RENDER_HIDDEN_LINE))
    {
		style = SoXtViewer::VIEW_HIDDEN_LINE;
    }
    else if (!strcmp(renderMode, RENDER_NO_TEXTURE))
    {
		style = SoXtViewer::VIEW_NO_TEXTURE;
    }
    else if (!strcmp(renderMode, RENDER_LOWRES))
    {
		style = SoXtViewer::VIEW_LOW_COMPLEXITY;
    }
    else if (!strcmp(renderMode, RENDER_WIREFRAME))
    {
		style = SoXtViewer::VIEW_LINE;
    }
    else if (!strcmp(renderMode, RENDER_POINTS))
    {
		style = SoXtViewer::VIEW_POINT;
    }
    else if (!strcmp(renderMode, RENDER_BBOX))
    {
		style = SoXtViewer::VIEW_BBOX;
    }
    else
    {
		printf("ERROR MleIvStage setRenderMode: unknown render mode '%s'\n", 
			renderMode);
		style = SoXtViewer::VIEW_AS_IS;
    }

    m_viewer->setDrawStyle(SoXtViewer::STILL, style);
#endif
#if defined(WIN32)
    SoWinViewer::DrawStyle style;
    if (!strcmp(renderMode, RENDER_AS_IS))
    {
		style = SoWinViewer::VIEW_AS_IS;
    }
    else if (!strcmp(renderMode, RENDER_HIDDEN_LINE))
    {
		style = SoWinViewer::VIEW_HIDDEN_LINE;
    }
    else if (!strcmp(renderMode, RENDER_NO_TEXTURE))
    {
		style = SoWinViewer::VIEW_NO_TEXTURE;
    }
    else if (!strcmp(renderMode, RENDER_LOWRES))
    {
		style = SoWinViewer::VIEW_LOW_COMPLEXITY;
    }
    else if (!strcmp(renderMode, RENDER_WIREFRAME))
    {
		style = SoWinViewer::VIEW_LINE;
    }
    else if (!strcmp(renderMode, RENDER_POINTS))
    {
		style = SoWinViewer::VIEW_POINT;
    }
    else if (!strcmp(renderMode, RENDER_BBOX))
    {
		style = SoWinViewer::VIEW_BBOX;
    }
    else
    {
		printf("ERROR MleIvStage setRenderMode: unknown render mode '%s'\n", 
			renderMode);
		style = SoWinViewer::VIEW_AS_IS;
    }

    m_viewer->setDrawStyle(SoWinViewer::STILL, style);
#endif
}

const char * MleIvStage::getRenderMode() const
{
#if defined(__sgi) || defined(__linux__)
    switch (m_viewer->getDrawStyle(SoXtViewer::STILL))
    {
		case SoXtViewer::VIEW_AS_IS: return RENDER_AS_IS;
		case SoXtViewer::VIEW_HIDDEN_LINE: return RENDER_HIDDEN_LINE;
		case SoXtViewer::VIEW_NO_TEXTURE: return RENDER_NO_TEXTURE;
		case SoXtViewer::VIEW_LOW_COMPLEXITY: return RENDER_LOWRES;
		case SoXtViewer::VIEW_LINE: return RENDER_WIREFRAME;
		case SoXtViewer::VIEW_POINT: return RENDER_POINTS;
		case SoXtViewer::VIEW_BBOX: return RENDER_BBOX;
    }

    printf("ERROR iv stage getRenderMode: unknown iv mode %d\n", 
	   m_viewer->getDrawStyle(SoXtViewer::STILL));
#endif
#if defined(WIN32)
    switch (m_viewer->getDrawStyle(SoWinViewer::STILL))
    {
		case SoWinViewer::VIEW_AS_IS: return RENDER_AS_IS;
		case SoWinViewer::VIEW_HIDDEN_LINE: return RENDER_HIDDEN_LINE;
		case SoWinViewer::VIEW_NO_TEXTURE: return RENDER_NO_TEXTURE;
		case SoWinViewer::VIEW_LOW_COMPLEXITY: return RENDER_LOWRES;
		case SoWinViewer::VIEW_LINE: return RENDER_WIREFRAME;
		case SoWinViewer::VIEW_POINT: return RENDER_POINTS;
		case SoWinViewer::VIEW_BBOX: return RENDER_BBOX;
    }

    printf("ERROR iv stage getRenderMode: unknown iv mode %d\n", 
	   m_viewer->getDrawStyle(SoWinViewer::STILL));
#endif

	return NULL;
}

int
MleIvStage::pushSet(MleSet *f)
{
    int i, index = -1;
    SoNode *setNode;
    MleIvStage::SetDrawOrder *setData, *insertAfter;

    MLE_ASSERT(f);

    //
    // Find the index of the matching set callback node under the
    // set separator.
    //
    for (i = m_sets->getNumChildren() - 1, setData = m_setDrawOrder;
		setData; i--, setData = setData->next)
	{
		if (f == setData->setInfo->set)
		{
			index = i;
			break;
		}
    }

    if(index > 0)
	{
		//
		// Push the set one layer down.
		//
		setNode = m_sets->getChild(index);
		MLE_ASSERT(setNode);
		if (f->isa("Mle2dSet"))
			((SoCallback *) setNode)->setCallback(NULL, NULL);
		setNode->ref();
		m_sets->removeChild(index);
		m_sets->insertChild(setNode, index - 1);
		setNode->unref();
		if (f->isa("Mle2dSet"))
			((SoCallback *) setNode)->setCallback(setCB,
								setData->setInfo);

		//
		// Update setDrawOrder list.
		//
		if (setData == m_setDrawOrder)
			m_setDrawOrder = setData->next;

		insertAfter = setData->next;
		if(setData->prev)
			setData->prev->next = setData->next;
		setData->next->prev = setData->prev;
		setData->prev = insertAfter;
		setData->next = insertAfter->next;
		insertAfter->next = setData;
		if (setData->next)
			setData->next->prev = setData;

		return 0;
    }
    else
	//
	// Already at the lowest layer.
	//
	return -1;
}

int
MleIvStage::pushSetToBottom(MleSet *f)
{
    int i, index = -1;
    SoNode *setNode;
    MleIvStage::SetDrawOrder *setData, *insertAfter;

    MLE_ASSERT(f);

    //
    // Find the index of the matching set callback node under the
    // set separator.
    //
    for (i = m_sets->getNumChildren() - 1, setData = m_setDrawOrder;
	setData; i--, setData = setData->next) {
	if (f == setData->setInfo->set) {
	    index = i;
	    break;
	}
    }

    if(index > 0)
	{
		//
		// Push the set to the bottom.
		//
		setNode = m_sets->getChild(index);
		MLE_ASSERT(setNode);
		if(f->isa("Mle2dSet"))
			((SoCallback *) setNode)->setCallback(NULL, NULL);
		setNode->ref();
		m_sets->removeChild(index);
		m_sets->insertChild(setNode, 0);
		setNode->unref();
		if (f->isa("Mle2dSet"))
			((SoCallback *) setNode)->setCallback(setCB,
								setData->setInfo);

		//
		// Update setDrawOrder list.
		//
		if (setData == m_setDrawOrder)
			m_setDrawOrder = setData->next;

		for (insertAfter = setData->next; insertAfter->next != NULL;)
			insertAfter = insertAfter->next;
		if (setData->prev)
			setData->prev->next = setData->next;
		setData->next->prev = setData->prev;
		setData->prev = insertAfter;
		setData->next = NULL;
		insertAfter->next = setData;

		return 0;
    }
    else
	//
	// Already at the lowest layer.
	//
	return -1;
}

int
MleIvStage::popSet(MleSet *f)
{
    int i, index = -1;
    SoNode *setNode;
    MleIvStage::SetDrawOrder *setData, *insertBefore;

    MLE_ASSERT(f);

    //
    // Find the index of the matching set callback node under the
    // set separator.
    //
    for (i = m_sets->getNumChildren() - 1, setData = m_setDrawOrder;
		setData; i--, setData = setData->next)
	{
		if (f == setData->setInfo->set)
		{
			index = i;
			break;
		}
    }

    if (index >= 0 && index < (m_sets->getNumChildren() - 1))
	{
		//
		// Pop the set one layer up.
		//
		setNode = m_sets->getChild(index);
		MLE_ASSERT(setNode);
		if (f->isa("Mle2dSet"))
			((SoCallback *) setNode)->setCallback(NULL, NULL);
		setNode->ref();
		m_sets->removeChild(index);
		m_sets->insertChild(setNode, index + 1);
		setNode->unref();
		if (f->isa("Mle2dSet"))
			((SoCallback *) setNode)->setCallback(setCB,
								setData->setInfo);

		//
		// Update setDrawOrder list.
		//
		insertBefore = setData->prev;
		setData->prev->next = setData->next;
		if (setData->next)
			setData->next->prev = setData->prev;
		setData->prev = insertBefore->prev;
		setData->next = insertBefore;
		insertBefore->prev= setData;
		if (setData->prev)
			setData->prev->next= setData;
		else
			m_setDrawOrder = setData;

		return 0;
    }
    else
	//
	// Already at the top layer.
	//
	return -1;
}

int
MleIvStage::popSetToTop(MleSet *f)
{
    int i, topIndex, index = -1;
    SoNode *setNode;
    MleIvStage::SetDrawOrder *setData, *insertBefore;

    MLE_ASSERT(f);

    //
    // Find the index of the matching set callback node under the
    // set separator.
    //
    for (i = m_sets->getNumChildren() - 1, setData = m_setDrawOrder;
	setData; i--, setData = setData->next)
	{
	if (f == setData->setInfo->set) {
	    index = i;
	    break;
	}
    }

    topIndex = m_sets->getNumChildren() - 1;
    if (index >= 0 && index < topIndex)
	{
		//
		// Pop the set to the top.
		//
		setNode = m_sets->getChild(index);
		MLE_ASSERT(setNode);
		if (f->isa("Mle2dSet"))
			((SoCallback *) setNode)->setCallback(NULL, NULL);
		setNode->ref();
		m_sets->removeChild(index);
		m_sets->insertChild(setNode, topIndex);
		setNode->unref();
		if (f->isa("Mle2dSet"))
			((SoCallback *) setNode)->setCallback(setCB,
								setData->setInfo);

		//
		// Update setDrawOrder list.
		//
		insertBefore = m_setDrawOrder;
		setData->prev->next = setData->next;
		if (setData->next)
			setData->next->prev = setData->prev;
		setData->prev = NULL;
		setData->next = insertBefore;
		insertBefore->prev= setData;
		m_setDrawOrder = setData;

		return 0;
    }
    else
	//
	// Already at the top layer.
	//
	return -1;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//
//	Routines ported from cosmo create 3d's CoLayer.c++
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//
// Sets up snapping for the current view.
//
/////////////////////////////////////////////////////////////////////////////

void
MleIvStage::initSnapping()
{
    // Compute the bounding box of the current scene
    //mvo: calc bbox manually
//    SbBox3f bbox = CoSceneManager::getBoundingBox(selectionNode);
    SbViewportRegion vpReg = SbViewportRegion(640,480);
    SoGetBoundingBoxAction bboxAction(vpReg);
    bboxAction.apply(m_sets);
    SbBox3f bbox = bboxAction.getBoundingBox();

    // Tell the target and source to set up their default positions
    // based on this bounding box
    m_target->setDefaultPosition(bbox);
    m_source->setDefaultPosition(bbox);

    // Fix their sizes
    updateTargetAndSource();
}

void
MleIvStage::setSnapping(int flag)
{
    m_snapper->enable(flag);
}

int
MleIvStage::hasSnappingTarget()
{
    return m_snapper->isEnabled();
}

/////////////////////////////////////////////////////////////////////////////
//
// Sets/returns whether scale snapping is enabled.
//
/////////////////////////////////////////////////////////////////////////////

void
MleIvStage::enableScaleSnapping(MlBoolean flag)
{
    m_snapper->enableScaleSnapping(flag);
}

MlBoolean
MleIvStage::isScaleSnappingEnabled() const
{
    return m_snapper->isScaleSnappingEnabled();
}

/////////////////////////////////////////////////////////////////////////////
//
// Moves all currently-selected objects so the master selection
// ends up at the snapping target.
//
// For now only single selection is supported.
//
/////////////////////////////////////////////////////////////////////////////

void
MleIvStage::moveToTarget()
{
    if (!m_snapper->isEnabled())
		return;

    // Move the master selection to the target, pulling the others
    // with it
    //mvo - single selection for now
    m_snapper->snapNow(0);
//    m_snapper->snapNow(selectionContext->getNumSelections() - 1);

    // Keep up to date with snapped changes. We do it this way because
    // when I tried to do it by value
    // changed callback, that caused an indirect endless loop 
    // between stage and snapper (info obj). --mvo 4/9/96
    // The 'FALSE' indicates to not generate callbacks to our client
    startDraggerCallback(FALSE);
    draggerCallback(FALSE);
    finishDraggerCallback(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
//
// Updates the target and source.
//
/////////////////////////////////////////////////////////////////////////////

void
MleIvStage::updateTargetAndSource()
{
    m_target->setCamera(getCamera());
    m_target->updateSize();
    m_source->setCamera(getCamera());
    m_source->updateSize();
}

/////////////////////////////////////////////////////////////////////////////
//
// Sets/returns whether 2D grid is enabled.
//
/////////////////////////////////////////////////////////////////////////////

#if 0

void
MleIvStage::enableGrid(Boolean flag)
{
    if (flag)
	{
		// Create it if this is the first time
		if (grid == NULL)
		{
			grid = new CtGrid2;
			// Add the subgraph containing the grid to the tool root
			toolRoot->addChild(grid);
			grid->setDisplay(TRUE);
		}

		else if (! grid->isDisplayed())
			grid->setDisplay(TRUE);
    }

    else if (grid->isDisplayed())
	grid->setDisplay(FALSE);
}

Boolean
MleIvStage::isGridEnabled() const
{
    return (grid != NULL && grid->isDisplayed());
}

#endif /* 0 */

/////////////////////////////////////////////////////////////////////////////
//
// Nudges all currently-selected objects in the given direction by
// the given number of pixels.
//
/////////////////////////////////////////////////////////////////////////////

void
MleIvStage::nudge(int direction, int numPixels)
{
    // Must have a selection to nudge.
    if (!m_activeRole)
    {
		printf("Warning: trying to nudge with no selection\n");
		return;
    }

    // XXX don't think we need to send copy of manipPath.
    m_nudger->nudge(m_manipPath->copy(), 
		  m_activeRole->getActor(), 
		  getCamera()->getViewVolume(), 
		  m_viewer->getViewportRegion(),
		  direction, 
		  numPixels);

    // After nudging manip, the object itself needs to move
    // --don't notify client because they ordered the nudge
    startDraggerCallback(FALSE);
    if (m_activeRole->isa("Mle3dRole"))
		draggerCallback(TRUE);
    else if (m_activeRole->isa("Mle2dRole"))
		transDraggerCallback(TRUE);
    finishDraggerCallback(FALSE);
}

//////////////////////////////////////////////////////////////////////
// Recalculates the clipping planes if auto clipping planes enabled.
// Should be called anytime an actor property changes that could
// effect actor transform.

void MleIvStage::recalcAutoClipPlanes()
{
    // Only recalc auto clip planes if feature is in use.
    if (m_viewer->isAutoClipping())
    {
		m_viewer->setAutoClipping(0);
		m_viewer->setAutoClipping(1);
    }
}


//////////////////////////////////////////////////////////////////////
//
// Misc Issues/To-do on Snapper implementation:
//
// May want to updateTargetAndSource() from some update() func, like
// cc3d's AuthorUI.c++ does.
//
// Need the CubeManip stuff or could we toss it?
//
//////////////////////////////////////////////////////////////////////


#endif	/* #ifdef MLE_REHEARSAL */
