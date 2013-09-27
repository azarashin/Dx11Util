#include <DX11Util/Effect/Dx11EffectSideBySideEx.h>
#include <DX11Util/Object/Dx11ObjectTargetTexture.h>
#include <d3dcompiler.h>
#include <DX11Util/Util/ShaderCompilerWrapper.h>


#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }	// ����}�N��

Dx11EffectSideBySideEx::Dx11EffectSideBySideEx(int _width, int _height, const EffectSideBySideExInfo* param)
	: buffer_max(4), mipmap_max(6), hdr_src_max(8)
{
	width = _width; 
	height = _height; 

	memcpy(&hmd_param, param, sizeof(EffectSideBySideExInfo)); 
}


Dx11EffectSideBySideEx::~Dx11EffectSideBySideEx(void)
{
}



HRESULT Dx11EffectSideBySideEx::Setup(Dx11Context* _context)
{
	HRESULT hr; 
	int i; 
	ID3D11Device* pd3dDevice = _context->GetDXDevice(); 
#if defined(DEBUG) || defined(_DEBUG)
UINT flagCompile = D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION
					| D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR;
#else
UINT flagCompile = D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR;
#endif

	// **********************************************************
	// compile vertex shader. 
	ID3DBlob* pBlobVS = NULL;
	hr = D3DCompileFromFileWrapper(
			L"shader\\sidebyside_ex.sh",  // file name
			NULL,          // macro definition
			NULL,          // include file
			"VS",          // run VS function
			"vs_4_0",      // vertex shader 4.0
			flagCompile, // compile option
			0,             // compile option for effect
			&pBlobVS,      // compiled byte code 
			NULL);          // no error message
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitDirect3D D3DX11CompileShaderFromFile", hr);
	}

	// make vertex shader. 
	hr = pd3dDevice->CreateVertexShader(
			pBlobVS->GetBufferPointer(), 
			pBlobVS->GetBufferSize(),   
			NULL,
			&pVertexShader);


	if (FAILED(hr)) {
		SAFE_RELEASE(pBlobVS);
		return DXTRACE_ERR(L"InitDirect3D pd3dDevice->CreateVertexShader", hr);
	}

	// **********************************************************
	// Layout
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0}, 
		{"TANGENT",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMFLOAT3)*2, D3D11_INPUT_PER_VERTEX_DATA, 0}, 
		{"BINORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMFLOAT3)*3, D3D11_INPUT_PER_VERTEX_DATA, 0}, 
		{"WEIGHT",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(XMFLOAT3)*4, D3D11_INPUT_PER_VERTEX_DATA, 0}, 
		{"WEIGHT_INDEX",    0, DXGI_FORMAT_R32G32B32A32_UINT, 0, sizeof(XMFLOAT3)*4 + sizeof(XMFLOAT4), D3D11_INPUT_PER_VERTEX_DATA, 0}, 
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(XMFLOAT3)*4 + sizeof(XMFLOAT4) + sizeof(UINT)*4, D3D11_INPUT_PER_VERTEX_DATA, 0}, 
		{"TEXTURE",    0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3)*4 + sizeof(XMFLOAT4)*2 + sizeof(UINT)*4, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	hr = pd3dDevice->CreateInputLayout(
			layout,                            // ��`�̔z��
			_countof(layout),                  // ��`�̗v�f��
			pBlobVS->GetBufferPointer(),       // �o�C�g�E�R�[�h�ւ̃|�C���^
			pBlobVS->GetBufferSize(),          // �o�C�g�E�R�[�h�̃T�C�Y
			&pInputLayout);                  // �󂯎��ϐ��̃|�C���^

	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateInputLayout", hr);
	}
	SAFE_RELEASE(pBlobVS);


	// **********************************************************
	// compile pixel shader
	ID3DBlob* pBlobPS = NULL;
	hr = D3DCompileFromFileWrapper(
			L"shader\\sidebyside_ex.sh",  // file name
			NULL,          // macro definition
			NULL,          // include file
			"PS",          // run VS function
			"ps_4_0",      // vertex shader 4.0
			flagCompile, // compile option
			0,             // compile option for effect
			&pBlobPS,      // compiled byte code 
			NULL);          // no error message
	if (FAILED(hr))
		return DXTRACE_ERR(L"InitDirect3D D3DX11CompileShaderFromFile", hr);

	hr = pd3dDevice->CreatePixelShader(
			pBlobPS->GetBufferPointer(), 
			pBlobPS->GetBufferSize(),    
			NULL,
			&pPixelShader); 

	SAFE_RELEASE(pBlobPS);  
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitDirect3D pd3dDevice->CreatePixelShader", hr);
	}

	// **********************************************************
	// define constant buffer. 
	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.Usage          = D3D11_USAGE_DYNAMIC;    
	cBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER; 
	cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;     
	cBufferDesc.MiscFlags      = 0;
	cBufferDesc.StructureByteStride = 0;

	// for view
	cBufferDesc.ByteWidth      = sizeof(EffectSideBySideExInfo);
	hr = pd3dDevice->CreateBuffer(&cBufferDesc, NULL, &const_buf);
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitDirect3D pd3dDevice->CreateBuffer", hr);
	}

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
		return DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateBlendState", hr);
	}


	// **********************************************************
	// �[�x/�X�e���V���E�X�e�[�g�E�I�u�W�F�N�g�̍쐬
	D3D11_DEPTH_STENCIL_DESC DepthStencil;
	DepthStencil.DepthEnable      = FALSE; // �[�x�e�X�g����
	DepthStencil.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ZERO; // �������܂Ȃ�
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
		return DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateDepthStencilState", hr);
	}

	pTextureSamplerWrap = new ID3D11SamplerState*[mipmap_max]; 
	pOldTextureSamplerWrap = new ID3D11SamplerState*[mipmap_max]; 
	for(i=0;i<mipmap_max;i++) {
		pOldTextureSamplerWrap[i] = 0; 

		// �T���v���[�̍쐬
		D3D11_SAMPLER_DESC descSampler;
		descSampler.Filter          = D3D11_FILTER_ANISOTROPIC;
		descSampler.AddressU        = D3D11_TEXTURE_ADDRESS_BORDER;
		descSampler.AddressV        = D3D11_TEXTURE_ADDRESS_BORDER;
		descSampler.AddressW        = D3D11_TEXTURE_ADDRESS_BORDER;
		descSampler.MipLODBias      = i;
		descSampler.MaxAnisotropy   = 2;
		descSampler.ComparisonFunc  = D3D11_COMPARISON_NEVER;
		descSampler.BorderColor[0]  = 0.0f;
		descSampler.BorderColor[1]  = 0.0f;
		descSampler.BorderColor[2]  = 0.0f;
		descSampler.BorderColor[3]  = 0.0f;
		descSampler.MinLOD          = -FLT_MAX;
		descSampler.MaxLOD          = FLT_MAX;
		hr = pd3dDevice->CreateSamplerState(&descSampler, &pTextureSamplerWrap[i]);
		if (FAILED(hr)) {
			return DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateSamplerState", hr);
		}
	}

	hdr_bright = new Dx11EffectHDRBright(); 
	hdr_bright->Setup(_context); 
	hdr_mix = new Dx11EffectHDRMix(); 
	hdr_mix->Setup(_context); 

	hdr_src = new Dx11ObjectTargetTexture*[hdr_src_max]; 
	for(i=0;i<hdr_src_max;i++) {
		hdr_src[i] = new Dx11ObjectTargetTexture(1<<i, 1<<i, true, 1); 
		hdr_src[i]->Setup(_context); 
	}

	left = new Dx11ObjectTargetTexture(width, height); 
	right  = new Dx11ObjectTargetTexture(width, height); 

	left->Setup(_context); 
	right->Setup(_context); 


	int bwidth = width; 
	int bheight = height; 
	for(i=0;i<16;i++) {
		if(bwidth < (1 << (i+1))) {
			bwidth = (1 << i); 
			break; 
		}
	}
	for(i=0;i<16;i++) {
		if(bheight < (1 << (i+1))) {
			bheight = (1 << i); 
			break; 
		}
	}


	left_buf = new Dx11ObjectTargetTexture*[buffer_max]; 
	right_buf = new Dx11ObjectTargetTexture*[buffer_max]; 
	for(i=0;i<buffer_max;i++) {
		left_buf[i] = new Dx11ObjectTargetTexture(bwidth, bheight, false, 1); 
		left_buf[i]->Setup(_context); 
		right_buf[i] = new Dx11ObjectTargetTexture(bwidth, bheight, false, 1); 
		right_buf[i]->Setup(_context); 
	}
