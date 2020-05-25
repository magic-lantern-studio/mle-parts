/*
 * MlebBitmap.java
 * Created on Jan 3, 2008
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
import java.io.InputStream;

//Import Android classes.
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Matrix;
import android.graphics.BitmapFactory;
import android.graphics.Bitmap.Config;

/**
 * This class provides support for a Bitmap that maintains the origin of the image.
 * 
 * @author Mark S. Millard
 */
public class MleBitmap
{
    /** The x coordinate for the origin of the image. */
    public int m_origin_x;
    /** The y coordinate for the origin of the image. */
    public int m_origin_y;
    
	/** A handle to the Bitmap. */
	protected Bitmap m_bitmap = null;
	/** A handle to the Canvas */
	protected Canvas m_canvas = null;
	
	// A lock for accessing resources.
	private boolean m_lock = false;
	
	// Hide the default constructor.
	private MleBitmap() {}
	
	/**
	 * Returns a Magic Lantern bitmap from the source bitmap. The new bitmap will be the same object as the source.
	 * The bitmap will either be mutable or immutable, depending upon the source parameter.
	 * 
	 * @param source The bitmap to replicate.
	 */
	public MleBitmap(Bitmap source)
	{
		m_bitmap = source;

        // Set the default origin to (0,0).
        m_origin_x = 0;
        m_origin_y = 0;
	}
	
	/**
	 * If <i>immutable</i> is <b>true</b>, then this constructor returns an immutable bitmap from the source bitmap.
	 * The new bitmap may be the same object as source, or a copy may have been made. If <i>immutable</i> is <b>false</b>,
	 * then the new bitmap will be the same object as the source.
	 * 
	 * @param source The bitmap to replicate.
	 * @param immutable A flag indicating whether the source will be mutable or not.
	 */
	public MleBitmap(Bitmap source, boolean immutable)
	{
		if (immutable)
			m_bitmap = Bitmap.createBitmap(source);
		else
			m_bitmap = source;

        // Set the default origin to (0,0).
        m_origin_x = 0;
        m_origin_y = 0;
	}
	
	/**
	 * Returns a mutable bitmap with the specified width and height.
	 * 
	 * @param width The width of the bitmap.
	 * @param height The height of the bitmap.
	 * @param config Bitmap configuration (i.e. ARGB_8888.
	 */
	public MleBitmap(int width, int height, Bitmap.Config config)
	{
		m_bitmap = Bitmap.createBitmap(width, height, config);

        // Set the default origin to (0,0).
        m_origin_x = 0;
        m_origin_y = 0;
	}
	
	/**
	 * Returns an immutable bitmap from the specified subset of the source bitmap.
	 * The new bitmap may be the same object as source, or a copy may have been made.
	 * 
	 * @param source The bitmap to subset.
	 * @param x The x coordinate of the first pixel in source.
	 * @param y The y coordinate of the first pixel in source.
	 * @param width The number of pixels in each row.
	 * @param height The number of rows.
	 */
	public MleBitmap(Bitmap source, int x, int y, int width, int height)
	{
		m_bitmap = Bitmap.createBitmap(source, x, y, width, height);

        // Set the default origin to (0,0).
        m_origin_x = 0;
        m_origin_y = 0;
	}
	
	/**
	 * Returns an immutable bitmap with the specified width and height,
	 * with each pixel value set to the corresponding value in the colors array.
	 * 
	 * @param colors An array of <i>Color</i> used to initialize the pixels. This array must be at
	 * least as large as width * height.
	 * @param width The width of the bitmap.
	 * @param height The height of the bitmap.
	 * @param config The bitmap configuration to create.
	 */
	public MleBitmap(int[] colors, int width, int height, Config config)
	{
		m_bitmap = Bitmap.createBitmap(colors, width, height, config);

        // Set the default origin to (0,0).
        m_origin_x = 0;
        m_origin_y = 0;
	}
	
