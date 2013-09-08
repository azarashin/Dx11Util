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

HRESULT Dx11CameraStandard::GetMatrix(XMFLOAT4X4* m)
{
	*m = mat; 
	return S_OK; 
}
HRESULT Dx11CameraStandard::GetBackwardMatrix(XMFLOAT4X4* m)
{
	*m = bmat; 
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
	m_pos = _pos; 
	XMVECTOR pos = XMLoadFloat3(&_pos); 
	XMVECTOR at = XMLoadFloat3(&_at); 
	XMVECTOR upper = XMLoadFloat3(&_upper); 
	XMVECTOR front = XMVector3Normalize(at - pos); 
	XMVECTOR back_at = pos - front; 
	XMMATRIX _mat = XMMatrixLookAtLH(pos, at, upper); 
	XMMATRIX _bmat = XMMatrixLookAtLH(pos, back_at, upper); 
	XMStoreFloat3(&m_dir, front); 

	XMStoreFloat4x4(&mat, _mat); 
	XMStoreFloat4x4(&bmat, _bmat); 
}

HRESULT Dx11CameraStandard::GetCamPos(XMFLOAT3* vec)
{
	*vec = m_pos; 
	return S_OK; 
}

HRESULT Dx11CameraStandard::GetCamFront(XMFLOAT3* vec)
{
	*vec = m_dir; 
	return S_OK; 
}
