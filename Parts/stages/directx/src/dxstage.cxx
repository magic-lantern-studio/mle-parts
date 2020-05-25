/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file dxstage.cxx
 * @ingroup MleParts
 *
 * This file implements the DirectX Stage.
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

// Include system header files.
#include <stdio.h>
#include <limits.h>
#include <windows.h>

// Include Magic Lantern Runtime Engine header files.
#include "mle/mlmalloc.h"
#include "mle/MleDirector.h"
#include "mle/MleScheduler.h"

// Include DirectX target header files.
#include "dxs/dxstage.h"
#include "mle/mlPlatformData.h"

// Private functions used by the stage.
static void QueryDesktopConfig( MleDesktop *pDesktop );

MLE_STAGE_SOURCE(DxStage,MleStage);

DxStage::DxStage(void)
{
    RECT  stRect;
    DWORD dwWinStyle;

    m_ready = FALSE;
	m_isWindowed = TRUE;
	m_resetting = FALSE;

    // Query the desktop to determine the screen's configuration.
    QueryDesktopConfig( &m_desktop );

    // Create the dirty bounding rectangle.
    m_curBlitBounds = (DxStageDirtyRect*) mlMalloc( sizeof(DxStageDirtyRect) );

    // Initialize the bounding rectangle.
    m_curBlitBounds->min_x = INT_MAX;
    m_curBlitBounds->min_y = INT_MAX;
    m_curBlitBounds->max_x = INT_MIN;
    m_curBlitBounds->max_y = INT_MIN;

	// Set the default background color (black).
	m_backgroundColor = D3DCOLOR_ARGB(0, 0, 0 , 0);

	// Initialize the default X, Y locations.
    m_x = 0;
    m_y = 0;

    // Set width and height.
    m_width  = ((MleWin32PlatformData*)(g_theTitle->m_platformData))->m_defaultWinWidth;
    m_height = ((MleWin32PlatformData*)(g_theTitle->m_platformData))->m_defaultWinHeight;

    // Capture the winstyle.
    dwWinStyle = ((MleWin32PlatformData *) (g_theTitle->m_platformData))->m_winStyle;

    // Calculate the aspect ratio.
    m_aspect = mlDiv( mlLongToScalar(m_width), mlLongToScalar(m_height) );

    // Calculate the windows rectangle needed to contain the client area
    // plus any overhead to support the window frame.
    stRect.left = 0;
    stRect.top  = 0;
    stRect.right  = m_width;
    stRect.bottom = m_height;
    
    AdjustWindowRect( &stRect, dwWinStyle, FALSE );

    // Create the window - pass stage pointer through lpvParam for callbacks.
    m_window = CreateWindow(
        ((MleWin32PlatformData*)(g_theTitle->m_platformData))->m_appName, 
        ((MleWin32PlatformData*)(g_theTitle->m_platformData))->m_appTitle,
        dwWinStyle,
        CW_USEDEFAULT, 
        CW_USEDEFAULT, 
        stRect.right - stRect.left,
        stRect.bottom - stRect.top,
        NULL,
        NULL,
        ((MleWin32PlatformData*)(g_theTitle->m_platformData))->m_appInstance,
        this);  

    MLE_ASSERT(m_window != NULL);
  
    // Get a global device context onto window that is used throughout 
    // application.  Window class has CS_OWNDC set.
    m_hdcWindow = GetDC(m_window);
    MLE_ASSERT(m_hdcWindow != NULL);

    // Default windows show mode, may be changed prior to init() call.
    m_showMode = SW_SHOW;

	// Allocate array of registered OnReset callbacks.
	m_onResetRegistry = new MlePtrArray(16);
	// Allocate array of registered OnLostDevice callbacks.
	m_onDeviceLostRegistry = new MlePtrArray(16);

	// Initialize DirectX.
	initDirectX();
}

