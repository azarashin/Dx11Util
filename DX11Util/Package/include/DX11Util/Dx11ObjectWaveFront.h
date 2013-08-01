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

#include <DX11Util/Dx11Object.h>

#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>

#include <xnamath.h>

#include <vector>
#include <string>

#include <dae.h>
#include <dom/domCOLLADA.h>






class Dx11ObjectWaveFront
	: public Dx11Object
{
public:
	Dx11ObjectWaveFront(ID3D11Device* _pd3dDevice, const char* _filename);
	virtual ~Dx11ObjectWaveFront(void);

	virtual void Setup();
	virtual void Term();

	virtual BOOL GetBufNum(unsigned int* num); 
	virtual BOOL GetVertexBuf(unsigned int num, ID3D11Buffer** buf); 
	virtual BOOL GetWeightBuf(unsigned int num, ID3D11Buffer** buf); 
	virtual BOOL GetIndexBuf(unsigned int num, ID3D11Buffer** buf); 

	virtual BOOL GetVertexNum(unsigned int idx, unsigned int* num); 
	virtual BOOL GetIndexNum(unsigned int idx, unsigned int* num); 


	// 描画
	HRESULT Render( ID3D11DeviceContext* pd3d_dc );

	// テクスチャを返す
	BOOL GetTexture(unsigned int num, ID3D11ShaderResourceView** pTexture); 

protected:
	std::vector<ID3D11Buffer*> m_pVB;			// 頂点バッファ
	std::vector<ID3D11Buffer*> m_pIB;			// インデックスバッファ
	std::vector<ID3D11ShaderResourceView*>		m_pTexture;		// テクスチャ

	std::vector<DWORD> m_dwVertices;					// 頂点数
	std::vector<DWORD> m_dwIndecies;			// インデックス数

	// オフセットの最大数を返す
	unsigned int getMaxOffset( domInput_local_offset_Array &input_array );
	// 三角形の場合
	HRESULT ConstructTriangles(domMesh *thisMesh, domTriangles *thisTriangles, ID3D11Device* pd3dDevice);
	// Polylisitの場合
	HRESULT ConstructPolylist(domMesh *thisMesh, domPolylist *thisPolylist, ID3D11Device* pd3dDevice);
	// Polygonの場合
	HRESULT ConstructPolygon(domMesh *thisMesh, domPolygons *thisPolygons, ID3D11Device* pd3dDevice);

private: 
	std::string filename; 
	ID3D11Device* pd3dDevice; 
};

