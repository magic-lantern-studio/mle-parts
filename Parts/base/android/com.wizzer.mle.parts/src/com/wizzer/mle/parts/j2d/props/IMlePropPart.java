/*
 * IMlePropPart.java
 * Created on Dec 1, 2004
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
package com.wizzer.mle.parts.j2d.props;

// Import Magic Lantern Runtime Engine classes.
import com.wizzer.mle.runtime.core.IMleObject;
import com.wizzer.mle.runtime.core.MleRuntimeException;

/**
 * This class provides a common interface for Magic Lantern properties.
 * <p>
 * The interface provids a common mechanism for property values to be
 * transported between objects in the Magic Lantern Runtime Engine.
 * In general, the methods provided by <code>IMlePropPart</code> are used
 * by Actors to communicate with thier respective Roles.
 * </p>
 * 
 * @author Mark S. Millard
 */
public interface IMlePropPart
{
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
     * error occurs whil pushing the property value.
     */
    public boolean push(IMleObject obj) throws MleRuntimeException;
    
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
     */
    public boolean pull(IMleObject obj) throws MleRuntimeException;
    
    /**
     * Determine if this property is equal to the one passed in through
     * the argument <b>property</b>.
     * 
     * @param property The <code>IMlePropPart</code> to test for equality.
     * 
     * @return <b>true</b> is returned if the properties are equal.
     * Otherwise, <b>false</b> will be returned.
     */
    public boolean equals(IMlePropPart property);
    
    /**
     * Get a string representation of this property.
     * 
     * @return A <code>String</code> is returned.
     */
    public String toString();
}
