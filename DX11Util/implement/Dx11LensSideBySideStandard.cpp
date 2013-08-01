#include <DX11Util/Dx11LensSideBySideStandard.h>


Dx11LensSideBySideStandard::Dx11LensSideBySideStandard(void)
{
}


Dx11LensSideBySideStandard::~Dx11LensSideBySideStandard(void)
{
}

HRESULT Dx11LensSideBySideStandard::SetIsLeft(bool _is_left)
{
	return S_OK; 
}


HRESULT Dx11LensSideBySideStandard::Setup()
{
	return S_OK; 
}

HRESULT Dx11LensSideBySideStandard::SetParameter(float near_width, float near_height, float _near, float _far)
{
	width = near_width; 
	height = near_height; 
	dnear = _near; 
	dfar = _far; 
	hangle = atan((near_width / 2.0f) / dnear) * 2.0f; 
	vangle = atan((near_height / 2.0f) / dnear) * 2.0f; 

	XMMATRIX _mat = XMMatrixPerspectiveLH(width, height, dnear, dfar); 

	XMStoreFloat4x4(&mat, _mat); 

	return S_OK; 
}

HRESULT Dx11LensSideBySideStandard::SetParameterFov(float fov, float aspect, float _near, float _far)
{
	hangle = fov; 
	dnear = _near; 
	dfar = _far; 
	width = (float)(tan(hangle / 2.0) * dnear * 2.0); 
	height = width / aspect; 
	vangle = atan((height / 2.0f) / dnear) * 2.0f; 

	XMMATRIX _mat = XMMatrixPerspectiveFovLH(width, height, dnear, dfar); 

	XMStoreFloat4x4(&mat, _mat); 

	return S_OK; 
}


HRESULT Dx11LensSideBySideStandard::GetMatrix(XMMATRIX* m)
{
	*m = XMLoadFloat4x4(&mat); 
	return S_OK; 
}

HRESULT Dx11LensSideBySideStandard::Term()
{
	return S_OK; 
}


HRESULT Dx11LensSideBySideStandard::GetDistanceFar(float* distance)
{
	*distance = dfar; 
	return S_OK; 
}

HRESULT Dx11LensSideBySideStandard::GetDistanceNear(float* distance)
{
	*distance = dnear; 
	return S_OK; 
}

HRESULT Dx11LensSideBySideStandard::GetNearWidth(float* _width)
{
	*_width = width; 
	return S_OK; 
}

HRESULT Dx11LensSideBySideStandard::GetNearHeight(float* _height)
{
	*_height = height; 
	return S_OK; 
}


HRESULT Dx11LensSideBySideStandard::GetHAngle(float* angle)
{
	return S_OK; 
}

HRESULT Dx11LensSideBySideStandard::GetVAngle(float* angle)
{
	return S_OK; 
}
