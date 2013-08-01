#pragma once

#include <DX11Util/Dx11Scene.h>
#include <DX11Util/Dx11EffectStandard.h>
#include <DX11Util/Dx11ObjectGUICanvas.h>
#include <DX11Util/Dx11CameraStandard.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

class SampleScene05 : public Dx11Scene
{
public:
	SampleScene05(void);
	virtual ~SampleScene05(void);

	virtual void Setup(Dx11Context* context); 
	virtual void Update(); 
	virtual void Render(Dx11Context* context, Dx11Camera* camera, Dx11Lens* lens); 
	virtual void Term(Dx11Context* context); 

	virtual Dx11Scene* GetNext(Dx11Context* context); 

private: 

	float max; 

	Dx11ObjectGUICanvas* canvas; 
	Dx11EffectStandard* effect; 
	Dx11Camera* camera; 

	CvCapture *capture; 
	int cap_width, cap_height; 
};

