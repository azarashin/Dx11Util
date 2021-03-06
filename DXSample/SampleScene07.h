#pragma once

#include <DX11Util/Core/Dx11Scene.h>
#include <DX11Util/Object/Dx11XObject.h>
#include <DX11Util/Effect/Dx11XEffect.h>
#include <DX11Util/Lens/Dx11LensStandard.h>

// This sample show how to use Dx11XObject and Dx11XEffect. 
class SampleScene07 : public Dx11Scene
{
public:
	SampleScene07(void);
	virtual ~SampleScene07(void);

	virtual void Setup(Dx11Context* context); 
	virtual void Update(); 
	virtual bool RefCameraParameter(XMFLOAT3* pos, XMFLOAT3* to, XMFLOAT3* up); 
	virtual void Render(Dx11Context* context, Dx11Camera* camera, Dx11Lens* lens); 
	virtual void Term(Dx11Context* context); 

	virtual Dx11Scene* GetNext(Dx11Context* context); 

private: 
	int left; 
	Dx11XObject* text; 
	Dx11XEffect* effect; 

	double pp; 

	float max; 
};