DxStage::~DxStage(void)
{
	// Delete dirty region management structures.
    if (m_curBlitBounds)
        mlFree(m_curBlitBounds);

	// Delete array of registered OnReset callbacks.
	if (m_onResetRegistry)
	{
		while (m_onResetRegistry->getSize() > 0)
		{
			DxStageOnResetCallback *entry = (DxStageOnResetCallback *)((*m_onResetRegistry)[0]);
			m_onResetRegistry->remove(entry);
			delete entry;
		}
		delete m_onResetRegistry;
	}

	// Delete array of registered OnDeviceLost callbacks.
	if (m_onDeviceLostRegistry)
	{
		while (m_onDeviceLostRegistry->getSize() > 0)
		{
			DxStageOnDeviceLostCallback *entry = (DxStageOnDeviceLostCallback *)((*m_onDeviceLostRegistry)[0]);
			m_onDeviceLostRegistry->remove(entry);
			delete entry;
		}
		delete m_onDeviceLostRegistry;
	}

	// Release the DirectX device.
	ULONG count = m_d3dDevice->Release();

    // Destroy window if it is still attached.
    if (m_window) 
    {
        // Detach view.
        MLE_INST_STAGE_SET(m_window,NULL);
        DestroyWindow(m_window);
    }
}

void
DxStage::init()
{
    // Declare local variables
    MleCallbackId cbId;

    // Insert resize callback into event dispatch manager
    cbId = g_theTitle->m_theEventMgr->installEventCB(
        WM_SIZE,DxStage::resizeEventCB,(void *)this);

    // Bump priority of dispatched callback.
    g_theTitle->m_theEventMgr->changeEventCBPriority(WM_SIZE,cbId,
        MLE_RESIZE_STAGE_PRIORITY);

    // Insert palette callback into event dispatch manager.
    cbId = g_theTitle->m_theEventMgr->installEventCB(
        WM_PALETTECHANGED,DxStage::paintEventCB,(void *)this);

    // Insert palette callback into event dispatch manager.
    cbId = g_theTitle->m_theEventMgr->installEventCB(
        WM_QUERYNEWPALETTE,DxStage::paintEventCB,(void *)this);

    // Insert stage into the scheduler.
    g_theTitle->m_theScheduler->insertFunc(
        PHASE_STAGE,DxStage::screenblit,this,this);

    // Make the window appear.
    ShowWindow( m_window, m_showMode );

    // Call resize() to explicitly create the off screen buffer and pixelmaps.
    //   We use to rely on the WM_SIZE message to do this; however, we weren\'t
    //   garanteed that the message would be processed before we needed it in
    //   the rendering/set phase of the execution cycle (an asynchronous race
    //   could occur).
    resize( 0, 0, m_width, m_height );
}

IDirect3DSurface9 *
DxStage::getPixelBuffer(void) const
{
	// Retrieve the back buffer from the D3D device.
	IDirect3DSurface9 *backBuffer = NULL;
	HRESULT hr = m_d3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
	if (hr != D3D_OK)
	{
		DXTrace(__FILE__, __LINE__, hr, "Unable to get back buffer", true);
	}
	return backBuffer;
}

IDirect3DSurface9 *
DxStage::getDepthBuffer(void) const
{
	// Retrieve the depth-stencil surface from the D3D device.
	IDirect3DSurface9 *depthStencilSurface = NULL;
	HRESULT hr = m_d3dDevice->GetDepthStencilSurface(&depthStencilSurface);
	if (hr != D3D_OK)
	{
		DXTrace(__FILE__, __LINE__, hr, "Unable to get depth-stencil surface", true);
	}
	return depthStencilSurface;
}

MleSchedulerItem *
DxStage::addSet(DXSTAGE_RENDER_CALLBACK cb,MleSet *set)
{
    // Declare local variables.
    MleSchedulerItem *item;

    // Just pass this function on to the scheduler.
    item = g_theTitle->m_theScheduler->insertFunc(
        PHASE_SET, (MleSchedulerFunc)cb, set, this);

    return item;
}

void
DxStage::getSize(int *w,int *h)
{
    *w = m_width;
    *h = m_height;
}

void
DxStage::screenblit(void *stage)
{
    DxStage *dxStage = (DxStage *)stage;
    dxStage->blit(dxStage->m_hdcWindow);
}

