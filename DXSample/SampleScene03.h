#pragma once

#include <DX11Util/Dx11Scene.h>
#include <DX11Util/Dx11ObjectGUIStandard.h>
#include <DX11Util/Dx11EffectGUIStandard.h>
#include <DX11Util/Dx11EffectSideBySide.h>

class SampleScene03 : public Dx11Scene
{
public:
	SampleScene03(void);
	virtual ~SampleScene03(void);

	virtual void Setup(Dx11Context* context); 
	virtual void Update(); 
	virtual void Render(Dx11Context* context, Dx11Camera* camera, Dx11Lens* lens); 
	virtual void Term(Dx11Context* context); 

	virtual Dx11Scene* GetNext(Dx11Context* context); 

private: 
	float max; 

	Dx11EffectSideBySide* effect; 

	Dx11Scene* act; 
};

