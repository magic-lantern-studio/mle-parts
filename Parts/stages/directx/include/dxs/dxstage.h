/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file dxstage.h
 * @ingroup MleParts
 *
 * This file defines the DirectX Stage.
 *
 * @author Mark S. Millard
 * @date Nov 1, 2007
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

#ifndef __MLE_DXSTAGE_H_
#define __MLE_DXSTAGE_H_

// Include system header files.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <Dxerr.h>

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleStage.h"
#include "mle/MleScheduler.h"
#include "mle/MleEventDispatcher.h"
#include "mle/MlePtrArray.h"

// Include Magic Lantern Math header files.
#include "math/vector.h"


class MleSet;
typedef void (*DXSTAGE_RENDER_CALLBACK)(MleSet *);
typedef void (*DXSTAGE_ONDEVICERESET_CALLBACK)(unsigned int ev, void *callData);
typedef void (*DXSTAGE_ONDEVICELOST_CALLBACK)(unsigned int ev, void *callData);

/**
 * OnDeviceReset and OnDeviceLost events.
 */
enum OnDeviceResetEvent
{
	RELEASE_RESOURCE = 0,
	CREATE_RESOURCE = 1
};

/**
 * Structure specifying section of screen that has been drawn into.
 */
typedef struct _DxStageDirtyRect
{
    int min_x;
    int min_y;
    int max_x;
    int max_y;
} DxStageDirtyRect;

/**
 * Structure specifying callback entry for DirectX device reset.
 */
typedef struct _DxStageOnResetCallback
{
	void *m_calldata;
	DXSTAGE_ONDEVICERESET_CALLBACK m_callback;
} DxStageOnResetCallback;

/**
 * Structure specifying callback entry for DirectX device lost.
 */
typedef struct _DxStageOnDeviceLostCallback
{
	void *m_calldata;
	DXSTAGE_ONDEVICELOST_CALLBACK m_callback;
} DxStageOnDeviceLostCallback;

// XXX - This declaration is only temporary.
typedef struct
{
    // Spatial resolution.
    int       m_width;
    int       m_height;

    // Tonal resolution.
    int       m_bitsPerPixel;
    MlBoolean m_palettized;
    int       m_colorsAvailable;

    // Temporal resolution.
    int       m_refreshRate;
} MleDesktop;

/**
 * @brief This class implements a DirectX Stage based on Microsoft's
 * DirectX 9.0 platform.
 */
class DxStage : public MleStage
{
    MLE_STAGE_HEADER(DxStage);

  public:

	/**
	 * The default constructor.
	 */
    DxStage(void);

	/**
	 * The destructor.
	 */
	virtual ~DxStage(void);

  // Stage-specific API.
  //   This is new API (not inherited) for this particular stage type.
    
    // Buffer access.

	/**
	 * Get the back buffer DirectX surface.
	 *
	 * After using the DirectX surface, the surface must be
	 * released using <b>IDirect3DSurface9::Release()</b>. Failure
	 * to do so will result in memory leaks.
	 *
	 * @return A pointer to a Direct3D surface is returned.
	 */
    IDirect3DSurface9 *getPixelBuffer(void) const;

	/**
	 * Get the depth-stencil buffer DirectX surface.
	 *
	 * After using the DirectX surface, the surface must be
	 * released using <b>IDirect3DSurface9::Release()</b>. Failure
	 * to do so will result in memory leaks.
	 *
	 * @return A pointer to a Direct3D surface is returned.
	 */
    IDirect3DSurface9 *getDepthBuffer(void) const;

    //IDirect3DSurface9 *getFrontBuffer(void) const;

	/**
	 * Get the DirectX interface.
	 *
	 * @return A pointer to the Direct3D interface is returned.
	 */
	IDirect3D9 *getD3dObj(void) const { return m_d3dObj; }

	/**
	 * Get the DirectX device.
	 *
	 * @return A pointer to the Direct3D device is returned.
	 */
	IDirect3DDevice9 *getD3dDevice(void) const { return m_d3dDevice; }

	/**
	 * Get the DirectX device parameters.
	 *
	 * @param A pointer to the Direct3D device parameters is returned.
	 */
	D3DPRESENT_PARAMETERS *getD3dDeviceParameters(void) { return &m_d3dParameters; }


    /**
	 * This method installs the event and scheduler callbacks.
	 */
    virtual void init(void);
 
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
    virtual MleSchedulerItem *addSet(DXSTAGE_RENDER_CALLBACK cb,MleSet *set);

    /**
	 * This method is an accessor function.
	 *
	 * @param width The width of the Stage is returned.
	 * @param height The height of the Stage is returned.
	 */
    virtual void getSize(int *width,int *height);

    /**
	 * This method is used by screenblit() and the WM_PAINT message.
     * callback.
	 *
	 * @param newDC The new Device Context for blitting.
	 */
    void blit(HDC newDc);
    
    /**
	 * This method changes the size of the window managed by the stage.
     * It is used by the WM_SIZE callback.
	 *
	 * @param x The new x origin of the window.
	 * @param y The new y origin of the window.
	 * @param width The new width of the window.
	 * @param height The new height of the window.
	 */
    void resize(int x, int y, int width, int height);
    
    /**
	 * This method screen blits and empties the current dirty region.
     * It is a callback to be used during the PHASE_STAGE phase
     * of the execution cycle.
	 *
	 * @param stage A pointer to the stage.
	 */
    static void screenblit(void *stage);
    
    /**
	 * This callback is used to capture the WM_SIZE message. It is called
     * via the event dispatch manager.
	 *
	 * @param event The Magic Lantern event.
	 * @param callData Call data for the event callback.
	 * @param clientData Client data for the event callback
	 */
    static int resizeEventCB(MleEvent event,void *callData,void *clientData);

