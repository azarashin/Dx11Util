#include <DX11Util/Object/Dx11ObjectTargetTexture.h>

// #include <dxerr.h>

#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }


Dx11ObjectTargetTexture::Dx11ObjectTargetTexture(unsigned int _width, unsigned int _height)
{
	width = _width; 
	height = _height; 
	mipmap_level = 1; 
	readable = false; 
}

Dx11ObjectTargetTexture::Dx11ObjectTargetTexture(unsigned int _width, unsigned int _height, bool _readable, int _mipmap_level)
{
	width = _width; 
	height = _height; 
	mipmap_level = _mipmap_level; 
	readable = _readable; 
}



Dx11ObjectTargetTexture::~Dx11ObjectTargetTexture(void)
{
}



void Dx11ObjectTargetTexture::Setup(Dx11Context* _context)
{
	HRESULT hr; 
	ID3D11Device* pd3dDevice = _context->GetDXDevice(); 
	tDAEVERTEX v[4] = {
		{XMFLOAT3( 0.0f,  1.0f, 0.0f) , XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) , XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0), {0, 0, 0, 0}, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0), 1.0f, 0.0f}, 
		{XMFLOAT3( 1.0f,  1.0f, 0.0f) , XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) , XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0), {0, 0, 0, 0}, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0), 0.0f, 0.0f}, 
		{XMFLOAT3( 1.0f,  0.0f, 0.0f) , XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) , XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0), {0, 0, 0, 0}, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0), 0.0f, 1.0f}, 
		{XMFLOAT3( 0.0f,  0.0f, 0.0f) , XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) , XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0), {0, 0, 0, 0}, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0), 1.0f, 1.0f}, 
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
	desc.MipLevels = mipmap_level;    // �~�b�v�}�b�v ���x����
	desc.ArraySize = 1;    // �e�N�X�`���̔z��T�C�Y
	desc.Format    = DXGI_FORMAT_R32G32B32A32_FLOAT; // �t�H�[�}�b�g
	desc.SampleDesc.Count = 1; // �}���`�T���v�����O�̐ݒ�
	desc.Usage            = D3D11_USAGE_DEFAULT;  
	desc.BindFlags        = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; // �V�F�[�_ ���\�[�X
	if(readable) {
//		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE|D3D11_CPU_ACCESS_READ; // impossible to set this flag as output texture. 
	}
	hr = pd3dDevice->CreateTexture2D(&desc, NULL, &m_pTex);
	if (FAILED(hr)) {
		DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateTexture2D", hr);
		return; 
	}

	// �`��\�e�N�X�`���̕`��^�[�Q�b�g �r���[�̍쐬
	D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
	rtDesc.Format             = desc.Format; // �e�N�X�`���̃t�H�[�}�b�g���w��
	rtDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtDesc.Texture2D.MipSlice = 0;
	hr = pd3dDevice->CreateRenderTargetView(
			m_pTex,      // �r���[�����e�N�X�`�� ���\�[�X
			&rtDesc,           // �r���[�̐ݒ�
			&pRenderTargetView); // �r���[���󂯎��ϐ�
	if (FAILED(hr)) {
		DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateRenderTargetView", hr);
		return; 
	}

	// 2D�e�N�X�`���ɃA�N�Z�X����V�F�[�_ ���\�[�X �r���[�̐ݒ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format        = desc.Format;		// �t�H�[�}�b�g
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2D�e�N�X�`��
	srDesc.Texture2D.MostDetailedMip = 0; 
	srDesc.Texture2D.MipLevels       = mipmap_level;

	// �V�F�[�_ ���\�[�X �r���[�̍쐬
	hr = pd3dDevice->CreateShaderResourceView(
				m_pTex,       // �A�N�Z�X����e�N�X�`�� ���\�[�X
				&srDesc,          // �V�F�[�_ ���\�[�X �r���[�̐ݒ�
				&m_pTexture);  // �󂯎��ϐ�
	if (FAILED(hr)) {
		DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateShaderResourceView", hr);
		return; 
	}

	// �[�x/�X�e���V���E�e�N�X�`���̍쐬
	D3D11_TEXTURE2D_DESC descDepth = desc;
	descDepth.MipLevels = 1;       // �~�b�v�}�b�v�E���x����
	descDepth.ArraySize = 1;       // �z��T�C�Y
	descDepth.Format    = DXGI_FORMAT_D32_FLOAT;  // �t�H�[�}�b�g(�[�x�̂�)
	descDepth.Usage          = D3D11_USAGE_DEFAULT;      // �f�t�H���g�g�p�@
	descDepth.BindFlags      = D3D11_BIND_DEPTH_STENCIL; // �[�x/�X�e���V���Ƃ��Ďg�p
	descDepth.CPUAccessFlags = 0;   // CPU����̓A�N�Z�X���Ȃ�
	descDepth.MiscFlags      = 0;   // ���̑��̐ݒ�Ȃ�
	hr = pd3dDevice->CreateTexture2D(
	        &descDepth,         // �쐬����2D�e�N�X�`���̐ݒ�
	        NULL,               // 
	        &pDepthStencil);  // �쐬�����e�N�X�`�����󂯎��ϐ�
	if (FAILED(hr)) {
		DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateTexture2D", hr);
		return; 
	}
	
	// �[�x/�X�e���V���E�r���[�̍쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format             = descDepth.Format;            // �r���[�̃t�H�[�}�b�g
	descDSV.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags              = 0;
	descDSV.Texture2D.MipSlice = 0;
	hr = pd3dDevice->CreateDepthStencilView(
	        pDepthStencil,       // �[�x/�X�e���V���E�r���[�����e�N�X�`��
	        &descDSV,              // �[�x/�X�e���V���E�r���[�̐ݒ�
	        &pDepthStencilView); // �쐬�����r���[���󂯎��ϐ�
	if (FAILED(hr)) {
		DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateDepthStencilView", hr);
		return; 
	}


    // �r���[�|�[�g�̐ݒ�
    ViewPort[0].TopLeftX = 0.0f;    // �r���[�|�[�g�̈�̍���X���W�B
    ViewPort[0].TopLeftY = 0.0f;    // �r���[�|�[�g�̈�̍���Y���W�B
    ViewPort[0].Width    = (FLOAT)width;  // �r���[�|�[�g�̈�̕�
    ViewPort[0].Height   = (FLOAT)height;  // �r���[�|�[�g�̈�̍���
    ViewPort[0].MinDepth = 0.0f; // �r���[�|�[�g�̈�̐[�x�l�̍ŏ��l
    ViewPort[0].MaxDepth = 1.0f; // �r���[�|�[�g�̈�̐[�x�l�̍ő�l

	// **********************************************************
	// create blend state object
	D3D11_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
	BlendState.AlphaToCoverageEnable  = FALSE;
	BlendState.IndependentBlendEnable = FALSE;
	BlendState.RenderTarget[0].BlendEnable           = TRUE;

	BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; 
	BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; 
	BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; 

	BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; 
	BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; 
	BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX; 


	BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = pd3dDevice->CreateBlendState(&BlendState, &pBlendState);
	if (FAILED(hr)) {
		DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateBlendState", hr);
		return; 
	}

	// ���X�^���C�U�E�X�e�[�g�E�I�u�W�F�N�g�̍쐬
	D3D11_RASTERIZER_DESC RSDesc;
	RSDesc.FillMode = D3D11_FILL_SOLID;   // ���ʂɕ`�悷��
	RSDesc.CullMode = D3D11_CULL_NONE;    // ���ʂ�`�悷��
	RSDesc.FrontCounterClockwise = FALSE; // ���v��肪�\��
	RSDesc.DepthBias             = 0;
	RSDesc.DepthBiasClamp        = 0;
	RSDesc.SlopeScaledDepthBias  = 0;
	RSDesc.DepthClipEnable       = TRUE;
	RSDesc.ScissorEnable         = FALSE;
	RSDesc.MultisampleEnable     = FALSE;
	RSDesc.AntialiasedLineEnable = FALSE;
	hr = pd3dDevice->CreateRasterizerState(&RSDesc, &pRasterizerState);
	if (FAILED(hr)) {
		DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateRasterizerState", hr);
		return; 
	}

	// **********************************************************
	// �[�x/�X�e���V���E�X�e�[�g�E�I�u�W�F�N�g�̍쐬
	D3D11_DEPTH_STENCIL_DESC DepthStencil;
	DepthStencil.DepthEnable      = TRUE; // �[�x�e�X�g����
	DepthStencil.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ALL; // ��������
	DepthStencil.DepthFunc        = D3D11_COMPARISON_LESS; // ��O�̕��̂�`��
	DepthStencil.StencilEnable    = FALSE; // �X�e���V���E�e�X�g�Ȃ�
	DepthStencil.StencilReadMask  = 0;     // �X�e���V���ǂݍ��݃}�X�N�B
	DepthStencil.StencilWriteMask = 0;     // �X�e���V���������݃}�X�N�B
			// �ʂ��\�������Ă���ꍇ�̃X�e���V���E�e�X�g�̐ݒ�
	DepthStencil.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;  // �ێ�
	DepthStencil.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;  // �ێ�
	DepthStencil.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;  // �ێ�
	DepthStencil.FrontFace.StencilFunc        = D3D11_COMPARISON_NEVER; // ��Ɏ��s
			// �ʂ����������Ă���ꍇ�̃X�e���V���E�e�X�g�̐ݒ�
	DepthStencil.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;   // �ێ�
	DepthStencil.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;   // �ێ�
	DepthStencil.BackFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;   // �ێ�
	DepthStencil.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS; // ��ɐ���
	hr = pd3dDevice->CreateDepthStencilState(&DepthStencil,
											   &pDepthStencilState);
	if (FAILED(hr)) {
		DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateDepthStencilState", hr);
		return; 
	}


