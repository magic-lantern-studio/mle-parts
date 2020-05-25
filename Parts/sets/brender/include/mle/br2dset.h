/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file br2dset.h
 * @ingroup MleParts
 *
 * This file contains the class for a 2D Set targeting the BRender
 * platform.
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

#ifndef __MLE_BR2DSET_H_
#define __MLE_BR2DSET_H_

// Include Magic Lantern Math header files.
#include "math/scalar.h"
#include "math/vector.h"

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleSet.h"
#include "mle/MleScheduler.h"
#ifdef WIN32
#include "mle/MleEventDispatcher.h"
#endif /* WIN32 */

// Include Magic Lantern Extension header files.
#ifdef WIN32
#include "mle/pcstage.h"
#include "mle/dib.h"
#endif /* WIN32 */


class MleRole;
class Mle2dRole;


#ifdef WIN32
// Type used for viewBoundsCallback routines.
typedef void (*Mle2dSetViewBoundsCallback)(Mle2dRole *);
#endif /* WIN32 */


/**
 * @brief This class defines the BRender 2D Set.
 */
class Mle2dSet: public MleSet 
{
    MLE_SET_HEADER(Mle2dSet);

  public:

	Mle2dSet(void);

    virtual ~Mle2dSet(void);

    virtual void init(void);

    // Attach and detach a Role to the 2dSet's scene graph (currently
    // implemented as a linked-list).

    // attach() - Attach newRole after curRole.  If curRole is NULL,
	// newRole is attached to the end of the list.
    virtual void attach(MleRole *curRole, MleRole *newRole);

    // detach() - Detach curRole in the list.
    //
    virtual void detach(MleRole *curRole);

    // This method is used to traverse the scene graph and render each
    // 2D delegate.
    //
    static void render(MleSet *set);

#ifdef MLE_REHEARSAL

    // This method is used to query the forum geometry.
    //
    virtual void	getBounds(MlScalar min[3], MlScalar max[3]);
    
    // This method is used for picking in tools.
    //
    virtual MleRole *pickDelegate(int x, int y);

    // Rearranging the actors drawing order with this Set.
    // pushActor() - Pushes the actor one layer down.  If success, 0 is
    //		     returned; otherwise, -1 is returned.  If the actor is
    //		     already at the bottom, no action is taken and -1 is
    //		     returned.
    // pushActorToBottom() - Same as pushActor() except the actor is pushed
    //			     to the bottom layer.  If success, 0 is returned;
    //			     otherwise, -1 is returned.  If the actor is
    //			     already at the bottom, no action is taken and -1
    //			     is returned.
    // popActor() - Pops the actor one layer up.  If success, 0 is returned;
    //		    otherwise, -1 is returned.  If the actor is already at
    //		    the top, no action is taken and -1 is returned.
    // popActorToTop() - Same as popActor() except the actor is popped to the
    //			 top layer.  If success, 0 is returned; otherwise, -1
    //			 is returned.  If the actor is already at the top, no
    //			 action is taken and -1 is returned.

    virtual int		pushActor(MleActor *actor);

    virtual int		pushActorToBottom(MleActor *actor);
 
    virtual int		popActor(MleActor *actor);

    virtual int		popActorToTop(MleActor *actor);

#endif	/* MLE_REHEARSAL */

#ifdef WIN32

    // This method is used to set the scheduler id for the render method
    // (so that we can remove it during destruction).
    //
    inline void setRenderCBId(MleSchedulerItem *id)
    {
        m_renderCBId = id;
    }

    // This method is used to set the viewBoundsCallback member variable.
    // The viewBoundsCallback is a function which will be called for each
    // delegate in the 2dSet's scene graph as the scene graph is
    // rendered.
    //
    inline Mle2dSetViewBoundsCallback setViewBoundsCallback(
      Mle2dSetViewBoundsCallback callback)
    {
	    Mle2dSetViewBoundsCallback old_cb = m_viewBoundsCallback;
	    m_viewBoundsCallback = callback;
	    return(old_cb);
    };

    // This method is used to get the width of the Set.
    //
    inline int getWidth(void)
    {
	    return m_dib.getWidth();
    }

    // This method is used to get the height of the Set.
    //
    inline int getHeight(void)
    {
	    return m_dib.getHeight();
    }

    // This method is used to get the address of the DIB object.
    //
    inline MleDIB *getDib(void)
    {
	    return(&m_dib);
    }

    // This method is used to get the address of the DIB Palette object.
    //
    inline MlPaletteType *getPalette(void) { return(&m_pal); }

    // This method is used to resize the forum.
    //
    static int resizeEventCB(MleEvent event,void *callData,void *clientData);

#endif /* WIN32 */

    // The properties exposed in the DWP.

    // position - contains x and y position in screen coordinate,
    // (0, 0) corresponds to the lower left corner of the window.
    MlVector2 position;

    // size - contains width and height in screen coordinate.
    MlVector2 size;

  protected:

    //
    // The headRole is the first Role drawn.  This makes the
    // headRole always appear in the bottom layer among all the
    // Roles attached to this Set.  Similiarly, the last Role
    // in this list always appear in the top layer.
    //
    Mle2dRole *m_headRole;

#ifdef WIN32

    Mle2dSetViewBoundsCallback m_viewBoundsCallback;

    // All management of screen clears and dirty rectangles is being
    // done in the Set (probably will change).

    #define MAX_DIRTY_RECTANGLES 16

    friend void ViewBoundsCallback(Mle2dRole *role);

    //
    // Table of dirty rectangles.
    //
    brwin_dirty_rect m_dirtyRectangles[MAX_DIRTY_RECTANGLES];
    int	m_lastDirtyRectangle;

    //
    // Union of rectangles that were drawn during this frame.
    //
    brwin_dirty_rect m_renderBounds;

    //
    // Union of rectangles that were cleared during this frame.
    //
    brwin_dirty_rect m_clearBounds;

    //
    // DIB interface to off-screen buffer. Note that this will
    // be initialized to point to the pixelBuffer used by the
    // BRender stage.
    //
    MleDIB m_dib;
    MlPaletteType m_pal;

#endif	/* WIN32 */

  private:

#ifdef WIN32
    MleSchedulerItem *m_renderCBId;  // Render callback id (scheduler).
#ifndef _GDK_
    MleCallbackId m_resizeCBId;      // Resize callback id (event dispatcher).
#endif /* _GDK_ */
#endif /* WIN32 */
};


#endif	/* __MLE_BR2DSET_H_ */