// Use system to copy the pixels from the local buffer to the frame buffer.
void
DxStage::blit(HDC newDc)
{
    // Check if the local screen related data structures are ready.
    if (m_ready != TRUE)
        return;
  
    // Blit the apropriate region to the screen.
    if (m_curBlitBounds->min_x < m_curBlitBounds->max_x)
    {
		HRESULT hr;

		RECT region;
		region.left = m_x + m_curBlitBounds->min_x;
		//region.bottom = m_y + m_curBlitBounds->min_y;
		region.top = m_y + m_curBlitBounds->min_y;
		region.right = m_x +  m_curBlitBounds->max_x;
		//region.top = m_y + m_curBlitBounds->max_y;
		region.bottom = m_y + m_curBlitBounds->max_y;

		//hr = m_d3dDevice->Present(&region, &region, m_window, NULL);
		hr = m_d3dDevice->Present(NULL, NULL, NULL, NULL);
		if (hr != D3D_OK)
		{
			DXTrace(__FILE__, __LINE__, hr, "Unable to present back buffer", true);
			if (hr == D3DERR_DEVICELOST)
			{
				// Notify registered objects that the device has been lost
				// so that the objects may act appropriately.
				for (int i = 0; i < m_onDeviceLostRegistry->getSize(); i++)
				{
					DxStageOnDeviceLostCallback *entry = (DxStageOnDeviceLostCallback *)((*m_onDeviceLostRegistry)[i]);
					DXSTAGE_ONDEVICELOST_CALLBACK cb = entry->m_callback;
					(*cb)(RELEASE_RESOURCE, entry->m_calldata);
				}

				// Attempt to recover from a lost device.
				while (hr == D3DERR_DEVICELOST)
				{
					// Poll the device to see if we can reset it.
					do
					{
						::Sleep(1000);
						hr = m_d3dDevice->TestCooperativeLevel();
					} while (hr != D3DERR_DEVICENOTRESET);

					// Invoke callbacks so that registered objects can release DirectX resources.
					for (int i = 0; i < m_onResetRegistry->getSize(); i++)
					{
						DxStageOnResetCallback *entry = (DxStageOnResetCallback *)((*m_onResetRegistry)[i]);
						DXSTAGE_ONDEVICERESET_CALLBACK cb = entry->m_callback;
						(*cb)(RELEASE_RESOURCE, entry->m_calldata);
					}

					// Ok, reset the device.
					if (FAILED(m_d3dDevice->Reset(&m_d3dParameters)))
					{
						// Failed to reset, keep trying ...
						hr = D3DERR_DEVICELOST;
					} else
					{
						// Invoke callbacks so that registered objects can recreate DirectX resources.
						for (int i = 0; i < m_onResetRegistry->getSize(); i++)
						{
							DxStageOnResetCallback *entry = (DxStageOnResetCallback *)((*m_onResetRegistry)[i]);
							DXSTAGE_ONDEVICERESET_CALLBACK cb = entry->m_callback;
							(*cb)(CREATE_RESOURCE, entry->m_calldata);
						}
					}
				}
			}
		}

#if 0   // Not sure why this doesn't work to clear the Render Target.
		// Clear the back buffer, depth and stencil buffers.
		D3DRECT bounds[1];
		bounds[0].x1 = m_curBlitBounds->min_x;
		bounds[0].y1 = m_curBlitBounds->min_y;
		bounds[0].x2 = m_curBlitBounds->max_x;
		bounds[0].y2 = m_curBlitBounds->max_y;
		DWORD flags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL;

		//hr = m_d3dDevice->Clear(1, bounds, flags, 0x00000000, 1.0f, 0);
		hr = m_d3dDevice->Clear(0, NULL, flags, 0x00000000, 1.0f, 0);
		if (hr != D3D_OK)
		{
			DXTrace(__FILE__, __LINE__, hr, "Unable to clear device", true);
		}
#endif /* 0 */

		// Clear the back buffer with the background color. Note this obviates the need
		// for the Set(s) to do any screen clear.
		setBackground(m_backgroundColor);
	}

    // Reset the bounding rectangle.
    m_curBlitBounds->min_x = INT_MAX;
    m_curBlitBounds->min_y = INT_MAX;
    m_curBlitBounds->max_x = INT_MIN;
    m_curBlitBounds->max_y = INT_MIN;
}

