/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file pcstage.cxx
 * @ingroup MlParts
 *
 * This file implements a BRender Stage for the Windows platform.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2000-2025 Wizzer Works
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

// Include system header files.
#include <limits.h>
#include <stdio.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

// Include Magic Lantern Runtime Engine header file.
#include "mle/MleScheduler.h"
#include "mle/MleDirector.h"

// Include PC BRender target header files.
#include "mle/pcstage.h"
#include "mle/mlPlatformData.h"


// Private functions used by the stage
static void QueryDesktopConfig( MleDesktop *pDesktop );


MLE_STAGE_SOURCE(PCBRenderStage,BrStage);


// The PCBRenderStage constructor creates window for the stage to use.
// Some of this functionality will ultimately be moved out.  For example 
// the application palette choice will be more user driven.  


PCBRenderStage::PCBRenderStage(void)
{
    RECT  stRect;
    DWORD dwWinStyle;

    m_ready = FALSE;

    // Query the desktop to determine the screen's configuration
    QueryDesktopConfig( &m_desktop );

    // Create the dirty bounding rectangle.
    m_curBlitBounds = (brwin_dirty_rect*) mlMalloc( sizeof(brwin_dirty_rect) );

    // Initialize the bounding rectangle.
    m_curBlitBounds->min_x = INT_MAX;
    m_curBlitBounds->min_y = INT_MAX;
    m_curBlitBounds->max_x = INT_MIN;
    m_curBlitBounds->max_y = INT_MIN;

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
 
    /*
     * Calculate the windows rectangle needed to contain the client area
     * plus any overhead to support the window frame.
     */
   
    stRect.left = 0;
    stRect.top  = 0;
    stRect.right  = m_width;
    stRect.bottom = m_height;
    
    AdjustWindowRect( &stRect, dwWinStyle, FALSE );

    // Initialize the renderer.  
    BrBegin();

    // XXX - configure for 8 bit indexed palette mode
    BrZbBegin(BR_PMT_INDEX_8,BR_PMT_DEPTH_16);
    
    // Create the window - pass stage pointer through lpvParam for callbacks
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
    // application.  Window class has CS_OWNDC set
    m_hdcWindow = GetDC(m_window);
    MLE_ASSERT(m_hdcWindow != NULL);

    m_palette  = NULL;
    m_hPalette = NULL;

    // Default windows show mode, may be changed prior to init() call.
    m_showMode = SW_SHOW;
}


PCBRenderStage::~PCBRenderStage(void)
{
    BrZbEnd();
    BrEnd();

    // Destroy window if it is still attached.
    if (m_window) 
    {
        // Detach view.
        MLE_INST_STAGE_SET(m_window,NULL);
        DestroyWindow(m_window);
    }

    if (m_palette != NULL)
	{
        delete m_palette;
        m_palette = NULL;
    }

    if (m_pixelBuffer != NULL)
	{
        BrPixelmapFree(m_pixelBuffer);
        m_pixelBuffer = NULL;
    }

    if (m_depthBuffer != NULL)
	{
        BrPixelmapFree(m_depthBuffer);           
        m_depthBuffer = NULL;
    }

    if (m_curBlitBounds)
        mlFree(m_curBlitBounds);

    if ( m_hPalette )
    {
        DeleteObject( m_hPalette );
	    m_hPalette = NULL;
    }
}


void
PCBRenderStage::init()
{
    // Declare local variables
    MleCallbackId cbId;

    // Insert resize callback into event dispatch manager
    cbId = g_theTitle->m_theEventMgr->installEventCB(
        WM_SIZE,PCBRenderStage::resizeEventCB,(void *)this);

    // Bump priority of dispatched callback.
    g_theTitle->m_theEventMgr->changeEventCBPriority(WM_SIZE,cbId,
        MLE_RESIZE_STAGE_PRIORITY);

    // Insert palette callback into event dispatch manager.
    cbId = g_theTitle->m_theEventMgr->installEventCB(
        WM_PALETTECHANGED,PCBRenderStage::paintEventCB,(void *)this);

    // Insert palette callback into event dispatch manager.
    cbId = g_theTitle->m_theEventMgr->installEventCB(
        WM_QUERYNEWPALETTE,PCBRenderStage::paintEventCB,(void *)this);

    // Insert stage into the scheduler.
    g_theTitle->m_theScheduler->insertFunc(
        PHASE_STAGE,PCBRenderStage::screenblit,this,this);

    //if ( m_desktop.m_palettized )
      initPalette( NULL );

    // Make the window appear.
    ShowWindow( m_window, m_showMode );

    // Call resize() to explicitly create the off screen buffer and pixelmaps.
    //   We use to rely on the WM_SIZE message to do this; however, we weren\'t
    //   garanteed that the message would be processed before we needed it in
    //   the rendering/set phase of the execution cycle (an asynchronous race
    //   could occur).
    resize( 0, 0, m_width, m_height );
}


