/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file comstg.h
 * @ingroup MlParts
 *
 * @author Mark S. Millard
 * @date Jan 24, 2006
 *
 * This file defines common class utilities for Stages targeting the
 * Windows platform.
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

#ifndef __MLE_COMSTG_H_
#define __MLE_COMSTG_H_

// Include system header files.
#include <windows.h>
#include <windowsx.h>

// Include Magic Lantern header files.
#include "mle/mlTypes.h"


//
// Blit helper class
//

// Abstract Base Class


class MleWin32Blit
{
    // Declare member variables.

  public:

    void *m_bits;
    
  protected:

    int m_width;
    int m_height;
    
    // Declare member functions.

  public:

    MleWin32Blit() {};

    virtual ~MleWin32Blit() {};
    
    virtual MlBoolean allocate(HDC screen, HPALETTE hPalette,
        int width, int height) = 0;

    virtual void deallocate() = 0;

    virtual void blit(HDC screen, int dest_x, int dest_y, 
	    int src_x, int src_y, int width, int height) = 0;
};


//
// This class is used to blit a DIBSection.
//


class MleDIBSectionBlit : public MleWin32Blit
{
    // Declare member variables.

  protected:

    HBITMAP m_bitmap;

    static MlBoolean g_initState;
    static FARPROC g_CreateDIBSection_P;
    
    // Declare member functions.

  public:

    MleDIBSectionBlit();

    virtual ~MleDIBSectionBlit();
    
    MlBoolean allocate(HDC screen, HPALETTE hPalette,
        int width, int height);

    void deallocate();

    void blit(HDC screen, int dest_x, int dest_y, 
	    int src_x, int src_y, int width, int height);

#ifndef _GDK_
    void mapColorPalette(HDC screen,HPALETTE palette);
#endif /* _GDK_ */
};

//
// Constants for palette depth.
//
#define MLE_PALETTE_SIZE    256
#define MLE_BIT_COUNT       8

#endif /* __MLE_COMSTG_H_ */
