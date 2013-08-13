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
#include <DX11Util/Motion/Dx11XMotion.h>

#include <d3d11.h>
#include <d3dx11.h>
// #include <dxerr.h>

#include <xnamath.h>

#include <vector>
#include <map>
#include <string>

class Dx11XMotionManager : public Dx11Motion
{
public: 
	Dx11XMotionManager(int bone_max); 
	virtual ~Dx11XMotionManager(); 

	virtual HRESULT Setup(); 
	virtual HRESULT Update(); 
	virtual HRESULT GetMatrix(XMMATRIX* mat); 
	virtual HRESULT GetNumberOfMatrix(int* num); 
	virtual HRESULT Term(); 

	/**
	 * change animation immediately. 
	 **/ 
	void SelectAnimation(std::string, bool loop); 
	void ReserveNextAnimation(std::string, bool loop); 

	/**
	 * added instance of Dx11XMotion which is already setup. It will be managed by Dx11XMotionManager. 
	 * Dx11XMotionManager will delete the instance. 
	 **/ 
	void AddAnimation(std::string name, Dx11XMotion* _motion); 

private: 
	Dx11XMotion* current; 
	Dx11XMotion* next; 
	std::map<std::string, Dx11XMotion*> motion_map; 
	bool loop_flag; 

}; 
