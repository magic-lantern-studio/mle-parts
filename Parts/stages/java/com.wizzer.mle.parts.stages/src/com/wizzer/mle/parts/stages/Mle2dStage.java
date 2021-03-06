/*
 * Mle2dStage.java
 * Created on Feb 11, 2005
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
package com.wizzer.mle.parts.stages;

// Import standard Java classes.
import java.awt.Graphics;
import java.awt.Color;
import java.awt.Component;
import java.awt.Insets;
import java.awt.Dimension;

// Import Magic Lantern Runtime classes.
import com.wizzer.mle.runtime.MleTitle;
import com.wizzer.mle.runtime.core.MleRuntimeException;
import com.wizzer.mle.runtime.core.MleStage;
import com.wizzer.mle.runtime.core.MleSet;
import com.wizzer.mle.runtime.core.MleSize;
import com.wizzer.mle.runtime.core.IMleCallbackId;
import com.wizzer.mle.runtime.event.MleEvent;
import com.wizzer.mle.runtime.event.MleEventCallback;
import com.wizzer.mle.runtime.event.MleEventManager;
import com.wizzer.mle.runtime.scheduler.MleTask;

// Import Magic Lantern Math classes.
import com.wizzer.mle.math.MlVector2;
import com.wizzer.mle.math.MlScalar;

// Import Magic Lantern Parts classes.
import com.wizzer.mle.parts.MleBounds;
import com.wizzer.mle.parts.j2d.MleBufferedImage;
import com.wizzer.mle.parts.j2d.MleJ2dPlatformData;
import com.wizzer.mle.parts.j2d.MleRenderEngine;
import com.wizzer.mle.parts.j2d.stages.I2dStage;


/**
 * This class implements a simple Stage for 2D applications.
 * 
 * @author Mark S. Millard
 */
public class Mle2dStage extends MleStage implements I2dStage
{
    private String DEFAULT_STAGE_NAME = "MLE 2D Stage";  /* The default stage name. */

    /** The application <code>Frame</code> for the stage. */
    protected MleApplicationFrame m_windowFrame;
    
    /** The current offscreen buffer. */
    protected MleBufferedImage m_pixelBuffer = null;
    
    /** The aspect ratio of the width to height of the stage. */
	protected float m_aspect;
	/** The boundary region for the current blitting region. */
	protected MleBounds m_curBlitBounds = null;
	
	/** x position of raster on stage. */
	protected int m_x;
	/** y position of raster on stage. */
	protected int m_y;
	/** Size of stage. */
	protected MleSize m_size = null;
	
	/** The mode for showing the application frame. <b>true</b> = show window */
	public boolean m_showMode;
	
    // true when buffer is ready for blit.
	private boolean m_ready;
	
    /**
     * This inner class is used to process resize events.
     */
    protected class Mle2dStageResizeCallback extends MleEventCallback
    {
        /**
         * The default constructor.
         */
        public Mle2dStageResizeCallback()
        {
            super();
            // Do nothing extra.
        }
        
        /**
         * The callback dispatch method. This method is responsible for
         * handling the <i>resize</i> event.
         * 
         * @param event The event that is being dispatched by the handler.
         * @param clientData Client data registered with this handler.
         * 
         * @return If the event is successfully dispatched, then <b>true</b> should be
         * returned. Otherwise, <b>false</b> should be returned.
         * 
     	 * @see com.wizzer.mle.runtime.event.IMleEventCallback#dispatch(com.wizzer.mle.runtime.event.MleEvent, java.lang.Object)
    	 */
    	public boolean dispatch(MleEvent event,Object clientData)
    	{
    	    boolean result = true;
    	    
    	    // Cast the client data to the stage being resized.
    	    Mle2dStage theStage = (Mle2dStage) clientData;
    	    // Get the width and height from the event call data.
    	    MleSize newSize = (MleSize)event.getCallData();
    	    
    	    // Rebuild the off screen buffer.
    	    try
    	    {
    	        theStage.resize(0, 0, (int)newSize.getWidth(), (int)newSize.getHeight());
    	    } catch (MleRuntimeException ex)
    	    {
    	        result = false;
    	    }
    	    
    	    return result;
    	}
    }
    
