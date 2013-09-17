#include "SampleScene08.h"
#include <DX11Util/Lens/Dx11LensStandard.h>


SampleScene08::SampleScene08(void)
{
}


SampleScene08::~SampleScene08(void)
{
}


void SampleScene08::Setup(Dx11Context* context)
{
	left = 0; 
	effect = new Dx11EffectCapture(); 
	effect->Setup(context); 

	object = new Dx11ObjectGUIMovie(L"sample.mpg"); 
	object->Setup(context); 
	object->Start(); 
}

void SampleScene08::Update()
{
	left++; 
}
 
void SampleScene08::Render(Dx11Context* context, Dx11Camera* camera, Dx11Lens* lens)
{
	Dx11LensStandard stlens; 
	stlens.Setup(); 
	stlens.SetParameter(10.0f, 7.0f, 1.0f, 1000.0f); 

	if(lens == 0) {
		lens = &stlens; 
	}

	object->Update(context); 

	stlens.Setup(); 
	stlens.SetParameter(10.0f, 7.0f, 1.0f, 1000.0f); 
	effect->Update(context, object, 0.25, 0.25f, 0.5f, 0.5, 50.0f, &stlens, 1.0f, 0.0f); 
//	effect->Update(context, object, 0.25*0 + sin(left * 3.141592f / 30) * 0.25f*0, 0.0f, 1, 1, 50.0f, lens, 1.0f, 0.0f); 
}
 
void SampleScene08::Term(Dx11Context* context)
{
	if(effect) {
		effect->Term(context); 
		delete effect; 
		effect = 0; 
	}
	if(object) {
		object->Term(); 
		delete object; 
		object = 0; 
	}
}
 

Dx11Scene* SampleScene08::GetNext(Dx11Context* context)
{
	if(object->IsRunning()) {
		return this; 
	}
	return 0; 
}
 
bool SampleScene08::RefCameraParameter(XMFLOAT3* pos, XMFLOAT3* to, XMFLOAT3* up)
{
	return false; 
}