#if 1
	hdr_buf = new Dx11ObjectTargetTexture*[mipmap_max]; 
	for(i=0;i<mipmap_max;i++) {
		bwidth /= 2; 
		bheight /= 2; 
		hdr_buf[i] = new Dx11ObjectTargetTexture(bwidth, bheight, false, 1); 
		hdr_buf[i]->Setup(_context); 
	}
#endif 

	gui_standard = new Dx11EffectGUIStandard(); 
	gui_standard->Setup(_context); 
	
	hdr_br_param_cur.br_offset[0] = hdr_br_param_cur.br_offset[1] = hdr_br_param_cur.br_offset[2] = 1.0f; 
	hdr_br_param_cur.br_offset[3] = 0.0f;
	hdr_br_param_cur.br_weight[0] = hdr_br_param_cur.br_weight[1] = hdr_br_param_cur.br_weight[2] = 1.0f; 
	hdr_br_param_cur.br_weight[3] = 1.0f;

	hdr_mix_param_cur.bright_num = mipmap_max+2; 
	hdr_mix_param_cur.weight_enable = 1; 
	hdr_mix_param_cur.offset[0] = hdr_mix_param_cur.offset[1] = hdr_mix_param_cur.offset[2] = 0.0f; 
	hdr_mix_param_cur.offset[3] = 0.0f; 
	hdr_mix_param_cur.weight[0] = hdr_mix_param_cur.weight[1] = hdr_mix_param_cur.weight[2] = 1.0f; 
	hdr_mix_param_cur.weight[3] = 1.0f; 


	return S_OK; 
}

