#pragma once

#include <DX11Util/Core/Dx11Scene.h>
#include <DX11Util/Effect/Dx11EffectGUITrans.h>
#include <DX11Util/Effect/Dx11EffectGUIStandard.h>
#include <DX11Util/Object/Dx11ObjectGUICanvas.h>
#include <DX11Util/Camera/Dx11CameraStandard.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

// Play movie with Dx11EffectGUITrans. It only change distance from camera. 
class SampleScene05 : public Dx11Scene
{
public:
	SampleScene05(void);
	virtual ~SampleScene05(void);

	virtual void Setup(Dx11Context* context); 
	virtual void Update(); 
	virtual bool RefCameraParameter(XMFLOAT3* pos, XMFLOAT3* to, XMFLOAT3* up); 
	virtual void Render(Dx11Context* context, Dx11Camera* camera, Dx11Lens* lens); 
	virtual void Term(Dx11Context* context); 

	virtual Dx11Scene* GetNext(Dx11Context* context); 

private: 

	float max; 

	Dx11ObjectGUICanvas* canvas; 
	Dx11EffectGUITrans* effect; 
	Dx11EffectGUIStandard* effect2; 
	Dx11Camera* camera; 

	CvCapture *capture; 
	int cap_width, cap_height; 
};

