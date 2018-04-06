/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file Mle3dSet.h
 * @ingroup MleParts
 *
 * This file contains generic support for a 3D Set implemented
 * as a Windows DLL.
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

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MLE3DSET_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MLE3DSET_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef MLE3DSET_EXPORTS
 #if defined(WIN32)
  #define MLE3DSET_API __declspec(dllexport)
 #else
  #define MLE3DSET_API
 #endif
#else
 #if defined(WIN32)
  #define MLE3DSET_API __declspec(dllimport)
 #else
  #define MLE3DSET_API
 #endif
#endif

/* Empty define to avoid errors when _not_ compiling an MSWindows DLL. */
#ifndef MLE3DSET_API
# define MLE3DSET_API
#endif /* !MLE_DWP_API */
