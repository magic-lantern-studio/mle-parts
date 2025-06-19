/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file brstage.h
 * @ingroup MleParts
 *
 * This file defines the BRender Stage.
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


#ifndef __MLE_BRSTAGE_H_
#define __MLE_BRSTAGE_H_

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleStage.h"
#include "mle/MleScheduler.h"

// Include Magic Lantern Math header files.
#include "math/scalarbr.h"
#include "math/vector.h"

// Include BRender target header files.
#include "mle/brendapi.h"

class MleSet;
struct br_pixelmap;


// Structure specifying section of screen that has been drawn into.
typedef struct  brwin_dirty_rect
{
    int min_x;
    int min_y;
    int max_x;
    int max_y;
} brwin_dirty_rect;


class BrStage : public MleStage
{
    MLE_STAGE_HEADER(BrStage);

  public:

    BrStage(void);
    
    // Stage-specific API.
    //   This is new API (not inherited) for this particular stage type.
    
    // Buffer access.

    br_pixelmap *getPixelBuffer(void) const { return m_pixelBuffer; }

    br_pixelmap *getDepthBuffer(void) const { return m_depthBuffer; }

    br_pixelmap *getFrontBuffer(void) const { return m_frontBuffer; }
    
    /**
     * Registers a new Set with this stage
     * and returns a pointer to the scheduled render item.
     */
    virtual MleSchedulerItem *addSet(void (*render)(MleSet *),MleSet *set);
    
#ifdef _WINDOWS

    // Marks a region of the screen that needs to be blitted at frame end.
    void dirtyRegion(brwin_dirty_rect* newRegion);

#endif /* _WINDOWS */

    // Beginning of properties.

    // "size" property contains width and height in pixels.
    MlVector2 size;

  protected:

    br_pixelmap *m_pixelBuffer;
    br_pixelmap *m_frontBuffer;
    br_pixelmap *m_depthBuffer;
    
#ifdef _WINDOWS

    MlScalar m_aspect;
    brwin_dirty_rect *m_curBlitBounds;

#endif
};

#endif /* __MLE_BRSTAGE_H_ */
