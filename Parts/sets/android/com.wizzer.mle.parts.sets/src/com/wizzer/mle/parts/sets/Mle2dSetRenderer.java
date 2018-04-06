/*
 * Mle2dSetRenderer.java
 * Created on Feb 16, 2005
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
package com.wizzer.mle.parts.sets;

// Import standard Java classes.

// Import Android classes.
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;

// Import Magic Lantern Parts classes.
import com.wizzer.mle.parts.MleBounds;
import com.wizzer.mle.parts.j2d.MleBitmap;
import com.wizzer.mle.parts.j2d.MleRenderEngine;
import com.wizzer.mle.parts.stages.Mle2dStage;
import com.wizzer.mle.parts.roles.Mle2dRole;

/**
 * This class is used to render the Roles associated with a
 * <code>Mle2dSet</code>.
 * 
 * @author Mark S. Millard
 */
public class Mle2dSetRenderer extends MleRenderEngine
{
    // The maximum number of dirty rectangles.
    private static int MAX_DIRTY_RECTANGLES = 16;
    
    // Table of dirty rectangles.
    private MleBounds[] m_dirtyRectangles = new MleBounds[MAX_DIRTY_RECTANGLES];
    private int	m_lastDirtyRectangle;

    // Union of rectangles that were drawn during this frame.
    private MleBounds m_renderBounds = new MleBounds();

    // Union of rectangles that were cleared during this frame.
    private MleBounds m_clearBounds = null;

    /**
     * The default constructor.
     */
    public Mle2dSetRenderer()
    {
        super();
        
        init();
    }

    /**
     * A constructor that initializes the name of the renderer thread.
     * 
     * @param name The name of the renderer.
     */
    public Mle2dSetRenderer(String name)
    {
        super(name);
        
        init();
    }
    
    // Initialize the renderer.
    private void init()
    {
        // Initialize the list of boundary extents.
        m_lastDirtyRectangle = 0;
        for (int i = 0; i < MAX_DIRTY_RECTANGLES; i++)
        {
            m_dirtyRectangles[i] = new MleBounds();
        }
    }

    // Calculate the view bounds, or more commonly known as the
    // extent of the dirty rectangles, for the specifed Role.
    private void viewBoundsCallback(Mle2dRole role)
    {
        // Declare local variables.
        MleBounds dp = m_dirtyRectangles[0];
        float x, y;
        float width, height;
        float[] min = new float[2], max = new float[2];
        Mle2dStage theStage = (Mle2dStage) Mle2dStage.getInstance();
        int xoffset = theStage.getPixelBuffer().m_origin_x;
        int yoffset = theStage.getPixelBuffer().m_origin_y;

        role.getBounds(min, max);
        x = min[0] - xoffset;
        y = min[1] - yoffset;
        width = max[0] - min[0] + 1;
        height = max[1] - min[1] + 1;

        // Accumulate total bounding rectangle.
        if (x < m_renderBounds.m_min_x)
            m_renderBounds.m_min_x = (int)(x);
        if (y < m_renderBounds.m_min_y)
            m_renderBounds.m_min_y = (int)(y);
        if (x + width > m_renderBounds.m_max_x + 1)
            m_renderBounds.m_max_x = (int)(x) + (int)(width) - 1;
        if (y + height > m_renderBounds.m_max_y + 1)
            m_renderBounds.m_max_y = (int)(y) + (int)(height) - 1;

        // Store into dirty rectangle list; if list is full, merge current
        // into last dirty rectangle.
        if (m_lastDirtyRectangle >= MAX_DIRTY_RECTANGLES)
    	{
            dp = m_dirtyRectangles[m_lastDirtyRectangle - 1];
            if (x < dp.m_min_x)
                dp.m_min_x = (int)(x); 
            if (y < dp.m_min_y)
                dp.m_min_y = (int)(y);
            if (x + width > dp.m_max_x + 1)
                dp.m_max_x = (int)(x) + (int)(width) - 1;
            if (y + height > dp.m_max_y + 1)
                dp.m_max_y = (int)(y) + (int)(height) - 1;
        } else
    	{
            dp = m_dirtyRectangles[m_lastDirtyRectangle];
            dp.m_min_x = (int)(x);
            dp.m_min_y = (int)(y);
            dp.m_max_x = (int)(x) + (int)(width) - 1;
            dp.m_max_y = (int)(y) + (int)(height) - 1;

            m_lastDirtyRectangle++;
        }
    }

    /**
     * Thread execution.
     * <p>
     * This method is used to traverse the Role scene graph and render each
     * 2D Role.
     * </p>
     */
    public void run()
    {
        // Get the Stage from the call data.
        Mle2dStage theStage = (Mle2dStage) m_callData;
        // Get the Set from the client data.
        Mle2dSet theSet = (Mle2dSet) m_clientData;
        
        // Set clearBounds based on the union of dirty rectangles,
        // this was computed as the renderBounds of the previous frame.
        m_clearBounds = new MleBounds(m_renderBounds);

        // Clear screen using dirty rectangle list;
        // the screen clear probably should be done in the stage.
        int rectIndex;
        MleBounds dp;
        int xoffset = theStage.getPixelBuffer().m_origin_x;
        int yoffset = theStage.getPixelBuffer().m_origin_y;

        for (rectIndex = 0, dp = m_dirtyRectangles[0]; 
    	     rectIndex < m_lastDirtyRectangle; 
    	     rectIndex++, dp = m_dirtyRectangles[rectIndex])
    	{
            // Clear buffers.
        	Paint paint = new Paint();
        	paint.setColor(theStage.getBackgroundColor());
        	Canvas canvas = theStage.getPixelBuffer().getCanvas();
        	Rect r = new Rect();
        	r.left = dp.m_min_x - xoffset;
        	r.right = dp.m_max_x - dp.m_min_x + 1;
        	r.top = dp.m_min_y - yoffset;
        	r.bottom = dp.m_max_y - dp.m_min_y + 1;
        	canvas.drawRect(r,paint);
        }

        // Reset dirty rectangle list.
        m_lastDirtyRectangle = 0;
        m_renderBounds.m_min_x = MleBounds.INT_MAX;
        m_renderBounds.m_min_y = MleBounds.INT_MAX;
        m_renderBounds.m_max_x = MleBounds.INT_MIN;
        m_renderBounds.m_max_y = MleBounds.INT_MIN;
        
        // Process all 2D Roles attached to this set.
        Mle2dRole nextRole = theSet.m_headRole;
        MleBitmap pixelBuffer = theStage.getPixelBuffer();

        while (nextRole != null)
    	{
    		nextRole.draw(pixelBuffer.getCanvas());

            // Invoke the callback function on this Role.
            viewBoundsCallback(nextRole);

            nextRole = nextRole.m_next;
        }

        // Tell the Stage the latest regions that need to be blitted.
        theStage.dirtyRegion(m_renderBounds);
        theStage.dirtyRegion(m_clearBounds);
    }

}
