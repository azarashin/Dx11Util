#pragma once

#include <DX11Util/Core/Dx11Scene.h>
#include <DX11Util/Object/Dx11ObjectGUIStandard.h>
#include <DX11Util/Effect/Dx11EffectGUIStandard.h>

// This sample show how to display image UI. 
class SampleScene02 : public Dx11Scene
{
public:
	SampleScene02(void);
	virtual ~SampleScene02(void);

	virtual void Setup(Dx11Context* context); 
	virtual void Update(); 
	virtual bool RefCameraParameter(XMFLOAT3* pos, XMFLOAT3* to, XMFLOAT3* up); 
	virtual void Render(Dx11Context* context, Dx11Camera* camera, Dx11Lens* lens); 
	virtual void Term(Dx11Context* context); 

	virtual Dx11Scene* GetNext(Dx11Context* context); 

private: 
	int left; 
	Dx11ObjectGUIStandard* text; 
	Dx11EffectGUIStandard* effect; 


	float max; 
};

