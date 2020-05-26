/*
 * MleImageProperty.java
 * Created on Feb 23, 2005
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
package com.wizzer.mle.parts.props;

// Import Magic Lantern Runtime Engine classes.
import com.wizzer.mle.runtime.core.IMleObject;
import com.wizzer.mle.runtime.core.MleActor;
import com.wizzer.mle.runtime.core.MleRole;
import com.wizzer.mle.runtime.core.MleMediaRef;
import com.wizzer.mle.runtime.core.MleRuntimeException;

// Import Magic Lantern Parts classes.
import com.wizzer.mle.parts.j2d.props.IMlePropPart;
import com.wizzer.mle.parts.j2d.props.IImageProperty;
import com.wizzer.mle.parts.roles.MleImageCarrier;

/**
 * This class implements an image property.
 * 
 * @author Mark S. Millard
 */
public class MleImageProperty implements IImageProperty
{
    // The media reference managed by the property.
    private MleMediaRef m_image = null;

    /**
     * The default constructor.
     */
    public MleImageProperty()
    {
        super();
    }
    
    /**
     * Get the property value.
     * 
     * @return A <code>MleMediaRef</code> is returned.
     */
    public MleMediaRef getProperty()
    {
        return m_image;
    }
    
    /**
     * Set the property value.
     * 
     * @param reference This argument should be a <code>MleMediaRef</code>.
     */
    public void setProperty(MleMediaRef reference)
    {
    	m_image = reference;
    }

    /**
     * Push the property value to a corresponding element in the
     * system. The object implementing this interface inherently
     * knows how the property value will be pushed.
     * 
     * @param obj The Magic Lantern object containing the properties
     * being pushed. In most cases, this will be a <code>MleActor</code>.
     * 
     * @return <b>true</b> is returned if the property was successfully
     * pushed. Otherwise, <b>false</b> will be returned.
     * 
     * @throws MleRuntimeException This exception is thrown if an
     * error occurs while pushing the property value.
     * 
     * @see com.wizzer.mle.parts.j2d.props.IMlePropPart#push(com.wizzer.mle.runtime.core.IMleObject)
     */
    public boolean push(IMleObject obj)
        throws MleRuntimeException
    {
        MleActor actor = (MleActor) obj;
        MleImageCarrier.set(actor.getRole(),m_image);
        
        return true;
    }

    /**
     * Pull the property value from a corresponding element in the
     * system. The object implementing this interface inherently
     * knows how the property value will be pulled.
     * 
     * @param obj The Magic Lantern object containing the properties
     * being updated. In most cases, this will be a <code>MleActor</code>.
     * 
     * @return <b>true</b> is returned if the property was successfully
     * pulled. Otherwise, <b>false</b> will be returned.
     * 
     * @throws MleRuntimeException This exception is thrown if an
     * error occurs whil pulling the property value.
     * 
     * @see com.wizzer.mle.parts.j2d.props.IMlePropPart#pull(com.wizzer.mle.runtime.core.IMleObject)
     */
    public boolean pull(IMleObject obj)
        throws MleRuntimeException
    {
        MleRole role = (MleRole) obj;

        return false;
    }

    /**
     * Determine if this property is equal to the one passed in through
     * the argument <b>property</b>.
     * 
     * @param property The <code>IMlePropPart</code> to test for equality.
     * 
     * @return <b>true</b> is returned if the properties are equal.
     * Otherwise, <b>false</b> will be returned.
     * 
     * @see com.wizzer.mle.parts.j2d.props.IMlePropPart#equals(com.wizzer.mle.parts.j2d.props.IMlePropPart)
     */
    public boolean equals(IMlePropPart property)
    {
        boolean retValue = false;
        
        if (property instanceof MleImageProperty)
        {
            MleImageProperty tmp = (MleImageProperty)property;
            if (tmp.m_image.equals(this.m_image))
                retValue = true;
        }
        
        return retValue;
    }

    /**
     * Get the width of the image.
     * 
     * @return An integer value is returned.
     */
    public int getWidth(IMleObject obj)
    {
        MleActor actor = (MleActor) obj;
        return MleImageCarrier.getWidth(actor.getRole());
    }
    
    /**
     * Get the height of the image.
     * 
     * @return An integer value is returned.
     */
    public int getHeight(IMleObject obj)
    {
        MleActor actor = (MleActor) obj;
        return MleImageCarrier.getHeight(actor.getRole());
    }
}
