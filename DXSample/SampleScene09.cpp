#include "SampleScene09.h"
#include <DX11Util/Lens/Dx11LensStandard.h>


SampleScene09::SampleScene09(void)
{
}


SampleScene09::~SampleScene09(void)
{
}


void SampleScene09::Setup(Dx11Context* context)
{
	left = 0; 
	effect = new Dx11EffectCapture(); 
	effect->Setup(context); 

	object = new Dx11ObjectGUIWebCam(); 
	object->Setup(context); 
}

void SampleScene09::Update()
{
	left++; 
}
 
void SampleScene09::Render(Dx11Context* context, Dx11Camera* camera, Dx11Lens* lens)
{
	Dx11LensStandard stlens; 
	stlens.Setup(); 
	stlens.SetParameter(10.0f, 7.0f, 1.0f, 1000.0f); 

	if(lens == 0) {
		lens = &stlens; 
	}

	object->Update(context); 

	effect->Update(context, object, 0.25*0 + sin(left * 3.141592f / 30) * 0.25f*0, 0.0f, 1, 1, 50.0f, lens, 1.0f, 0.0f); 
}
 
void SampleScene09::Term(Dx11Context* context)
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
 

Dx11Scene* SampleScene09::GetNext(Dx11Context* context)
{
	if(left < 300) {
		return this; 
	}
	return 0; 
}
 
bool SampleScene09::RefCameraParameter(XMFLOAT3* pos, XMFLOAT3* to, XMFLOAT3* up)
{
	return false; 
}
