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

// Import Android classes.
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.util.Log;

// Import Magic Lantern Runtime Engine classes.
import com.wizzer.mle.runtime.MleTitle;
import com.wizzer.mle.runtime.ResourceManager;
import com.wizzer.mle.runtime.core.MleMediaRef;
import com.wizzer.mle.runtime.core.MleRuntimeException;

// Import Magic Lantern Parts classes.
import com.wizzer.mle.parts.j2d.MleBitmap;
import com.wizzer.mle.parts.j2d.MleJ2dPlatformData;
import com.wizzer.mle.parts.j2d.mrefs.IImageMediaRef;

/**
 * This class implements a Media Reference that can load images
 * from a reference that specifies a file name.
 * 
 * @author Mark S. Millard
 */
public class MleImageMediaRef extends MleMediaRef implements IImageMediaRef
{
	/** Handle to Android resources. */
	protected Resources m_resources = null;
	/** Handle to Android <code>R</code> class for retrieving resource identifiers. */
	protected Class m_R = null;
	
    /**
     * The default constructor.
     */
    public MleImageMediaRef()
    {
        super();
        
        // Set the handle to the Android title resources.
        MleJ2dPlatformData platformData = (MleJ2dPlatformData)(MleTitle.getInstance().m_platformData);
        Context appContext = platformData.m_context;
        m_resources = appContext.getResources();
        m_R = platformData.m_R;
    }
    
    /**
     * Read the image data from a local file name.
     * 
     * @return The loaded image is returned as an <code>Image</code>.
     * 
     * @throws MleRuntimeException This exception is thrown if the
     * meida reference data can not be successfully read.
     */
    public Bitmap read()
        throws MleRuntimeException
    {
        Bitmap image = null;
        
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
    			try
    			{
    	        // Attempt to load the file from the resource cache.
    			String subclassName = m_R.getName() + "$raw";
    			Class subclass = Class.forName(subclassName);
    	        int id = ResourceManager.getResourceId(subclass, filename);
    	        image = MleBitmap.blockingLoad(m_resources, id);
    			} catch (ClassNotFoundException ex)
    			{
    				Log.e(MleTitle.DEBUG_TAG, ex.getMessage());
    				image = null;
    			}
     		}
        }
        
        return image;
    }
}