int
DxStage::resizeEventCB(MleEvent event,void *callData,void *clientData)
{
    // Declare local variables.
	DxStage *theStage = (DxStage *)clientData;
    Win32CallData *eventData = (Win32CallData *)callData;

	// We do not want to resize if the DirectX device is being reset.
	if (theStage->isResetting())
		return 0;

    // Rebuild the off screen buffer.
    theStage->resize(0, 
                  0,
                  LOWORD(eventData->lParam),
                  HIWORD(eventData->lParam));

    return 0;
}

// Change size of the window managed by the stage.
void
DxStage::resize(int newX, int newY, int newWidth, int newHeight)
{
    // Take stage out of ready state.
    m_ready = FALSE;

	// Make sure something has changed.
#if 0
	if ((newX == m_x) && (newY == m_y) &&
		(newWidth == m_width) && (newHeight == m_height))
	{
		m_ready = TRUE;
		return;
	}
#endif /* 0 */
  
    // Store new values
    m_x = newX;
    m_y = newY;
    m_width = newWidth;
    m_height = newHeight;

    // Allocate new buffers.
	HRESULT hr;
	IDirect3DSurface9 *pixelBuffer;
	IDirect3DSurface9 *depthBuffer;
	hr = m_d3dDevice->CreateRenderTarget(
		m_width,                          // Width of render target surface.
		m_height,                         // Height of render target surface.
		D3DFMT_X8R8G8B8,                  // Format of render target surface.
		D3DMULTISAMPLE_NONE,              // Multisampling buffer type.
		0,                                // Multisampling quality level.
		true,                             // Is surface lockable.
		&pixelBuffer,                     // Returned surface.
		NULL                              // Reserved, must be NULL.
	);
	if (hr != D3D_OK)
	{
		DXTrace(__FILE__, __LINE__, hr, "CreateRenderTarget() failed during resize.", TRUE);
	}
	hr = m_d3dDevice->CreateDepthStencilSurface(
		m_width,                          // Width of render target surface.
		m_height,                         // Height of render target surface.
		D3DFMT_D24S8,                     // Format of depth-stencil surface.
		D3DMULTISAMPLE_NONE,              // Multisampling buffer type.
		0,                                // Multisampling quality level.
		true,                             // Discard.
		&depthBuffer,                     // Returned surface.
		NULL                              // Reserved, must be NULL.
	);
	if (hr != D3D_OK)
	{
		DXTrace(__FILE__, __LINE__, hr, "CreateDepthStencilSurface() failed during resize.", TRUE);
	}

    MLE_ASSERT(pixelBuffer != NULL);
    MLE_ASSERT(depthBuffer != NULL);
  
    // Remember camera aspect ratios.
    m_aspect = mlDiv(mlLongToScalar(m_width),mlLongToScalar(m_height));
  
    // Clear buffers.
	unsigned int i, j;
	D3DLOCKED_RECT lockedRect;
	hr = pixelBuffer->LockRect(&lockedRect, 0, D3DLOCK_DISCARD);
	DWORD *data = (DWORD *)lockedRect.pBits;
	for (i = 0; i < (unsigned int)m_height; i++)
	{
		for (j = 0; j < (unsigned int)m_width; j++)
		{
			int index = i * lockedRect.Pitch / 4 + j;
			data[index] = 0x00000000L;
		}
	}
	pixelBuffer->UnlockRect();

	// Don't need to clear the depth-stencil surface. It was created as
	// an un-lockable surface.
#if 0
	hr = m_depthBuffer->LockRect(&lockedRect, 0, D3DLOCK_DISCARD);
	data = (DWORD *)lockedRect.pBits;
	for (i = 0; i < (unsigned int)m_height; i++)
	{
		for (j = 0; j < (unsigned int)m_width; j++)
		{
			int index = i * lockedRect.Pitch / 4 + j;
			data[index] = 0xFFFFFFFFL;
		}
	}
	m_depthBuffer->UnlockRect();
#endif /* 0 */

	// Update the DirectX device.
	hr = m_d3dDevice->SetRenderTarget(0, pixelBuffer);
	if (hr != D3D_OK)
	{
		DXTrace(__FILE__, __LINE__, hr, "Unable to set render target", true);
	}
	hr = m_d3dDevice->SetDepthStencilSurface(depthBuffer);
		if (hr != D3D_OK)
	{
		DXTrace(__FILE__, __LINE__, hr, "Unable to set depth stencil surface", true);
	}
	m_d3dParameters.BackBufferWidth = m_width;
	m_d3dParameters.BackBufferHeight = m_height;
	ULONG count = pixelBuffer->Release();
	count = depthBuffer->Release();

    // Reblit whole buffer on next update.
    m_curBlitBounds->min_x = 0;
    m_curBlitBounds->min_y = 0;
    m_curBlitBounds->max_x = m_width;
    m_curBlitBounds->max_y = m_height;
  
    // The stage is now ready to go again.
    m_ready = TRUE;
}

