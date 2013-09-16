#pragma once

#include <DX11Util/Core/Dx11Scene.h>
#include <DX11Util/Effect/Dx11EffectCapture.h>
#include <DX11Util/Object/Dx11ObjectGUICapture.h>
#include <DX11Util/Object/Dx11ObjectGUIMovie.h>
#include <DX11Util/Object/Dx11ObjectGUIWebCam.h>

class SampleScene09 : public Dx11Scene
{
public:
	SampleScene09(void);
	virtual ~SampleScene09(void);

	virtual void Setup(Dx11Context* context); 
	virtual void Update(); 
	virtual bool RefCameraParameter(XMFLOAT3* pos, XMFLOAT3* to, XMFLOAT3* up); 
	virtual void Render(Dx11Context* context, Dx11Camera* camera, Dx11Lens* lens); 
	virtual void Term(Dx11Context* context); 

	virtual Dx11Scene* GetNext(Dx11Context* context); 

	void SetNext(Dx11Scene* context); 
private: 
	int left; 
	Dx11EffectCapture* effect; 
//	Dx11ObjectGUIMovie* object; 
	Dx11ObjectGUIWebCam* object; 

};