    /**
     * This inner class is used to blit the offscreen buffer to
     * the stage's current <code>Graphics</code> context.
     */
    protected class Mle2dStageBlitterTask extends Thread
    {
        // The stage.
        private Mle2dStage m_stage = null;

        /**
         * A constructor that initializes the stage to update
         * by blitting the offscreen buffer to the current
         * <code>Graphics</code> context.
         * 
         * @param stage A reference to the 2d stage.
         */
        public Mle2dStageBlitterTask(Mle2dStage stage)
        {
            super();
            m_stage = stage;
        }
        
        /**
         * Execute the blit.
         */
        public void run()
        {
            if (m_stage == null)
                return;

            m_stage.blit(m_stage.m_windowFrame.getGraphics());
        }       
    }

    // Beginning of properties.

    /** The "size" property contains width and height in pixels. */
    public MlVector2 size;

    /**
     * The default constructor.
     */
    public Mle2dStage()
    {
        super();
        
        // Not ready for rendering.
        m_ready = false;
        
        // Set the aspect ration to be 1:1.
        m_aspect = MlScalar.ML_SCALAR_ONE;
        
        // Create the dirty bounding rectangle.
        m_curBlitBounds = new MleBounds();
        
        // Initialize the bounding rectangle.
        m_curBlitBounds.m_min_x = MleBounds.INT_MAX;
        m_curBlitBounds.m_min_y = MleBounds.INT_MAX;
        m_curBlitBounds.m_max_x = MleBounds.INT_MIN;
        m_curBlitBounds.m_max_y = MleBounds.INT_MIN;
        
        // Initialize the default X, Y locations.
        m_x = 0;
        m_y = 0;
        
        // Initialize the size of the stage.
        int width = ((MleJ2dPlatformData)(MleTitle.getInstance().m_platformData)).m_width;
        int height = ((MleJ2dPlatformData)(MleTitle.getInstance().m_platformData)).m_height;
        m_size = new MleSize(width,height);
        
        // Calculate the aspect ratio.
        m_aspect = (float)m_size.getWidth() / (float)m_size.getHeight();
        
        // Create the window component.
        m_windowFrame = new MleApplicationFrame(DEFAULT_STAGE_NAME,width,height);
        
        // Note: the Graphics device context is extracted from the Application Frame,
        // so there is no need to save it here.
        
        // Default windows show mode, may be changed prior to init() call.
        m_showMode = true;
        
        // Set the Stage.
        g_theStage = this;
    }

    /**
     * Get the current offscreen buffer.
     * 
     * @return A <code>MleBufferedImage</code> is returned.
     */
    public synchronized MleBufferedImage getPixelBuffer()
    {
        return m_pixelBuffer;
    }
    
    /**
	 * Registers a new Set with this Stage.
     * 
     * @param renderer The rendering thread for the Set being registered.
     * @param set The Set to register.
     * 
     * @return A reference to the scheduled renderer task is returned.
	 */
    public synchronized MleTask addSet(MleRenderEngine renderer,MleSet set)
    {
        MleTask item = new MleTask(renderer);
        renderer.setCallData(this);
        renderer.setClientData(set);

        // Just pass this function on to the scheduler.
        MleTitle.getInstance().m_theScheduler.addTask(MleTitle.g_theSetPhase,item);

        return item;
    }

    /**
     * Marks a region of the screen that needs to be blitted at frame end.
     * 
     * @param newRegion The new extent of the region that is dirty.
     */
    public synchronized void dirtyRegion(MleBounds newRegion)
    {
        int min_x,min_y,max_x,max_y;
        int offset_x = 0;
        int offset_y = 0;

        if ( m_pixelBuffer != null )
        {
            offset_x = m_pixelBuffer.m_origin_x;
            offset_y = m_pixelBuffer.m_origin_y;
        }

        min_x = newRegion.m_min_x + offset_x;
        if ( min_x < 0 )
            min_x = MleBounds.INT_MAX;

        min_y = newRegion.m_min_y + offset_y;
        if ( min_y < 0 )
            min_y = MleBounds.INT_MAX;

        max_x = newRegion.m_max_x + offset_x;
        if ( max_x < 0 )
            max_x = MleBounds.INT_MIN;

        max_y = newRegion.m_max_y + offset_y;
        if ( max_y < 0 )
            max_y = MleBounds.INT_MIN;

        if ( m_curBlitBounds.m_min_x > min_x ) 
            m_curBlitBounds.m_min_x = min_x;

        if ( m_curBlitBounds.m_min_y > min_y ) 
            m_curBlitBounds.m_min_y = min_y;

        if ( m_curBlitBounds.m_max_x < max_x ) 
            m_curBlitBounds.m_max_x = max_x;
        
        if ( m_curBlitBounds.m_max_y < max_y ) 
            m_curBlitBounds.m_max_y = max_y;
    }