int
DxStage::paintEventCB( MleEvent event, void *callData, void *clientData )
{
    // Declare local variables.
    DxStage       *stage     = (DxStage *) clientData;
    Win32CallData *eventData = (Win32CallData *) callData;
    long          lResult    = 0L;

    switch( eventData->uMsg )
    {
        case WM_PAINT:
        {
 	        PAINTSTRUCT stPaint;

            BeginPaint(eventData->hwnd, &stPaint);
	        stage->blit(stPaint.hdc);
  	        EndPaint(eventData->hwnd, &stPaint);

            break;
		}

        case WM_PALETTECHANGED:
			// Not managing palettes for now (may do so in future).
			break;

		case WM_QUERYNEWPALETTE:
		    // Not managing palettes for now (may do so in future).
			break;

		default:
			break;
	}

    return lResult;
}

void
DxStage::dirtyRegion(DxStageDirtyRect* newRegion)
{
    int min_x,min_y,max_x,max_y;
    int offset_x = 0;
    int offset_y = 0;

    min_x = newRegion->min_x + offset_x;
    if ( min_x < 0 )
        min_x = INT_MAX;

    min_y = newRegion->min_y + offset_y;
    if ( min_y < 0 )
        min_y = INT_MAX;

    max_x = newRegion->max_x + offset_x;
    if ( max_x < 0 )
        max_x = INT_MIN;

    max_y = newRegion->max_y + offset_y;
    if ( max_y < 0 )
        max_y = INT_MIN;

    if ( m_curBlitBounds->min_x > min_x ) 
        m_curBlitBounds->min_x = min_x;

    if ( m_curBlitBounds->min_y > min_y ) 
        m_curBlitBounds->min_y = min_y;

    if ( m_curBlitBounds->max_x < max_x ) 
        m_curBlitBounds->max_x = max_x;
    
    if ( m_curBlitBounds->max_y < max_y ) 
        m_curBlitBounds->max_y = max_y;
}

