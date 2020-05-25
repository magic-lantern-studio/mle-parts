/*
 * I2dSet.java
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
package com.wizzer.mle.parts.j2d.sets;

// Import Magic Lantern Runtime Engine classes.
import com.wizzer.mle.runtime.core.MleRole;
import com.wizzer.mle.runtime.core.MleRuntimeException;

/**
 * This interface defines a Set that can be used for 2.5D applications.
 * 
 * @author Mark S. Millard
 */
public interface I2dSet
{
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
    public void attachRoles(MleRole curR, MleRole newR)
    	throws MleRuntimeException;
    
    /**
     * Detach specified Role from the layer list.
     * 
     * @param curR The Role to detach.
	 * 
	 * @throws MleRuntimeException This exception is thrown if <b>curR</b>
	 * is <b>null</b>.
     */
    public void detach(MleRole curR) throws MleRuntimeException;
}
