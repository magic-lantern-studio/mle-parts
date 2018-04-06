/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file DxsPixelFormat.cxx
 * @ingroup MleParts
 *
 * This file contains the implementation for the DxsPixelFormat class.
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

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

// Include Magic Lantern DirectX Stage header files.
#include "dxs/DxsPixelFormat.h"

DxsPixelFormat::DxsPixelFormat()
{
	// Do nothing extra.
}

DxsPixelFormat::~DxsPixelFormat()
{
	// Does nothing.
}

void *
DxsPixelFormat::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
DxsPixelFormat::operator delete(void *p)
{
	mlFree(p);
}

DWORD DxsPixelFormat::GetRBitLoc(D3DFORMAT format)
{
	return GetShiftValue(GetRBitMask(format));
}

DWORD DxsPixelFormat::GetGBitLoc(D3DFORMAT format)
{
	return GetShiftValue(GetGBitMask(format));
}

DWORD DxsPixelFormat::GetBBitLoc(D3DFORMAT format)
{
	return GetShiftValue(GetBBitMask(format));
}

DWORD DxsPixelFormat::GetABitLoc(D3DFORMAT format)
{
	return GetShiftValue(GetABitMask(format));
}

DWORD DxsPixelFormat::GetBitDept(D3DFORMAT format)
{
	if (format == D3DFMT_R5G6B5) return 16;
	if (format == D3DFMT_A8R8G8B8) return 32;
	if (format == D3DFMT_R8G8B8) return 24;
	if (format == D3DFMT_A4R4G4B4) return 16;
	if (format == D3DFMT_A1R5G5B5) return 16;
	if (format == D3DFMT_X8R8G8B8) return 32;
	if (format == D3DFMT_X1R5G5B5) return 16;
	if (format == D3DFMT_A2B10G10R10) return 32;

	return 0;
}

HRESULT DxsPixelFormat::GetDXSPixelFormat(D3DFORMAT format, DXS_PIXELFMT *dtpixelfmt)
{
	if (dtpixelfmt == NULL) return DXS_ERROR;

	// Check if pixelformat is supported
	if ((format != D3DFMT_R5G6B5) &&
		(format != D3DFMT_A8R8G8B8) &&
		(format != D3DFMT_R8G8B8) &&
		(format != D3DFMT_A4R4G4B4) &&
		(format != D3DFMT_A1R5G5B5) &&
		(format != D3DFMT_X1R5G5B5) &&
		(format != D3DFMT_A2B10G10R10) &&
		(format != D3DFMT_X8R8G8B8)) return DXS_INVALID_FORMAT;

	dtpixelfmt->Format = format;
	dtpixelfmt->RShift = GetRBitLoc(format);
	dtpixelfmt->GShift = GetGBitLoc(format);
	dtpixelfmt->BShift = GetBBitLoc(format);
	dtpixelfmt->AShift = GetABitLoc(format);
	dtpixelfmt->BitsPerPixel = GetBitDept(format);
	dtpixelfmt->ABMask = GetABitMask(format);
	dtpixelfmt->RBMask = GetRBitMask(format);
	dtpixelfmt->GBMask = GetGBitMask(format);
	dtpixelfmt->BBMask = GetBBitMask(format);
	dtpixelfmt->AMaxVal = GetAMaxVal(format);
	dtpixelfmt->RMaxVal = GetRMaxVal(format);
	dtpixelfmt->GMaxVal = GetGMaxVal(format);
	dtpixelfmt->BMaxVal = GetBMaxVal(format);

	return DXS_OK;
}

DWORD DxsPixelFormat::GetABitMask(D3DFORMAT format)
{
	if (format == D3DFMT_R5G6B5) return 0x00000000;
	if (format == D3DFMT_A8R8G8B8) return 0xff000000;
	if (format == D3DFMT_R8G8B8) return 0x00000000;
	if (format == D3DFMT_A4R4G4B4) return 0x0000f000;
	if (format == D3DFMT_A1R5G5B5) return 0x00008000;
	if (format == D3DFMT_X8R8G8B8) return 0x00000000;
	if (format == D3DFMT_X1R5G5B5) return 0x00008000;
	if (format == D3DFMT_A2B10G10R10) return 0xc0000000;

	return 0xff;
}

DWORD DxsPixelFormat::GetRBitMask(D3DFORMAT format)
{
	if (format == D3DFMT_R5G6B5) return 0x0000f800;
	if (format == D3DFMT_A8R8G8B8) return 0x00ff0000;
	if (format == D3DFMT_R8G8B8) return 0x00ff0000;
	if (format == D3DFMT_A4R4G4B4) return 0x00000f00;
	if (format == D3DFMT_A1R5G5B5) return 0x00007c00;
	if (format == D3DFMT_X8R8G8B8) return 0x00ff0000;
	if (format == D3DFMT_X1R5G5B5) return 0x00007c00;
	if (format == D3DFMT_A2B10G10R10) return 0x000003ff;

	return 0xff;
}

