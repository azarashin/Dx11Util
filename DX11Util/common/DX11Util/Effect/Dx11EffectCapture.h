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

#include <DX11Util/Effect/Dx11Effect.h>
#include <DX11Util/Camera/Dx11Camera.h>
#include <DX11Util/Lens/Dx11Lens.h>
#include <DX11Util/Core/Dx11Context.h>

#include <d3d11.h>

// #include <dxerr.h>

#include <xnamath.h>

#include <vector>

class Dx11EffectCapture 
{
public:
	Dx11EffectCapture();
	virtual ~Dx11EffectCapture(void);

	virtual HRESULT Setup(Dx11Context* _context); 
	virtual HRESULT Update(Dx11Context* _context, Dx11Object* obj, Dx11Motion* mot, Dx11Camera* cam, Dx11Lens* lens, float trans, float inv); 
	virtual HRESULT Update(Dx11Context* _context, Dx11Object* obj, float px, float py, float width, float height, float depth, Dx11Lens* lens, float trans, float inv); 
	virtual HRESULT Term(Dx11Context* _context); 


private: 
	ID3D11VertexShader*       pVertexShader; 
	ID3D11GeometryShader*     pGeometryShader; 
	ID3D11PixelShader*        pPixelShader; 
	ID3D11InputLayout*        pInputLayout; 

	ID3D11Buffer* const_buf; 

	ID3D11BlendState*         pBlendState;			// ブレンド・ステート・オブジェクト
	ID3D11DepthStencilState*  pDepthStencilState;	// 深度/ステンシル・ステート・オブジェクト
};

