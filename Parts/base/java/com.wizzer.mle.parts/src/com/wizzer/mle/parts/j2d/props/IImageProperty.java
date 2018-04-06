/*
 * IImageProperty.java
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
package com.wizzer.mle.parts.j2d.props;

// Import Magic Lantern Runtime Engine classes.
import com.wizzer.mle.runtime.core.IMleObject;

// Import Magic Lantern Parts classes.
import com.wizzer.mle.parts.j2d.props.IMlePropPart;
import com.wizzer.mle.parts.j2d.mrefs.IImageMediaRef;

/**
 * This interface defines an image property.
 * 
 * @author Mark S. Millard
 */
public interface IImageProperty extends IMlePropPart
{
    /**
     * Get the width of the image.
     * 
     * @param obj A reference to a Magic Lantern Object interface.
     * 
     * @return An integer value is returned.
     */
    public int getWidth(IMleObject obj);
    
    /**
     * Get the height of the image.
     * 
     * @param obj A reference to a Magic Lantern Object interface.
     * 
     * @return An integer value is returned.
     */
    public int getHeight(IMleObject obj);
}