void
DxStage::initDirectX()
{
	HRESULT hr;;

	// Create the Direct 3D  object.
	m_d3dObj = Direct3DCreate9( D3D_SDK_VERSION );

	// Get the device capabilities.
    D3DCAPS9 d3dCapabilities;
	m_d3dObj->GetDeviceCaps(
		D3DADAPTER_DEFAULT,  // Primary display adapter.
		D3DDEVTYPE_HAL,      // Device type = hardware.
		&d3dCapabilities     // Structure containing device capabilties.
	);

	D3DDISPLAYMODE d3dDisplayMode;
	unsigned int modeCount = m_d3dObj->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8);
	for (unsigned int i = 0; i < modeCount; i++)
	{
		hr = m_d3dObj->EnumAdapterModes(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8, i, &d3dDisplayMode);
	}

	// Can we use hardware vertex processing?
	int vp = 0;
	if (d3dCapabilities.DevCaps &D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		// Yes, save in 'vp' the fact that hardware vertex processing
		// is supported.
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		// No, save in 'vp' the fact that we must use software
		// vertex processing.
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// Fill out parameters structure.
	ZeroMemory( &m_d3dParameters, sizeof(D3DPRESENT_PARAMETERS));
	m_d3dParameters.BackBufferWidth            = 0;
	m_d3dParameters.BackBufferHeight           = 0;
	m_d3dParameters.BackBufferFormat           = D3DFMT_X8R8G8B8;
	m_d3dParameters.BackBufferCount            = 1;
	m_d3dParameters.MultiSampleType            = D3DMULTISAMPLE_NONE;
	m_d3dParameters.MultiSampleQuality         = 0;
	m_d3dParameters.SwapEffect                 = D3DSWAPEFFECT_COPY;
	m_d3dParameters.hDeviceWindow              = m_window;
	m_d3dParameters.Windowed                   = true;
	m_d3dParameters.EnableAutoDepthStencil     = true;
	m_d3dParameters.AutoDepthStencilFormat     = D3DFMT_D24S8;
	m_d3dParameters.Flags                      = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	m_d3dParameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_d3dParameters.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Create the device interface.
	hr = m_d3dObj->CreateDevice(
		D3DADAPTER_DEFAULT,         // Primary display adapter.
		D3DDEVTYPE_HAL,             // Device type = hardware.
		m_window,                   // Window associated with device.
		vp,                         // Vertex processing type.
		&m_d3dParameters,           // Present parameters.
		&m_d3dDevice                // Returned created device
	);
	if (hr != D3D_OK)
	{
		DXTrace(__FILE__, __LINE__, hr, "CreateDevice() failed during DirectX initialization", TRUE);
	}
}

void
DxStage::setBackground(D3DCOLOR color)
{
	// Retrieve the back buffer to draw into.
	IDirect3DSurface9 *backBuffer;
	HRESULT hr = m_d3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
	if (hr != D3D_OK)
	{
		DXTrace(__FILE__, __LINE__, hr, "Unable to get back buffer", true);
		return;
	}

	// Get the surface description.
	D3DSURFACE_DESC bbDesc;
	hr = backBuffer->GetDesc(&bbDesc);
	if (hr != D3D_OK)
	{
		DXTrace(__FILE__, __LINE__, hr, "Unable to get surface description for back buffer", true);
		return;
	}

	// Get a pointer to the surface pixel data.
	D3DLOCKED_RECT lockedRect;
	backBuffer->LockRect(&lockedRect, 0, 0);
	DWORD *pBits = (DWORD *)lockedRect.pBits;

	// Iterate through each pixel in the surface and set it to the background color.
	for (unsigned int i = 0; i < bbDesc.Height; i++)
	{
		for (unsigned int j = 0; j < bbDesc.Width; j++)
		{
			int index = i * lockedRect.Pitch / 4 + j;
			pBits[index] = color;
		}
	}

	// Unlock the back buffer.
	backBuffer->UnlockRect();

	// And release it.
	ULONG count = backBuffer->Release();
}

static HMENU g_hMenu = NULL;			// Handle to store menu between toggles.
static long  g_savedWindowedWidth;		// Saved windowed width.
static long  g_savedWindowedHeight;		// Saved windowed height.
static RECT  g_windowRect;				// Saved Windows windowed rectangle.

