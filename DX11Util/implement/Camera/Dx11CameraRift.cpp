#include <DX11Util/Camera/Dx11CameraRift.h>



Dx11CameraRift::Dx11CameraRift()
{
}


Dx11CameraRift::~Dx11CameraRift(void)
{
}

HRESULT Dx11CameraRift::Setup()
{
	pos = XMFLOAT3(0.0f, 0.0f, 0.0f); 
	at = XMFLOAT3(0.0f, 0.0f, 1.0f); 
	upper = XMFLOAT3(0.0f, 1.0f, 0.0f); 
	base_length = 0.08f; // 8cm
	is_left = false; 
	return S_OK; 
}

HRESULT Dx11CameraRift::GetMatrix(XMMATRIX* m)
{
	*m = XMLoadFloat4x4(&mat); 
	return S_OK; 
}
HRESULT Dx11CameraRift::GetBackwardMatrix(XMMATRIX* m)
{
	*m = XMLoadFloat4x4(&bmat); 
	return S_OK; 
}

HRESULT Dx11CameraRift::Term()
{
	return S_OK; 
}



void Dx11CameraRift::SetCameraDirection(const XMFLOAT3& _pos, const XMFLOAT3& _at, const XMFLOAT3& _upper)
{
	pos = _pos; 
	at = _at; 
	upper = _upper; 
}

void Dx11CameraRift::SetBaseLength(float _base_length)
{
	base_length = _base_length; 
} 
HRESULT Dx11CameraRift::SetIsLeft(bool _is_left)
{
	is_left = _is_left; 
	return S_OK; 
}

HRESULT Dx11CameraRift::Update()
{
	XMVECTOR _upper, _at, _pos; 
	XMMATRIX _mat, _bmat; 
	_upper = XMLoadFloat3(&upper); 
	_at = XMLoadFloat3(&at); 
	_pos = XMLoadFloat3(&pos); 

	XMVECTOR rvec, nrvec, sdspos, front;
	XMVECTOR back_at = _pos - (_at - _pos); 
	front = _at - _pos; 

	rvec = XMVector3Cross(front, _upper); 
	nrvec = XMVector3Normalize(rvec); 
	sdspos = _pos; 
	if(is_left) {
		sdspos -= nrvec * base_length; 
	} else {
		sdspos += nrvec * base_length; 
	}
	_mat = XMMatrixLookAtLH(sdspos, _at, _upper); 
	_bmat = XMMatrixLookAtLH(sdspos, back_at, _upper); 

	XMStoreFloat4x4(&mat, _mat); 
	XMStoreFloat4x4(&bmat, _bmat); 

	return S_OK; 
}

