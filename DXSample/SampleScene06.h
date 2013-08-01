#pragma once

#include <DX11Util/Dx11Scene.h>
#include <DX11Util/Dx11ObjectGUIText.h>
#include <DX11Util/Dx11EffectGUITrans.h>
#include <DX11Util/Dx11LensStandard.h>


class SampleScene06 : public Dx11Scene
{
public:
	SampleScene06(void);
	virtual ~SampleScene06(void);

	virtual void Setup(Dx11Context* context); 
	virtual void Update(); 
	virtual void Render(Dx11Context* context, Dx11Camera* camera, Dx11Lens* lens); 
	virtual void Term(Dx11Context* context); 

	virtual Dx11Scene* GetNext(Dx11Context* context); 

private: 
	int left; 
	Dx11ObjectGUIText* text; 
	Dx11EffectGUITrans* effect; 


	float max; 

	Dx11Scene* scene2;
};

