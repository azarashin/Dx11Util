#include "StdAfx.h"

#include "SampleScene04.h"
#include "SampleScene01.h"


SampleScene04::SampleScene04(void)
{
}


SampleScene04::~SampleScene04(void)
{
}

void SampleScene04::Setup(Dx11Context* context)
{
	canvas = 0; 
	max = 0; 
	image = 0; 

	capture = cvCreateCameraCapture(0); 
	if(capture == 0) {
		capture = cvCreateFileCapture("sample.mp4"); 
	}

	if(capture == 0) {
		return; 
	}
	cap_width = cvGetCaptureProperty (capture, CV_CAP_PROP_FRAME_WIDTH);
	cap_height = cvGetCaptureProperty (capture, CV_CAP_PROP_FRAME_HEIGHT);
	cap_fps = cvGetCaptureProperty (capture, CV_CAP_PROP_FPS);

	canvas = new Dx11ObjectGUICanvas(cap_width, cap_height); 
	canvas->Setup(context); 

	effect = new Dx11EffectGUIStandard(); 
	effect->Setup(context); 

	left_time = 0.0f; 
	image = cvCreateImage(cvSize(cap_width, cap_height), IPL_DEPTH_8U, 3); 

}

void SampleScene04::Update()
{
	max++; 

	IplImage *frame = 0; 

	if(cap_fps == 0) {
		return; 
	}

	left_time += cap_fps; 
	if(left_time > 60) {
		left_time -= 60; 
		if(capture) {
			frame = cvQueryFrame (capture);
			cvCopyImage(frame, image); 
		}
	}
}
 
void SampleScene04::Render(Dx11Context* context, Dx11Camera* camera, Dx11Lens* lens)
{
	D3D11_MAPPED_SUBRESOURCE map_info; 
	int x, y, offset; 

	IplImage *frame = 0; 
	if(cap_fps == 0) {
		if(capture) {
			frame = cvQueryFrame (capture);
			cvCopyImage(frame, image); 
		}
	}

	if(image) {
		canvas->Map(context, &map_info); 
		offset = 0; 
		for(y=0;y<cap_height;y++) {
			for(x=0;x<cap_width;x++) {
				unsigned char* out = &(((unsigned char*)map_info.pData)[offset + x * 4]); 
				out[0] = image->imageData[y * image->widthStep + x * image->nChannels + 2]; 
				out[1] = image->imageData[y * image->widthStep + x * image->nChannels + 1]; 
				out[2] = image->imageData[y * image->widthStep + x * image->nChannels + 0]; 
				out[3] = 255; 
			}

			offset += map_info.RowPitch; 
		}
		canvas->Unmap(context); 
	}

	EffectGUIStandardInfo info = {0.25 + sin(max * 3.141592f / 30) * 0.25f, 0, 0.25f, 0.25f, 1.0f, 0, 0, 0}; 

	effect->Update(context, canvas, &info); 
}
 
void SampleScene04::Term(Dx11Context* context)
{
	if(image) {
		cvReleaseImage(&image); 
	}
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
}
 

Dx11Scene* SampleScene04::GetNext(Dx11Context* context)
{
	if(max > 60 * 60 * 2) {
		return 0; 
	}
	return this; 
}
 
