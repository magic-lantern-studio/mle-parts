/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file Mle2dRole.h
 * @ingroup MleParts
 *
 * On MSWindows platforms, one of these defines must always be set when
 * building application programs:
 * <p>
 * "MLE_DLL", when the application programmer is using the
 * library in the form of a dynamic link library (DLL)
 * </p><p>
 * "MLE_NOT_DLL", when the application programmer is using the
 * library in the form of a static object library (LIB)
 * </p><p>
 * Note that either MLE_DLL or MLE_NOT_DLL _must_ be defined by
 * the application programmer on MSWindows platforms, or else the
 * #error statement will hit. Set up one or the other of these two
 * defines in your compiler environment according to how the library
 * was built -- as a DLL (use "MLE_DLL") or as a LIB (use
 * "MLE_NOT_DLL").
 * </p><p>
 * (Setting up defines for the compiler is typically done by either
 * adding something like "/DMLE_DLL" to the compiler's argument
 * line (for command-line build processes), or by adding the define to
 * the list of preprocessor symbols in your IDE GUI (in the MSVC IDE,
 * this is done from the "Project"->"Settings" menu, choose the "C/C++"
 * tab, then "Preprocessor" from the dropdown box and add the
 * appropriate define)).
 * </p><p>
 * It is extremely important that the application programmer uses the
 * correct define, as using "MLE_NOT_DLL" when "MLE_DLL" is
 * correct is likely to cause mysterious crashes.
 * </p>
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

/* Precaution to avoid an error easily made by the application programmer. */
#ifdef MLE_2DROLE_API
#error Leave the internal MLE_2DROLE_API define alone.
#endif /* MLE_2DROLE_API */

// Magic Lantern export rules.
# ifdef MLE_2DROLE_EXPORTS
#   ifdef MLE_MAKE_DLL
#     define MLE_2DROLE_API __declspec(dllexport)
#   endif /* MLE_MAKE_DLL */
# else /* ! MLE_2DROLE_EXPORTS */
#   ifdef MLE_DLL
#     define MLE_2DROLE_API __declspec(dllimport)
#   else /* ! MLE_DLL */
#     ifndef MLE_NOT_DLL
#       error Define either MLE_DLL or MLE_NOT_DLL as appropriate for your linkage! See mle/Mle2DRole.h for further instructions.
#     endif /* MLE_NOT_DLL */
#   endif /* ! MLE_DLL */
# endif /* ! MLE_2DROLE_EXPORTS */

/* Empty define to avoid errors when _not_ compiling an MSWindows DLL */
#ifndef MLE_2DROLE_API
# define MLE_2DROLE_API
#endif /* ! MLE_2DROLE_API */