	/**
	 * Returns an immutable bitmap from the specified subset of the source bitmap.
	 * The new bitmap may be the same object as source, or a copy may have been made.
	 * 
	 * @param source The bitmap to subset.
	 * @param x The x coordinate of the first pixel in source.
	 * @param y The y coordinate of the first pixel in source.
	 * @param width The number of pixels in each row.
	 * @param height The number of rows.
	 * @param m The transformation matrix.
	 * @param filter set to <b>true</b> if the source should be filtered.
	 * Only applies if the matrix contains more than just translation.
	 */
	public MleBitmap(Bitmap source, int x, int y, int width, int height, Matrix m, boolean filter)
	{
		m_bitmap = Bitmap.createBitmap(source, x, y, width, height, m, filter);

        // Set the default origin to (0,0).
        m_origin_x = 0;
        m_origin_y = 0;
	}
	
	/**
	 * Get the associated <code>Bitmap</code>.
	 * 
	 * @return The <code>android.graphics.Bitmap</code> is returned.
	 */
	public Bitmap getBitmap()
	{
		return m_bitmap;
	}
	
	/**
	 * Get the associated <i>Canvas</i>
	 * 
	 * @return The <code>android.graphics.Canvas</code> is returned. May be <b>null</b>.
	 */
	public Canvas getCanvas()
	{
		return m_canvas;
	}
	
	/**
	 * Set the associated <code>Canvas</code>.
	 * 
	 * @param canvas The <code>android.graphics.Canvas</code> to set.
	 */
	public void setCanvas(Canvas canvas)
	{
		m_canvas = canvas;
		m_canvas.setBitmap(m_bitmap);
	}
	
	/**
	 * Lock the Bitmap resources.
	 */
	public void lock()
	{
		m_lock = true;
	}
	
	/**
	 * Unlock the Bitmap resources.
	 */
	public void unlock()
	{
		m_lock = false;
	}
	
	/**
	 * Check to see if the Bitmap is locked.
	 * 
	 * @return <b>true</b> will be returned if the Bitmap is locked.
	 * Otherwise <b>false</b> will be returned.
	 */
	public boolean isLocked()
	{
		return m_lock;
	}
	
	/**
	 * Release any system resources.
	 */
	public void dispose()
	{
		// Does nothing for now.
	}
	
    /**
     * Load an image from a file.
     * <p>
     * This method will not return until the image data is fully loaded.
     * </p>
     * 
     * @param filename The name of the file to load.
     * 
     * @return If the image is successfully loaded, then a
     * <code>Bitmap</code> will be returned. If the image is not successfully loaded,
     * then <b>null</b> will be returned.
     */
	public static Bitmap blockingLoad(String filename)
	{
		Bitmap bitmap = BitmapFactory.decodeFile(filename);
		return bitmap;
	}
	
	/**
	 * Load an image from an Android resource.
     * <p>
     * This method will not return until the image data is fully loaded.
     * </p>
	 * 
	 * @param resources The application's resources.
	 * @param id A resource identifier.
	 * 
	 * @return If the image is successfully loaded, then a
     * <code>Bitmap</code> will be returned. If the image is not successfully loaded,
     * then <b>null</b> will be returned.
	 */
	public static Bitmap blockingLoad(Resources resources, int id)
	{
		Bitmap bitmap = BitmapFactory.decodeResource(resources, id);
		return bitmap;
	}
	
	/**
	 * Load an image from an input stream.
     * <p>
     * This method will not return until the image data is fully loaded.
     * </p>
	 * 
	 * @param is The input stream.
	 * 
	 * @return If the image is successfully loaded, then a
     * <code>Bitmap</code> will be returned. If the image is not successfully loaded,
     * then <b>null</b> will be returned.
	 */
	public static Bitmap blockingLoad(InputStream is)
	{
		Bitmap bitmap = BitmapFactory.decodeStream(is);
		return bitmap;
	}
	
	/**
     * Create a <code>MleBitmap</code> from the specified source.
     * 
     * @param source The <code>Bitmap</code> to create a <code>MleBitmap</code>
     * from.
     * 
     * @return A <code>MleBitmap</code> is returned upon success.
     * Otherwise, <b>null</b> will be returned.
     */
	public static MleBitmap makeBitmapImage(Bitmap source)
	{
		return new MleBitmap(source);
	}
}
