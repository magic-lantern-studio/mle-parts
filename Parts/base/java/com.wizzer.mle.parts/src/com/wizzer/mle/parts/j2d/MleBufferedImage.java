/*
 * MleBufferedImage.java
 * Created on Feb 11, 2005
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
package com.wizzer.mle.parts.j2d;

// Import standard Java classes.
import java.awt.image.BufferedImage;
import java.awt.image.ColorModel;
import java.awt.image.IndexColorModel;
import java.awt.image.WritableRaster;
import java.awt.Component;
import java.awt.MediaTracker;
import java.awt.Image;
import java.awt.Toolkit;
import java.awt.Graphics2D;
import java.net.URL;
import java.util.Hashtable;

// Import Magic Lantern Math classes.


/**
 * This class provides a BufferedImage that maintains the origin of the image.
 * 
 * @author Mark S. Millard
 */
public class MleBufferedImage extends BufferedImage
{
    /** The x coordinate for the origin of the image. */
    public int m_origin_x;
    /** The y coordinate for the origin of the image. */
    public int m_origin_y;
    

    /**
     * Constructs a BufferedImage of one of the predefined image types.
     * The ColorSpace for the image is the default sRGB space.
     * 
     * @param width The width of the created image.
     * @param height The height of the created image.
     * @param imageType The image type of the created image.
     * 
     * @see java.awt.image.BufferedImage
     */
    public MleBufferedImage(int width, int height, int imageType)
    {
        super(width, height, imageType);
        
        // Set the default origin to (0,0).
        m_origin_x = 0;
        m_origin_y = 0;
    }

    /**
     * Constructs a BufferedImage of one of the predefined image types:
     * TYPE_BYTE_BINARY or TYPE_BYTE_INDEXED.
     * <p>
     * If the image type is TYPE_BYTE_BINARY, the number of entries in the color model
     * is used to determine whether the image should have 1, 2, or 4 bits per pixel.
     * If the color model has 1 or 2 entries, the image will have 1 bit per pixel.
     * If it has 3 or 4 entries, the image with have 2 bits per pixel.
     * If it has between 5 and 16 entries, the image will have 4 bits per pixel.
     * Otherwise, an IllegalArgumentException will be thrown.
     * </p>
     * 
     * @param width The width of the created image.
     * @param height The height of the created image.
     * @param imageType The image type of the created image.
     * @param cm The IndexColorModel of the created image.
     * 
     * @see java.awt.image.BufferedImage
     */
    public MleBufferedImage(int width, int height, int imageType, IndexColorModel cm)
    {
        super(width, height, imageType, cm);
        
        // Set the default origin to (0,0).
        m_origin_x = 0;
        m_origin_y = 0;
    }

    /**
     * Constructs a new BufferedImage with a specified ColorModel and Raster.
     * <p>
     * If the number and types of bands in the SampleModel of the Raster do not
     * match the number and types required by the ColorModel to represent its
     * color and alpha components, a RasterFormatException is thrown.
     * This method can multiply or divide the color Raster data by alpha to match
     * the alphaPremultiplied state in the ColorModel.
     * Properties for this BufferedImage can be established by passing in a Hashtable
     * of String/Object pairs.
     * </p>
     * 
     * @param cm ColorModel for the new image.
     * @param raster Raster for the image data.
     * @param isRasterPremultiplied If true, the data in the raster has been premultiplied with alpha.
     * @param properties Hashtable of String/Object pairs.
     * 
     * @see java.awt.image.BufferedImage
     */
    public MleBufferedImage(ColorModel cm, WritableRaster raster, boolean isRasterPremultiplied,
            Hashtable properties)
    {
        super(cm, raster, isRasterPremultiplied, properties);
        
        // Set the default origin to (0,0).
        m_origin_x = 0;
        m_origin_y = 0;
    }

    /**
     * Dispose of <code>Image</code> resources.
     */
    public synchronized void dispose()
    {
        // Do thing for now.
    }
    
    // A proxy for an ImageObserver.
    private static final Component g_component = new Component() {};
    // A tracker for loading images.
    private static final MediaTracker g_tracker = new MediaTracker(g_component);
    // A unique image identifier.
    private static int g_ID = 0;
    
    /**
     * Wait for a specified image to load.
     * 
     * @param image The <code>Image</code> to load.
     * 
     * @return If the Image loads successfully, then <b>true</b> is returned.
     * Otherwise, <b>false</b> will be returned.
     */
    public static boolean waitForImage(Image image)
    {
        int id;
        
        // Bump the Image identifier.
        synchronized(g_component) {id = g_ID++; }
        
        // Add the image to the tracker.
        g_tracker.addImage(image, id);
        
        // Wait for the image to load.
        try
        {
            g_tracker.waitForID(id);
        } catch (InterruptedException ex)
        {
            return false;
        }
        
        // Check for errors.
        if (g_tracker.isErrorID(id))
            return false;
            
        return true;
    }
    
    /**
     * Load an image from a file.
     * <p>
     * This method will not return until the image data is fully loaded.
     * </p>
     * 
     * @param path The name of the file to load.
     * 
     * @return If the image is successfully loaded, then an
     * <code>Image</code> will be returned. If the image is no successfully loaded,
     * then <b>null</b> will be returned.
     */
    public static Image blockingLoad(String path)
    {
        Image image = Toolkit.getDefaultToolkit().getImage(path);
        if (waitForImage(image) == false) return null;
        return image;
    }
    
    /**
     * Load an image from a URL.
     * <p>
     * This method will not return until the image data is fully loaded.
     * </p>
     * 
     * @param url The name of the URL to load.
     * 
     * @return If the image is successfully loaded, then an
     * <code>Image</code> will be returned. If the image is no successfully loaded,
     * then <b>null</b> will be returned.
     */    
    public static Image blockingLoad(URL url)
    {
        Image image = Toolkit.getDefaultToolkit().getImage(url);
        if (waitForImage(image) == false) return null;
        return image;
    }

    /**
     * Create a <code>MleBufferedImage</code> from the specified image.
     * <p>
     * A default image type of <b>TYPE_INT_ARGB</b> will be used.
     * </p>
     * 
     * @param image The <code>Image</code> to create a <code>MleBufferedImage</code>
     * from.
     * 
     * @return A <code>MleBufferedImage</code> is returned upon success.
     * Otherwise, <b>null</b> will be returned.
     */
    public static MleBufferedImage makeBufferedImage(Image image)
    {
        return makeBufferedImage(image, MleBufferedImage.TYPE_INT_ARGB);
    }
    
    /**
     * Create a <code>MleBufferedImage</code> from the specified image using the
     * specified image type.
     * 
     * @param image The <code>Image</code> to create a <code>MleBufferedImage</code>
     * from.
     * @param imageType The tyep of <code>MleBufferedImage</code> to create.
     * 
     * @return A <code>MleBufferedImage</code> is returned upon success.
     * Otherwise, <b>null</b> will be returned.
     */
    public static MleBufferedImage makeBufferedImage(Image image, int imageType)
    {
        // Load the image.
        if (waitForImage(image) == false)
            return null;
        
        MleBufferedImage bufferedImage = new MleBufferedImage(
            image.getWidth(null), image.getHeight(null), imageType);
        Graphics2D g = bufferedImage.createGraphics();
        g.drawImage(image, null, null);
        return bufferedImage;
    }
}