#if 0
	// �T���v���[�̍쐬
	D3D11_SAMPLER_DESC descSampler;
	descSampler.Filter          = D3D11_FILTER_ANISOTROPIC;
    descSampler.AddressU        = D3D11_TEXTURE_ADDRESS_WRAP;
    descSampler.AddressV        = D3D11_TEXTURE_ADDRESS_WRAP;
    descSampler.AddressW        = D3D11_TEXTURE_ADDRESS_WRAP;
    descSampler.MipLODBias      = 0.0f;
    descSampler.MaxAnisotropy   = 2;
    descSampler.ComparisonFunc  = D3D11_COMPARISON_NEVER;
    descSampler.BorderColor[0]  = 0.0f;
    descSampler.BorderColor[1]  = 0.0f;
    descSampler.BorderColor[2]  = 0.0f;
    descSampler.BorderColor[3]  = 0.0f;
    descSampler.MinLOD          = -FLT_MAX;
    descSampler.MaxLOD          = FLT_MAX;
	hr = pd3dDevice->CreateSamplerState(&descSampler, &pTextureSamplerWrap);
	if (FAILED(hr)) {
		DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateSamplerState", hr);
		return; 
	}
#endif

	pOldRenderTargetView = 0; 
	pOldDepthStencilView = 0; 
	pOldBlendState = 0; 
	pOldRasterizerState = 0; 
	pOldDepthStencilState = 0; 
}