    /**
     * Get the size of the stage.
     * 
     * @return The size of the stage's component is returned.
     * Magic Lantern 1.0 supports one component per stage: this is the
     * default component size.
     */
    public synchronized MleSize getSize()
    {
        return m_size;
    }
    
    /**
     * Get the background color.
     * 
     * @return A <code>Color</code> is returned.
     */
    public synchronized Color getBackgroundColor()
    {
        return m_windowFrame.getBackgroundColor();
    }
    
    /**
     * Set the background color.
     * 
     * @param color The <code>Color</code> to set.
     */
    public synchronized void setBackgroundColor(Color color)
    {
    	m_windowFrame.setBackgroundColor(color);
    }

    /**
     * Initialize the stage.
     * <p>
     * The stage creates an event callback for managing the resize event.
     * It also registers a task with the scheduler for performing
     * double buffering blit of the offscreen buffer to the onscreen buffer.
     * </p>
     * 
     * @throws MleRuntimeException This exception is thrown if the
     * stage can not be successfully initialized.
     */
    public synchronized void init() throws MleRuntimeException
    {
        // Declare local variables.
        IMleCallbackId cbId;

        // Insert resize callback into event dispatch manager.
        Mle2dStageResizeCallback resizeEventCB = new Mle2dStageResizeCallback();
        cbId = MleTitle.getInstance().m_theDispatcher.installEventCB(
                MleEventManager.MLE_SIZE,resizeEventCB,this);

        // Bump priority of dispatched callback.
        MleTitle.getInstance().m_theDispatcher.changeCBPriority(
                MleEventManager.MLE_SIZE,cbId,
                MleEventManager.MLE_RESIZE_STAGE_PRIORITY);

        // Insert stage blitter into the scheduler.
        Mle2dStageBlitterTask blitter = new Mle2dStageBlitterTask(this);
        MleTask item = new MleTask(blitter,"2D Stage Renderer");
        MleTitle.getInstance().m_theScheduler.addTask(MleTitle.g_theStagePhase,item);
 
        // Make the window appear.
        m_windowFrame.show(m_showMode);

        // Call resize() to explicitly create the off screen buffer and pixelmaps.
        resize( 0, 0, (int)m_size.getWidth(), (int)m_size.getHeight() );
    }

    /**
     * Handle resizing and buffer management.
     * 
     * @param x The x position of the raster to resize.
     * @param y The y position of the raster to resize.
     * @param width The width of the raster to resize.
     * @param height The height of the raster to resize.
     * 
     * @throws MleRuntimeException This exception is thrown if the stage can
     * not be resized.
     */
    public synchronized void resize(int x, int y, int width, int height)
        throws MleRuntimeException
    {
        // Not ready for rendering.
        m_ready = false;

        // Store new values.
        m_x = x;
        m_y = y;
        m_size = new MleSize(width,height);
      
        // Release old buffer resources.
        if (m_pixelBuffer != null)
            m_pixelBuffer.dispose();

        // Allocate new buffers.
        m_pixelBuffer = new MleBufferedImage(width,height,MleBufferedImage.TYPE_INT_ARGB);
        if (m_pixelBuffer == null)
            throw new MleRuntimeException();
        
        m_pixelBuffer.m_origin_x = m_pixelBuffer.getWidth()/2;
        m_pixelBuffer.m_origin_y = m_pixelBuffer.getHeight()/2;
      
        // Remember camera aspect ratios.
        m_aspect = (float)m_size.getWidth() / (float)m_size.getHeight();
      
        // Clear buffers.
        Graphics offG = m_pixelBuffer.getGraphics();
        offG.setColor(m_windowFrame.getBackground());
        offG.fillRect(0, 0, width, height);
      
        // Reblit whole buffer on next update.
        m_curBlitBounds.m_min_x = 0;
        m_curBlitBounds.m_min_y = 0;
        m_curBlitBounds.m_max_x = m_pixelBuffer.getWidth();
        m_curBlitBounds.m_max_y = m_pixelBuffer.getHeight();
        
        // Ready for rendering.
        m_ready = true;
    }
    
