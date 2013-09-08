#include <DX11Util/Motion/Dx11MotionStandard.h>



Dx11MotionStandard::Dx11MotionStandard(XMFLOAT3 size)
{
	XMStoreFloat4x4(&size_mat, XMMatrixScaling(size.x, size.y, size.z)); 
}


Dx11MotionStandard::~Dx11MotionStandard(void)
{
}



HRESULT Dx11MotionStandard::Setup()
{
	return S_OK; 
}

HRESULT Dx11MotionStandard::Update()
{
	return S_OK; 
}

HRESULT Dx11MotionStandard::GetMatrix(XMFLOAT4X4* _mat)
{
	*_mat = mat; 
	return S_OK; 
}

HRESULT Dx11MotionStandard::Term()
{
	return S_OK; 
}

void Dx11MotionStandard::SetDirection(XMFLOAT3 _pos, XMFLOAT3 _front, XMFLOAT3 _upper)
{
	XMVECTOR pos = XMLoadFloat3(&_pos); 
	XMVECTOR front = XMLoadFloat3(&_front); 
	XMVECTOR upper = XMLoadFloat3(&_upper); 

	XMVECTOR org = XMLoadFloat3(&XMFLOAT3(0, 0, 0)); 

	XMVECTOR det; 

	XMMATRIX mat1 = XMMatrixLookAtLH(org, front, upper); 
	XMMATRIX mat2 = XMMatrixInverse(&det, mat1); 
	XMMATRIX mat3 = (XMMatrixTranslation(_pos.x, _pos.y, _pos.z)); // 右手座標系なので転置する 
	XMMATRIX result = XMLoadFloat4x4(&size_mat) * mat2 * mat3; // 右手座標系としているので、行列を左から右へとかける
	XMStoreFloat4x4(&mat, result); 

}

HRESULT Dx11MotionStandard::GetNumberOfMatrix(int* num)
{
	*num = 1; 
	return S_OK; 
}
