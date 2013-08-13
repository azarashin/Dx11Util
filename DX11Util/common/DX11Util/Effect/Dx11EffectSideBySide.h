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

#include <DX11Util/Object/Dx11ObjectGUI.h>
#include <DX11Util/Motion/Dx11Motion.h>
#include <DX11Util/Core/Dx11Context.h>
#include <DX11Util/Core/Dx11Scene.h>
#include <DX11Util/Object/Dx11ObjectTargetTexture.h>
#include <DX11Util/Camera/Dx11CameraSideBySide.h>
#include <DX11Util/Lens/Dx11LensSideBySide.h>


#include <d3d11.h>
#include <d3dx11.h>
// #include <dxerr.h>

#include <xnamath.h>

#include <vector>

typedef struct {
    FLOAT LensCenter[2];
    FLOAT ScreenCenter[2];
    FLOAT Scale[2];
    FLOAT ScaleIn[2];
    FLOAT HmdWarpParam[4];
	FLOAT ChromaAbCorrection[4]; 

	FLOAT left; // should be 0 in initialize phase. 
	FLOAT dummy[3]; // should be 0 in initialize phase. 
} EffectSideBySideInfo; 

class Dx11EffectSideBySide
{
public:
	Dx11EffectSideBySide(int _width, int _height, const EffectSideBySideInfo* param);
	virtual ~Dx11EffectSideBySide(void);


	virtual HRESULT Setup(Dx11Context* _context); 
	virtual HRESULT Update(Dx11Context* _context, Dx11Scene* scene, Dx11CameraSideBySide* camera, Dx11LensSideBySide* lens); 
	virtual HRESULT Term(Dx11Context* _context); 

protected:
	ID3D11VertexShader*       pVertexShader; 
	ID3D11PixelShader*        pPixelShader; 
	ID3D11InputLayout*        pInputLayout; 
	 
	ID3D11Buffer* const_buf; 

	ID3D11BlendState*         pBlendState;			// ブレンド・ステート・オブジェクト
	ID3D11DepthStencilState*  pDepthStencilState;	// 深度/ステンシル・ステート・オブジェクト

	int width, height; 
	char* data; 

	Dx11ObjectTargetTexture* left; 
	Dx11ObjectTargetTexture* right; 

	EffectSideBySideInfo hmd_param; 
};

