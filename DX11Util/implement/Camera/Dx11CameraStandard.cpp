#include <DX11Util/Camera/Dx11CameraStandard.h>



Dx11CameraStandard::Dx11CameraStandard(void)
{
}


Dx11CameraStandard::~Dx11CameraStandard(void)
{
}

HRESULT Dx11CameraStandard::Setup()
{
	return S_OK; 
}

HRESULT Dx11CameraStandard::GetMatrix(XMMATRIX* m)
{
	*m = XMLoadFloat4x4(&mat); 
	return S_OK; 
}
HRESULT Dx11CameraStandard::GetBackwardMatrix(XMMATRIX* m)
{
	*m = XMLoadFloat4x4(&bmat); 
	return S_OK; 
}

HRESULT Dx11CameraStandard::Term()
{
	return S_OK; 
}

void Dx11CameraStandard::SetCameraDirection(const XMFLOAT3& pos, const XMFLOAT3& at)
{
	SetCameraDirection(pos, at, XMFLOAT3(0.0f, 1.0f, 0.0f)); 
}

void Dx11CameraStandard::SetCameraDirection(const XMFLOAT3& _pos, const XMFLOAT3& _at, const XMFLOAT3& _upper)
{
	XMVECTOR pos = XMLoadFloat3(&_pos); 
	XMVECTOR at = XMLoadFloat3(&_at); 
	XMVECTOR upper = XMLoadFloat3(&_upper); 
	XMVECTOR front = at - pos; 
	XMVECTOR back_at = pos - front; 
	XMMATRIX _mat = XMMatrixLookAtLH(pos, at, upper); 
	XMMATRIX _bmat = XMMatrixLookAtLH(pos, back_at, upper); 

	XMStoreFloat4x4(&mat, _mat); 
	XMStoreFloat4x4(&bmat, _bmat); 
}
