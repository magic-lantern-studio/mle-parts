/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file dxsdefs.h
 * @ingroup MleParts
 *
 * This file contains error definitions for the DirectX Stage
 * utilties.
 *
 * @author Mark S. Millard
 * @date Nov 16, 2007
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

#ifndef __MLE_DXSDEFS_H_
#define __MLE_DXSDEFS_H_

// Function return values.
#define DXS_OK						1 /**< Function was successful. */
#define DXS_ERROR					0 /**< Function was not successful. */
#define DXS_NOT_INITIALIZED			2 /**< Object has not yet been initialized. */
#define DXS_COULD_NOT_OPEN_FILE		3 /**< File could not be found or opened. */
#define DXS_FILE_READ_ERROR			4 /**< Could not read from file. */
#define DXS_FILE_WRITE_ERROR		5 /**< Could not write to file. */
#define DXS_COMPRESSION_ERROR		6 /**< Compression error. */
#define DXS_DECOMPRESSION_ERROR		7 /**< Decompresison error. */
#define DXS_INVALID_FORMAT			8 /**< Invalid data format. */

#endif /* __MLE_DXSDEFS_H_ */
