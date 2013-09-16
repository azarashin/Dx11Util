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

#include <DX11Util/Core/Dx11Context.h>
#include <DX11Util/Object/Dx11ObjectGUI.h>
#include <DX11Util/Object/Dx11ObjectGUICanvas.h>

#include <d3d11.h>

// #include <dxerr.h>

#include <xnamath.h>
#include <windows.h>
#include <dshow.h>
#include <stdio.h>
#include <conio.h>

#include "strmif.h"
#include "Aviriff.h"
#include "dvdmedia.h"
#include "bdaiface.h"

#include <vector>
#include <string>


interface ISampleGrabberCB : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE SampleCB(
        double SampleTime,
        IMediaSample *pSample) = 0;

    virtual HRESULT STDMETHODCALLTYPE BufferCB(
        double SampleTime,
        BYTE *pBuffer,
        LONG BufferLen) = 0;

};

interface ISampleGrabber : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE SetOneShot(
        BOOL OneShot) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetMediaType(
        const AM_MEDIA_TYPE *pType) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetConnectedMediaType(
        AM_MEDIA_TYPE *pType) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetBufferSamples(
        BOOL BufferThem) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetCurrentBuffer(
        LONG *pBufferSize,
        LONG *pBuffer) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetCurrentSample(
        IMediaSample **ppSample) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetCallback(
        ISampleGrabberCB *pCallback,
        LONG WhichMethodToCallback) = 0;

};

/**
 *
 * @note IMPORTANT: link to Strmbase.lib when you use this class. 
 **/ 
class Dx11ObjectGUICapture
	: public Dx11ObjectGUI
{
public:
	Dx11ObjectGUICapture();
	virtual ~Dx11ObjectGUICapture(void);

	void SetupCanvas(Dx11Context* _context, int width, int height);
	void TermCanvas();


	BOOL GetBufNum(unsigned int* num); 

	BOOL GetVertexBuf(unsigned int num, ID3D11Buffer** buf); 
	BOOL GetIndexBuf(unsigned int num, ID3D11Buffer** buf); 
	BOOL GetWeightBuf(unsigned int num, ID3D11Buffer** buf); 

	BOOL GetVertexNum(unsigned int idx, unsigned int* num); 
	BOOL GetIndexNum(unsigned int idx, unsigned int* num); 

	BOOL GetTexture(unsigned int num, ID3D11ShaderResourceView** pTexture); 
	BOOL GetTextureNum(unsigned int* num); 

	void Update(Dx11Context* context); 
	virtual void GetPicture(char* buf) = 0; 

protected:
	IPin *GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir);
	IPin * GetInPin( IBaseFilter * pFilter);
	IPin * GetOutPin( IBaseFilter * pFilter);

private:
	Dx11ObjectGUICanvas* canvas; 

};

