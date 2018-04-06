/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file pcstage.h
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 * @date Jan 24, 2006
 *
 * This file defines a BRender Stage for the Windows platform.
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

#ifndef __MLE_PCSTAGE_H_
#define __MLE_PCSTAGE_H_


// Include system header files.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

// Include Magic Lantern header files.
#include "mle/MleEventDispatcher.h"

// Include PC target header files.
#include "mle/brstage.h"
#include "mle/comstg.h"
#include "mle/win32pal.h"


typedef MleWin32Palette MlPaletteType;   // Win32 application GDI palette.

// This declaration is only temporary
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


class PCBRenderStage : public BrStage
{
    MLE_STAGE_HEADER(PCBRenderStage);
    
    // Declare member functions.

  public:

    PCBRenderStage(void);

    virtual ~PCBRenderStage(void);
    
    // This method installs the event and scheduler callbacks.
    //
    virtual void init(void);
    
    // This method registers a new Set with this stage.
    virtual MleSchedulerItem *addSet(void (*render)(MleSet *),MleSet *set);
    
    // This method is an accessor function.
    virtual void getSize(int *width,int *height);

    // This method is used by screenblit() and the WM_PAINT message.
    // callback.
    void blit(HDC newDc);
    
    // This method changes the size of the window managed by the stage.
    // It is used by the WM_SIZE callback.
    void resize(int x, int y, int width, int height);
    
    // This method screen blits and empties current dirty region.
    // It is a callback to be used during the PHASE_STAGE phase
    // of the execution cycle.
    static void screenblit(void *stage);
    
    // This callback is used to capture the WM_SIZE message. It is called
    // via the event dispatch manager.
    //
    static int resizeEventCB(MleEvent event,void *callData,void *clientData);

    // This callback is used to capture the WM_SIZE, WM_QUERYNEWPALETTE and
    // WM_PALETTECHANGED messages. It is called via the event dispatch manager.
    // 
    //
    static int paintEventCB(MleEvent event,void *callData,void *clientData);

    // This method is used to get the off-screen buffer used by
    // the stage for rendering
    inline char *getOffScreenBuffer(void)
    { return (char *) m_blt.m_bits; }

    // This method is an accessor function.
    inline const MleDesktop *getDesktop(void)
    { return &m_desktop; }

    // Initialize the palette.
    MlBoolean initPalette(PALETTEENTRY *palEnts);
    
    // Set a new palette.
    MlPaletteType *setPalette(MlPaletteType *newPalette);
    
    // Update the current palette in the window dc
    MlBoolean updatePalette(void);

   // declare member variables.

    HWND m_window;               // Handle to output window

    HDC m_hdcWindow;             // Handle to a DC for the window

    int m_showMode;              // Indication of how to show window
                                 // via the ShowWindow() function

    MlPaletteType *m_palette;

  private:
    /*
     * The  desktop structure contains information relating to the
     * spatial, tonal and temporal resolutions of the presentation
     * surface.
     */
    MleDesktop m_desktop;
    HPALETTE   m_hPalette;

    int m_x, m_y;              // Position in window of image.
    int m_width, m_height;     // Size of stage.

    MlBoolean m_ready;         // TRUE when buffer is ready for blit.

    // Current off screen buffer.
    MleDIBSectionBlit m_blt;      // Bitblt helper class instance.
};


//
// Macros for accessing stage which is stored with window instance.
//
#define MLE_INST_STAGE_SET(hwnd,stage) SetWindowLong((hwnd),0,(LONG)(stage))
#define MLE_INST_STAGE_GET(hwnd) ((PCBRenderStage*)GetWindowLong((hwnd),0))

#if !defined(_GDK_)
//
// Macro for retrieving the window DC stored in the Win32 PC BRender stage.
//
#define MLE_GET_WINDOW_DC() ((PCBRenderStage *)MleStage::g_theStage)->m_hdcWindow

//
// Macros for event dispatching priorities. Note that this ordering is
// important. The Stage resize callback must be executed prior to the
// Set resize callback because the Set uses the new off-screen buffer
// which is reallocated by the Stage resize callback. Also, the Stage
// resize paint callback is executed after all other resize events have
// been processed. The resize paint callback redraws the world in the new
// off-screen buffer.
//
#define MLE_RESIZE_STAGE_PRIORITY       10
#define MLE_RESIZE_SET_PRIORITY          9
#define MLE_RESIZEPAINT_STAGE_PRIORITY  -1

#endif /* !_GDK_ */


#endif /* __MLE_PCSTAGE_H_  */
