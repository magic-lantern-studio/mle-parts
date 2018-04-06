/*
 * MleShutdownCallback.java
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
package com.wizzer.mle.parts;

// Import Magic Lantern Runtime Engine classes.
import com.wizzer.mle.runtime.core.MleStage;
import com.wizzer.mle.runtime.core.MleRuntimeException;
import com.wizzer.mle.runtime.event.MleEvent;
import com.wizzer.mle.runtime.event.MleEventCallback;
import com.wizzer.mle.runtime.event.MleEventManager;

/**
 * This class implements an event callback that can be used
 * for shutting down a Magic Lantern title.
 * 
 * @author Mark S. Millard
 */
public class MleShutdownCallback extends MleEventCallback
{
    /**
     * The default constructor.
     */
    public MleShutdownCallback()
    {
        super();
    }

    /**
     * Dispatch the event.
     * 
     * @param event The event that caused this callback to be invoked.
     * @param clientData The client data associated with the event.
     * 
     * @return <b>true</b> will be returned if the event was successfully
     * dispatched. Otherwise. <b>false</b> will be returned.
     * 
     * @see com.wizzer.mle.runtime.event.IMleEventCallback#dispatch(com.wizzer.mle.runtime.event.MleEvent, java.lang.Object)
     */
    public boolean dispatch(MleEvent event, Object clientData)
    {
        boolean retValue = false;
        try
        {
            MleStage.getInstance().dispose();
            MleEventManager.setExitStatus(true);
            retValue = true;
        } catch (MleRuntimeException ex)
        {
            System.out.println("Unable to dispatch MleShutdownCallback.");
        }

        return retValue;
    }

}