HRESULT Dx11EffectSideBySideEx::Update(Dx11Context* _context, Dx11Scene* scene, Dx11CameraSideBySide* camera, Dx11LensSideBySide* lens)
{
	HRESULT hr; 
	unsigned int max; 
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	ID3D11DeviceContext* context = _context->GetDXDC(); 
	EffectSideBySideExInfo * param;
	EffectSideBySideExInfo iparam; 
	param = &iparam; 


	memcpy(param, &hmd_param, sizeof(EffectSideBySideExInfo)); 

	// ���E���ꂼ�ꃌ���_�����O
	left->Start(_context); 
	if(camera) {
		camera->SetIsLeft(true); 
		camera->Update(); 
	}
	if(lens) {
		lens->SetIsLeft(true); 
	}
	scene->Render(_context, camera, lens); 
	left->End(_context); 

	right->Start(_context); 
	if(camera) {
		camera->SetIsLeft(false); 
		camera->Update(); 
	}
	if(lens) {
		lens->SetIsLeft(false); 
	}
	scene->Render(_context, camera, lens); 
	right->End(_context); 


	context->PSGetSamplers(0, 1/*mipmap_max*/, pOldTextureSamplerWrap);
	context->PSSetSamplers(0, 1/*mipmap_max*/, pTextureSamplerWrap);

#if 1
	EffectGUIStandardInfo gsinfo; 
	gsinfo.x = 0.0f; 
	gsinfo.y = 0.0f; 
	gsinfo.w = 1.0f; 
	gsinfo.h = 1.0f; 
	gsinfo.trans = 1.0f; 
	gsinfo.inv = 0.0f; 
#endif

	int i, j; 

	// left hdr
	EffectHDRBrightInfo hdr_br_param; 
	EffectHDRMixInfo hdr_mix_param; 
#if 1
	hdr_src[hdr_src_max-1]->Start(_context); 
	gui_standard->Update(_context, left, &gsinfo); 
	hdr_src[hdr_src_max-1]->End(_context); 
	for(i=hdr_src_max-1;i>0;i--) {
		hdr_src[i-1]->Start(_context); 
		gui_standard->Update(_context, hdr_src[i], &gsinfo); 
		hdr_src[i-1]->End(_context); 
	}
	
	
	
	hdr_buf[0]->Start(_context); 
	hdr_bright->Update(_context, left, hdr_src[0], &hdr_br_param_cur); 
	hdr_buf[0]->End(_context); 
	for(i=1;i<mipmap_max;i++) {
		hdr_br_param.br_offset[0] = hdr_br_param.br_offset[1] = hdr_br_param.br_offset[2] = 0.0f; 
		hdr_br_param.br_offset[3] = 0.0f;
		hdr_br_param.br_weight[0] = hdr_br_param.br_weight[1] = hdr_br_param.br_weight[2] = 1.0f;// / (float)i; 
		hdr_br_param.br_weight[3] = 1.0f;
		hdr_buf[i]->Start(_context); 
		hdr_bright->Update(_context, hdr_buf[i-1], hdr_src[0], &hdr_br_param); 
		hdr_buf[i]->End(_context); 
	}

	left_buf[0]->Start(_context); 
//	gui_standard->Update(_context, hdr_src[0], &gsinfo); 
	hdr_mix->Update(_context, left, hdr_buf, hdr_src[0], mipmap_max,&hdr_mix_param_cur); 
	left_buf[0]->End(_context); 
	
	

	// right hdr
	hdr_buf[0]->Start(_context); 
	hdr_bright->Update(_context, right, hdr_src[0], &hdr_br_param_cur); 
	hdr_buf[0]->End(_context); 
	for(i=1;i<mipmap_max;i++) {
		hdr_br_param.br_offset[0] = hdr_br_param.br_offset[1] = hdr_br_param.br_offset[2] = 0.0f; 
		hdr_br_param.br_offset[3] = 0.0f;
		hdr_br_param.br_weight[0] = hdr_br_param.br_weight[1] = hdr_br_param.br_weight[2] = 1.0f;// / (float)i; 
		hdr_br_param.br_weight[3] = 1.0f;
		hdr_buf[i]->Start(_context); 
		hdr_bright->Update(_context, hdr_buf[i-1], hdr_src[0], &hdr_br_param); 
		hdr_buf[i]->End(_context); 
	}
	
	right_buf[0]->Start(_context); 
//	gui_standard->Update(_context, right, &gsinfo); 
	hdr_mix->Update(_context, right, hdr_buf, hdr_src[0], mipmap_max,&hdr_mix_param_cur); 
	right_buf[0]->End(_context); 
#else
	gui_standard->Update(_context, right, &gsinfo); 
#endif


	FLOAT oldBlendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	FLOAT BlendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	ID3D11BlendState*         old_pBlendState;			// �u�����h�E�X�e�[�g�E�I�u�W�F�N�g
	ID3D11DepthStencilState*  old_pDepthStencilState;	// �[�x/�X�e���V���E�X�e�[�g�E�I�u�W�F�N�g

	UINT old_mask, old_stencilref; 
	// �u�����h�E�X�e�[�g�E�I�u�W�F�N�g��ޔ�
	context->OMGetBlendState(&old_pBlendState, oldBlendFactor, &old_mask);
	// �[�x/�X�e���V���E�X�e�[�g�E�I�u�W�F�N�g��ޔ�
	context->OMGetDepthStencilState(&old_pDepthStencilState, &old_stencilref);

	// OM�Ƀu�����h�E�X�e�[�g�E�I�u�W�F�N�g��ݒ�
	context->OMSetBlendState(pBlendState, BlendFactor, 0xffffffff);
	// OM�ɐ[�x/�X�e���V���E�X�e�[�g�E�I�u�W�F�N�g��ݒ�
	context->OMSetDepthStencilState(pDepthStencilState, 0);


#if 1
	// left

	hr = context->Map(
	                  const_buf,              // �}�b�v���郊�\�[�X
	                  0,                       // �T�u���\�[�X�̃C���f�b�N�X�ԍ�
	                  D3D11_MAP_WRITE_DISCARD, // �������݃A�N�Z�X
	                  0,                       //
	                  &MappedResource);        // �f�[�^�̏������ݐ�|�C���^
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitBackBuffer  g_pImmediateContext->Map", hr);  // ���s
	}

	// �f�[�^��������
	param->left = -0.5f; 
	CopyMemory(MappedResource.pData, param, sizeof(EffectSideBySideExInfo ));
	// �}�b�v����
	context->Unmap(const_buf, 0);


	// VS�ɒ��_�V�F�[�_��ݒ�
	context->VSSetShader(pVertexShader, NULL, 0);
	// VS�ɒ萔�o�b�t�@��ݒ�
	context->VSSetConstantBuffers(0, 1, &const_buf);

	// GS�ɃW�I���g���E�V�F�[�_��ݒ�
	context->GSSetShader(NULL, NULL, 0);
	// GS�ɒ萔�o�b�t�@��ݒ�
	context->GSSetConstantBuffers(0, 1, &const_buf);

	// PS�Ƀs�N�Z���E�V�F�[�_��ݒ�
	context->PSSetShader(pPixelShader, NULL, 0);
	// PS�ɒ萔�o�b�t�@��ݒ�
	context->PSSetConstantBuffers(0, 1, &const_buf);

	j = 0; 
	left_buf[j]->GetBufNum(&max); 

	for(unsigned int i=0;i<max;i++)
	{
		unsigned int vn, in; 
		ID3D11Buffer* vb; 
		ID3D11Buffer* ib; 

		left_buf[j]->GetVertexBuf(i, &vb); 
		left_buf[j]->GetIndexBuf(i, &ib); 

		left_buf[j]->GetVertexNum(i, &vn); 
		left_buf[j]->GetIndexNum(i, &in); 

		UINT strides[1] = {sizeof(tDAEVERTEX)}; 
		UINT offsets[1] = {0}; 
		ID3D11Buffer* vbs[1] = {vb}; 

		context->IASetVertexBuffers(0, 1, vbs, strides, offsets); 

		context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0); 
		context->IASetInputLayout(pInputLayout);
		context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// PS�ɃV�F�[�_�E���\�[�X�E�r���[��ݒ�
		ID3D11ShaderResourceView* srv[1]; 
		left_buf[j]->GetTexture(i, &(srv[0])); 
		context->PSSetShaderResources(
	        0,              // �ݒ肷��ŏ��̃X���b�g�ԍ�
	        1,              // �ݒ肷��V�F�[�_�E���\�[�X�E�r���[�̐�
	        srv);			// �ݒ肷��V�F�[�_�E���\�[�X�E�r���[�̔z��

		context->DrawIndexed( in, 0, 0); 

