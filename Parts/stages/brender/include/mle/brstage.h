/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file brstage.h
 * @ingroup MleParts
 *
 * This file defines the BRender Stage.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
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
    
#ifdef WIN32

    // Marks a region of the screen that needs to be blitted at frame end.
    void dirtyRegion(brwin_dirty_rect* newRegion);

#endif /* WIN32 */

    // Beginning of properties.

    // "size" property contains width and height in pixels.
    MlVector2 size;

  protected:

    br_pixelmap *m_pixelBuffer;
    br_pixelmap *m_frontBuffer;
    br_pixelmap *m_depthBuffer;
    
#ifdef WIN32

	MlScalar m_aspect;
	brwin_dirty_rect *m_curBlitBounds;

#endif
};

#endif /* __MLE_BRSTAGE_H_ */
