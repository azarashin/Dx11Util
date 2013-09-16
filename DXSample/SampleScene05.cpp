#include "StdAfx.h"

#include <DX11Util/Lens/Dx11LensStandard.h>

#include "SampleScene05.h"
#include "SampleScene01.h"


SampleScene05::SampleScene05(void)
{
}


SampleScene05::~SampleScene05(void)
{
}

void SampleScene05::Setup(Dx11Context* context)
{
	canvas = 0; 
	max = 0; 

	capture = cvCreateCameraCapture(0); 
	if(capture == 0) {
		capture = cvCreateFileCapture("sample.mp4"); 
	}

	if(capture == 0) {
		return; 
	}
	cap_width = cvGetCaptureProperty (capture, CV_CAP_PROP_FRAME_WIDTH);
	cap_height = cvGetCaptureProperty (capture, CV_CAP_PROP_FRAME_HEIGHT);

	canvas = new Dx11ObjectGUICanvas(cap_width, cap_height); 
	canvas->Setup(context); 

	effect = new Dx11EffectGUITrans(); 
	effect->Setup(context); 
	effect2 = new Dx11EffectGUIStandard(); 
	effect2->Setup(context); 

	camera = new Dx11CameraStandard(); 

}

void SampleScene05::Update()
{
	max++; 
}
 
void SampleScene05::Render(Dx11Context* context, Dx11Camera* _camera, Dx11Lens* lens)
{
	D3D11_MAPPED_SUBRESOURCE map_info; 
	int x, y, offset; 
	IplImage *frame = 0; 
	if(capture) {
		frame = cvQueryFrame (capture);
	} else {
		return; 
	}


	if(frame) {
		canvas->Map(context, &map_info); 
		offset = 0; 
		for(y=0;y<cap_height;y++) {
			for(x=0;x<cap_width;x++) {
				unsigned char* out = &(((unsigned char*)map_info.pData)[offset + x * 4]); 
				out[0] = frame->imageData[y * frame->widthStep + x * frame->nChannels + 2]; 
				out[1] = frame->imageData[y * frame->widthStep + x * frame->nChannels + 1]; 
				out[2] = frame->imageData[y * frame->widthStep + x * frame->nChannels + 0]; 
				out[3] = 255; 
			}

			offset += map_info.RowPitch; 
		}
		canvas->Unmap(context); 
	}

	EffectGUIStandardInfo info = {0.25 + sin(max * 3.141592f / 30) * 0.25f, 0, 0.25f, 0.25f, 1.0f, 0.0f, 0, 0}; 
	Dx11LensStandard stlens; 
	stlens.Setup(); 
	stlens.SetParameter(10.0f, 7.0f, 1.0f, 1000.0f); 

	if(lens == 0) {
		lens = &stlens; 
	}

	effect->Update(context, canvas, 0.25 + sin(max * 3.141592f / 30) * 0.25f, 0.0f, 0.25f, 0.25f, 50.0f, lens, 1.0f, 0.0f); 

//	effect2->Update(context, canvas, &info); 
}
 
void SampleScene05::Term(Dx11Context* context)
{
	if(capture) {
		cvReleaseCapture(&capture); 
	}
	if(canvas) {
		canvas->Term(); 
		delete canvas; 
		canvas = 0; 
	}

	if(effect) {
		effect->Term(context); 
		delete effect; 
	}
	if(effect2) {
		effect2->Term(context); 
		delete effect2; 
	}
}
 

Dx11Scene* SampleScene05::GetNext(Dx11Context* context)
{
	if(max > 60 * 60 * 2) {
		return 0; 
	}
	return this; 
}
 
bool SampleScene05::RefCameraParameter(XMFLOAT3* pos, XMFLOAT3* to, XMFLOAT3* up)
{
	return false; 
}
