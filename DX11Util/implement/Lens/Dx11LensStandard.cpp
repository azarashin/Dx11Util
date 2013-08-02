#include <DX11Util/Dx11LensStandard.h>


Dx11LensStandard::Dx11LensStandard(void)
{
}


Dx11LensStandard::~Dx11LensStandard(void)
{
}


HRESULT Dx11LensStandard::Setup()
{
	return S_OK; 
}

HRESULT Dx11LensStandard::SetParameter(float near_width, float near_height, float _near, float _far)
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

HRESULT Dx11LensStandard::SetParameterFov(float fov, float aspect, float _near, float _far)
{
	hangle = fov; 
	dnear = _near; 
	dfar = _far; 
	width = (float)(tan(hangle / 2.0) * dnear * 2.0); 
	height = width / aspect; 
	vangle = atan((height / 2.0f) / dnear) * 2.0f; 

	XMMATRIX _mat = XMMatrixPerspectiveFovLH(vangle, aspect, dnear, dfar); 

	XMStoreFloat4x4(&mat, _mat); 

	return S_OK; 
}


HRESULT Dx11LensStandard::GetMatrix(XMMATRIX* m)
{
	*m = XMLoadFloat4x4(&mat); 
	return S_OK; 
}

HRESULT Dx11LensStandard::Term()
{
	return S_OK; 
}


HRESULT Dx11LensStandard::GetDistanceFar(float* distance)
{
	*distance = dfar; 
	return S_OK; 
}

HRESULT Dx11LensStandard::GetDistanceNear(float* distance)
{
	*distance = dnear; 
	return S_OK; 
}

HRESULT Dx11LensStandard::GetNearWidth(float* _width)
{
	*_width = width; 
	return S_OK; 
}

HRESULT Dx11LensStandard::GetNearHeight(float* _height)
{
	*_height = height; 
	return S_OK; 
}


HRESULT Dx11LensStandard::GetHAngle(float* angle)
{
	return S_OK; 
}

HRESULT Dx11LensStandard::GetVAngle(float* angle)
{
	return S_OK; 
}
