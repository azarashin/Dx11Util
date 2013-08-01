#pragma once

#include <DX11Util/Dx11Scene.h>
#include <DX11Util/Dx11ObjectGUIStandard.h>
#include <DX11Util/Dx11EffectGUIStandard.h>


class SampleScene02 : public Dx11Scene
{
public:
	SampleScene02(void);
	virtual ~SampleScene02(void);

	virtual void Setup(Dx11Context* context); 
	virtual void Update(); 
	virtual void Render(Dx11Context* context, Dx11Camera* camera, Dx11Lens* lens); 
	virtual void Term(Dx11Context* context); 

	virtual Dx11Scene* GetNext(Dx11Context* context); 

private: 
	int left; 
	Dx11ObjectGUIStandard* text; 
	Dx11EffectGUIStandard* effect; 


	float max; 
};

