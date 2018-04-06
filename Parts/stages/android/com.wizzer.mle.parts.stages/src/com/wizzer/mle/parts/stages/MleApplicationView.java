/*
 * MleApplicationFrame.java
 * Created on Dec 21, 2004
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

// Import Android classes.
import android.view.View;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.graphics.Color;

// Import Magic Lantern Runtime Engine classes.
import com.wizzer.mle.parts.j2d.MleBitmap;
import com.wizzer.mle.runtime.MleTitle;
import com.wizzer.mle.runtime.core.MleSize;
import com.wizzer.mle.runtime.event.IMleEventCallback;
import com.wizzer.mle.runtime.event.MleEventManager;

/**
 * This class is a utility for creating a <code>View</code> in
 * which a Magic Lantern application can render into.
 * 
 * @author Mark S. Millard
 */
public class MleApplicationView extends View
{
    /** The default width of the <code>View</code>. */
    public static int DEFAULT_WIDTH = 320;
    /** The default height of the <code>View</code>. */
    public static int DEFAULT_HEIGHT = 480;
    /** The default background color. */
    public static int DEFAULT_COLOR = Color.BLACK;
    
    /** The name of the <code>View</code>. */
    protected String m_name;
    /** The width of the <code>View</code>. */
    protected int m_width;
    /** The height of the <code>View</code>. */
    protected int m_height;
    /** The background color. */
    protected int m_backgroundColor;
    
    // A bitmap buffer for the View's canvas.
    private MleBitmap m_backbuffer;

    /**
     * The default constructor.
     * <p>
     * The default size, 320x480, is used.
     * </p>
     * 
     * @param context An interface to global information about an application environment.
     */
    public MleApplicationView(Context context)
    {
    	super(context);
    	
        m_name = "MleApplicationView v1.0";
        
        // Set the size of the View.
        m_width = DEFAULT_WIDTH;
        m_height = DEFAULT_HEIGHT;
        // Set the default background color.
        m_backgroundColor = DEFAULT_COLOR;
        
        m_backbuffer = null;

        // Initialize the View behavior.
        init();
    }

    /**
     * A constructor that specifies the name of the application
     * frame.
     * <p>
     * The default size, 320x480, is used.
     * </p>
     * 
     * @param context An interface to global information about an application environment.
     * @param name The name of the application.
     */
    public MleApplicationView(Context context, String name)
    {
        super(context);
        
        m_name = name;

        // Set the size of the View
        m_width = DEFAULT_WIDTH;
        m_height = DEFAULT_HEIGHT;
        // Set the default background color.
        m_backgroundColor = DEFAULT_COLOR;

        m_backbuffer = null;

        // Initialize the View behavior.
        init();
    }

    /**
     * A constructor that specifies the name of the application
     * frame.
     * 
     * @param context An interface to global information about an application environment.
     * @param name The name of the application.
     * @param w The width of the applicaiton.
     * @param h The height of the application.
     */
    public MleApplicationView(Context context, String name, int w, int h)
    {
        super(context);
        
        m_name = name;

        // Set the size of the View
        m_width = w;
        m_height = h;
        // Set the default background color.
        m_backgroundColor = DEFAULT_COLOR;

        m_backbuffer = null;

        // Initialize the View behavior.
        init();
    }

   /**
    * Initializes the user interface behavior.
    */
    protected void init()
    {
        // Set the size of the View.
        setMeasuredDimension(m_width, m_height);
        
        onSizeChanged(m_width, m_height, 0, 0);
        
        // Center the View.
        //center();
        
        // Add a listener for closing the Window.
        // XXX - We should make this a dispatched event.
        //addWindowListener(new WindowAdapter() {
        //    public void windowClosing(WindowEvent e)
        //    {
        //        // Generate a MLE_QUIT event.
        //        MleTitle.getInstance().m_theDispatcher.processEvent(
        //            MleEventManager.MLE_QUIT,null,IMleEventCallback.MLE_EVENT_IMMEDIATE);
        //    }
        //});
    }
    
