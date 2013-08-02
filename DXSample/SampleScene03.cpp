#include "StdAfx.h"

#include "SampleScene03.h"
#include "SampleScene01.h"
#include "SampleScene06.h"


#include <DX11Util/Camera/Dx11CameraSideBySide.h>
#include <DX11Util/Camera/Dx11CameraSideBySideStandard.h>
#include <DX11Util/Camera/Dx11CameraRift.h>
#include <DX11Util/Lens/Dx11LensSideBySide.h>
#include <DX11Util/Lens/Dx11LensSideBySideStandard.h>
#include <DX11Util/Lens/Dx11LensRift.h>



SampleScene03::SampleScene03(void)
{
	act = new SampleScene06(); 
}


SampleScene03::~SampleScene03(void)
{
}

void SampleScene03::Setup(Dx11Context* context)
{
	EffectSideBySideInfo param; 
	bool is7Inch = true; 

	param.LensCenter[0] = 0.5f; 
	param.LensCenter[1] = 0.5f; 

	param.ScreenCenter[0] = 0.0f; 
	param.ScreenCenter[1] = 0.0f; 

	param.Scale[0] = 1.0f; 
	param.Scale[1] = 1.0f; 

	param.ScaleIn[0] = 1.0f; 
	param.ScaleIn[1] = 1.0f; 

	if (is7Inch)
    {
        // 7" screen.
        param.HmdWarpParam[0]      = 1.0f;
        param.HmdWarpParam[1]      = 0.22f;
        param.HmdWarpParam[2]      = 0.24f;
        param.HmdWarpParam[3]      = 0.0f;

//        param->EyeToScreenDistance = 0.041f;
    }
    else
    {
        param.HmdWarpParam[0]      = 1.0f;
        param.HmdWarpParam[1]      = 0.18f;
        param.HmdWarpParam[2]      = 0.115f;
        param.HmdWarpParam[3]      = 0.0f;

/*
	if (HResolution == 1920) {
			hmdInfo->EyeToScreenDistance = 0.040f;
		} else {
			hmdInfo->EyeToScreenDistance = 0.0387f;
		}
	*/
    }

	param.ChromaAbCorrection[0] = 0.996f;
	param.ChromaAbCorrection[1] = -0.004f;
	param.ChromaAbCorrection[2] = 1.014f;
	param.ChromaAbCorrection[3] = 0.0f;


	effect = new Dx11EffectSideBySide(640, 480, &param); 
	effect->Setup(context); 

	if(act) {
		act->Setup(context); 
	}

}

void SampleScene03::Update()
{
	act->Update(); 
}
 
void SampleScene03::Render(Dx11Context* context, Dx11Camera* camera, Dx11Lens* _lens)
{
#if 0
	Dx11CameraSideBySideStandard cam; 
	cam.Setup(); 
	cam.SetCameraDirection(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 200), XMFLOAT3(0, 1, 0)); 

	Dx11LensSideBySideStandard lens; 
	lens.Setup(); 
	lens.SetParameter(640.0f, 480.0f, 100.0f, 10000.0f); 
#else 
	Dx11CameraRift cam; 
	cam.Setup(); 
	cam.SetCameraDirection(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 200), XMFLOAT3(0, 1, 0)); 

	Dx11LensRift lens(0.14976f, 0.0935f, 0.0f, 0.041f,  0.0635f, 0.064f, 640, 480, 0, 10000.0f); 
//	Dx11LensRift lens(0.14976f, 0.0935f, 0.0f, 100.041f,  0.0635f, 0.064f, 640, 480, 0, 10000.0f); 
	lens.Setup(); 

	Dx11LensSideBySideStandard lens2; 
	lens2.Setup(); 
	lens2.SetParameter(640.0f, 480.0f, 100.0f, 10000.0f); 
#endif

	effect->Update(context, act, &cam, &lens); 
}
 
void SampleScene03::Term(Dx11Context* context)
{
	if(act) {
		act->Term(context); 
	}
}
 

Dx11Scene* SampleScene03::GetNext(Dx11Context* context)
{
	Dx11Scene* next = act->GetNext(context); 
	if(next == 0) {
		act->Term(context); 
		delete act; 
		act = 0; 
		return 0; 
	}
	if(act != next) {
		act->Term(context); 
		act = next; 
		act->Setup(context); 
	}
	return this; 
}
 
