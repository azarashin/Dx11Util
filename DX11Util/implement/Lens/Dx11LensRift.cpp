#include <DX11Util/Lens/Dx11LensRift.h>



Dx11LensRift::Dx11LensRift(
		float h_screen_size 
		, float v_screen_size 
		, float v_screen_center 
		, float eye_to_screen_distance 
		, float lens_separaton_distance
		, float interpopilary_distance 
		, float h_resolution 
		, float v_resolution 
		, float param_k[]
		, float distance_to_limit
	)
{
	original_lens = new Dx11LensStandard(); 
	original_lens->Setup(); 
	is_left = false; 
	shift = 0.0f; 
	float aspect; 

	float d_lens = lens_separaton_distance; 
	float h_meter = h_screen_size / 4.0f - d_lens / 2.0f; 
	shift = 4 * h_meter / h_screen_size; 
	aspect = h_resolution / (v_resolution); 
	float fov = (float)(2.0 * atan(v_screen_size / (2.0 * eye_to_screen_distance))); 

	original_lens->SetParameterFov(fov, aspect, eye_to_screen_distance, eye_to_screen_distance + distance_to_limit); 
}


Dx11LensRift::~Dx11LensRift(void)
{
	delete original_lens; 
}


HRESULT Dx11LensRift::Setup()
{
	is_left = false; 
	return S_OK; 
}



HRESULT Dx11LensRift::GetDistanceFar(float* distance)
{
	return original_lens->GetDistanceFar(distance); 
}

HRESULT Dx11LensRift::GetDistanceNear(float* distance)
{
	return original_lens->GetDistanceNear(distance); 
}

HRESULT Dx11LensRift::GetNearWidth(float* width)
{
	return original_lens->GetNearWidth(width); 
}

HRESULT Dx11LensRift::GetNearHeight(float* height)
{
	return original_lens->GetNearHeight(height); 
}

HRESULT Dx11LensRift::GetMatrix(XMFLOAT4X4* m)
{
	XMMATRIX mat, smat; 
	XMFLOAT4X4 xmf; 
	HRESULT ret = original_lens->GetMatrix(&xmf);  
	mat = XMLoadFloat4x4(&xmf); 
	if(is_left) {
		smat = XMMatrixTranslation(shift, 0, 0); 
	} else {
		smat = XMMatrixTranslation(-shift, 0, 0); 
	}

	XMStoreFloat4x4(m, mat * smat); 

	return ret; 
}

HRESULT Dx11LensRift::GetHAngle(float* angle)
{
	return original_lens->GetHAngle(angle); 
}

HRESULT Dx11LensRift::GetVAngle(float* angle)
{
	return original_lens->GetVAngle(angle); 
}

HRESULT Dx11LensRift::Term()
{
	return original_lens->Term(); 
}

HRESULT Dx11LensRift::SetIsLeft(bool _is_left)
{
	is_left = _is_left; 
	return S_OK; 
}
