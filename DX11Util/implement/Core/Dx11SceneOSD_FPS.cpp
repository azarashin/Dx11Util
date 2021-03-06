#include <DX11Util/Core/Dx11SceneOSD_FPS.h>




Dx11SceneOSD_FPS::Dx11SceneOSD_FPS(void)
{
}


Dx11SceneOSD_FPS::~Dx11SceneOSD_FPS(void)
{
}

void Dx11SceneOSD_FPS::Setup(Dx11Context* context)
{
	text = new Dx11ObjectGUIText(64); 
	text->Setup(context); 

	effect = new Dx11EffectGUIStandard(); 
	effect->Setup(context); 

}

void Dx11SceneOSD_FPS::Update()
{
}
 
void Dx11SceneOSD_FPS::Render(Dx11Context* context, Dx11Camera* camera, Dx11Lens* lens)
{
	wchar_t buf[32]; 
	float fps_info = 0.0f;
	float fps_info2 = 0.0f;
	context->GetFPS(&fps_info); 
	fps_info2 = fps_info; 
	swprintf_s(buf, 32, L"FPS %.2f", fps_info2); 
//	wsprintf(buf, L"FPS %f", fps); 
	float w = text->SetText(context, buf); 

	EffectGUIStandardInfo info = {0, 1.0f - 0.125f, 0.125f * w, 0.125f, 
		0.5f, 0.5f, 0, 0}; // �������i0.5f), �F�����](0,5f)

	effect->Update(context, text, &info); 
}
 
void Dx11SceneOSD_FPS::Term(Dx11Context* context)
{
	text->Term(); 
	effect->Term(context); 

	delete text; 
	delete effect; 
}
 

Dx11Scene* Dx11SceneOSD_FPS::GetNext(Dx11Context* context)
{
	return 0; 
}
 
bool Dx11SceneOSD_FPS::RefCameraParameter(XMFLOAT3* pos, XMFLOAT3* to, XMFLOAT3* up)
{
	return false; 
}
