/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file dx2dset.h
 * @ingroup MleParts
 *
 * This file contains the class for a 2D Set targeting the DirectX
 * platform.
 *
 * @author Mark S. Millard
 * @date Nov 13, 2007
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

#ifndef __MLE_DX2DSET_H_
#define __MLE_DX2DSET_H_

// Include system header files.
#include "d3d9.h"

// Include Magic Lantern Math header files.
#include "math/scalar.h"
#include "math/vector.h"

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleSet.h"
#include "mle/MleScheduler.h"
#include "mle/MleEventDispatcher.h"

// Include Magic Lantern DirectX Stage header files.
#include "dxs/dxstage.h"
#include "dxs/DxsSurface.h"

class MleRole;
class Mle2dRole;


// Type used for viewBoundsCallback routines.
typedef void (*Mle2dSetViewBoundsCallback)(Mle2dRole *);


/**
 * @brief This class defines the DirectX 2D Set.
 */
class Mle2dSet: public MleSet 
{
    MLE_SET_HEADER(Mle2dSet);

  public:

	/**
	 * The default constructor.
	 */
	Mle2dSet(void);

	/**
	 * The desctructor.
	 */
    virtual ~Mle2dSet(void);

	/**
	 * Initialize the Set.
	 */
    virtual void init(void);

    // Attach and detach a Role to the 2dSet's scene graph (currently
    // implemented as a linked-list).

    /**
	 * Attach a Role to the Set.  If <i>curRole</i> is <b>NULL<b>, then
	 * <i>newRole</i> is attached to the end of the list. Otherwise it will
	 * be attached following the <i>curRole</i>.
	 *
	 * @param curRole The role to attach the new one too.
	 * @param newRole The role being attached to the Set.
	 */
    virtual void attach(MleRole *curRole, MleRole *newRole);

    /**
	 * Detach the specified Role from the Set.
	 *
	 * @param curRole The Role to detach from the Set.
     */
    virtual void detach(MleRole *curRole);

    /**
	 * This method is used to traverse the scene graph and render each
     * 2D role.
	 *
	 * @param set The Set to render.
	 *
	 * @see attach()
     */
    static void render(MleSet *set);

    /**
	 * This method is used to set the scheduler id for the render method
     * (so that it can be removed during destruction).
	 *
	 * @param id The scheduler id for the render method.
     */
    inline void setRenderCBId(MleSchedulerItem *id)
    {
        m_renderCBId = id;
    }

	/**
	 * This routine is called when the stage is preparing to reset
	 * the DirectX device. It is used to release the offscreen buffer.
	 *
	 * @param ev The event causing the callback to be invoked.
	 * @param calldata A pointer to the Set that was registerd to be
	 * notified that the DirectX device is being reset.
	 */
	static void onReset(unsigned int ev, void *calldata);

    /**
	 * This method is used to set the viewBoundsCallback member variable.
     * The viewBoundsCallback is a function which will be called for each
     * Role in the 2dSet's scene graph as the scene graph is
     * rendered.
     */
    inline Mle2dSetViewBoundsCallback setViewBoundsCallback(
        Mle2dSetViewBoundsCallback callback)
    {
	    Mle2dSetViewBoundsCallback old_cb = m_viewBoundsCallback;
	    m_viewBoundsCallback = callback;
	    return(old_cb);
    };

    /**
	 * This method is used to get the width of the Set.
	 *
	 * @param The width, in pixels, is returned.
     */
    inline unsigned int getWidth(void)
    {
		return m_offscreenSurface.getWidth();
    }

    /**
	 * This method is used to get the height of the Set.
	 *
	 * @param The height, in pixels, is returned.
     */
    inline unsigned int getHeight(void)
    {
		return m_offscreenSurface.getHeight();
    }

    /**
	 * This method is used to get the DirectX surface used
	 * to render the Set.
     */
    inline IDirect3DSurface9 *getOffscreenBuffer(void)
    {
	    return m_offscreenSurface.getD3DSurface();
    }

    // This method is used to get the address of the DIB Palette object.
    //
    //inline MlPaletteType *getPalette(void) { return(&m_pal); }

	/**
	 * This method changes the size of the offscreen buffer.
	 *
	 * @param width The new width of the buffer.
	 * @param height The new height of the buffer.
	 */
    MlBoolean resize(int width, int height);

    /**
	 * This method is used to resize the Set.
	 *
	 * @param event The Magic Lantern event.
	 * @param callData Data associated with the event.
	 * @param clienData Data associated with the callback.
	 *
	 * @return If the callback is succussful, then 1 will be returned.
	 * Otherwise, 0 will be returned.
     */
    static int resizeEventCB(MleEvent event,void *callData,void *clientData);

  // The properties exposed in the DWP/DPP.

    /**
	 * The position of the Set.  It contains x and y position in screen coordinates,
     * where (0, 0) corresponds to the lower left corner of the window.
	 */
    MlVector2 position;

    /**
	 * The size of the Set. It contains width and height in screen coordinates.
	 */
    MlVector2 size;

  protected:

    /**
     * The headRole is the first Role drawn.  This makes the
     * headRole always appear in the bottom layer among all the
     * Roles attached to this Set.  Similiarly, the last Role
     * in this list always appear in the top layer.
     */
    Mle2dRole *m_headRole;

	/** The callback used to calculate the boundary of the view. */
    Mle2dSetViewBoundsCallback m_viewBoundsCallback;

    // All management of screen clears and dirty rectangles is being
    // done in the Set (probably will change).

    #define MAX_DIRTY_RECTANGLES 16

    friend void ViewBoundsCallback(Mle2dRole *role);

    /** Table of dirty rectangles. */
    DxStageDirtyRect m_dirtyRectangles[MAX_DIRTY_RECTANGLES];
	/** The last dirty rectangle in the table. */
    int	m_lastDirtyRectangle;

    /** Union of rectangles that were drawn during this frame. */
    DxStageDirtyRect m_renderBounds;

    /** Union of rectangles that were cleared during this frame. */
    DxStageDirtyRect m_clearBounds;
	/** The Set's offscreen buffer to render to. */
	DxsSurface m_offscreenSurface;

  private:

    MleSchedulerItem *m_renderCBId;	// Render callback id (scheduler).
    MleCallbackId m_resizeCBId;		// Resize callback id (event dispatcher).
};


#endif	/* __MLE_DX2DSET_H_ */