void Dx11ObjectTargetTexture::Term()
{
	SAFE_RELEASE(pRenderTargetView); 
	SAFE_RELEASE(m_pTex); 
	SAFE_RELEASE(m_pTexture); 
	SAFE_RELEASE(m_pIB); 
	SAFE_RELEASE(m_pVB); 
}

BOOL Dx11ObjectTargetTexture::GetBufNum(unsigned int* num)
{
	*num = 1; 
	return true; 
}

BOOL Dx11ObjectTargetTexture::GetVertexBuf(unsigned int num, ID3D11Buffer** buf)
{
	*buf = m_pVB; 
	return true; 
}

BOOL Dx11ObjectTargetTexture::GetWeightBuf(unsigned int num, ID3D11Buffer** buf)
{
	*buf = 0; 
	return false; 
}

BOOL Dx11ObjectTargetTexture::GetIndexBuf(unsigned int num, ID3D11Buffer** buf)
{
	*buf = m_pIB; 
	return true; 
}

BOOL Dx11ObjectTargetTexture::GetTexture(unsigned int num, ID3D11ShaderResourceView** pTexture)
{
	*pTexture = m_pTexture; 
	return true; 
}

BOOL Dx11ObjectTargetTexture::GetVertexNum(unsigned int idx, unsigned int* num)
{
	*num = 4; 
	return true; 
}

