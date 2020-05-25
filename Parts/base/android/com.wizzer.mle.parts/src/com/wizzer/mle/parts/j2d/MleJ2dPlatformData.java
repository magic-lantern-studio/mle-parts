/*
 * MleJ2dPlatformData.java
 * Created on Feb 14, 2005
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

// Import Android classes.
import android.content.Context;

/**
 * This class encapsulates data that is specific to the Android platform.
 * <p>
 * The Android platform is a Java platform that is compatible with the
 * Android API.
 * </p>
 * 
 * @author Mark S. Millard
 */
public class MleJ2dPlatformData
{
    /** The width of the platform display component. */
    public int m_width = 320;
    /** The height of the platform display component. */
    public int m_height = 480;
    /** The application context for an Android Activity. */
    public Context m_context = null;
    /** The Android Resource class <code>R</code>. */
    public Class m_R = null;

    /**
     * The default constructor.
     */
    public MleJ2dPlatformData()
    {
        super();
    }

}
