/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file DxsSurface.h
 * @ingroup MleParts
 *
 * This file contains the implementation for the DxsSurface class.
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

// Include Magic Lantern DirectX Stage header files.
#include "dxs/dxstage.h"
#include "dxs/DxsSurface.h"


DxsSurface::DxsSurface()
{
	m_D3DDevice = NULL;
	m_D3DSurface = NULL;

	m_width = 0;
	m_height = 0;
	m_format = D3DFMT_UNKNOWN;
}

DxsSurface::~DxsSurface()
{
	if (m_D3DSurface != NULL) 
	{
		ULONG count = m_D3DSurface->Release();
		m_D3DSurface = NULL;
	}

	m_width = 0;
	m_height = 0;
	m_format = D3DFMT_UNKNOWN;
}

void *
DxsSurface::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
DxsSurface::operator delete(void *p)
{
	mlFree(p);
}

HRESULT DxsSurface::createSurface(UINT Width, UINT Height, D3DFORMAT PixelFormat)
{
	if (m_D3DDevice == NULL) return DXS_NOT_INITIALIZED;

	if (m_D3DDevice->CreateOffscreenPlainSurface(Width, Height, PixelFormat, D3DPOOL_DEFAULT, &m_D3DSurface, NULL) != D3D_OK)
		return DXS_ERROR;

	m_width = Width;
	m_height = Height;
	m_format = PixelFormat;

	return DXS_OK;
}

HRESULT DxsSurface::deleteSurface()
{
	if (m_D3DSurface != NULL)
	{
		ULONG count = m_D3DSurface->Release();
		m_D3DSurface = NULL;
	}

	return DXS_OK;
}

// Should be called to set the D3DDevice before doing anything with the surface
HRESULT DxsSurface::init(LPDIRECT3DDEVICE9 pD3DDevice)
{
	if (pD3DDevice == NULL) return DXS_NOT_INITIALIZED;

	m_D3DDevice = pD3DDevice;

	return DXS_OK;
}

LPDIRECT3DSURFACE9 DxsSurface::getD3DSurface()
{
	return m_D3DSurface;
}

// Crappy function which quickly loads an image into the surface, uses D3DX functions,
// and defaults to stage image size.
// TODO: Write code to create a surface and load an image into it regardless of surface format.
// NOTE: This function currently creates the surface m_D3DSurface using the D3DPOOL_SCRATCH flag. For
// best performance, use the D3DPOOL_DEFAULT flag with a supported pixelformat for the creation of m_D3DSurface.
//
// For surfaces compatible with GDI (for use in windowed mode) use pixelformats D3DFMT_X1R5G5B5, D3DFMT_R5G6B5, 
// or D3DFMT_X8R8G8B8 (check the format of the backbuffer surface and use that for best performance).
// For surfaces compatible with the D3DPOOL_DEFAULT flag use pixelformats D3DFMT_X8R8G8B8, D3DFMT_A8R8G8B8, 
// D3DFMT_A2B10G10R10, D3DFMT_X1R5G5B5, D3DFMT_A1R5G5B5, and D3DFMT_R5G6B5
HRESULT DxsSurface::createSurfaceFromFile(LPCTSTR FileName, D3DFORMAT PixelFormat)
{
	if (m_D3DDevice == NULL) return DXS_NOT_INITIALIZED;

	// Retrieve the size of the stage.
	int stageWidth, stageHeight;
	DxStage *theStage = (DxStage *)MleStage::g_theStage;
	theStage->getSize(&stageWidth, &stageHeight);

	// Create a temporary surface to load the image into. The size of the surface should not be
	// larger than the stage.
	LPDIRECT3DSURFACE9 tempSurf;
	if (m_D3DDevice->CreateOffscreenPlainSurface(stageWidth, stageHeight, PixelFormat, D3DPOOL_SCRATCH, &tempSurf, NULL) != D3D_OK)
		return DXS_ERROR;

	// Load the image into the temporary surface.
	D3DXIMAGE_INFO imageInfo;
	if (D3DXLoadSurfaceFromFileA(tempSurf, NULL, NULL, FileName, NULL, D3DX_FILTER_LINEAR, D3DCOLOR_ARGB(255,0,0,0), &imageInfo) != D3D_OK) 
	{
		ULONG count = tempSurf->Release();
		return DXS_ERROR;
	}

	// Create a surface the size of the image.
	if (m_D3DDevice->CreateOffscreenPlainSurface(imageInfo.Width, imageInfo.Height, PixelFormat, D3DPOOL_SCRATCH, &m_D3DSurface, NULL) != D3D_OK)
	{
		ULONG count = tempSurf->Release();
		return DXS_ERROR;
	}

	// Transfer the image in the temporary surface over to the retained surface.
	RECT bounds;
	bounds.left = 0;
	bounds.top = 0;
	bounds.right = imageInfo.Width;
	bounds.bottom = imageInfo.Height;
	if (D3DXLoadSurfaceFromSurface(m_D3DSurface, NULL, &bounds, tempSurf, NULL, NULL, D3DX_FILTER_LINEAR, 0) != D3D_OK)
	{
		ULONG count = m_D3DSurface->Release();
		count = tempSurf->Release();
		return DXS_ERROR;
	}

	// Release the temporary surface.
	ULONG count = tempSurf->Release();

	// Remember the image properties.
	m_width = imageInfo.Width;
	m_height = imageInfo.Height;
	m_format = PixelFormat;

	return DXS_OK;
}

