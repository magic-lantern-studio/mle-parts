/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file Mle2dSet.h
 * @ingroup MleParts
 *
 * This file contains generic support for a 2D Set implemented
 * as a Windows DLL.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2018-2024 Wizzer Works
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

/* Precaution to avoid an error easily made by the application programmer. */
#ifdef MLE_2DSET_API
#error Leave the internal MLE_2DSET_API define alone.
#endif /* MLE_2DSET_API */

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MLE_2DSET_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MLE_2DSET_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
# ifdef MLE_2DSET_EXPORTS
#   ifdef MLE_MAKE_DLL
#     define MLE_2DSET_API __declspec(dllexport)
#   endif /* MLE_MAKE_DLL */
# else /* ! MLE_2DSET_EXPORTS */
#   ifdef MLE_DLL
#     define MLE_2DSET_API __declspec(dllimport)
#   else /* ! MLE_DLL */
#     ifndef MLE_NOT_DLL
#       error Define either MLE_DLL or MLE_NOT_DLL as appropriate for your linkage! See mle/Mle2dSet.h for further instructions.
#     endif /* MLE_NOT_DLL */
#   endif /* ! MLE_DLL */
# endif /* ! MLE_2DSET_EXPORTS */


/* Empty define to avoid errors when _not_ compiling an MSWindows DLL. */
#ifndef MLE_2DSET_API
#define MLE_2DSET_API
#endif /* !MLE2DSET_API */