    /**
	 * This callback is used to capture the WM_SIZE, WM_QUERYNEWPALETTE and
     * WM_PALETTECHANGED messages. It is called via the event dispatch manager.
	 *
	 * @param event The Magic Lantern event.
	 * @param callData Call data for the event callback.
	 * @param clientData Client data for the event callback
	 */
    static int paintEventCB(MleEvent event,void *callData,void *clientData);

    /**
	 * This method is used to get the PC Desktop configuration.
	 */
    inline const MleDesktop *getDesktop(void)
    { return &m_desktop; }

	/**
	 * Set the background color.
	 *
	 * @param color The background color to set.
	 */
	void setBackground(D3DCOLOR color);

    /**
	 * Marks a region of the screen that needs to be blitted at frame end.
	 */
    void dirtyRegion(DxStageDirtyRect* newRegion);

	/**
	 * Toggle between windowed mode and fullscreen.
	 *
	 * @return If the mode was successfully toggled, then <b>TRUE</b> will be
	 * returned. Otherwise, <b>FALSE</b> will be returned.
	 */
	MlBoolean toggleFullScreen(void);

	/**
	 * Determine if the stage is resetting the DirectX device.
	 *
	 * @param <b>TRUE</b> will be returned if the stage is in the process
	 * of resetting the DirectX device. Otherwise <b>FALSE</b> will be returned.
	 */
	inline MlBoolean isResetting(void)
	{ return m_resetting; }

	/**
	 * Add a callback to be notified when the DirectX device
	 * will be reset. This gives registered objects a chance
	 * to release DirectX resources prior to the device reset.
	 *
	 * @param callback The callback function to add.
	 * @param calldata Data associated with the callback;
	 */
	void addResetCallback(DXSTAGE_ONDEVICERESET_CALLBACK callback, void *calldata);

	/**
	 * Remove the specified callback function from the list of
	 * routines to be notified when a DirectX device is going
	 * to be reset.
	 *
	 * @param callback The callback function to remove.
	 */
	void removeResetCallback(DXSTAGE_ONDEVICERESET_CALLBACK callback);

	/**
	 * Add a callback to be notified when the DirectX device
	 * is lost. This gives registered objects a chance
	 * to release DirectX resources prior to the device reset.
	 *
	 * @param callback The callback function to add.
	 * @param calldata Data associated with the callback;
	 */
	void addDeviceLostCallback(DXSTAGE_ONDEVICELOST_CALLBACK callback, void *calldata);

	/**
	 * Remove the specified callback function from the list of
	 * routines to be notified when a DirectX device is lost.
	 *
	 * @param callback The callback function to remove.
	 */
	void removeDeviceLostCallback(DXSTAGE_ONDEVICELOST_CALLBACK callback);

  private:

	// Initialize DirectX.
    void initDirectX(void);

  // Beginning of Magic Lantern properties.

  public:

    /** "size" property contains width and height in pixels. */
    MlVector2 size;

  // Declare member variables.

  public:

	/** Handle to output window. */
	HWND m_window;
	/** Handle to a DC for the window. */
	HDC  m_hdcWindow;
	/** Indication of how to show window via the ShowWindow() function. */
	int  m_showMode;

  protected:

	/** The DirectX interface. */
	IDirect3D9 *m_d3dObj;
	/** The primary DirectX device. */
	IDirect3DDevice9 *m_d3dDevice;
	/** The present parameters. */
	D3DPRESENT_PARAMETERS m_d3dParameters;

	/** The aspect ratio of the stage, width/height. */
	MlScalar m_aspect;
	/** The background color for the stage. */
	D3DCOLOR m_backgroundColor;
	/** The current dirty rectangle region for blitting. */
	DxStageDirtyRect *m_curBlitBounds;
	/** Array of registered callbacks for resetting DirectX. */
	MlePtrArray *m_onResetRegistry;
	/** Array of registered callbacks for device lost notification. */
	MlePtrArray *m_onDeviceLostRegistry;

  private:

    // The  desktop structure contains information relating to the
    // spatial, tonal and temporal resolutions of the presentation
    // surface.
    MleDesktop m_desktop;

    int m_x, m_y;              // Position in window of surface.
    int m_width, m_height;     // Size of stage.

    MlBoolean m_ready;         // TRUE when buffer is ready for blit.
	MlBoolean m_isWindowed;    // TRUE when stage is in a window.
	MlBoolean m_resetting;     // TRUE when the DirectX device is being reset.
};

// Macros for accessing stage which is stored with window instance.
#define MLE_INST_STAGE_SET(hwnd,stage) SetWindowLong((hwnd),0,(LONG)(stage))
#define MLE_INST_STAGE_GET(hwnd) ((DxStage *)GetWindowLong((hwnd),0))

// Macro for retrieving the window DC stored in the Win32 DirectX stage.
#define MLE_GET_WINDOW_DC() ((DxStage *)MleStage::g_theStage)->m_hdcWindow

// Macros for event dispatching priorities. Note that this ordering is
// important. The Stage resize callback must be executed prior to the
// Set resize callback because the Set uses the new off-screen buffer
// which is reallocated by the Stage resize callback. Also, the Stage
// resize paint callback is executed after all other resize events have
// been processed. The resize paint callback redraws the world in the new
// off-screen buffer.
#define MLE_RESIZE_STAGE_PRIORITY       10
#define MLE_RESIZE_SET_PRIORITY          9
#define MLE_RESIZEPAINT_STAGE_PRIORITY  -1

#endif /* __MLE_DXSTAGE_H_ */