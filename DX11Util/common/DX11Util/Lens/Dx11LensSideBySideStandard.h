//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                               for PIT 
//
// Copyright (C) 2010-2011, azarashin, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall Azarashin or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#pragma once

#include <d3d11.h>

// #include <d3dx10math.h>
// #include <dxerr.h>

#include <xnamath.h>

#include <vector>

#include <DX11Util/Lens/Dx11LensSideBySide.h>


class Dx11LensSideBySideStandard : public Dx11LensSideBySide
{
public:
	Dx11LensSideBySideStandard(void);
	virtual ~Dx11LensSideBySideStandard(void);

	virtual HRESULT Setup(); 
	virtual HRESULT SetParameter(float near_width, float near_height, float _near, float _far); 
	virtual HRESULT SetParameterFov(float fov, float aspect, float _near, float _far); 
	virtual HRESULT GetDistanceFar(float* distance); 
	virtual HRESULT GetDistanceNear(float* distance); 
	virtual HRESULT GetNearWidth(float* width); 
	virtual HRESULT GetNearHeight(float* height); 
	virtual HRESULT GetMatrix(XMFLOAT4X4* m); 
	virtual HRESULT GetHAngle(float* angle); 
	virtual HRESULT GetVAngle(float* angle); 
	virtual HRESULT Term(); 
	virtual HRESULT SetIsLeft(bool _is_left); 

private:
	float width; 
	float height; 
	float dnear; 
	float dfar; 
	float hangle; 
	float vangle; 

	XMFLOAT4X4 mat; 
};

