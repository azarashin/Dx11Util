#include <DX11Util/Object/Dx11ObjectGUICanvas.h>

#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }


Dx11ObjectGUICanvas::Dx11ObjectGUICanvas(unsigned int _width, unsigned int _height)
{
	width = _width; 
	height = _height; 
}


Dx11ObjectGUICanvas::~Dx11ObjectGUICanvas(void)
{
}



void Dx11ObjectGUICanvas::Setup(Dx11Context* _context)
{
	HRESULT hr; 
	ID3D11Device* pd3dDevice = _context->GetDXDevice(); 
	tDAEVERTEX v[4] = {
		{XMFLOAT3( 0.0f,  1.0f, 0.0f) , XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0), {0, 0, 0, 0}, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0), 1.0f, 0.0f}, 
		{XMFLOAT3( 1.0f,  1.0f, 0.0f) , XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) , XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0), {0, 0, 0, 0}, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0), 0.0f, 0.0f}, 
		{XMFLOAT3( 1.0f,  0.0f, 0.0f) , XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) , XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0), {0, 0, 0, 0}, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0), 0.0f, 1.0f}, 
		{XMFLOAT3( 0.0f,  0.0f, 0.0f) , XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) , XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0), {0, 0, 0, 0}, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0), 1.0f, 1.0f}, 
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

	// **********************************************************
	// �`��\�e�N�X�`���̍쐬
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width     = width;  // ��
	desc.Height    = height;  // ����
	desc.MipLevels = 1;    // �~�b�v�}�b�v ���x����
	desc.ArraySize = 1;    // �e�N�X�`���̔z��T�C�Y
	desc.Format    = DXGI_FORMAT_R8G8B8A8_UNORM; // �t�H�[�}�b�g
	desc.SampleDesc.Count = 1; // �}���`�T���v�����O�̐ݒ�
	desc.Usage            = D3D11_USAGE_DYNAMIC;  // ���I(Dynamic)�g�p�@
	desc.BindFlags        = D3D11_BIND_SHADER_RESOURCE; // �V�F�[�_ ���\�[�X
	desc.CPUAccessFlags   = D3D11_CPU_ACCESS_WRITE;     // CPU�ŏ�������
	hr = pd3dDevice->CreateTexture2D(&desc, NULL, &m_pTex);
	if (FAILED(hr)) {
		return; 
	}

	// 2D�e�N�X�`���ɃA�N�Z�X����V�F�[�_ ���\�[�X �r���[�̐ݒ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format        = desc.Format;		// �t�H�[�}�b�g
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2D�e�N�X�`��
	srDesc.Texture2D.MostDetailedMip = 0;   // �ŏ��̃~�b�v�}�b�v ���x��
	srDesc.Texture2D.MipLevels       = -1;  // ���ׂẴ~�b�v�}�b�v ���x��

	// �V�F�[�_ ���\�[�X �r���[�̍쐬
	hr = pd3dDevice->CreateShaderResourceView(
				m_pTex,       // �A�N�Z�X����e�N�X�`�� ���\�[�X
				&srDesc,          // �V�F�[�_ ���\�[�X �r���[�̐ݒ�
				&m_pTexture);  // �󂯎��ϐ�
	if (FAILED(hr)) {
		return; 
	}
}

void Dx11ObjectGUICanvas::Map(Dx11Context* _context, D3D11_MAPPED_SUBRESOURCE* resource)
{
	ID3D11DeviceContext* context = _context->GetDXDC(); 
	HRESULT hr; 
	// ���\�[�X���}�b�v����
	hr = context->Map(
	                  m_pTex,				// �}�b�v���郊�\�[�X
	                  0,						// �T�u���\�[�X�̃C���f�b�N�X�ԍ�
	                  D3D11_MAP_WRITE_DISCARD,	// �������݃A�N�Z�X
	                  0,						//
	                  resource);			// �f�[�^�̏������ݐ�|�C���^
	if (FAILED(hr)) {
		return; 
	}
}

void Dx11ObjectGUICanvas::Unmap(Dx11Context* _context)
{
	ID3D11DeviceContext* context = _context->GetDXDC(); 
	context->Unmap(m_pTex, 0);
}


void Dx11ObjectGUICanvas::Term()
{
	SAFE_RELEASE(m_pTexture); 
	SAFE_RELEASE(m_pIB); 
	SAFE_RELEASE(m_pVB); 
}

BOOL Dx11ObjectGUICanvas::GetBufNum(unsigned int* num)
{
	*num = 1; 
	return true; 
}

BOOL Dx11ObjectGUICanvas::GetVertexBuf(unsigned int num, ID3D11Buffer** buf)
{
	*buf = m_pVB; 
	return true; 
}

BOOL Dx11ObjectGUICanvas::GetWeightBuf(unsigned int num, ID3D11Buffer** buf)
{
	*buf = 0; 
	return false; 
}

BOOL Dx11ObjectGUICanvas::GetIndexBuf(unsigned int num, ID3D11Buffer** buf)
{
	*buf = m_pIB; 
	return true; 
}

BOOL Dx11ObjectGUICanvas::GetTexture(unsigned int num, ID3D11ShaderResourceView** pTexture)
{
	*pTexture = m_pTexture; 
	return true; 
}

BOOL Dx11ObjectGUICanvas::GetVertexNum(unsigned int idx, unsigned int* num)
{
	*num = 4; 
	return true; 
}

BOOL Dx11ObjectGUICanvas::GetIndexNum(unsigned int idx, unsigned int* num)
{
	*num = 6; 
	return true; 
}

BOOL Dx11ObjectGUICanvas::GetTextureNum(unsigned int* num)
{
	*num = 1; 
	return true; 
}