MlBoolean 
DxStage::toggleFullScreen()
{
	HRESULT hr;

	// Disable rendering.
	m_ready = FALSE;
	m_resetting = TRUE;

	// Invoke callbacks so that registered objects can release DirectX resources.
	for (int i = 0; i < m_onResetRegistry->getSize(); i++)
	{
		DxStageOnResetCallback *entry = (DxStageOnResetCallback *)((*m_onResetRegistry)[i]);
		DXSTAGE_ONDEVICERESET_CALLBACK cb = entry->m_callback;
		(*cb)(RELEASE_RESOURCE, entry->m_calldata);
	}

	// Are we in windowed mode at the moment?
	if (m_isWindowed)
	{
		// Remember the Window position and size.
		GetWindowRect(m_window,&g_windowRect);

		// Set the fullscreen mode style, clear menu if attached.
		SetWindowLong(m_window, GWL_STYLE, WS_POPUP | WS_SYSMENU | WS_VISIBLE);
		if (g_hMenu == NULL)
		{
			g_hMenu = GetMenu(m_window);
			SetMenu(m_window, NULL);
		}
		
		// Save the windowed client dimensions.
		g_savedWindowedWidth = m_width;
		g_savedWindowedHeight = m_height;
		
		// Set the Presentation Parameters, specifically ...
		m_d3dParameters.Windowed = FALSE;
		m_d3dParameters.BackBufferWidth = m_desktop.m_width;
		//m_d3dParameters.BackBufferWidth = 0;
		m_d3dParameters.BackBufferHeight = m_desktop.m_height;
		//m_d3dParameters.BackBufferHeight = 0;
		m_d3dParameters.FullScreen_RefreshRateInHz = m_desktop.m_refreshRate;
		// Other params are inherited from m_d3dParameters.
		
		// Reset D3D device. Any device dependent objects
		// that aren't managed or in system memory have
		// to be released before we call Reset and recreated
		// after the reset, ditto for going back to windowed
		// mode. We also have to reset render states and matrices
		// after the call to Reset.
		hr = m_d3dDevice->Reset(&m_d3dParameters);
		if (FAILED(hr))
		{
			DXTrace(__FILE__, __LINE__, hr, "Failed to reset device", true);

			// Couldn't do the change, set things back.
			DWORD dwWinStyle = ((MleWin32PlatformData *) (g_theTitle->m_platformData))->m_winStyle;
			SetWindowLong(m_window, GWL_STYLE, dwWinStyle);
			if (g_hMenu != NULL)
			{
				SetMenu(m_window, g_hMenu);
				g_hMenu = NULL;
			}
			
			// Set the window position.
			SetWindowPos(m_window, HWND_NOTOPMOST, g_windowRect.left, g_windowRect.top,
				(g_windowRect.right - g_windowRect.left),
				(g_windowRect.bottom - g_windowRect.top), SWP_SHOWWINDOW);

			return false;
		}
		
		m_width = m_desktop.m_width;
		m_height = m_desktop.m_height;
		m_isWindowed = false;
	}
	else // We're in fullscreen mode at the moment.
	{
		// Set the windowed mode style, reset menu if needed.
		DWORD dwWinStyle = ((MleWin32PlatformData *) (g_theTitle->m_platformData))->m_winStyle;
		SetWindowLong(m_window, GWL_STYLE, dwWinStyle);
		if (g_hMenu != NULL)
		{
			SetMenu(m_window, g_hMenu);
			g_hMenu = NULL;
		}
		
		// Set the Presentation Parameters, specifically
		m_d3dParameters.Windowed = TRUE;
		m_d3dParameters.BackBufferWidth = g_savedWindowedWidth;
		m_d3dParameters.BackBufferHeight = g_savedWindowedHeight;
		m_d3dParameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // Should be 0
		// Other params are inherited from m_d3dParameters.
		
		// Reset D3D device.
		hr = m_d3dDevice->Reset(&m_d3dParameters);
		if (FAILED(hr))
		{
			DXTrace(__FILE__, __LINE__, hr, "Failed to reset device", true);
			return false;
		}
		
		// Set the window position.
		SetWindowPos(m_window, HWND_NOTOPMOST, g_windowRect.left, g_windowRect.top,
			(g_windowRect.right - g_windowRect.left),
			(g_windowRect.bottom - g_windowRect.top), SWP_SHOWWINDOW);
		
		m_width = g_savedWindowedWidth;
		m_height = g_savedWindowedHeight;
		m_isWindowed = true;
	}

	// Invoke callbacks so that registered objects can recreate DirectX resources.
	for (int i = 0; i < m_onResetRegistry->getSize(); i++)
	{
		DxStageOnResetCallback *entry = (DxStageOnResetCallback *)((*m_onResetRegistry)[i]);
		DXSTAGE_ONDEVICERESET_CALLBACK cb = entry->m_callback;
		(*cb)(CREATE_RESOURCE, entry->m_calldata);
	}

	// Enable rendering.
	m_ready = TRUE;
	m_resetting = FALSE;

	return true;
}

