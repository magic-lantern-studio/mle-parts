/*
 * MleImageCarrier.java
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
package com.wizzer.mle.parts.roles;

// Import Android classes.
import android.graphics.Bitmap;

// Import Magic Lantern Runtime Engine classes.
import com.wizzer.mle.runtime.core.MleRole;
import com.wizzer.mle.runtime.core.MleMediaRef;
import com.wizzer.mle.runtime.core.MleRuntimeException;

// Import Magic Lantern Parts classes.
import com.wizzer.mle.parts.mrefs.MleImageMediaRef;

/**
 * This class implements a carrier for propagating image properties
 * between Actors and Roles.
 * 
 * @author Mark S. Millard
 */
public class MleImageCarrier
{
    /**
     * Read the image from the specified Media Reference and update
     * the specified Role.
     * 
     * @param role The Role to update.
     * @param reference The Media Reference to obtain the image from.
     * 
     * @return If the image is successfully set on the Role, then
     * <b>true</b> will be returned. Otherwise, <b>false</b> will
     * be returned.
     * 
     * @throws MleRuntimeException This exception is thrown if
     * the specified parameters are <b>null</b>. It will also be
     * thrown if an error occurs while setting the property.
     */
    public static final boolean set(MleRole role,MleMediaRef reference)
        throws MleRuntimeException
    {
        boolean retValue = false;
        
        if ((role == null) || (reference == null))
            throw new MleRuntimeException("MleImageCarrier: Invalid input arguments.");
        
        MleImageRole imgRole = (MleImageRole) role;
        MleImageMediaRef imgReference = (MleImageMediaRef) reference;
        
        // Read the image using the Media Reference.
        Bitmap image = imgReference.read();

        // Set the image on the Role.
        if (image != null)
        {
            imgRole.setImage(image);
            retValue = true;
        }

        return retValue;
    }
    
    /**
     * Get the width of the image.
     * 
     * @return An integer value is returned.
     */
    public static int getWidth(MleRole role)
    {
        MleImageRole imgRole = (MleImageRole) role;
        float[] min = new float[2];
        float[] max = new float[2];
        imgRole.getBounds(min,max);
        int width = (int)(max[0] - min[0]);

        return width;
    }

    /**
     * Get the height of the image.
     * 
     * @return An integer value is returned.
     */
    public static int getHeight(MleRole role)
    {
        MleImageRole imgRole = (MleImageRole) role;
        float[] min = new float[2];
        float[] max = new float[2];
        imgRole.getBounds(min,max);
        int height = (int)(max[1] - min[1]);

        return height;
    }
    
    // Hide the default constructor.
    private MleImageCarrier() {}
}