//		pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVertices[i], 0, m_dwIndecies[i]/3);

//		pd3dDevice->SetTexture(0, NULL);
	}

#endif

	// right
	hr = context->Map(
	                  const_buf,              // �}�b�v���郊�\�[�X
	                  0,                       // �T�u���\�[�X�̃C���f�b�N�X�ԍ�
	                  D3D11_MAP_WRITE_DISCARD, // �������݃A�N�Z�X
	                  0,                       //
	                  &MappedResource);        // �f�[�^�̏������ݐ�|�C���^
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitBackBuffer  g_pImmediateContext->Map", hr);  // ���s
	}

	// �f�[�^��������
	param->left = 0.5f; 
	CopyMemory(MappedResource.pData, param, sizeof(EffectSideBySideExInfo ));
	// �}�b�v����
	context->Unmap(const_buf, 0);


	// VS�ɒ��_�V�F�[�_��ݒ�
	context->VSSetShader(pVertexShader, NULL, 0);
	// VS�ɒ萔�o�b�t�@��ݒ�
	context->VSSetConstantBuffers(0, 1, &const_buf);

	// GS�ɃW�I���g���E�V�F�[�_��ݒ�
	context->GSSetShader(NULL, NULL, 0);
	// GS�ɒ萔�o�b�t�@��ݒ�
	context->GSSetConstantBuffers(0, 1, &const_buf);

	// PS�Ƀs�N�Z���E�V�F�[�_��ݒ�
	context->PSSetShader(pPixelShader, NULL, 0);
	// PS�ɒ萔�o�b�t�@��ݒ�
	context->PSSetConstantBuffers(0, 1, &const_buf);

	j = 0; 
	right_buf[j]->GetBufNum(&max); 

	for(unsigned int i=0;i<max;i++)
	{
		unsigned int vn, in; 
		ID3D11Buffer* vb; 
		ID3D11Buffer* ib; 

		right_buf[j]->GetVertexBuf(i, &vb); 
		right_buf[j]->GetIndexBuf(i, &ib); 

		right_buf[j]->GetVertexNum(i, &vn); 
		right_buf[j]->GetIndexNum(i, &in); 

		UINT strides[1] = {sizeof(tDAEVERTEX)}; 
		UINT offsets[1] = {0}; 
		ID3D11Buffer* vbs[1] = {vb}; 

		context->IASetVertexBuffers(0, 1, vbs, strides, offsets); 

		context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0); 
		context->IASetInputLayout(pInputLayout);
		context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// PS�ɃV�F�[�_�E���\�[�X�E�r���[��ݒ�
		ID3D11ShaderResourceView* srv[1]; 
		right_buf[j]->GetTexture(i, &(srv[0])); 
		context->PSSetShaderResources(
	        0,              // �ݒ肷��ŏ��̃X���b�g�ԍ�
	        1,              // �ݒ肷��V�F�[�_�E���\�[�X�E�r���[�̐�
	        srv);			// �ݒ肷��V�F�[�_�E���\�[�X�E�r���[�̔z��

		context->DrawIndexed( in, 0, 0); 

