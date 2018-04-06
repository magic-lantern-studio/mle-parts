/*
 * MleImageMediaRef.java
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
package com.wizzer.mle.parts.mrefs;

// Import standard Java classes.
import java.awt.Image;

// Import Magic Lantern Runtime Engine classes.
import com.wizzer.mle.runtime.core.MleMediaRef;
import com.wizzer.mle.runtime.core.MleRuntimeException;

// Import Magic Lantern Parts classes.
import com.wizzer.mle.parts.j2d.MleBufferedImage;
import com.wizzer.mle.parts.j2d.mrefs.IImageMediaRef;

/**
 * This class implements a Media Reference that can load images
 * from a reference that specifies a file name.
 * 
 * @author Mark S. Millard
 */
public class MleImageMediaRef extends MleMediaRef implements IImageMediaRef
{
    /**
     * The default constructor.
     */
    public MleImageMediaRef()
    {
        super();
    }
    
    /**
     * Read the image data from a local file name.
     * 
     * @return The loaded image is returned as an <code>Image</code>.
     * 
     * @throws MleRuntimeException This exception is thrown if the
     * meida reference data can not be successfully read.
     */
    public Image read()
        throws MleRuntimeException
    {
        Image image = null;
        
        // Get the data associated with the media reference.
        byte[] buffer = m_references.m_buffer;
        
        // Read Image from external reference.
        if (buffer != null)
    	{
            // Set the reference for the converter.
    		m_converter.setReference(buffer);
    		
    		// Invoke the converter to prepare the local file.
    		String filename = m_converter.getFilename();

    		// true return means we downloaded successfully to a
    		// local file referred to by filename.
    		if (m_converter.conversionComplete())
    		{
    	        // Attempt to load the file.
    	        if (filename.endsWith(".gif"))
    	        {
    	            // Load a GIF image.
    	            image = MleBufferedImage.blockingLoad(filename);
    	        } else if (filename.endsWith(".png"))
    	        {
    	            // Load a PNG image.
    	            image = MleBufferedImage.blockingLoad(filename);
    	        } else if ((filename.endsWith(".jpg")) || (filename.endsWith(".jpeg")))
    	        {
    	            // Loag a JPEG image.
    	            image = MleBufferedImage.blockingLoad(filename);
    	        }
    		}
        }
        
        return image;
    }

}
