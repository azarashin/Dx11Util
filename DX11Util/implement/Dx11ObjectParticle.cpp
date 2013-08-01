#include <DX11Util/Dx11ObjectParticle.h>

#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }


Dx11ObjectParticle::Dx11ObjectParticle()
{
}


Dx11ObjectParticle::~Dx11ObjectParticle(void)
{
}



void Dx11ObjectParticle::Setup(Dx11Context* _context)
{
	HRESULT hr; 
	ID3D11Device* pd3dDevice = _context->GetDXDevice(); 
	tDAEVERTEX v[4] = {
		{XMFLOAT3(-0.5f,  0.5f, 0.0f) , XMFLOAT3(0.0f, 0.0f, 1.0f) , 0.0f, 0.0f}, 
		{XMFLOAT3( 0.5f,  0.5f, 0.0f) , XMFLOAT3(0.0f, 0.0f, 1.0f) , 1.0f, 0.0f}, 
		{XMFLOAT3( 0.5f, -0.5f, 0.0f) , XMFLOAT3(0.0f, 0.0f, 1.0f) , 1.0f, 1.0f}, 
		{XMFLOAT3(-0.5f, -0.5f, 0.0f) , XMFLOAT3(0.0f, 0.0f, 1.0f) , 0.0f, 1.0f}, 
	}; 

	D3D11_BUFFER_DESC vBufferDesc;
	vBufferDesc.Usage          = D3D11_USAGE_DEFAULT; 
	vBufferDesc.ByteWidth      = sizeof(tDAEVERTEX) * 4; 
	vBufferDesc.BindFlags      = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT; 
	vBufferDesc.CPUAccessFlags = 0;
	vBufferDesc.MiscFlags      = 0;
	vBufferDesc.StructureByteStride = 0;

	// ���_�o�b�t�@�̃T�u���\�[�X�̒�`
	D3D11_SUBRESOURCE_DATA vSubData;
	vSubData.pSysMem          = v;  // �o�b�t�@�E�f�[�^�̏����l
	vSubData.SysMemPitch      = 0;
	vSubData.SysMemSlicePitch = 0;

	// ���_�o�b�t�@�̍쐬
	hr = pd3dDevice->CreateBuffer(&vBufferDesc, &vSubData, &m_pVB);
	if (FAILED(hr)) {
		DXTRACE_ERR(L"InitDirect3D pd3dDevice->CreateBuffer", hr);
		return; 
	}

	// **********************************************************
	// �C���f�b�N�X�E�o�b�t�@�̒�`
	D3D11_BUFFER_DESC idxBufferDesc;
	idxBufferDesc.Usage          = D3D11_USAGE_DEFAULT;     // �f�t�H���g�g�p�@
	idxBufferDesc.ByteWidth      = sizeof(UINT) * 6;       // 2�~3���_
	idxBufferDesc.BindFlags      = D3D11_BIND_INDEX_BUFFER; // �C���f�b�N�X�E�o�b�t�@
	idxBufferDesc.CPUAccessFlags = 0;
	idxBufferDesc.MiscFlags      = 0;
	idxBufferDesc.StructureByteStride = 0;

	// �C���f�b�N�X�E�o�b�t�@�̃T�u���\�[�X�̏����l(���_ID)
	UINT idxVertexID[] = {
		0,1,2,  0,2,3,
	};

	D3D11_SUBRESOURCE_DATA idxSubData;
	idxSubData.pSysMem          = idxVertexID;  // �o�b�t�@�E�f�[�^�̏����l
	idxSubData.SysMemPitch      = 0;
	idxSubData.SysMemSlicePitch = 0;

	hr = pd3dDevice->CreateBuffer(&idxBufferDesc, &idxSubData, &m_pIB);
	if (FAILED(hr)) {
		DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateBuffer", hr);
		return; 
	}

}

void Dx11ObjectParticle::Term()
{
	SAFE_RELEASE(m_pIB); 
	SAFE_RELEASE(m_pVB); 
}

BOOL Dx11ObjectParticle::GetBufNum(unsigned int* num)
{
	*num = 1; 
	return true; 
}

BOOL Dx11ObjectParticle::GetVertexBuf(unsigned int num, ID3D11Buffer** buf)
{
	*buf = m_pVB; 
	return true; 
}

BOOL Dx11ObjectParticle::GetWeightBuf(unsigned int num, ID3D11Buffer** buf)
{
	*buf = 0; 
	return false; 
}

BOOL Dx11ObjectParticle::GetIndexBuf(unsigned int num, ID3D11Buffer** buf)
{
	*buf = m_pIB; 
	return true; 
}

BOOL Dx11ObjectParticle::GetTexture(unsigned int num, ID3D11ShaderResourceView** pTexture)
{
	*pTexture = 0;
	return true; 
}

BOOL Dx11ObjectParticle::GetVertexNum(unsigned int idx, unsigned int* num)
{
	*num = 4; 
	return true; 
}

BOOL Dx11ObjectParticle::GetIndexNum(unsigned int idx, unsigned int* num)
{
	*num = 6; 
	return true; 
}
