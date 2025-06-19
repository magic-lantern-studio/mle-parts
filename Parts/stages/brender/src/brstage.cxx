/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file brstage.cxx
 * @ingroup MleParts
 *
 * This file implements the BRender Stage.
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
#include <stdio.h>
#include <limits.h>

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleDirector.h"
#include "mle/MleScheduler.h"

// Include BRender target header files.
#include "mle/brstage.h"


MLE_STAGE_SOURCE(BrStage,MleStage);


#ifdef MLE_REHEARSAL
void
BrStage::initClass(void)
{
    mlRegisterStageClass(BrStage, MleStage);
    mlRegisterStageMember(BrStage, size, MlVector2);
}
#endif /* MLE_REHEARSAL */

BrStage::BrStage(void)
{
	m_pixelBuffer = NULL;
	m_frontBuffer = NULL;
	m_depthBuffer = NULL;
#ifdef _WINDOWS
	m_aspect = ML_SCALAR_ONE;
	m_curBlitBounds = NULL;
#endif /* _WINDOWS */
}

MleSchedulerItem *
BrStage::addSet(void (*render)(MleSet *),MleSet *set)
{
    // Declare local variables.
    MleSchedulerItem *item;

    // Just pass this function on to the scheduler
    item = g_theTitle->m_theScheduler->insertFunc(
        PHASE_SET,(MleSchedulerFunc)render,set,this);

    return item;
}

#ifdef _WINDOWS
void
BrStage::dirtyRegion(brwin_dirty_rect* newRegion)
{
    int min_x,min_y,max_x,max_y;
    int offset_x = 0;
    int offset_y = 0;

    if ( m_pixelBuffer )
    {
        offset_x = m_pixelBuffer->origin_x;
        offset_y = m_pixelBuffer->origin_y;
    }

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
#endif /* _WINDOWS */
