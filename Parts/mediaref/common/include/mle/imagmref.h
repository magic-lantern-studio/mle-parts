/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file imagmref.h
 * @ingroup MleParts
 *
 * This file defines the class for an Image Media Reference.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
//  Copyright (C) 2000-2011  Wizzer Works
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

#ifndef __MLE_IMAGMREF_H_
#define __MLE_IMAGMREF_H_

// Include runtime player header files.
#include "mle/mlTypes.h"
#include "mle/MleMediaRef.h"
#include "mle/Mle2dImageMediaRef.h"

#if defined(WIN32)
#if defined(MLE_REHEARSAL)
// Use FreeImage library for Rehearsal Player on Windows.
#include <FreeImage.h>
#else /* ! MLE_REHEARSAL */
#include "mle/dib.h"
#endif
#else /* ! WIN32 */
// Use FreeImage library.
#include <FreeImage.h>
#endif


/**
 * @brief This class implements a Media Reference loader for a still image.
 */
class MLE_2DIMAGEMEDIAREF_API MleImageMediaRef : public MleMediaRef
{
    MLE_MEDIAREF_HEADER(MleImageMediaRef);

#if defined(__linux__) || defined(MLE_REHEARSAL)

  private:

    /** The IO handler for FreeImage. */
    FreeImageIO m_io;

#endif /* MLE_REHEARSAL */

  public:

    MleImageMediaRef(void);

    virtual ~MleImageMediaRef(void);

#if defined(__linux__) || defined(MLE_REHEARSAL)

    MlBoolean read(FIBITMAP *&inImg);

#elif defined(WIN32)

    MlBoolean read(MleDIB &dib);

#endif /* WIN32 */
};


#endif /* __MLE_IMAGMREF_H_ */
