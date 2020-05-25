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
import java.awt.Color;
import java.awt.Frame;
import java.awt.Dimension;
import java.awt.Insets;
import java.awt.Toolkit;
import java.awt.Graphics;
import java.awt.event.WindowEvent;
import java.awt.event.WindowAdapter;
import java.awt.HeadlessException;

// Import Magic Lantern Runtime Engine classes.
import com.wizzer.mle.runtime.MleTitle;
import com.wizzer.mle.runtime.core.MleSize;
import com.wizzer.mle.runtime.event.IMleEventCallback;
import com.wizzer.mle.runtime.event.MleEventManager;

/**
 * This class is a utility for creating a <code>Frame</code> in
 * which a Magic Lantern application can render into.
 * 
 * @author Mark S. Millard
 */
public class MleApplicationFrame extends Frame
{
	/** The unique serialization identifier. */
	private static final long serialVersionUID = 5770866467258394638L;

	/** The default width of the <code>Frame</code>. */
    public static int DEFAULT_WIDTH = 640;
    /** The default height of the <code>Frame</code>. */
    public static int DEFAULT_HEIGHT = 480;
    
    /** The width of the <code>Frame</code>. */
    protected int m_width;
    /** The height of the <code>Frame</code>. */
    protected int m_height;
    /** The background color. */
    protected Color m_background = Color.BLACK;

    /**
     * The default constructor.
     * <p>
     * The default size, 640x40, is used.
     * </p>
     * 
     * @throws java.awt.HeadlessException This exception is thrown if
     * the application frame can not be constructed.
     */
    public MleApplicationFrame() throws HeadlessException
    {
        this("MleApplicationFrame v1.0");
        
        // Set the size of the Frame.
        m_width = DEFAULT_WIDTH;
        m_height = DEFAULT_HEIGHT;
    }

    /**
     * A constructor that specifies the name of the application
     * frame.
     * <p>
     * The default size, 640x40, is used.
     * </p>
     * 
     * @param name The name of the application.
     * 
     * @throws java.awt.HeadlessException This exception is thrown if
     * the application frame can not be constructed.
     */
    public MleApplicationFrame(String name) throws HeadlessException
    {
        super(name);

        // Set the size of the Frame
        m_width = DEFAULT_WIDTH;
        m_height = DEFAULT_HEIGHT;

        // Initialize the Frame behavior.
        init();
    }

    /**
     * A constructor that specifies the name of the application
     * frame.
     * 
     * @param name The name of the application.
     * @param w The width of the applicaiton.
     * @param h The height of the application.
     * 
     * @throws HeadlessException This exception is thrown if
     * the application frame can not be constructed.
     */
    public MleApplicationFrame(String name, int w, int h) throws HeadlessException
    {
        super(name);

        // Set the size of the Frame
        m_width = w;
        m_height = h;

        // Initialize the Frame behavior.
        init();
    }

   /**
    * Initialized the user interface behavior.
    */
    protected void init()
    {
        // Set the size of the Frame.
        setSize(m_width,m_height);
        
        // Set the background color.
        setBackground(m_background);
        
        // Center the Frame.
        center();
        
        // Add a listener for closing the Window.
        // XXX - We should make this a dispatched event.
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e)
            {
                //dispose();
                //System.exit(0);
                // Generate a MLE_QUIT event.
                MleTitle.getInstance().m_theDispatcher.processEvent(
                    MleEventManager.MLE_QUIT,null,IMleEventCallback.MLE_EVENT_IMMEDIATE);
            }
        });
    }
    
    /**
     * Center the <code>Frame</code> on the device.
     */
    public void center()
    {
        Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        Dimension frameSize = getSize();
        int x = (screenSize.width - frameSize.width) / 2;
        int y = (screenSize.height - frameSize.height) / 2;
        setLocation(x,y);
    }
    
    /**
     * Update the specified graphics context.
     * 
     * @param g The <code>Graphics</code> context to update.
     */
    public void update(Graphics g)
    {
        paint(g);
    }
    
    /**
     * Paint the specified graphics context.
     * 
     * @param g The <code>Graphics</code> context to display.
     */
    public void paint(Graphics g)
    {
        Dimension frameSize = getSize();
        if ((frameSize.width != m_width) || (frameSize.height != m_height))
        {
            // A resize event occurred. Dispatch the resize event callbacks in immediate mode.
            MleSize callData = new MleSize(frameSize.width,frameSize.height);
	        MleTitle.getInstance().m_theDispatcher.processEvent(MleEventManager.MLE_SIZE,
		            callData,IMleEventCallback.MLE_EVENT_IMMEDIATE);
	        m_width = frameSize.width;
	        m_height = frameSize.height;
        } else
        {
	        // Dispatch the paint event callbacks in immediate mode.
	        MleTitle.getInstance().m_theDispatcher.processEvent(MleEventManager.MLE_PAINT,null,
	            IMleEventCallback.MLE_EVENT_IMMEDIATE);
        }
    }
    
    /**
     * Set the frame's background color.
     * 
     * @param color The <code>Color</code> to set.
     */
    public void setBackgroundColor(Color color)
    {
    	m_background = color;
    	setBackground(color);
    	
    }
    
    /**
     * Get the frame's background color.
     * 
     * @return A <code>Color</code> is returned.
     */
    public Color getBackgroundColor()
    {
    	return m_background;
    }
    
    public void toFrameCoord(float[] coord)
    {
    	Insets insets = getInsets();
    	coord[0] = coord[0] + insets.left;
    	coord[1] = coord[1] + insets.top;
    }
}
