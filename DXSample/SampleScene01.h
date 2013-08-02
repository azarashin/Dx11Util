#pragma once

#include <DX11Util/Core/Dx11Scene.h>
#include <DX11Util/Object/Dx11ObjectGUIText.h>
#include <DX11Util/Effect/Dx11EffectGUIStandard.h>


class SampleScene01 : public Dx11Scene
{
public:
	SampleScene01(void);
	virtual ~SampleScene01(void);

	virtual void Setup(Dx11Context* context); 
	virtual void Update(); 
	virtual void Render(Dx11Context* context, Dx11Camera* camera, Dx11Lens* lens); 
	virtual void Term(Dx11Context* context); 

	virtual Dx11Scene* GetNext(Dx11Context* context); 

private: 
	int left; 
	Dx11ObjectGUIText* text; 
	Dx11EffectGUIStandard* effect; 

	float max; 

	Dx11Scene* scene2;
};

