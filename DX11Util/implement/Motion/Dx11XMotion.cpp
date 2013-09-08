#include <DX11Util/Motion/Dx11XMotion.h>



Dx11XMotion::Dx11XMotion(int _part_max, int _frame_max)
{
	num_frame = _frame_max; 
	num_part = _part_max; 
	parent = new int[num_part]; 
	bias = new XMFLOAT4X4[num_part]; 
	offset = new XMFLOAT4X4[num_part]; 
	motion = new XMFLOAT4X4[num_part * num_frame]; 
	mat_flag = new int[num_part]; 
	cash_matrix = new XMFLOAT4X4[num_part]; 
	current_frame = 0; 
}

Dx11XMotion::~Dx11XMotion()
{
	if(parent != 0) {
		delete[] parent; 
		parent = 0; 
	}
	if(bias != 0) {
		delete[] bias; 
		bias = 0; 
	}
	if(offset != 0) {
		delete[] offset; 
		offset = 0; 
	}
	if(motion != 0) {
		delete[] motion; 
		motion = 0; 
	}
	if(mat_flag != 0) {
		delete[] mat_flag; 
		mat_flag = 0; 
	}
	if(cash_matrix != 0) {
		delete[] cash_matrix; 
		cash_matrix = 0; 
	}
}

HRESULT Dx11XMotion::Setup()
{
	current_frame = -1; 
	return S_OK; 
}

HRESULT Dx11XMotion::Update()
{
	XMMATRIX mat; 
	if(current_frame +1 < num_frame) {
		current_frame++; 
	}


	int i;
	for(i=0;i<num_part;i++) {
		mat_flag[i] = 0; 
	}
	for(i=0;i<num_part;i++) {
		UpdateMatrix(i); 
	}

	for(i=0;i<num_part;i++) {
		mat = XMLoadFloat4x4(&cash_matrix[i]); 
		mat = XMLoadFloat4x4(&offset[i]) * mat; 
		XMStoreFloat4x4(&cash_matrix[i], mat); 
	}
	return S_OK; 
}

void Dx11XMotion::UpdateMatrix(int idx)
{
	XMVECTOR vec;  
	XMMATRIX mat; 
	if(parent[idx] >= 0) {
		if(mat_flag[parent[idx]] == 0) {
			UpdateMatrix(parent[idx]); 
		}
		mat =  
		XMMatrixInverse(&vec, XMLoadFloat4x4(&bias[idx])) *
		XMLoadFloat4x4(&motion[current_frame * num_part + idx]) *
		XMLoadFloat4x4(&bias[idx]) *
		XMLoadFloat4x4(&cash_matrix[parent[idx]]); 
	} else {
		mat =  
		XMMatrixInverse(&vec, XMLoadFloat4x4(&bias[idx])) *
		XMLoadFloat4x4(&motion[current_frame * num_part + idx]) *
		XMLoadFloat4x4(&bias[idx]);
	}
	XMStoreFloat4x4(&cash_matrix[idx], mat); 
	mat_flag[idx] = 1; 
}

HRESULT Dx11XMotion::GetMatrix(XMFLOAT4X4* mat) 
{
	int i;
	for(i=0;i<num_part;i++) {
		mat[i] = cash_matrix[i]; 
	}
	return S_OK; 
}

HRESULT Dx11XMotion::GetNumberOfMatrix(int* num)
{
	*num = num_part; 
	return S_OK; 
}

HRESULT Dx11XMotion::Term()
{
	return S_OK; 
}

void Dx11XMotion::Start()
{
	current_frame = 0; 
}

bool Dx11XMotion::IsFinished()
{
	return (current_frame + 1 >= num_frame); 
}

void Dx11XMotion::SetBase(const XMFLOAT4X4* _offset, const XMFLOAT4X4* _bias, int _part, int _parent)
{
	parent[_part] = _parent; 
	offset[_part] = *_offset; 
	bias[_part] = *_bias; 
}

void Dx11XMotion::SetMotion(const XMFLOAT4X4* _mot, int _part, int _frame)
{
	motion[_frame * num_part + _part] = *_mot; 
}
