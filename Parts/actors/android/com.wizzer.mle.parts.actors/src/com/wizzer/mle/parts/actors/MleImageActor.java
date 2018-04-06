/*
 * MleImageActor.java
 * Created on Feb 18, 2005
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
package com.wizzer.mle.parts.actors;

// Import standards Java classes.
import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;

// Import Magic Lantern Runtime Engine classes.
import com.wizzer.mle.runtime.core.MleActor;
import com.wizzer.mle.runtime.core.IMleProp;
import com.wizzer.mle.runtime.core.MleMediaRef;
import com.wizzer.mle.runtime.core.MleRuntimeException;
import com.wizzer.mle.runtime.dpp.MleDppException;
import com.wizzer.mle.runtime.dpp.MleDppLoader;

// Import Magic Lantern Parts classes.
import com.wizzer.mle.parts.props.MleImageProperty;
import com.wizzer.mle.parts.j2d.actors.IImageActor;
import com.wizzer.mle.parts.mrefs.MleImageMediaRef;


/**
 * This class implements an Actor that can process a file
 * and have it displayed as an image.
 * 
 * @author Mark S. Millard
 */
public class MleImageActor extends MleActor implements IImageActor
{
    // The properties exposed in the DWP are "filename".

    /**
     * Contains the name of the image file to display.
     */
    public MleImageProperty filename = null;

    /**
     * The default constructor.
     */
    public MleImageActor()
    {
        super();
    }

    /* (non-Javadoc)
     * @see com.wizzer.mle.runtime.core.MleActor#init()
     */
    public void init() throws MleRuntimeException
    {
        // Update the Role by pushing the property values.
        filename.push(this);
    }

    /* (non-Javadoc)
     * @see com.wizzer.mle.runtime.core.MleActor#dispose()
     */
    public void dispose() throws MleRuntimeException
    {
        // Do nothing for now.
    }

    /* (non-Javadoc)
     * @see com.wizzer.mle.runtime.core.IMleObject#getProperty(java.lang.String)
     */
    public Object getProperty(String name) throws MleRuntimeException
    {
        if (name != null)
        {
            if (name.equals("filename"))
                return filename;
        }

        // Specified name does not exist.
        throw new MleRuntimeException("MleImageActor: Unable to get property " + name + ".");
    }

    /* (non-Javadoc)
     * @see com.wizzer.mle.runtime.core.IMleObject#setProperty(java.lang.String, IMleProp)
     */
    public void setProperty(String name, IMleProp property)
            throws MleRuntimeException
    {
        if (name != null)
        {
            try
            {
	            if (name.equals("filename"))
	            {
	                // Read the data in from the input stream.
	                DataInputStream in = new DataInputStream(property.getStream());
	                byte[] data = new byte[property.getLength()];
	                in.readFully(data);
	                
	                // Create an image property and intialize it.
	                if (property.getType() == IMleProp.PROP_TYPE_MEDIAREF)
	                {
	                    // Assume it's coming from the DPP and data is an index into
	                    // the DPP Table-of-Contents.
	                    Integer index = new Integer(new String(data));
	                    
	                    // Retrieve the name from the DPP.
	                    try
	                    {
	                        filename = new MleImageProperty();
	                        MleMediaRef mref
                                = MleDppLoader.getInstance().mleLoadMediaRef(index.intValue());
	                        filename.setProperty(mref);
	                    } catch (MleDppException ex)
	                    {
	                        throw new MleRuntimeException(ex.getMessage());
	                    }
	                    
	                    // Notify property change listeners.
	                    notifyPropertyChange("filename", null, null);
	                } else
	                {
	                    filename = new MleImageProperty();
	                    MleImageMediaRef mref = new MleImageMediaRef();
	                    mref.registerMedia(0,data.length,data);
	                    filename.setProperty(mref);
	                    
	                    // Notify property change listeners.
	                    notifyPropertyChange("filename", null, null);
	                }
	                
	                return;
	            }
            } catch (IOException ex)
            {
                throw new MleRuntimeException("MleImageActor: Unable to set property " + name + ".");
            }
        }

        // Specified name does not exist.
        throw new MleRuntimeException("MleImageActor: Unable to set property " + name + ".");
    }

    /* (non-Javadoc)
     * @see com.wizzer.mle.runtime.core.IMleObject#setPropertyArray(java.lang.String, int, int, java.io.ByteArrayInputStream)
     */
    public void setPropertyArray(String name, int length, int nElements,
            ByteArrayInputStream value) throws MleRuntimeException
    {
        throw new MleRuntimeException("MleImageActor: Unable to set property array " + name + ".");
    }
    
    /**
     * Get the width of the image.
     * 
     * @return An integer value is returned.
     */
    public int getWidth()
    {
        return filename.getWidth(this);
    }

    /**
     * Get the height of the image.
     * 
     * @return An integer value is returned.
     */
    public int getHeight()
    {
        return filename.getHeight(this);
    }
}
