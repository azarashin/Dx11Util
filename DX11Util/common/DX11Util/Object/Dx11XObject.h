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
#include <d3dx11.h>
// #include <dxerr.h>

#include <xnamath.h>

#include <vector>

#include <DX11Util/Core/Dx11Context.h>



#include <Dx11Util/Object/Dx11Object.h>
#include <DX11Util/Motion/Dx11XMotionManager.h>
#include <DX11Util/Motion/Dx11XMotion.h>


typedef struct _Dx11XMaterial
{
	FLOAT diffuse[4]; // rgba
	FLOAT specular[3]; 
	FLOAT emissive[3]; 
	FLOAT power; // index of ID3D11ShaderResourceView

	int tex_index; 
} Dx11XMaterialStandard; 

typedef struct _Dx11XSubMesh
{
	int mat_index; // index of Dx11XMaterialStandard
	int start_index; // start of index in ID3D11Buffer
	int size; // size of index in ID3D11Buffer to be rendered. 
	float bounding_box[3][2]; 
} Dx11XSubMesh; 

typedef struct _Dx11XBufferInfo
{
	ID3D11Buffer* vb; 
	ID3D11Buffer* ib; 

	int vtx_num; // number of vertex; 
	int idx_num; // number of index; 

	int ref_frame; 
} Dx11XBufferInfo; 

class Dx11XObject : public Dx11Object
{
public: 
	Dx11XObject(const char* filename); 
	virtual ~Dx11XObject(); 

	virtual void Setup(Dx11Context* _context);
	virtual void Term();

	virtual BOOL GetBufNum(unsigned int* num); 
	virtual BOOL GetVertexBuf(unsigned int num, ID3D11Buffer** buf); 
	virtual BOOL GetWeightBuf(unsigned int num, ID3D11Buffer** buf); 
	virtual BOOL GetIndexBuf(unsigned int num, ID3D11Buffer** buf); 

	virtual BOOL GetVertexNum(unsigned int idx, unsigned int* num); 
	virtual BOOL GetIndexNum(unsigned int idx, unsigned int* num); 

	virtual BOOL GetTexture(unsigned int num, ID3D11ShaderResourceView** pTexture); 
	virtual BOOL GetTextureNum(unsigned int* num); 


	void GetMaterial(unsigned int id, unsigned int mat_id, Dx11XMaterialStandard* material); 
	int GetNumberOfMaterial(unsigned int id); 

	void GetSubMesh(unsigned int id, unsigned int mat_id, Dx11XSubMesh* sub_mesh); 
	int GetNumberOfSubMesh(unsigned int id); 

	XMFLOAT4X4 GetBindPose(unsigned int idx); 

private: 
	std::string filename; 
	Dx11XMotionManager* motion_manager; 

	std::vector<Dx11XBufferInfo> buffer_info; 
	std::vector<ID3D11ShaderResourceView*>	m_pTexture;	

	std::vector<std::vector<Dx11XMaterialStandard> > material; // [buffer][material]
	std::vector<std::vector<Dx11XSubMesh> > sub_mesh; // [buffer][sub_mesh]

	std::vector<XMFLOAT4X4> bindpose; 

};
