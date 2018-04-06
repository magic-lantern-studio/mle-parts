/*
 * Mle2dSet.java
 * Created on Dec 9, 2004
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
import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;

// Import Magic Lantern classes.
import com.wizzer.mle.runtime.MleTitle;
import com.wizzer.mle.runtime.core.MleSize;
import com.wizzer.mle.runtime.core.IMleCallbackId;
import com.wizzer.mle.runtime.core.MleRole;
import com.wizzer.mle.runtime.core.MleSet;
import com.wizzer.mle.runtime.core.MleStage;
import com.wizzer.mle.runtime.core.IMleProp;
import com.wizzer.mle.runtime.core.MleRuntimeException;
import com.wizzer.mle.runtime.event.MleEvent;
import com.wizzer.mle.runtime.event.MleEventCallback;
import com.wizzer.mle.runtime.event.MleEventManager;
import com.wizzer.mle.runtime.scheduler.MleTask;

import com.wizzer.mle.math.MlVector2;

// Import Magic Lantern Java2D target classes.
import com.wizzer.mle.parts.j2d.MleBitmap;
import com.wizzer.mle.parts.j2d.sets.I2dSet;
import com.wizzer.mle.parts.roles.Mle2dRole;
import com.wizzer.mle.parts.stages.Mle2dStage;

/**
 * This class implements a Set that can be used for 2.5D applications.
 * 
 * @author Mark S. Millard
 */
public class Mle2dSet extends MleSet implements I2dSet
{
    /** Render callback id (scheduler). */
    protected MleTask m_renderCBId = null;
    /** Resize callback id (event dispatcher). */
    protected IMleCallbackId m_resizeCBId = null;
    /** The <code>MleBitmap</code> to render into. */
    protected MleBitmap m_imageBuffer = null;

    // The properties exposed in the DWP are "position" and "size".

    /**
     * Contains x and y position in screen coordinate space;
     * (0, 0) corresponds to the lower left corner of the window.
     */
    public MlVector2 position = new MlVector2();;

    /** Contains width and height in screen coordinate space. */
    public MlVector2 size = new MlVector2();

    /**
     * The <b>m_headRole</p> is the first Role drawn.  This makes the
     * m_headRole always appear in the bottom layer among all the
     * Roles attached to this Set.  Similarly, the last Role
     * in this list always appears on the top layer.
     */
    protected Mle2dRole m_headRole = null;

    /**
     * This inner class is used to process resize events.
     */
    protected class Mle2dSetResizeCallback extends MleEventCallback
    {
        /**
         * The default constructor.
         */
        public Mle2dSetResizeCallback()
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
    	    
    	    // Get the Set and Stage.
    	    Mle2dSet theSet = (Mle2dSet) clientData;
    	    Mle2dStage theStage = (Mle2dStage) Mle2dStage.getInstance();
    	    
    	    // Let the Set handle the resize event.
    	    result = theSet.handleResizeEvent(theStage);

