/*
 * MleBounds.java
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
package com.wizzer.mle.parts;

/**
 * This class encapsulates a rectangular boundary region.
 * 
 * @author Mark S. Millard
 */
public class MleBounds
{
    /** The minimum (signed) int value. */
    public static int INT_MIN = (-2147483647 - 1);
    /** The maximum (signed) int value. */
    public static int INT_MAX = 2147483647;

    /** Lower bounds x coordinate value. */
    public int m_min_x = 0;
    /** Lower bounds y coordinate value. */
    public int m_min_y = 0;
    /** Upper bounds x coordinate value. */
    public int m_max_x = 0;
    /** Upper bounds y coordinate value. */
    public int m_max_y = 0;
    
    /**
     * The default constructor.
     */
    public MleBounds()
    {
        super();
    }

    /**
     * A constructor that initializes from the specified
     * boundary region, <i>bounds</i>.
     * 
     * @param bounds The boundary region to copy.
     */
    public MleBounds(MleBounds bounds)
    {
        super();
        
        m_min_x = bounds.m_min_x;
        m_min_y = bounds.m_min_y;
        m_max_x = bounds.m_max_x;
        m_max_y = bounds.m_max_y;
    }

}
