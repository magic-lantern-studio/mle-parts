/*
 * I2dRole.java
 * Created on Jul 11, 2007
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
package com.wizzer.mle.parts.j2d.roles;

/**
 * This interface defines a 2.5D role.
 * 
 * @author mmillard
 */
public interface I2dRole
{
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
    public void getBounds(float min[], float max[]);
    
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
    public void setBounds(float min[], float max[]);
}