BOOL Dx11ObjectTargetTexture::GetIndexNum(unsigned int idx, unsigned int* num)
{
	*num = 6; 
	return true; 
}


void Dx11ObjectTargetTexture::Start(Dx11Context* _context)
{
	ID3D11DeviceContext* context = _context->GetDXDC(); 
	FLOAT BlendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};

	context->OMGetRenderTargets(1, &pOldRenderTargetView,&pOldDepthStencilView);
	context->OMGetBlendState(&pOldBlendState, old_blend_factor, &old_blend_mask);
	context->OMGetDepthStencilState(&pOldDepthStencilState, &old_stencil_ref);
	old_num_viewport = 1; 
    context->RSGetViewports(&old_num_viewport, old_view_port);
	context->RSGetState(&pOldRasterizerState);
#if 0
	context->PSGetSamplers(0, 1, &pOldTextureSamplerWrap);
#endif

	context->OMSetRenderTargets(1, &pRenderTargetView,pDepthStencilView);
	context->OMSetBlendState(pBlendState, BlendFactor, 0xffffffff);
	context->OMSetDepthStencilState(pDepthStencilState, 0);
    context->RSSetViewports(1, ViewPort);
	context->RSSetState(pRasterizerState);
#if 0
	context->PSSetSamplers(0, 1, &pTextureSamplerWrap);
#endif

	_context->Start(); 
}

void Dx11ObjectTargetTexture::End(Dx11Context* _context)
{
	ID3D11DeviceContext* context = _context->GetDXDC(); 

	context->OMSetRenderTargets(1, &pOldRenderTargetView,pOldDepthStencilView);
	context->OMSetBlendState(pOldBlendState, old_blend_factor, old_blend_mask);
	context->OMSetDepthStencilState(pOldDepthStencilState, old_stencil_ref);
    context->RSSetViewports(old_num_viewport, old_view_port);
	context->RSSetState(pOldRasterizerState);
#if 0
	context->PSSetSamplers(0, 1, &pOldTextureSamplerWrap);
#endif
}

BOOL Dx11ObjectTargetTexture::GetTextureNum(unsigned int* num)
{
	*num = 1; 
	return true; 
}

int Dx11ObjectTargetTexture::GetMemorySize()
{
	if(readable) {
		return width * height * sizeof(float)*4; 
	}
	return 0; 
}

void Dx11ObjectTargetTexture::CopyToMemory(Dx11Context* _context, float* buffer)
{
	D3D11_MAPPED_SUBRESOURCE resource; 
	ID3D11DeviceContext* context = _context->GetDXDC(); 
	HRESULT hr; 
	hr = context->Map(
	                  m_pTex,				// �}�b�v���郊�\�[�X
	                  0,						// �T�u���\�[�X�̃C���f�b�N�X�ԍ�
	                  D3D11_MAP_WRITE_DISCARD,	// �������݃A�N�Z�X
	                  0,						//
	                  &resource);			// �f�[�^�̏������ݐ�|�C���^
	if (FAILED(hr)) {
		return; 
	}
	memcpy(buffer, resource.pData, width * height * sizeof(float)*4); 
	context->Unmap(m_pTex, 0);
}

