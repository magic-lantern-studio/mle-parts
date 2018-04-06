/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file DxsPixelFormat.h
 * @ingroup MleParts
 *
 * This file defines the interface for the DxsPixelFormat class.
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

#ifndef __MLE_DXSPIXELFORMAT_H_
#define __MLE_DXSPIXELFORMAT_H_

// Include Magic Lantern DirectX Stage header files.
#include "dxsdefs.h"
#include "d3dutil.h"

/** DXS color information. */
typedef struct _DXS_COLOR
{
	int Red;    /**< Red component. */
	int Green;  /**< Green component. */
	int Blue;   /**< Blue component. */
	int Alpha;  /**< Alpha component. */
} DXS_COLOR;

/** Surface pixel format information structure. */
typedef struct _DXS_PIXELFMT
{
	D3DFORMAT Format;  /**< Surface format. */
	DWORD RShift;      /**< Number of bits to shift red component. */
	DWORD GShift;      /**< Number of bits to shift green component. */
	DWORD BShift;      /**< Number of bits to shift blue component. */
	DWORD AShift;      /**< Number of bits to shift alpha component. */
	DWORD ABMask;      /**< Alpha component bit mask. */
	DWORD RBMask;      /**< Red component bit mask. */
	DWORD GBMask;      /**< Green component bit mask. */
	DWORD BBMask;      /**< Blue component bit mask. */

	// Maximum color component values for the pixelformat
	// Some surfaces have a max of 255, others 63, others 15 etc.
	DWORD AMaxVal;     /**< Maximum alpha component value. */
	DWORD RMaxVal;     /**< Maximum red component value. */
	DWORD GMaxVal;     /**< Maximum green component value. */
	DWORD BMaxVal;     /**< Maximum alpblueha component value. */
	int BitsPerPixel;  /**< Number of bits per pixel (i.e. 32, 16, 8). */
} DXS_PIXELFMT;

/**
 * @brief Utility class for holding pixel format information.
 */
class DxsPixelFormat  
{
  public:

	/**
	 * Override operator new.
	 *
	 * @param tSize The size, in bytes, to allocate.
	 */
	void* operator new(size_t tSize);

	/**
	 * Override operator delete.
	 *
	 * @param p A pointer to the memory to delete.
	 */
    void  operator delete(void *p);

	/**
	 * Get the maximum alpha value.
	 *
	 * @param format A valid DirectX surface format.
	 *
	 * @return A 4-byte value is returned.
	 */
	DWORD GetAMaxVal(D3DFORMAT format);

	/**
	 * Get the maximum red value.
	 *
	 * @param format A valid DirectX surface format.
	 *
	 * @return A 4-byte value is returned.
	 */
	DWORD GetRMaxVal(D3DFORMAT format);

	/**
	 * Get the maximum green value.
	 *
	 * @param format A valid DirectX surface format.
	 *
	 * @return A 4-byte value is returned.
	 */
	DWORD GetGMaxVal(D3DFORMAT format);

	/**
	 * Get the maximum blue value.
	 *
	 * @param format A valid DirectX surface format.
	 *
	 * @return A 4-byte value is returned.
	 */
	DWORD GetBMaxVal(D3DFORMAT format);

	/**
	 * Get the shift value for the specified mask.
	 *
	 * @param dmask An alpha, red, green or blue bit mask.
	 *
	 * @return A 4-byte value is returned.
	 */
	DWORD GetShiftValue(DWORD dmask);

	/**
	 * Get the alpha bit mask value.
	 *
	 * @param format A valid DirectX surface format.
	 *
	 * @return A 4-byte value is returned.
	 */
	DWORD GetABitMask(D3DFORMAT format);

	/**
	 * Get the red bit mask value.
	 *
	 * @param format A valid DirectX surface format.
	 *
	 * @return A 4-byte value is returned.
	 */
	DWORD GetRBitMask(D3DFORMAT format);

	/**
	 * Get the green bit mask value.
	 *
	 * @param format A valid DirectX surface format.
	 *
	 * @return A 4-byte value is returned.
	 */
	DWORD GetGBitMask(D3DFORMAT format);

	/**
	 * Get the blue bit mask value.
	 *
	 * @param format A valid DirectX surface format.
	 *
	 * @return A 4-byte value is returned.
	 */
	DWORD GetBBitMask(D3DFORMAT format);

	/**
	 * Get the DXS pixel format.
	 *
	 * @param format A valid DirectX surface format.
	 * @param dtpixelfmt A pointer to the DXS pixel format is returned
	 * as an output parameter.
	 *
	 * @return An HRESULT is returned.
	 */
	HRESULT GetDXSPixelFormat(D3DFORMAT format, DXS_PIXELFMT * dtpixelfmt);

	/**
	 * Get the number of bits per pixel.
	 *
	 * @param format A valid DirectX surface format.
	 *
	 * @return A 4-byte value is returned.
	 */
	DWORD GetBitDept(D3DFORMAT format);

	/**
	 * Get the location of the red bits.
	 *
	 * @param format A valid DirectX surface format.
	 *
	 * @return A 4-byte value is returned.
	 */
	DWORD GetRBitLoc(D3DFORMAT format);

	/**
	 * Get the location of the green bits.
	 *
	 * @param format A valid DirectX surface format.
	 *
	 * @return A 4-byte value is returned.
	 */
	DWORD GetGBitLoc(D3DFORMAT format);

	/**
	 * Get the location of the blue bits.
	 *
	 * @param format A valid DirectX surface format.
	 *
	 * @return A 4-byte value is returned.
	 */
	DWORD GetBBitLoc(D3DFORMAT format);

	/**
	 * Get the location of the alpha bits.
	 *
	 * @param format A valid DirectX surface format.
	 *
	 * @return A 4-byte value is returned.
	 */
	DWORD GetABitLoc(D3DFORMAT format);

	/**
	 * The default constructor.
	 */
	DxsPixelFormat();

	/**
	 * The destructor.
	 */
	virtual ~DxsPixelFormat();
};

#endif /* __MLE_DXSPIXELFORMAT_H_ */