    /**
     * Blit the offscreen buffer to the specified <code>Graphics</code> context.
     * 
     * @param g The <code>Graphics</code> context to blit to.
     */
    public synchronized void blit(Graphics g)
    {
        // Check if the local screen related data structures are ready.
        if (m_ready != true)
            return;
      
        // Blit the appropriate region of the screen.
        if (m_curBlitBounds.m_min_x < m_curBlitBounds.m_max_x)
        {
            int width = m_curBlitBounds.m_max_x + 1 - m_curBlitBounds.m_min_x;
            int height = m_curBlitBounds.m_max_y + 1 - m_curBlitBounds.m_min_y;
            float[] coord = new float[2];
            coord[0] = m_x;
            coord[1] = m_y;
            m_windowFrame.toFrameCoord(coord);
            g.drawImage(m_pixelBuffer,
                    (int)coord[0] + m_curBlitBounds.m_min_x, (int)coord[1] + m_curBlitBounds.m_min_y,
                    (int)coord[0] + m_curBlitBounds.m_min_x + width, (int)coord[1] + m_curBlitBounds.m_min_y + height,
                    m_curBlitBounds.m_min_x, m_curBlitBounds.m_min_y,
                    m_curBlitBounds.m_min_x + width, m_curBlitBounds.m_min_y + height,
                    null);
            /*
            g.drawImage(m_pixelBuffer,
                    m_x + m_curBlitBounds.m_min_x, m_y + m_curBlitBounds.m_min_y,
                    m_x + m_curBlitBounds.m_min_x + width, m_y + m_curBlitBounds.m_min_y + height,
                    m_curBlitBounds.m_min_x, m_curBlitBounds.m_min_y,
                    m_curBlitBounds.m_min_x + width, m_curBlitBounds.m_min_y + height,
                    null);
            */
        }
        
        // Clear the screen.
        Graphics offG = m_pixelBuffer.getGraphics();
        offG.setColor(m_windowFrame.getBackground());
        offG.fillRect(
            m_curBlitBounds.m_min_x - m_pixelBuffer.m_origin_x,
            m_curBlitBounds.m_min_y - m_pixelBuffer.m_origin_y,
            (m_curBlitBounds.m_max_x - m_curBlitBounds.m_min_x),
            (m_curBlitBounds.m_max_y - m_curBlitBounds.m_min_y));
     
        // Reset the bounding rectangle.
        m_curBlitBounds.m_min_x = MleBounds.INT_MAX;
        m_curBlitBounds.m_min_y = MleBounds.INT_MAX;
        m_curBlitBounds.m_max_x = MleBounds.INT_MIN;
        m_curBlitBounds.m_max_y = MleBounds.INT_MIN;
    }

    /**
     * Resize the application Frame to be the size of the specified
     * width and height.
     * 
     * @param width The width to resize the Stage to.
     * @param height The height to resize the Stage to.
     */
    public void sizeTo(int width, int height)
    {
        if (m_windowFrame.isDisplayable() == false)
            m_windowFrame.addNotify();
        
        Insets insets = m_windowFrame.getInsets();
        width = insets.left + insets.right + width;
        height = insets.top + insets.bottom + height;
        m_windowFrame.setSize(width,height);
    }

    /**
     * Resize the application Frame to be the size of the specified
     * component.
     * 
     * @param component The <code>Component</code> to rezise the
     * Stage to.
     */
    public void sizeToComponent(Component component)
    {
        if (m_windowFrame.isDisplayable() == false)
            m_windowFrame.addNotify();
        
        Insets insets = m_windowFrame.getInsets();
        Dimension size = component.getPreferredSize();
        int width = insets.left + insets.right + size.width;
        int height = insets.top + insets.bottom + size.height;
        m_windowFrame.setSize(width,height);
    }
    
    /**
     * Clean up the Stage and dispose of resources.
     */
    public void dispose()
    	throws MleRuntimeException
    {
        // Disable rendering.
        m_ready = false;
        
        // Dispose of UI resources.
        m_windowFrame.dispose();
    }
}
