/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file comstg.h
 * @ingroup MlParts
 *
 * This file defines common class utilities for Stages targeting the
 * Windows platform.
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