    	    return result;
    	}
    }
    
    /** The resize event callback. */
    protected Mle2dSetResizeCallback m_resizeEventCB = null;
    
    /**
     * The default constructor.
     * <p>
     * The default position is set to (0,0). The default size is set to 320x480.
     * </p>
     */
    public Mle2dSet()
    {
        // Initialize default property values.
        position.setValue(0, 0);
        size.setValue(320, 480);
       
        // Add the Set to the Stage.
        MleStage theStage = MleStage.getInstance();
        Mle2dSetRenderer renderer = new Mle2dSetRenderer();
        renderer.setName("2D Set Renderer");
        MleTask id = ((Mle2dStage)theStage).addSet(renderer,this);
        
        // Remember the task so we can dispose of it later.
        setRenderCBId(id);
        
        // Create the offscreen buffer using the resize event callback handler.
        handleResizeEvent((Mle2dStage)theStage);
    }

    /* (non-Javadoc)
     * @see com.wizzer.mle.runtime.core.MleSet#init()
     */
    public synchronized void init() throws MleRuntimeException
    {
        // Create a new callback for handling the resize event.
        m_resizeEventCB = new Mle2dSetResizeCallback();
        
        // Insert resize callback into event dispatch manager.
        m_resizeCBId = MleTitle.getInstance().m_theDispatcher.installEventCB(
                MleEventManager.MLE_SIZE,m_resizeEventCB,this);

        // Bump priority of dispatched callback.
        MleTitle.getInstance().m_theDispatcher.changeCBPriority(
            MleEventManager.MLE_SIZE,m_resizeCBId,
            MleEventManager.MLE_RESIZE_SET_PRIORITY);
    }

    /* (non-Javadoc)
     * @see com.wizzer.mle.runtime.core.MleSet#dispose()
     */
    public synchronized void dispose() throws MleRuntimeException
    {
        // Unschedule the set render() function for the stage only.
        if (m_renderCBId != null)
            MleTitle.getInstance().m_theScheduler.deleteTask(MleTitle.g_theSetPhase,m_renderCBId);

        // Uninstall resize event callback.
        if (m_resizeCBId != null)
            MleTitle.getInstance().m_theDispatcher.uninstallEventCB(
                MleEventManager.MLE_SIZE,m_resizeCBId);

        // Delete all attached Roles. The dispose() method of each 2D Role will
        // call this set's detach() method and update the m_headrole pointer.
        // So when the m_headrole is null, all the attached Roles are
        // disposed of.
        while (m_headRole != null)
    	    m_headRole.dispose();

    }

    /* (non-Javadoc)
     * @see com.wizzer.mle.runtime.core.IMleObject#getProperty(java.lang.String)
     */
    public synchronized Object getProperty(String name)
    	throws MleRuntimeException
    {
        if (name != null)
        {
            if (name.equals("position"))
                return position;
            else if (name.equals("size"))
                return size;
        }

        // Specified name does not exist.
        throw new MleRuntimeException("Mle2dSet: Unable to get property " + name + ".");
    }

    /* (non-Javadoc)
     * @see com.wizzer.mle.runtime.core.IMleObject#setProperty(java.lang.String, IMleProp)
     */
    public synchronized void setProperty(String name, IMleProp property)
    	throws MleRuntimeException
    {
        if (name != null)
        {
            try
            {
	            if (name.equals("position"))
	            {
	            	// Remember old value.
	            	MlVector2 oldValue = new MlVector2(position);

	                // Expecting 2 single-precision floating-point values.
	                DataInputStream in = new DataInputStream(property.getStream());
	                float x = in.readFloat();
	                float y = in.readFloat();
	                position.setValue(x,y);
	                
                    // Notify listeners of the change.
	                MlVector2 newValue = new MlVector2(position);
	                this.notifyPropertyChange("position", oldValue, newValue);

	                return;
	                
	            } else if (name.equals("size"))
	            {
	            	// Remember old value.
	            	MlVector2 oldValue = new MlVector2(size);
	            	
	                // Expecting 2 single-precision floating-point values.
	                DataInputStream in = new DataInputStream(property.getStream());
	                float x = in.readFloat();
	                float y = in.readFloat();
	                size.setValue(x,y);
	                
	                // Notify listeners of the change.
	                MlVector2 newValue = new MlVector2(size);
	                this.notifyPropertyChange("size", oldValue, newValue);

	                return;
	            }
            } catch (IOException ex)
            {
                throw new MleRuntimeException("Mle2dSet: Unable to set property " + name + ".");
            }
        }
        
        // Specified name does not exist.
        throw new MleRuntimeException("Mle2dSet: Unable to set property " + name + ".");
    }

    /* (non-Javadoc)
     * @see com.wizzer.mle.runtime.core.IMleObject#setPropertyArray(java.lang.String, int, int, java.io.ByteArrayInputStream)
     */
    public synchronized void setPropertyArray(String name, int length, int nElements, ByteArrayInputStream value)
    	throws MleRuntimeException
    {
        throw new MleRuntimeException("Mle2dSet: Unable to set property array " + name + ".");
    }

    /**
     * Attach <b>newR</b> after <b>curR</b>.  If curR is <b>null</b>,
	 * newR is attached to the end of the list.
	 * 
	 * @param curR The Role to attach to. May be <b>null</b>
	 * @param newR The Role to attach.
	 * 
	 * @throws MleRuntimeException This exception is thrown if <b>newR</b>
	 * is <b>null</b>.
	 */
    public synchronized void attachRoles(MleRole curR, MleRole newR)
    	throws MleRuntimeException
    {
        Mle2dRole currentRole, newRole;

        if (newR == null)
            throw new MleRuntimeException("Mle2dSet: Role may not be null.");
        
        newRole = (Mle2dRole)newR;

        if (curR != null)
    	{
    		// Attach after "curR" Role.
    		currentRole = (Mle2dRole)curR;

    		newRole.m_next = currentRole.m_next;
    		newRole.m_prev = currentRole;
    		currentRole.m_next = newRole;
    		if (newRole.m_next != null)
    			newRole.m_next.m_prev = newRole;
    		
    	} else
    	{
    		// Attach to the end of the list.
    		if (m_headRole != null)
    		{
    			for (currentRole = m_headRole; currentRole.m_next != null; )
    				currentRole = currentRole.m_next;
    			currentRole.m_next = newRole;
    			newRole.m_prev = currentRole;
    			newRole.m_next = null;
    			
    		} else
    		{
    			m_headRole = newRole;
    			newRole.m_next = null;
    			newRole.m_prev = null;
    		}
        }
    }

    /**
     * Detach specified Role from the layer list.
     * 
     * @param curR The Role to detach.
	 * 
	 * @throws MleRuntimeException This exception is thrown if <b>curR</b>
	 * is <b>null</b>.
     */
    public synchronized void detach(MleRole curR)
    	throws MleRuntimeException
    {
        Mle2dRole role;

        if (curR == null)
            throw new MleRuntimeException("Mle2dSet: Role may not be null.");
         
        role = (Mle2dRole)curR;

        if (role == m_headRole)
    	{
    		if (m_headRole.m_next != null)
    			m_headRole.m_next.m_prev = null;
    		m_headRole = m_headRole.m_next;
    		
        } else
    	{
    		role.m_prev.m_next = role.m_next;
    		if (role.m_next != null)
    			role.m_next.m_prev = role.m_prev;
        }
    }
    
    /**
     * This method is used to set the scheduler id for the render method
     * (so that we can remove it during destruction).
     */
    public synchronized void setRenderCBId(MleTask id)
    {
        m_renderCBId = id;
    }
    
    /**
     * Handle the resize event.
     * 
     * @param theStage The Stage associated with this Set.
     * 
     * @return <b>true</b> is returned if the resize event is successfully handled.
     * Otherwise, <b>false</b> is returned.
     */
    public synchronized boolean handleResizeEvent(Mle2dStage theStage)
    {
	    // Retrieve width and height of the Stage.
	    // XXX --  This is not really correct; the width and height should be
	    //         obtained from 2D set properties.
	    MleSize size = theStage.getSize();
	    
	    // Get the Stage's offscreen buffer.
	    MleBitmap stageBuffer = theStage.getPixelBuffer();

	    // Clean up the old offscreen buffer.
	    if (this.m_imageBuffer != null)
	        this.m_imageBuffer.dispose();
	    
	    // Create an offscreen buffer that shares the one used by the Stage.
	    this.m_imageBuffer = new MleBitmap(stageBuffer.getBitmap());
	    
	    return true;
    }

}
