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

#include <DX11Util/Dx11ObjectWaveFront.h>


#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

// under construction! 
Dx11ObjectWaveFront::Dx11ObjectWaveFront(ID3D11Device* _pd3dDevice, const char* _filename)
	: Dx11Object()
{
	pd3dDevice = _pd3dDevice; 
	filename = _filename; 
}


Dx11ObjectWaveFront::~Dx11ObjectWaveFront(void)
{
}





unsigned int Dx11ObjectWaveFront::getMaxOffset( domInput_local_offset_Array &input_array ) {
	unsigned int maxOffset = 0;
	for ( unsigned int i = 0; i < input_array.getCount(); i++ ) {
		if ( input_array[i]->getOffset() > maxOffset ) {
			maxOffset = (unsigned int)input_array[i]->getOffset();
		}
	}
	return maxOffset;
}

void Dx11ObjectWaveFront::Setup()
{


}

void Dx11ObjectWaveFront::Term()
{
}

BOOL Dx11ObjectWaveFront::GetVertexNum(unsigned int idx, unsigned int* num)
{
	*num = m_dwVertices[idx]; 
	return true; 
}

BOOL Dx11ObjectWaveFront::GetIndexNum(unsigned int idx, unsigned int* num)
{
	*num = m_dwIndecies[idx]; 
	return true; 
}


BOOL Dx11ObjectWaveFront::GetBufNum(unsigned int* num)
{
	*num = m_pVB.size(); 
	return TRUE; 
}

BOOL Dx11ObjectWaveFront::GetTexture(unsigned int num,  ID3D11ShaderResourceView** pTexture)
{
	if(num > m_pTexture.size() || m_pTexture.size()==0)
	{
		*pTexture = NULL;
		return FALSE;
	}

	*pTexture = m_pTexture[num];
	return TRUE;
}

BOOL Dx11ObjectWaveFront::GetVertexBuf(unsigned int num, ID3D11Buffer** buf)
{
	if(num > m_pVB.size() || m_pVB.size()==0)
	{
		*buf = NULL;
		return FALSE;
	}

	*buf = m_pVB[num];
	return TRUE;
}


BOOL Dx11ObjectWaveFront::GetWeightBuf(unsigned int num, ID3D11Buffer** buf)
{
	*buf = NULL;
	return FALSE;
}

BOOL Dx11ObjectWaveFront::GetIndexBuf(unsigned int num, ID3D11Buffer** buf)
{
	if(num > m_pIB.size() || m_pIB.size()==0)
	{
		*buf = NULL;
		return FALSE;
	}

	*buf = m_pIB[num];
	return TRUE;
}
