/*
 * MleRenderEngine.java
 * Created on Feb 14, 2005
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
package com.wizzer.mle.parts.j2d;

// Import Magic Lantern Parts classes.

/**
 * This class implements the base class for a render engine on the
 * Java2D platform.
 * 
 * @author Mark S. Millard
 */
public abstract class MleRenderEngine extends Thread
{
    /** Call data */
    protected Object m_callData = null;
    /** Client data. */
    protected Object m_clientData = null;

    /**
     * The default constructor.
     */
    public MleRenderEngine()
    {
        super();
    }

    /**
     * A constructor that initializes the name of the renderer thread.
     * 
     * @param name The name of the renderer.
     */
    public MleRenderEngine(String name)
    {
        super(name);
    }
    
    /**
     * Set the render engine's call data.
     * 
     * @param data Generic data for a callback architecture.
     */
    public void setCallData(Object data)
    {
        m_callData = data;
    }

    /**
     * Get the render engine's call data.
     * 
     * @return The call data is returned. It may be <b>null</b>.
     */
    public Object getCallData()
    {
        return m_callData;
    }

    /**
     * Set the render engine's client data.
     * 
     * @param data Generic data for a callback architecture.
     */
    public void setClientData(Object data)
    {
        m_clientData = data;
    }
    
    /**
     * Get the render engine's client data.
     * 
     * @return The client data is returned. It may be <b>null</b>.
     */
    public Object getClientData()
    {
        return m_clientData;
    }
}
