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
#include <DX11Util/Core/Dx11Context.h>

#include <d3d11.h>

// #include <dxerr.h>

#include <xnamath.h>

#include <vector>
#include <string>


class Dx11ObjectTargetTexture
	: public Dx11ObjectGUI
{
public:
	Dx11ObjectTargetTexture(unsigned int _width, unsigned int _height);
	virtual ~Dx11ObjectTargetTexture(void);

	virtual void Setup(Dx11Context* _context);

	void Start(Dx11Context* _context); 
	void End(Dx11Context* _context); 

	virtual void Term();

	virtual BOOL GetBufNum(unsigned int* num); 

	virtual BOOL GetVertexBuf(unsigned int num, ID3D11Buffer** buf); 
	virtual BOOL GetIndexBuf(unsigned int num, ID3D11Buffer** buf); 
	virtual BOOL GetWeightBuf(unsigned int num, ID3D11Buffer** buf); 

	virtual BOOL GetVertexNum(unsigned int idx, unsigned int* num); 
	virtual BOOL GetIndexNum(unsigned int idx, unsigned int* num); 

	virtual BOOL GetTexture(unsigned int num, ID3D11ShaderResourceView** pTexture); 
	virtual BOOL GetTextureNum(unsigned int* num); 


private: 
	unsigned int width; 
	unsigned int height; 

	ID3D11Buffer* m_pVB; 
	ID3D11Buffer* m_pIB; 
	ID3D11Texture2D* m_pTex;
	ID3D11ShaderResourceView*	m_pTexture;	
	ID3D11RenderTargetView* pRenderTargetView; 
	ID3D11RenderTargetView* pOldRenderTargetView; 

	ID3D11Texture2D*         pDepthStencil;           // �[�x/�X�e���V���E�e�N�X�`��
	ID3D11DepthStencilView*  pDepthStencilView;       // �[�x/�X�e���V���E�r���[
	ID3D11DepthStencilView*  pOldDepthStencilView;       
	D3D11_VIEWPORT          ViewPort[1], old_view_port[16];		// '16' is a tempolary value. It should be fixed. 

	ID3D11BlendState*         pBlendState;			// �u�����h�E�X�e�[�g�E�I�u�W�F�N�g
	ID3D11RasterizerState*	  pRasterizerState;	// ���X�^���C�U�E�X�e�[�g�E�I�u�W�F�N�g
	ID3D11DepthStencilState*  pDepthStencilState;	// �[�x/�X�e���V���E�X�e�[�g�E�I�u�W�F�N�g
	ID3D11BlendState*         pOldBlendState;			// �u�����h�E�X�e�[�g�E�I�u�W�F�N�g
	ID3D11RasterizerState*	  pOldRasterizerState;	// ���X�^���C�U�E�X�e�[�g�E�I�u�W�F�N�g
	ID3D11DepthStencilState*  pOldDepthStencilState;	// �[�x/�X�e���V���E�X�e�[�g�E�I�u�W�F�N�g

	ID3D11SamplerState*       pTextureSamplerWrap; 
	ID3D11SamplerState*       pOldTextureSamplerWrap; 

	FLOAT old_blend_factor[4]; 
	UINT old_blend_mask; 
	UINT old_stencil_ref; 
	UINT old_num_viewport; 
};

