#include "StdAfx.h"
#include "SampleScene02.h"


SampleScene02::SampleScene02(void)
{
}


SampleScene02::~SampleScene02(void)
{
}

void SampleScene02::Setup(Dx11Context* context)
{
	left = 0; 
	max = 300.0f; 
	text = new Dx11ObjectGUIStandard("image.dds"); 
	text->Setup(context); 

	effect = new Dx11EffectGUIStandard(); 
	effect->Setup(context); 

}

void SampleScene02::Update()
{
	left++; 
}
 
void SampleScene02::Render(Dx11Context* context, Dx11Camera* camera, Dx11Lens* lens)
{
	wchar_t buf[32]; 

	EffectGUIStandardInfo info = {0.25 + sin(left * 3.141592f / 30) * 0.25f, 0, 0.25f, 0.25f, 1.0f, 0, 0, 0}; 

	effect->Update(context, text, &info); 
}
 
void SampleScene02::Term(Dx11Context* context)
{
	text->Term(); 
	effect->Term(context); 

	delete text; 
	delete effect; 
}
 

Dx11Scene* SampleScene02::GetNext(Dx11Context* context)
{
	if(left < 300) {
		return this; 
	}
	return 0; 
}
 
bool SampleScene02::RefCameraParameter(XMFLOAT3* pos, XMFLOAT3* to, XMFLOAT3* up)
{
	return false; 
}