// Find an entry in the registry matching the specified callback.
static DxStageOnResetCallback *findResetCallback(
    MlePtrArray *registry, DXSTAGE_ONDEVICERESET_CALLBACK callback)
{
	DxStageOnResetCallback *found = NULL;
	for (int i = 0; i < registry->getSize(); i++)
	{
		DxStageOnResetCallback *entry = (DxStageOnResetCallback *)((*registry)[i]);
		DXSTAGE_ONDEVICERESET_CALLBACK cb = entry->m_callback;
		if (cb == callback)
		{
			found = entry;
			break;
		}
	}

	return found;
}

void
DxStage::addResetCallback(DXSTAGE_ONDEVICERESET_CALLBACK callback, void *calldata)
{
	// Check to see if callback has already been added.
	DxStageOnResetCallback *found = findResetCallback(m_onResetRegistry, callback);
	if (found == NULL)
	{
		// A new entry, so create one and add it to the list.
		DxStageOnResetCallback *entry = new DxStageOnResetCallback();
		entry->m_callback = callback;
		entry->m_calldata = calldata;
		m_onResetRegistry->add(entry);
	} else
	{
		// Replace call data.
		found->m_calldata = calldata;
	}
}

void
DxStage::removeResetCallback(DXSTAGE_ONDEVICERESET_CALLBACK callback)
{
	DxStageOnResetCallback *found = findResetCallback(m_onResetRegistry, callback);
	if (found != NULL)
	{
		// Found it, so remove it and delete the entry.
		m_onResetRegistry->remove(found);
		delete found;
	}
}

// Find an entry in the registry matching the specified callback.
static DxStageOnDeviceLostCallback *findDeviceLostCallback(
    MlePtrArray *registry, DXSTAGE_ONDEVICERESET_CALLBACK callback)
{
	DxStageOnDeviceLostCallback *found = NULL;
	for (int i = 0; i < registry->getSize(); i++)
	{
		DxStageOnDeviceLostCallback *entry = (DxStageOnDeviceLostCallback *)((*registry)[i]);
		DXSTAGE_ONDEVICELOST_CALLBACK cb = entry->m_callback;
		if (cb == callback)
		{
			found = entry;
			break;
		}
	}

	return found;
}

void
DxStage::addDeviceLostCallback(DXSTAGE_ONDEVICELOST_CALLBACK callback, void *calldata)
{
	// Check to see if callback has already been added.
	DxStageOnDeviceLostCallback *found = findDeviceLostCallback(m_onDeviceLostRegistry, callback);
	if (found == NULL)
	{
		// A new entry, so create one and add it to the list.
		DxStageOnDeviceLostCallback *entry = new DxStageOnDeviceLostCallback();
		entry->m_callback = callback;
		entry->m_calldata = calldata;
		m_onDeviceLostRegistry->add(entry);
	} else
	{
		// Replace call data.
		found->m_calldata = calldata;
	}
}

void
DxStage::removeDeviceLostCallback(DXSTAGE_ONDEVICELOST_CALLBACK callback)
{
	DxStageOnDeviceLostCallback *found = findDeviceLostCallback(m_onDeviceLostRegistry, callback);
	if (found != NULL)
	{
		// Found it, so remove it and delete the entry.
		m_onDeviceLostRegistry->remove(found);
		delete found;
	}
}

static void QueryDesktopConfig( MleDesktop *pDesktop )
{
    HDC hDC;
    
    if ( ! pDesktop )
      return;

    hDC = GetDC( NULL );
    pDesktop->m_bitsPerPixel = GetDeviceCaps( hDC, BITSPIXEL );
    ReleaseDC( NULL, hDC );

    pDesktop->m_width  = GetSystemMetrics( SM_CXSCREEN );
    pDesktop->m_height = GetSystemMetrics( SM_CYSCREEN );

    if ( pDesktop->m_bitsPerPixel == 8 )
    {
        pDesktop->m_palettized = TRUE;
        //pDesktop->m_colorsAvailable = MLE_PC_PALETTE_SIZE;
		pDesktop->m_colorsAvailable = 256; // XXX - Can we get this from the device caps?
    }
    else
        pDesktop->m_palettized = FALSE;

    pDesktop->m_refreshRate = 60; // XXX - Determine programitically.
}
