/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file DxsSurface.h
 * @ingroup MleParts
 *
 * This file defines the interface for the DxsSurface class.
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

#ifndef __MLE_DXSSURFACE_H_
#define __MLE_DXSSURFACE_H_

// Include Magic Lantern DirectX Stage header files.
#include "DxsPixelFormat.h"

/**
 * @brief This class encapsulates a DirectX surface for rendering.
 */
class DxsSurface  
{
  public:

	/**
	 * The default constructor.
	 */
  	DxsSurface();

	/**
	 * The destructor.
	 */
	virtual ~DxsSurface();

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
	 * This method will blit this surface to the specified destination.
	 *
	 * @param dest The destination surface.
	 * @param x The x position in the destination to blit to.
	 * @param y The y position in the destination to blit to.
	 * @param srcRect The region to blit from this surface.
	 *
	 * @return An HRESULT is returned.
	 */
	HRESULT bltFast(LPDIRECT3DSURFACE9 dest, int x, int y, RECT srcRect);

	/**
	 * Create a surface from the specified file.
	 *
	 * @param filename The name of the file to read.
	 * @param pixelFormat The DirectX surface format to create.
	 *
	 * @return An HRESULT is returned.
	 */
	HRESULT createSurfaceFromFile(LPCTSTR filename, D3DFORMAT pixelFormat);

	/**
	 * Get the DirextX surface associated with this class
	 *
	 * @return A pointer to a DirectX surface is returned. If one has not
	 * yet been created, then <b>NULL</b> will be returned.
	 */
	LPDIRECT3DSURFACE9 getD3DSurface();

	/**
	 * Initialize the DXS surface.
	 *
	 * @param pD3DDevice The DirctX 9 device.
	 *
	 * @return An HRESULT is returned.
	 */
	HRESULT init(LPDIRECT3DDEVICE9 pD3DDevice);

	/**
	 * Create a suface.
	 *
	 * @param width The width of the surface to create.
	 * @param height The height of the surface to create.
	 * @param pixelFormat The DirectX surface format to create.
	 *
	 * @return An HRESULT is returned.
	 */
	HRESULT createSurface(UINT width, UINT height, D3DFORMAT pixelFormat);

	/**
	 * Delete the surface.
	 *
	 * @return An HRESULT is returned.
	 */
	HRESULT deleteSurface();

	/**
	 * Get the width of the surface.
	 *
	 * @return The width is returned as an unsigned integer.
	 */
	UINT getWidth() { return m_width; }

	/**
	 * Get the height of the surface.
	 *
	 * @return The width is returned as an unsigned integer.
	 */
	UINT getHeight() { return m_height; }

  protected:

	/** The pixel format. */
	DxsPixelFormat m_pixelFormat;

	/** A pointer to the DirectX device. */
	LPDIRECT3DDEVICE9 m_D3DDevice;

	/** A pointer to the associated DirectX surface. */
	LPDIRECT3DSURFACE9 m_D3DSurface;

	/** The width of the surface. */
	UINT m_width;

	/** The height of the surface. */
	UINT m_height;

	/** The DirectX surface format. */
	D3DFORMAT m_format;
};

/** Convenience type definition for a DXS surface. */
typedef DxsSurface DXS_SURFACE, *LPDXS_SURFACE;

#endif /* __MLE_DXSSURFACE_H_ */
