/*
 * IImageActor.java
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
package com.wizzer.mle.parts.j2d.actors;

// Import standards Java classes.

// Import Magic Lantern Runtime Engine classes.

// Import Magic Lantern Parts classes.

/**
 * This class implements an Actor that can process a file
 * and have it displayed as an image.
 * 
 * @author Mark S. Millard
 */
public interface IImageActor
{
    // The properties exposed in the DWP are "filename".
    
    /**
     * Get the width of the image.
     * 
     * @return An integer value is returned.
     */
    public int getWidth();

    /**
     * Get the height of the image.
     * 
     * @return An integer value is returned.
     */
    public int getHeight();
}
