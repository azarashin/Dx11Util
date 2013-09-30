#include "StdAfx.h"
#include "SampleScene07.h"

#include <DX11Util/Motion/Dx11MotionStandard.h>
#include <DX11Util/Camera/Dx11CameraStandard.h>
#include <DX11Util/Lens/Dx11LensStandard.h>


SampleScene07::SampleScene07(void)
{
}


SampleScene07::~SampleScene07(void)
{
}

void SampleScene07::Setup(Dx11Context* context)
{
	left = 0; 
	max = 300.0f; 
	text = new Dx11XObject("wood.x"); 
	text->Setup(context); 


	effect = new Dx11XEffect(); 
	effect->Setup(context); 
	pp = 0; 

}

void SampleScene07::Update()
{
	left++; 

	pp += 0.01; 
}
 
void SampleScene07::Render(Dx11Context* context, Dx11Camera* camera, Dx11Lens* _lens)
{

//	EffectGUIStandardInfo info = {0, 0, 0.125f * w, 0.125f, 
//		1.0f, 0.5f, 0, 0}; // 完全不透明（1.0f), 色半反転(0,5f)
	Dx11MotionStandard mot(XMFLOAT3(1, 1, 1)); 
	Dx11CameraStandard cam; 
	Dx11LensStandard lens; 

//	mot.SetDirection(XMFLOAT3(0, 0, 1000.0), XMFLOAT3(0.57735f, 0.57735f, -0.57735f), XMFLOAT3(0, 1, 0)); 
	mot.SetDirection(XMFLOAT3(0, -3, 7.0), XMFLOAT3(cos(pp), 0, sin(pp)), XMFLOAT3(0, 1, 0)); 

	cam.Setup(); 
	cam.SetCameraDirection(XMFLOAT3(0, 0, 2000), XMFLOAT3(0, 0, 200), XMFLOAT3(0, 1, 0)); 

	lens.Setup(); 
	lens.SetParameter(640.0f, 480.0f, 100.0f, 10000.0f); 

	Dx11Camera* act_cam; 
	Dx11Lens* act_lens; 

	if(camera == 0) {
		act_cam = &cam; 
	} else {
		act_cam = camera; 
	}

	if(_lens == 0) {
		act_lens = &lens; 
	} else {
		act_lens = _lens; 
	}

	effect->Update(context, text, &mot, act_cam, act_lens); 
}
 
void SampleScene07::Term(Dx11Context* context)
{
	text->Term(); 
	effect->Term(context); 

	delete text; 
	delete effect;
}
 

Dx11Scene* SampleScene07::GetNext(Dx11Context* context)
{
	if(left < 3000) {
		return this; 
	}
	return 0; 
}
 
bool SampleScene07::RefCameraParameter(XMFLOAT3* pos, XMFLOAT3* to, XMFLOAT3* up)
{
	*pos = XMFLOAT3(0, 0, 0); 
	*to = XMFLOAT3(0, 0, 200); 
	*up = XMFLOAT3(0, 1, 0); 
	return true; 
}