MleSchedulerItem *
PCBRenderStage::addSet(void (*render)(MleSet *),MleSet *set)
{
    // Declare local variables.
    MleSchedulerItem *item;

    // Add the function to the scheduler.
    item = g_theTitle->m_theScheduler->insertFunc(
        PHASE_SET,(MleSchedulerFunc)render,set,this);

    return item;
}


void
PCBRenderStage::getSize(int *w,int *h)
{
    *w = m_width;
    *h = m_height;
}


void
PCBRenderStage::screenblit(void *stage)
{
    PCBRenderStage *aStage = (PCBRenderStage *)stage;
    aStage->blit(aStage->m_hdcWindow);
}


// Use system to copy the pixels from the local buffer to the frame buffer.
void
PCBRenderStage::blit(HDC newDc)
{
    // Check if the local screen related data structures are ready.
    if (m_ready != TRUE)
        return;
  
    // Blit the apropriate region to the screen.
    if (m_curBlitBounds->min_x < m_curBlitBounds->max_x)
    {
        m_blt.blit(newDc,
            m_x + m_curBlitBounds->min_x, m_y + m_curBlitBounds->min_y,
            m_curBlitBounds->min_x, m_curBlitBounds->min_y,
            m_curBlitBounds->max_x + 1 - m_curBlitBounds->min_x,    // width
            m_curBlitBounds->max_y + 1 - m_curBlitBounds->min_y);   // height
    }

    // Clear the screen.  Note this obviates the need for the 3D forum
    // to do any screen clear.
    BrPixelmapRectangleFill(m_pixelBuffer,
        (br_int_16)m_curBlitBounds->min_x - m_pixelBuffer->origin_x,
        (br_int_16)m_curBlitBounds->min_y - m_pixelBuffer->origin_y,
        (br_int_16)(m_curBlitBounds->max_x - m_curBlitBounds->min_x),
        (br_int_16)(m_curBlitBounds->max_y - m_curBlitBounds->min_y),
        0);
      
    BrPixelmapRectangleFill(m_depthBuffer,
        (br_int_16)m_curBlitBounds->min_x - m_pixelBuffer->origin_x,
        (br_int_16)m_curBlitBounds->min_y - m_pixelBuffer->origin_y,
        (br_int_16)(m_curBlitBounds->max_x - m_curBlitBounds->min_x),
        (br_int_16)(m_curBlitBounds->max_y - m_curBlitBounds->min_y),
        0xFFFFFFFF);


    // Reset the bounding rectangle.
    m_curBlitBounds->min_x = INT_MAX;
    m_curBlitBounds->min_y = INT_MAX;
    m_curBlitBounds->max_x = INT_MIN;
    m_curBlitBounds->max_y = INT_MIN;
}


int
PCBRenderStage::resizeEventCB(MleEvent event,void *callData,void *clientData)
{
    // Declare local variables.
    PCBRenderStage *stage = (PCBRenderStage *)clientData;
    Win32CallData *eventData = (Win32CallData *)callData;

    // Rebuild the off screen buffer.
    stage->resize(0, 
                  0,
                  LOWORD(eventData->lParam),
                  HIWORD(eventData->lParam));

    return 0;
}

// Change size of the window managed by the stage.
void
PCBRenderStage::resize(int newX, int newY, int newWidth, int newHeight)
{
    // Take stage out of ready state.
    m_ready = FALSE;
  
    // Store new values
    m_x = newX;
    m_y = newY;
    m_width = newWidth;
    m_height = newHeight;
  
    // Reallocate the screen buffer structure.
    m_blt.deallocate();

    // When no buffer, leave us in a non-ready state.
    if (m_blt.allocate(m_hdcWindow, m_palette->getPaletteHandle(), m_width, m_height) ==
        FALSE) 
        return;
  
    // Release old BRender buffers.
    if (m_pixelBuffer != NULL)
        BrPixelmapFree(m_pixelBuffer);

    if (m_depthBuffer != NULL)
        BrPixelmapFree(m_depthBuffer);           

    // Allocate new buffers .
    m_pixelBuffer = BrPixelmapAllocate(BR_PMT_INDEX_8,
        (br_uint_16)m_width,(br_uint_16)m_height,
        m_blt.m_bits,BR_PMAF_INVERTED);
    m_pixelBuffer->origin_x = m_pixelBuffer->width/2;
    m_pixelBuffer->origin_y = m_pixelBuffer->height/2;

    m_depthBuffer = BrPixelmapMatch(m_pixelBuffer, BR_PMMATCH_DEPTH_16);
    m_depthBuffer->origin_x = m_depthBuffer->width/2;
    m_depthBuffer->origin_y = m_depthBuffer->height/2;
  
    MLE_ASSERT(m_pixelBuffer != NULL);
    MLE_ASSERT(m_depthBuffer != NULL);
  
    // Remember camera aspect ratios.
    m_aspect = mlDiv(mlLongToScalar(m_width),mlLongToScalar(m_height));
  
    // Clear buffers.
    BrPixelmapFill(m_pixelBuffer,0); 
    BrPixelmapFill(m_depthBuffer,0xFFFF);
  
    // Reblit whole buffer on next update.
    m_curBlitBounds->min_x = 0;
    m_curBlitBounds->min_y = 0;
    m_curBlitBounds->max_x = m_pixelBuffer->width;
    m_curBlitBounds->max_y = m_pixelBuffer->height;
  
    // The stage is now ready to go again.
    m_ready = TRUE;
}