// Blit utility for rendering into a DirectX surface.
HRESULT DxsSurface::bltFast(LPDIRECT3DSURFACE9 Dest, int x, int y, RECT srcRect)
{
	if (m_D3DDevice == NULL) return DXS_NOT_INITIALIZED;
	if (m_D3DSurface == NULL) return DXS_NOT_INITIALIZED;
	if (Dest == NULL) return DXS_ERROR;

	// Clipping
	if (srcRect.left < 0) srcRect.left = 0;
	if (srcRect.left > (int)(m_width)) srcRect.left = (m_width);
	if (srcRect.top < 0) srcRect.top = 0;
	if (srcRect.top > (int)(m_height)) srcRect.top = (m_height);

	if (srcRect.right < srcRect.left) srcRect.right = srcRect.left;
	if (srcRect.right > (int)(m_width)) srcRect.right = (m_width);
	if (srcRect.bottom < srcRect.top) srcRect.bottom = srcRect.top;
	if (srcRect.bottom > (int)(m_height)) srcRect.bottom = (m_height);

	UINT nHeight = srcRect.bottom-srcRect.top;
	UINT nWidth = srcRect.right-srcRect.left;

	D3DSURFACE_DESC sdesc;
	Dest->GetDesc(&sdesc);

	// Clipping
	if (x < 0)
	{	
		nWidth += x;
		x = 0;
	}
    if (x+nWidth > sdesc.Width)
	{
		nWidth = sdesc.Width-x;
	}
	if (y < 0)
	{
		nHeight += y;
		y = 0;
	}
	if (y+nHeight > sdesc.Height)
	{
		nHeight = sdesc.Height-y;
	}

	// Return if we don't have anything to blit
	if ((nHeight <= 0) || (nWidth <= 0)) return DXS_OK;

	RECT destRect; destRect.left=x; destRect.top=y; destRect.right=x+nWidth; destRect.bottom=y+nHeight;

	// Get source and destination pixelformats
	// Optimization: Remove this and do it somewhere else where you can do it once since it makes things slower.
	DXS_PIXELFMT spFmt, dpFmt;
	int ret = m_pixelFormat.GetDXSPixelFormat(m_format, &spFmt);
	if (ret != DXS_OK) return ret;
	ret = m_pixelFormat.GetDXSPixelFormat(sdesc.Format, &dpFmt);
	if (ret != DXS_OK) return ret;

	D3DLOCKED_RECT slRect;
	D3DLOCKED_RECT dlRect;

	if (m_D3DSurface->LockRect(&slRect, &srcRect, D3DLOCK_READONLY) == D3D_OK)
	{
		if (Dest->LockRect(&dlRect, &destRect, 0) == D3D_OK)
		{
			BYTE* Source = (BYTE*)slRect.pBits;
			BYTE* Dest = (BYTE*)dlRect.pBits;

			for (UINT y1=0; y1<nHeight; y1++)
			{
				BYTE* Source2 = (BYTE*)Source;
				BYTE* Dest2 = (BYTE*)Dest;

				// If the surface formats would be the same, and we just want to do a SRCCOPY blit, we could just do the following to make it fast:
				//*(WORD*)Dest2 = *(WORD*)Source2;

				// But the following code illustrates breaking up a pixel from the source surface in ARGB components,
				// and then reassembling them on the destination surface, after color conversion if needed. With this 
				// code you can add blending effects such as translucency, multiply, overlay etc.
				// In an application with realtime animation, like a game, however you would not want to work with 
				// many different surface formats, because color conversion makes the blit slower. The best 
				// thing to do would be to use one surface format that best suites your needs.
				
				for (UINT x1=0; x1<nWidth; x1++)
				{
					if (spFmt.BitsPerPixel == 16)
					{
						WORD Sp = *(WORD*)Source2;
											
						DWORD alpha = ((Sp & spFmt.ABMask) >> spFmt.AShift);
						DWORD red = ((Sp & spFmt.RBMask) >> spFmt.RShift);
						DWORD green = ((Sp & spFmt.GBMask) >> spFmt.GShift);
						DWORD blue = ((Sp & spFmt.BBMask) >> spFmt.BShift);

						// Color conversion
						if (spFmt.Format != dpFmt.Format)
						{
							alpha = (DWORD) ((float)((float)alpha/(float)spFmt.AMaxVal) * (float)dpFmt.AMaxVal);
							red = (DWORD) ((float)((float)red/(float)spFmt.RMaxVal) * (float)dpFmt.RMaxVal);
							green = (DWORD) ((float)((float)green/(float)spFmt.GMaxVal) * (float)dpFmt.GMaxVal);
							blue = (DWORD) ((float)((float)blue/(float)spFmt.BMaxVal) * (float)dpFmt.BMaxVal);
						}

						if (dpFmt.BitsPerPixel == 16)
						{
  							WORD* Dp = (WORD*)Dest2;
							*Dp = (WORD) ((((alpha) << dpFmt.AShift) & dpFmt.ABMask) |
										(((red) << dpFmt.RShift) & dpFmt.RBMask) |
										(((green) << dpFmt.GShift) & dpFmt.GBMask) |
										(((blue) << dpFmt.BShift) & dpFmt.BBMask));
						}
						else if (dpFmt.BitsPerPixel > 16)
						{
  							DWORD* Dp = (DWORD*)Dest2;
							*Dp = (DWORD) ((((alpha) << dpFmt.AShift) & dpFmt.ABMask) |
										(((red) << dpFmt.RShift) & dpFmt.RBMask) |
										(((green) << dpFmt.GShift) & dpFmt.GBMask) |
										(((blue) << dpFmt.BShift) & dpFmt.BBMask));
						}
					}
					else if (spFmt.BitsPerPixel > 16)
					{
						DWORD Sp = *(DWORD*)Source2;
											
						DWORD alpha = ((Sp & spFmt.ABMask) >> spFmt.AShift);
						DWORD red = ((Sp & spFmt.RBMask) >> spFmt.RShift);
						DWORD green = ((Sp & spFmt.GBMask) >> spFmt.GShift);
						DWORD blue = ((Sp & spFmt.BBMask) >> spFmt.BShift);

						// Color conversion
						if (spFmt.Format != dpFmt.Format)
						{
							alpha = (DWORD) ((float)((float)alpha/(float)spFmt.AMaxVal) * (float)dpFmt.AMaxVal);
							red = (DWORD) ((float)((float)red/(float)spFmt.RMaxVal) * (float)dpFmt.RMaxVal);
							green = (DWORD) ((float)((float)green/(float)spFmt.GMaxVal) * (float)dpFmt.GMaxVal);
							blue = (DWORD) ((float)((float)blue/(float)spFmt.BMaxVal) * (float)dpFmt.BMaxVal);
						}
					
						if (dpFmt.BitsPerPixel == 16)
						{
  							WORD* Dp = (WORD*)Dest2;
							*Dp = (WORD) ((((alpha) << dpFmt.AShift) & dpFmt.ABMask) |
										(((red) << dpFmt.RShift) & dpFmt.RBMask) |
										(((green) << dpFmt.GShift) & dpFmt.GBMask) |
										(((blue) << dpFmt.BShift) & dpFmt.BBMask));
						}
						else if (dpFmt.BitsPerPixel > 16)
						{
  							DWORD* Dp = (DWORD*)Dest2;
							*Dp = (DWORD) ((((alpha) << dpFmt.AShift) & dpFmt.ABMask) |
										(((red) << dpFmt.RShift) & dpFmt.RBMask) |
										(((green) << dpFmt.GShift) & dpFmt.GBMask) |
										(((blue) << dpFmt.BShift) & dpFmt.BBMask));
						}
					}

					Dest2+=dpFmt.BitsPerPixel/8;
					Source2+=spFmt.BitsPerPixel/8;
				}

				Dest+=dlRect.Pitch;
				Source+=slRect.Pitch;
			}
		}
		else
		{
			m_D3DSurface->UnlockRect();

			return DXS_ERROR;
		}

		Dest->UnlockRect();
	}
	else return DXS_ERROR;

	m_D3DSurface->UnlockRect();

	return DXS_OK;
}
