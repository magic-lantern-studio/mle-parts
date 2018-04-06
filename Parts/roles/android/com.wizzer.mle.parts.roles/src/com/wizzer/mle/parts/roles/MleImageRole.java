/*
 * MleImageRole.java
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
package com.wizzer.mle.parts.roles;

// Import Android classes.
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;

// Import Magic Lantern Parts classes.
import com.wizzer.mle.parts.j2d.MleBitmap;
import com.wizzer.mle.runtime.core.MleActor;
import com.wizzer.mle.runtime.core.MleRuntimeException;
import com.wizzer.mle.runtime.core.MleSet;

/**
 * This class implements a Role that can display an image.
 * 
 * @author Mark S. Millard
 */
public class MleImageRole extends Mle2dRole
{
    /** The image container. */
    protected MleBitmap m_image = null;

    /**
     * The default constructor.
     */
    public MleImageRole()
    {
        super();
    }

    /**
     * A constructor that binds itself to the specified Actor.
     * 
     * @param actor The Actor associated with this Role.
     */
    public MleImageRole(MleActor actor)
    {
        super(actor);
    }

    /**
     * Render the image into the specified offscreen buffer.
     * 
     * @param buffer The offscreen buffer.
     * 
     * @see com.wizzer.mle.parts.j2d.roles.Mle2dRole#draw(com.wizzer.mle.parts.j2d.MleBitmap)
     */
    public void draw(Canvas buffer)
    {
        //System.out.println("Drawing MLE bitmap image.");

        if (m_image != null)
        {
        	Paint paint = new Paint();
            buffer.drawBitmap(m_image.getBitmap(),0,0, paint);
        }
    }
    
    /**
     * Initialize the Role.
     * <p>
     * There are no side affects.
     * </p>
     */
    public void init()
    {
        // Do nothing for now.
    }
    
    /**
     * Set the image to be managed by this Role.
     * 
     * @param image The <code>Bitmap</code> that this Role will manage.
     */
    public void setImage(Bitmap image)
    {
        // Set the extent of the Role.
        if (image != null)
        {
            float[] min = new float[2];
            float[] max = new float[2];
            min[0] = 0;
            min[1] = 0;
            max[0] = image.getWidth();
            max[1] = image.getHeight();
            setBounds(min,max);
        }
        
        // Create a MLE bitmap image from the loaded image.
        m_image = MleBitmap.makeBitmapImage(image);
    }
    
    /**
     * Get the image managed by this Role.
     * 
     * @return An <code>Bitmap</code> is returned. <b>null</b> may be returned if
     * the image has not yet been specified.
     * 
     * @see setImage(Bitmap image)
     */
    public Bitmap getImage()
    {
        return m_image.getBitmap();
    }

}