    /**
     * Dispose of system resources.
     */
    public void dispose()
    {
    	// Does nothing for now.
    }
    
    /**
     * Get the background color for the view.
     * 
     * @return A color represented by a compressed <code>Color</code> value.
     */
    public int getBackgroundColor()
    {
    	return m_backgroundColor;
    }
    
    /**
     * Get the backbuffer associated with this view.
     * 
     * @return A <code>MleBitmap</code> is returned. If one has not yet been created,
     * then <b>null</b> will be returned.
     */
    public MleBitmap getBackbuffer()
    {
    	return m_backbuffer;
    }
  
    /**
     * Center the <code>View</code> on the device.
     */
    public void center()
    {
    	Rect rect = new Rect();
    	//getWindowFrame(rect);
    	this.getDrawingRect(rect);
    	int frameWidth = rect.width();
    	int frameHeight = rect.height();
    	int viewWidth = getWidth();
    	int viewHeight = getHeight();
        int x = (frameWidth - viewWidth) / 2;
        int y = (frameHeight - viewHeight) / 2;
        //setLocation(x,y);
    }
    
    /**
     * Update the specified graphics context.
     * 
     * @param c The <code>Canvas</code> context to update.
     */
    public void update(Canvas c)
    {
        onDraw(c);
    }
    
    /**
     * Paint the specified graphics context.
     * 
     * @param c The <code>Canvas</code> context to display.
     */
    @Override
    public void onDraw(Canvas c)
    {
    	// Lock the back buffer.
    	m_backbuffer.lock();
    	
        // Dispatch the paint event callbacks in immediate mode.
        MleTitle.getInstance().m_theDispatcher.processEvent(MleEventManager.MLE_PAINT,null,
            IMleEventCallback.MLE_EVENT_IMMEDIATE);
        
        // Update the View.
        c.drawBitmap(m_backbuffer.getBitmap(), 0, 0, null);
        
        // Unlock the back buffer.
        m_backbuffer.unlock();
    }
    
    /**
     * Method that is called whenever the size of this view changes.
     * 
     * @param w The width to resize to.
     * @param h The height to resize to.
     * @param oldw The old width.
     * @param pdlh The old height.
     */ 
    @Override 
    protected void onSizeChanged(int w, int h, int oldw, int oldh) 
    { 
    	// Get the current width and height of the bitmap.
    	int curW = m_backbuffer != null ? m_backbuffer.getBitmap().getWidth() : 0; 
    	int curH = m_backbuffer != null ? m_backbuffer.getBitmap().getHeight() : 0; 

/*
    	// Don' do anything if the current width and height are larger
    	// than the requested size change.
    	if (curW >= w && curH >= h)
    	{ 
    		return; 
    	} 

    	if (curW < w) curW = w; 
    	if (curH < h) curH = h;
*/
    	// Don' do anything if the current width and height are the same.
    	if (curW == w && curH == h)
    		return;
    	
    	curW = w;
    	curH = h;

    	// Create the bitmap and canvas that will be used to draw everything.
    	Bitmap newBitmap = Bitmap.createBitmap(curW, curH, Bitmap.Config.ARGB_8888); 
    	Canvas newCanvas = new Canvas();

    	newCanvas.setBitmap(newBitmap); 

    	// As long as the bitmap exists, draw it on the canvas.
    	if (m_backbuffer != null)
    	{ 
    		newCanvas.drawBitmap(m_backbuffer.getBitmap(), 0, 0, null); 
    	} 

    	// Save the bitmap and canvas for use later.
    	m_backbuffer = new MleBitmap(newBitmap); 
    	m_backbuffer.setCanvas(newCanvas);
    	
    	m_width = curW;
    	m_height = curH;
    	
        // A resize event occurred. Dispatch the resize event callbacks in immediate mode.
        MleSize callData = new MleSize(m_width, m_height);
        MleTitle.getInstance().m_theDispatcher.processEvent(MleEventManager.MLE_SIZE,
	            callData,IMleEventCallback.MLE_EVENT_IMMEDIATE);
    } 
}
