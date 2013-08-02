#pragma once

#include <DX11Util/Core/Dx11Scene.h>
#include <DX11Util/Effect/Dx11EffectGUIStandard.h>
#include <DX11Util/Object/Dx11ObjectGUICanvas.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

class SampleScene04 : public Dx11Scene
{
public:
	SampleScene04(void);
	virtual ~SampleScene04(void);

	virtual void Setup(Dx11Context* context); 
	virtual void Update(); 
	virtual void Render(Dx11Context* context, Dx11Camera* camera, Dx11Lens* lens); 
	virtual void Term(Dx11Context* context); 

	virtual Dx11Scene* GetNext(Dx11Context* context); 

private: 
	int max; 

	Dx11ObjectGUICanvas* canvas; 
	Dx11EffectGUIStandard* effect; 

	CvCapture *capture; 
	int cap_width, cap_height; 
	float cap_fps; 
	IplImage* image; 
	float left_time; 
};

