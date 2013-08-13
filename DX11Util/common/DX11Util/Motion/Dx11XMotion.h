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

#include <DX11Util/Motion/Dx11Motion.h>

#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>

#include <xnamath.h>

#include <vector>

#include <string>

typedef struct {
	std::string name; 
	int parent; 
	XMFLOAT4X4 base;
	XMFLOAT4X4 mot; 
} Dx11MotionInfo; 

class Dx11XMotion : public Dx11Motion
{
public: 
	Dx11XMotion(int _part_max, int _frame_max); 
	virtual ~Dx11XMotion(); 

	virtual HRESULT Setup(); 
	virtual HRESULT Update(); 

	/**
	 * At first, check te number of matrix. The area which parameter mat show must have capacity to cover num of XMMMatrix.  
	 **/ 
	virtual HRESULT GetMatrix(XMMATRIX* mat); 
	virtual HRESULT GetNumberOfMatrix(int* num); 
	virtual HRESULT Term(); 

	void Start(); 
	bool IsFinished(); 

	void SetBase(const XMFLOAT4X4* _offset, const XMFLOAT4X4* _bias, int _part, int _parent); 
	void SetMotion(const XMFLOAT4X4* _mot, int _part, int _frame); 

	void UpdateMatrix(int idx); 

private: 
	int num_frame; 
	int num_part; 
	int current_frame; 
	int* parent; 
	int* mat_flag; // 0: not updated. 1: already updated. 
	XMFLOAT4X4* bias; 
	XMFLOAT4X4* offset; 
	XMFLOAT4X4* motion; 
	XMFLOAT4X4* cash_matrix; 

}; 

