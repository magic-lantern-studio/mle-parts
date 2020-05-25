/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file imagmref.cxx
 * @ingroup MleParts
 *
 * This file implements the class for an Image Media Reference
 * targeting the BRender platform.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
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

// Include system header files
#if defined(WIN32)
#include <windows.h>
#elif defined(MLE_REHEARSAL) || defined(__sgi)
#include <il/ilConfigure.h>
#include <il/ilFileImg.h>
#endif /* WIN32 */

// Include runtime player header files
#include "mle/imagmref.h"
#include "mle/mlFileIO.h"


MLE_MEDIAREF_SOURCE(MleImageMediaRef,MleMediaRef);


MleImageMediaRef::MleImageMediaRef()
{
#if defined(MLE_REHEARSAL) || defined(__sgi)
    // Need to turn off hardware acceleration so IL won't make any
    // Iris gl call.  Also turn off multithreading in IL.
#if 0
    ilHwAccelerate(ilHwNone);
    ilMpSetMaxProcs(0, 0);
#endif
    ilHwSetGlobalEnable(ilHwAccelOff);
    ilMpSetMaxComputeThreads(0);
#endif /* MLE_REHEARSAL or __sgi */
}


MleImageMediaRef::~MleImageMediaRef()
{
    // Do nothing
}


#if defined(WIN32)
MlBoolean MleImageMediaRef::read(MleDIB &dib)
{
    // Declare local variables
    FILE *fp;
    BOOL status = FALSE;
    char *buffer = m_references->buffer;

    // open DIB file
    // Note: Microsoft distinguishes between text and binary mode;
    //       make sure the file is opened in binary mode so that
    //       translations are suppressed.
    if (buffer)
	{
        // Invoke the converter to prepare the local file
	    m_converter->setReference(buffer);
	    char *filename = m_converter->getFilename();

	    // TRUE return means we downloaded successfully to buffer
    	if (m_converter->getFile())
		{
	        fp = mlFOpen(filename,"rb");
	        if (fp)
			{
	            status = TRUE;
			}
        }
    }
    if (FALSE == status)
	{
        return(FALSE);
    }

    // Load DIB file
    status = dib.load(fp);

    // Close DIB file and return
    mlFClose(fp);
    return(status);
}


#elif defined(MLE_REHEARSAL) || defined(__sgi)

MlBoolean
MleImageMediaRef::read(ilFileImg *&inImg)
{
    // Declare local variables
    char *filename;
    MlBoolean status = FALSE;
    char *buffer = m_references->buffer;

    if (buffer) 
	{
		// Invoke the converter to prepare the local file.
		//
		m_converter->setReference(buffer);
		filename = m_converter->getFilename();

		//
		// TRUE return means we downloaded successfully to buffer.
		//
		if (m_converter->getFile())
		{
			//
			// Open the image file and return the file image handle.
			//
#if 0
		    inImg = ilOpenImgFile(filename, "r");
#endif
			inImg = new ilFileImg(filename);
			if(inImg)
			    status = TRUE;
		}
    }

    return(status);
}
#endif /* WIN32 */
