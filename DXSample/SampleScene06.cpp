#include "StdAfx.h"
#include "SampleScene06.h"

#include <DX11Util/Motion/Dx11MotionStandard.h>
#include <DX11Util/Camera/Dx11CameraStandard.h>
#include <DX11Util/Lens/Dx11LensStandard.h>


SampleScene06::SampleScene06(void)
{
}


SampleScene06::~SampleScene06(void)
{
}

void SampleScene06::Setup(Dx11Context* context)
{
	left = 0; 
	max = 300.0f; 
	text = new Dx11ObjectGUIText(64); 
	text->Setup(context); 


	effect = new Dx11EffectGUITrans(); 
	effect->Setup(context); 

}

void SampleScene06::Update()
{
	left++; 
}
 
void SampleScene06::Render(Dx11Context* context, Dx11Camera* camera, Dx11Lens* _lens)
{
	wchar_t buf[32]; 
	wsprintf(buf, L"カウント: %d", left); 
	float w = text->SetText(context, buf); 

//	EffectGUIStandardInfo info = {0, 0, 0.125f * w, 0.125f, 
//		1.0f, 0.5f, 0, 0}; // 完全不透明（1.0f), 色半反転(0,5f)
	Dx11MotionStandard mot(XMFLOAT3(300, 200, 300)); 
	Dx11CameraStandard cam; 
	Dx11LensStandard lens; 

	mot.SetDirection(XMFLOAT3(0, 0, 400), XMFLOAT3(0.57735f, 0.57735f, -0.57735f), XMFLOAT3(0, 1, 0)); 

	cam.Setup(); 
	cam.SetCameraDirection(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 200), XMFLOAT3(0, 1, 0)); 

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
 
void SampleScene06::Term(Dx11Context* context)
{
	text->Term(); 
	effect->Term(context); 

	delete text; 
	delete effect;
}
 

Dx11Scene* SampleScene06::GetNext(Dx11Context* context)
{
	if(left < 3000) {
		return this; 
	}
	return 0; 
}
 