DWORD DxsPixelFormat::GetGBitMask(D3DFORMAT format)
{
	if (format == D3DFMT_R5G6B5) return 0x000007e0;
	if (format == D3DFMT_A8R8G8B8) return 0x0000ff00;
	if (format == D3DFMT_R8G8B8) return 0x0000ff00;
	if (format == D3DFMT_A4R4G4B4) return 0x000000f0;
	if (format == D3DFMT_A1R5G5B5) return 0x000003e0;
	if (format == D3DFMT_X8R8G8B8) return 0x0000ff00;
	if (format == D3DFMT_X1R5G5B5) return 0x000003e0;
	if (format == D3DFMT_A2B10G10R10) return 0x000ffc00;

	return 0xff;
}

DWORD DxsPixelFormat::GetBBitMask(D3DFORMAT format)
{
	if (format == D3DFMT_R5G6B5) return 0x0000001f;
	if (format == D3DFMT_A8R8G8B8) return 0x000000ff;
	if (format == D3DFMT_R8G8B8) return 0x000000ff;
	if (format == D3DFMT_A4R4G4B4) return 0x0000000f;
	if (format == D3DFMT_A1R5G5B5) return 0x0000001f;
	if (format == D3DFMT_X8R8G8B8) return 0x000000ff;
	if (format == D3DFMT_X1R5G5B5) return 0x0000001f;
	if (format == D3DFMT_A2B10G10R10) return 0x3ff00000;

	return 0xff;
}

DWORD DxsPixelFormat::GetShiftValue(DWORD dmask)
{
	DWORD dwShift = 0;

	if (dmask)
	{
		while ((dmask & 1) == 0 )
		{
			dwShift++;
			dmask >>= 1;
		}
	}
    
	return dwShift;
}

DWORD DxsPixelFormat::GetAMaxVal(D3DFORMAT format)
{
	if (format == D3DFMT_R5G6B5) return 0;
	if (format == D3DFMT_A8R8G8B8) return 255;
	if (format == D3DFMT_R8G8B8) return 0;
	if (format == D3DFMT_A4R4G4B4) return 15;
	if (format == D3DFMT_A1R5G5B5) return 1;
	if (format == D3DFMT_X8R8G8B8) return 0;
	if (format == D3DFMT_X1R5G5B5) return 0;
	if (format == D3DFMT_A2B10G10R10) return 3;

	return 0;
}

DWORD DxsPixelFormat::GetRMaxVal(D3DFORMAT format)
{
	if (format == D3DFMT_R5G6B5) return 31;
	if (format == D3DFMT_A8R8G8B8) return 255;
	if (format == D3DFMT_R8G8B8) return 255;
	if (format == D3DFMT_A4R4G4B4) return 15;
	if (format == D3DFMT_A1R5G5B5) return 31;
	if (format == D3DFMT_X8R8G8B8) return 255;
	if (format == D3DFMT_X1R5G5B5) return 31;
	if (format == D3DFMT_A2B10G10R10) return 1023;

	return 0;
}

DWORD DxsPixelFormat::GetGMaxVal(D3DFORMAT format)
{
	if (format == D3DFMT_R5G6B5) return 63;
	if (format == D3DFMT_A8R8G8B8) return 255;
	if (format == D3DFMT_R8G8B8) return 255;
	if (format == D3DFMT_A4R4G4B4) return 15;
	if (format == D3DFMT_A1R5G5B5) return 31;
	if (format == D3DFMT_X8R8G8B8) return 255;
	if (format == D3DFMT_X1R5G5B5) return 31;
	if (format == D3DFMT_A2B10G10R10) return 1023;

	return 0;
}

DWORD DxsPixelFormat::GetBMaxVal(D3DFORMAT format)
{
	if (format == D3DFMT_R5G6B5) return 31;
	if (format == D3DFMT_A8R8G8B8) return 255;
	if (format == D3DFMT_R8G8B8) return 255;
	if (format == D3DFMT_A4R4G4B4) return 15;
	if (format == D3DFMT_A1R5G5B5) return 31;
	if (format == D3DFMT_X8R8G8B8) return 255;
	if (format == D3DFMT_X1R5G5B5) return 31;
	if (format == D3DFMT_A2B10G10R10) return 1023;

	return 0;
}