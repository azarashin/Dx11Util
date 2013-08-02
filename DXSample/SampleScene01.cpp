#include "StdAfx.h"
#include "SampleScene01.h"

#include "SampleScene02.h"


SampleScene01::SampleScene01(void)
{
	scene2 = new SampleScene02(); 
}


SampleScene01::~SampleScene01(void)
{
	delete scene2; 
}

void SampleScene01::Setup(Dx11Context* context)
{
	left = 0; 
	max = 300.0f; 
	text = new Dx11ObjectGUIText(64); 
	text->Setup(context); 

	effect = new Dx11EffectGUIStandard(); 
	effect->Setup(context); 

}

void SampleScene01::Update()
{
	left++; 
}
 
void SampleScene01::Render(Dx11Context* context, Dx11Camera* camera, Dx11Lens* lens)
{
	wchar_t buf[32]; 
	wsprintf(buf, L"�J�E���g: %d", left); 
	float w = text->SetText(context, buf); 

	EffectGUIStandardInfo info = {0, 0, 0.125f * w, 0.125f, 
		1.0f, 0.5f, 0, 0}; // ���S�s�����i1.0f), �F�����](0,5f)

	effect->Update(context, text, &info); 
}
 
void SampleScene01::Term(Dx11Context* context)
{
	text->Term(); 
	effect->Term(context); 

	delete text; 
	delete effect; 
}
 

Dx11Scene* SampleScene01::GetNext(Dx11Context* context)
{
	if(left < 300) {
		return this; 
	}
	return scene2; 
}
 