int PCBRenderStage::paintEventCB( MleEvent event,
                                    void   *callData,
                                    void   *clientData )
{
    // Declare local variables.
    PCBRenderStage *stage     = (PCBRenderStage *) clientData;
    Win32CallData  *eventData = (Win32CallData *) callData;
    long           lResult   = 0L;

    if ( stage->m_palette )
        stage->m_hPalette = stage->m_palette->getPaletteHandle();

    switch( eventData->uMsg )
    {
        case WM_PAINT:
        {
 	        PAINTSTRUCT stPaint;

            BeginPaint( eventData->hwnd, &stPaint );
            if ( stage->m_hPalette )
            {
                SelectPalette( stPaint.hdc, stage->m_hPalette, FALSE );
                RealizePalette( stPaint.hdc );
			}

	        stage->blit( stPaint.hdc );
  	        EndPaint( eventData->hwnd, &stPaint );

            break;
		}

        case WM_PALETTECHANGED:
	        if ( !stage->m_hPalette )
	            break;

			  if ( eventData->hwnd == (HWND) eventData->wParam )
				  break;

			  UnrealizeObject( stage->m_hPalette );
			  InvalidateRect( eventData->hwnd, NULL, FALSE );
			  break;

		  case WM_QUERYNEWPALETTE:
		  {
			  HDC hDC = MLE_GET_WINDOW_DC();

			  if( !stage->m_hPalette )
				  break;

			  UnrealizeObject( stage->m_hPalette );
			  SelectPalette( hDC, stage->m_hPalette, FALSE );
			  RealizePalette( hDC );
			  InvalidateRect( eventData->hwnd, NULL, FALSE );
			  lResult = 1L;
			  break;
		  }

		  default:
			  break;
      }

    return lResult;
  }


MlBoolean PCBRenderStage::initPalette(PALETTEENTRY *palEntries)
{
    static MlBoolean paletteInitalized = FALSE;

    if (paletteInitalized == FALSE)
    {
        m_palette = NULL;
        paletteInitalized = TRUE;
    }

    // Declare local variables.
    MleWin32Palette *oldPalette = m_palette;	// Save old palette.

    // Clear the system palette.
    MlePCPalette::clearSystemPalette();

    m_palette = new MleWin32Palette(palEntries);
    MLE_ASSERT(m_palette);

    if (m_hdcWindow)
	{
        // Set and realize the palette in the screen dc.
        m_palette->setSystemPalette();

        // Set and realize the palette in the window dc.
        updatePalette();
    }

    // Delete the old palette, if there was one.
    if (oldPalette)
        delete oldPalette;

    return TRUE;
}


// Set a new palette.  Returns previous palette if successful.
MleWin32Palette *PCBRenderStage::setPalette(MleWin32Palette *newPalette)
{
    MleWin32Palette *oldPalette = m_palette;

    if (newPalette == NULL)
        return NULL;

    // Set this->palette data member, and call its set function.
    m_palette = newPalette;

    // Set and realize the palette in the screen dc.
    m_palette->setSystemPalette();

    // Set and realize the palette in the window dc.
    updatePalette();

    // flush the GDI
    GdiFlush();

    // Patch DIB section color table.
    m_blt.mapColorPalette(m_hdcWindow,newPalette->getPaletteHandle());

    // Return the previous palette.
    return oldPalette;
}


MlBoolean PCBRenderStage::updatePalette()
{
    HDC windowDC = MLE_GET_WINDOW_DC();
    UINT numMapped;

    // Select and realize the palette in the window dc.
    if (SelectPalette(windowDC, m_palette->getPaletteHandle(), FALSE) == NULL)
        return FALSE;
    numMapped = RealizePalette(windowDC);
    if (numMapped == GDI_ERROR)
        return FALSE;

    return TRUE;
}

static void QueryDesktopConfig( MleDesktop *pDesktop )
{
    HDC hDC;
    
    if( !pDesktop )
      return;

    hDC = GetDC( NULL );
    pDesktop->m_bitsPerPixel = GetDeviceCaps( hDC, BITSPIXEL );
    ReleaseDC( NULL, hDC );

    pDesktop->m_width       = GetSystemMetrics( SM_CXSCREEN );
    pDesktop->m_height      = GetSystemMetrics( SM_CYSCREEN );

    if( pDesktop->m_bitsPerPixel == 8 )
    {
        pDesktop->m_palettized = TRUE;
        pDesktop->m_colorsAvailable = MLE_PC_PALETTE_SIZE;
    }
    else
      pDesktop->m_palettized = FALSE;

    pDesktop->m_refreshRate = 0; // XXX - unimplemented
}
