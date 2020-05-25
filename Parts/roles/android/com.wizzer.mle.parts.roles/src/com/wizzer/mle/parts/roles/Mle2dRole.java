/*
 * Mle2dRole.java
 * Created on Dec 8, 2004
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

// Declare package.
package com.wizzer.mle.parts.roles;

// Import Android classes.
import android.graphics.Canvas;

// Import Magic Lantern Runtime Engine classes.
import com.wizzer.mle.runtime.core.MleActor;
import com.wizzer.mle.runtime.core.MleRole;
import com.wizzer.mle.runtime.core.MleRuntimeException;

// Import Magic Lantern Math classes
import com.wizzer.mle.math.MlScalar;

// Import Magic Lantern Parts classes.
import com.wizzer.mle.parts.j2d.roles.I2dRole;
import com.wizzer.mle.parts.j2d.sets.I2dSet;

/**
 * The 2D Role base class.
 * <p>
 * The 2D Role base class is used for subclassing to create a
 * Role that can be attached to the <code>Mle2dSet</code>.  All the 2D
 * Roles must initialize their bounding box and provide a draw
 * function for rendering during runtime.
 * </p>
 * 
 * @see Mle2dSet
 */
public abstract class Mle2dRole extends MleRole implements I2dRole
{
    /** The previous Role in the rendering layer list. */
    public Mle2dRole m_prev = null;
    /** The next Role in the rendering layer list. */
    public Mle2dRole m_next = null;
    /** The minimum coordinate value of the bounding region. */
    protected float[] m_bBoxMin = new float[2];
    /** The maximum coordinate value of the bounding region. */
    protected float[] m_bBoxMax = new float[2];

    /**
     * The default constructor. Note that an Actor is not attached
     * to the Role when using this constructor.
     * <p>
     * A default bounding region is established from min = (0,0)
     * to max = (639,479).
     * </p>
     */
    public Mle2dRole()
    {
        super();
        m_bBoxMin[0] = MlScalar.ML_SCALAR_ZERO;
        m_bBoxMin[1] = MlScalar.ML_SCALAR_ZERO;
        m_bBoxMax[0] = 639;
        m_bBoxMax[1] = 479;
    }

    /**
     * A constructor that attaches the specifed Actor.
     * <p>
     * A default bounding region is established from min = (0,0)
     * to max = (639,479).
     * </p>
      * 
     * @param actor The Actor to attach.
     */
    public Mle2dRole(MleActor actor)
    {
        super(actor);
        m_bBoxMin[0] = MlScalar.ML_SCALAR_ZERO;
        m_bBoxMin[1] = MlScalar.ML_SCALAR_ZERO;
        m_bBoxMax[0] = 639;
        m_bBoxMax[1] = 479;
    }

    /**
     * Get the bounding box of the Role.
     * <p>
     * It is specified in pixels
     * relative to the corresponding Set's coordinate system,
     * (0, 0) corresponds to the lower left corner (origin of the
     * corresponding Set).
     * </p>
     * 
     * @param min An array of two elements representing the
     * minimal (x, y) coordinate of the bounding region.
     * @param max An array of two elements representing the
     * maximum (x, y) coordinate of the bounding region.
     */
    public synchronized void getBounds(float min[], float max[])
    {
        min[0] = m_bBoxMin[0];
        min[1] = m_bBoxMin[1];
        max[0] = m_bBoxMax[0];
        max[1] = m_bBoxMax[1];

    }
 
	/** Set the bounding box of the Role. 
	 * <p>
	 * It is specified in pixels
     * relative to the corresponding Set's coordinate system,
     * (0, 0) corresponds to the lower left corner (origin of the
     * corresponding Set).
     * </p>
     *
     * @param min An array of two elements representing the
     * minimal (x, y) coordinate of the bounding region.
     * @param max An array of two elements representing the
     * maximum (x, y) coordinate of the bounding region.
     */
    public synchronized void setBounds(float min[], float max[])
    {
        m_bBoxMin[0] = min[0];
        m_bBoxMin[1] = min[1];
        m_bBoxMax[0] = max[0];
        m_bBoxMax[1] = max[1];
    }

    /**
     * Draw into the specified buffer.
     * <p>
     * This function is called by the renderer in <code>Mle2dSet</code>
     * during the stage phase to draw the Role on the Android platform.
     * </p>
     * 
     * @param buffer The buffer to render into.
     */
    public abstract void draw(Canvas buffer);

	/**
	 * Dispose all resources associated with the Role.
	 */
	public synchronized void dispose()
	{
	    I2dSet theSet = (I2dSet)m_set;
	    try
	    {
	        theSet.detach(this);
	    } catch (MleRuntimeException ex)
	    {
	        // Should not occur.
	    }
	}

}