//		pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVertices[i], 0, m_dwIndecies[i]/3);

//		pd3dDevice->SetTexture(0, NULL);
	}

	context->PSSetSamplers(0, 1/*mipmap_max*/, pOldTextureSamplerWrap);

	// �u�����h�E�X�e�[�g�E�I�u�W�F�N�g�𕜌�
	context->OMSetBlendState(old_pBlendState, oldBlendFactor, old_mask);
	// �[�x/�X�e���V���E�X�e�[�g�E�I�u�W�F�N�g�𕜌�
	context->OMSetDepthStencilState(old_pDepthStencilState, old_stencilref);
	return S_OK; 
}

HRESULT Dx11EffectSideBySideEx::Term(Dx11Context* _context)
{
	int i; 

	hdr_bright->Term(_context); 
	delete hdr_bright; 
	hdr_bright = 0; 

	hdr_mix->Term(_context); 
	delete hdr_mix; 
	hdr_mix = 0; 

	for(i=0;i<buffer_max;i++) {
		SAFE_RELEASE(pTextureSamplerWrap[i]); 
	}
	delete[] pTextureSamplerWrap; 
	delete[] pOldTextureSamplerWrap; 

	gui_standard->Term(_context); 
	delete gui_standard; 

	for(i=0;i<mipmap_max;i++) {
		hdr_buf[i]->Term(); 
		delete hdr_buf[i]; 
	}
	delete[] hdr_buf; 

	for(i=0;i<buffer_max;i++) {
		left_buf[i]->Term(); 
		delete left_buf[i]; 
		right_buf[i]->Term(); 
		delete right_buf[i]; 
	}
	delete[] left_buf; 
	delete[] right_buf; 

	left->Term(); 
	delete left; 
	right->Term(); 
	delete right; 

	for(i=0;i<hdr_src_max;i++) {
		hdr_src[i]->Term(); 
		delete hdr_src[i]; 
	}
	delete[] hdr_src; 
	hdr_src = 0; 

	SAFE_RELEASE(pBlendState); 
	SAFE_RELEASE(pDepthStencilState); 
	return S_OK; 
}

