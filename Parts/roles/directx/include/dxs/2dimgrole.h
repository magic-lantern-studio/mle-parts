/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 2dimgd.h
 * @ingroup MleParts
 *
 * This file defines the class for a 2D Image Role targeting
 * the DirectX platform.
 *
 * @author Mark S. Millard
 * @date Nov 20, 2007
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

#ifndef __MLE_2DIMGR_H_
#define __MLE_2DIMGR_H_

// Include system header files.

// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "math/vector.h"
#include "mle/MleMediaRef.h"
#include "mle/2drole.h"

// Include Magic Lantern DirectX header files.
#include "dxs/DxsSurface.h"

// Declare external classes.
class MleActor;


/**
 * This class defines the interface for a 2D Image Role targeting
 * the DirectX platform.
 */
class Mle2dImgRole : public Mle2dRole 
{

    MLE_ROLE_HEADER(Mle2dImgRole);

  public:

	/**
	 * A constructor that sets the associated Actor.
	 *
	 * @param actor A pointer to the actor to associate with this Role.
	 */
	Mle2dImgRole(MleActor *actor);

	/**
	 * The destructor.
	 */
    virtual ~Mle2dImgRole(void);

    /**
	 * Update the position where the image is drawn.  The position is
     * in screen coordinates and lower left is the image origin.
	 *
	 * @param pos The position of the image, in screen coordinates.
	 */
    void screenLocation(MlVector2 &pos);

    /**
	 * This method will read in a single pixelmap image.  It can also be used to
     * read in a different pixelmap image during runtime.
	 *
	 * @param img The Media Reference to load.
	 */
    void load(MlMediaRef img);

    /**
	 * The display of the image can be toggled on or off.
	 *
	 * @param state If <b>TRUE</b>, then the image will be displayed.
	 * If <b>FALSE</b> then the image will be hidden.
	 */
    void display(int state);

    /**
	 * The render() function of the 2D Set calls this function to update
     * this Role every cycle.
	 *
	 * @param data A pointer to the data to be drawn.
	 */
    virtual void draw(void *data);

	/**
	 * This routine is called when the stage is preparing to reset
	 * the DirectX device. It is used to release the offscreen buffer.
	 *
	 * @param ev The event causing the callback to be invoked.
	 * @param calldata A pointer to the Role that was registerd to be
	 * notified that the DirectX device is being reset.
	 */
	static void onReset(unsigned int ev, void *calldata);

  protected:

	/** Image origin (lower left) in screen coordinates. */
    MlVector2 screenPosition;
	/** The display state, TRUE for on, FALSE for off. */
    int displayState;
	/** A pointer to the DirectX surface. */
    LPDXS_SURFACE m_surface;

  private:

    // Remember media reference to reload on DirectX reset.
	MlMediaRef m_imgMediaRef;
};


#endif /* __MLE_2DIMGR_H_ */
