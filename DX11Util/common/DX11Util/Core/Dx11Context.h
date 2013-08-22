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

// #include <dxerr.h>

#include "xnamath.h"

#include <vector>

class Dx11Context; 

#define DXTRACE_ERR(x, y) (y)

#include <DX11Util/Core/Dx11Scene.h>


class Dx11Context
{
public:
	Dx11Context(void);
	virtual ~Dx11Context(void);

	/**
	 * Setup context. This method will be called at first. 
	 * Developer need to design their original scene which implement Dx11Scene and set it to context. 
	 * @param hwnd handle of window. You will set the return value of CreateWindow. 
	 * @param first_scene instance of Dx11Scene which developer design as their own application. 
	 **/ 
	HRESULT Setup(HWND hwnd, Dx11Scene* first_scene, int width, int height); 

	/**
	 * Update. 
	 * 
	 **/
	HRESULT Update(); 
	HRESULT Term(); 

	ID3D11DeviceContext* GetDXDC(); 
	ID3D11Device* GetDXDevice(); 

	HRESULT WindowResize(int width, int height); 
	HRESULT ToggleFullScreen(); 



	HRESULT Start(); 
	HRESULT Activate(); 
	HRESULT Flush(); 

	HRESULT ClearBackground(float ClearColor[4]); 
	HRESULT GetFPS(float* _fps); 

private: 
	HRESULT InitBackBuffer(int width, int height); 

protected:
//�@�\���x���̔z��
	D3D_FEATURE_LEVEL* pFeatureLevels; 
	UINT              FeatureLevels;   // �z��̗v�f��
	D3D_FEATURE_LEVEL FeatureLevelsSupported; // �f�o�C�X�쐬���ɕԂ����@�\���x��

// �C���^�[�t�F�C�X
	ID3D11Device*           pD3DDevice;        // �f�o�C�X
	ID3D11DeviceContext*    pImmediateContext; // �f�o�C�X�E�R���e�L�X�g
	IDXGISwapChain*         pSwapChain;        // �X���b�v�E�`�F�C��

	ID3D11RenderTargetView* pRenderTargetView; // �`��^�[�Q�b�g�E�r���[
	D3D11_VIEWPORT          ViewPort[1];				// �r���[�|�[�g

	ID3D11Texture2D*         pDepthStencil;           // �[�x/�X�e���V���E�e�N�X�`��
	ID3D11DepthStencilView*  pDepthStencilView;       // �[�x/�X�e���V���E�r���[

	ID3D11BlendState*         pBlendState;			// �u�����h�E�X�e�[�g�E�I�u�W�F�N�g
	ID3D11RasterizerState*	  pRasterizerState;	// ���X�^���C�U�E�X�e�[�g�E�I�u�W�F�N�g
	ID3D11DepthStencilState*  pDepthStencilState;	// �[�x/�X�e���V���E�X�e�[�g�E�I�u�W�F�N�g

	ID3D11SamplerState*       pTextureSamplerWrap; // �T���v���[


	Dx11Scene* current_scene; 
	LARGE_INTEGER now; 
	LARGE_INTEGER freq;  
	float fps; 
};

