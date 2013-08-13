#include <DX11Util/Effect/Dx11XEffect.h>


typedef struct  { // Warning: XMMATRIX �ł͂Ȃ�XMFLOAT4X4 ���g���悤�ɏC�����邱��
	XMMATRIX World;		// ���[���h�ϊ��s��
	XMMATRIX View;		// �r���[�ϊ��s��
	XMMATRIX Projection;	// �����ϊ��s��
} Dx11XEffectCBuffer;

typedef struct {
	XMFLOAT4X4 bind_pose; 
	FLOAT diffuse[4]; // r, g, b, a
	FLOAT specular[3]; // r, g, b
	FLOAT emmisive[3]; // r, g, b
	FLOAT power; 
	UINT tex_enable; 
} Dx11XMaterial; 

#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }	// ����}�N��


Dx11XEffect::Dx11XEffect(void)
{
}


Dx11XEffect::~Dx11XEffect(void)
{
}


HRESULT Dx11XEffect::Setup(Dx11Context* _context)
{
	HRESULT hr; 
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
	hr = D3DX11CompileFromFile(
			L"shader\\xfile_notexture.sh",  // file name
			NULL,          // macro definition
			NULL,          // include file
			"VS",          // run VS function
			"vs_4_0",      // vertex shader 4.0
			flagCompile, // compile option
			0,             // compile option for effect
			NULL,          // finish function after compiling
			&pBlobVS,      // compiled byte code 
			NULL,          // no error message
			NULL);         // return 
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
		return DXTRACE_ERR(L"InitDirect3D pd3dDevice->CreateVertexShader", hr);
	}

	// **********************************************************
	// Layout
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0}, 
		{"WEIGHT",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(XMFLOAT3)*2, D3D11_INPUT_PER_VERTEX_DATA, 0}, 
		{"WEIGHT_INDEX",    0, DXGI_FORMAT_R32G32B32A32_UINT, 0, sizeof(XMFLOAT3)*2 + sizeof(XMFLOAT4), D3D11_INPUT_PER_VERTEX_DATA, 0}, 
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(XMFLOAT3)*2 + sizeof(XMFLOAT4) + sizeof(UINT)*4, D3D11_INPUT_PER_VERTEX_DATA, 0}, 
		{"TEXTURE",    0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3)*2 + sizeof(XMFLOAT4)*2 + sizeof(UINT)*4, D3D11_INPUT_PER_VERTEX_DATA, 0}
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
	hr = D3DX11CompileFromFile(
			L"shader\\xfile_notexture.sh",  // file name
			NULL,          // macro definition
			NULL,          // include file
			"PS",          // run PS function
			"ps_4_0",      // Pixel shader
			flagCompile, //  compile option
			0,             // compile option for effect
			NULL,          // finish function after compiling
			&pBlobPS,      // compiled byte code 
			NULL,          // no error message
			NULL);         // return
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
	// define constant buffer. [0]
	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.Usage          = D3D11_USAGE_DYNAMIC;    
	cBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER; 
	cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;     
	cBufferDesc.MiscFlags      = 0;
	cBufferDesc.StructureByteStride = 0;

	// for view
	cBufferDesc.ByteWidth      = sizeof(Dx11XEffectCBuffer);
	hr = pd3dDevice->CreateBuffer(&cBufferDesc, NULL, &const_buf);
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitDirect3D pd3dDevice->CreateBuffer", hr);
	}

	// **********************************************************
	// define constant buffer. [1] for material / part
	cBufferDesc.Usage          = D3D11_USAGE_DYNAMIC;    
	cBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER; 
	cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;     
	cBufferDesc.MiscFlags      = 0;
	cBufferDesc.StructureByteStride = 0;

	// for view
	cBufferDesc.ByteWidth      = sizeof(Dx11XEffectCBuffer);
	hr = pd3dDevice->CreateBuffer(&cBufferDesc, NULL, &const_buf4part);
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
	DepthStencil.DepthEnable      = TRUE; 
	DepthStencil.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ALL; 
	DepthStencil.DepthFunc        = D3D11_COMPARISON_ALWAYS; // ��O�̕��̂�`��
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

	return S_OK; 
}

HRESULT Dx11XEffect::Update(Dx11Context* _context, Dx11XObject* xobj, Dx11Motion* mot, Dx11Camera* cam, Dx11Lens* lens)
{
	HRESULT hr; 
	unsigned int max; 
	int i, j; 
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	Dx11XEffectCBuffer buf; 
	ID3D11DeviceContext* context = _context->GetDXDC(); 


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


	cam->GetMatrix(&(buf.View)); 

	lens->GetMatrix(&(buf.Projection)); 

	mot->GetMatrix(&(buf.World)); 


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
	CopyMemory(MappedResource.pData, &buf, sizeof(Dx11XEffectCBuffer));
	// �}�b�v����
	context->Unmap(const_buf, 0);

	// ***************************************
	// IA�ɒ��_�o�b�t�@��ݒ�
	// IA�ɓ��̓��C�A�E�g�E�I�u�W�F�N�g��ݒ�(���_�o�b�t�@�Ȃ�)
	context->IASetInputLayout(NULL);
	// IA�Ƀv���~�e�B�u�̎�ނ�ݒ�
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VS�ɒ��_�V�F�[�_��ݒ�
	context->VSSetShader(pVertexShader, NULL, 0);
	// VS�ɒ萔�o�b�t�@��ݒ�
	context->VSSetConstantBuffers(0, 1, &const_buf);
	context->VSSetConstantBuffers(2, 1, &const_buf4part);

	// PS�Ƀs�N�Z���E�V�F�[�_��ݒ�
	context->PSSetShader(pPixelShader, NULL, 0);
	// PS�ɒ萔�o�b�t�@��ݒ�
	context->PSSetConstantBuffers(0, 1, &const_buf);
	context->PSSetConstantBuffers(2, 1, &const_buf4part);


	xobj->GetBufNum(&max); 

	for(i=max-1;i>=0;i--)
	{
		unsigned int vn, in; 
		ID3D11Buffer* vb; 
		ID3D11Buffer* ib; 
		ID3D11Buffer* uv; 

		xobj->GetVertexBuf(i, &vb); 
		xobj->GetIndexBuf(i, &ib); 

		xobj->GetVertexNum(i, &vn); 
		xobj->GetIndexNum(i, &in); 

		if((vb ==0) || (ib == 0) || (vn == 0) || (in == 0)) {
			continue; 
		}

		UINT strides[1] = {sizeof(tDAEVERTEX)}; 
		UINT offsets[1] = {0}; 
		ID3D11Buffer* vbs[1] = {vb}; 

		context->IASetVertexBuffers(0, 1, vbs, strides, offsets); 

		context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0); 
		context->IASetInputLayout(pInputLayout);
		context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Dx11XMaterial buf_part; 
		Dx11XSubMesh submesh; 
		Dx11XMaterialStandard material; 
		int sub_mesh_max = xobj->GetNumberOfSubMesh(i); 

		buf_part.bind_pose = xobj->GetBindPose(i); 
		for(j=0;j<sub_mesh_max;j++) {
			xobj->GetSubMesh(i, j, &submesh); 
			xobj->GetMaterial(i, submesh.mat_index, &material); 

			memcpy(buf_part.diffuse, material.diffuse, sizeof(FLOAT)*4); 
			memcpy(buf_part.specular, material.specular, sizeof(FLOAT)*3); 
			buf_part.power = material.power; 
			memcpy(buf_part.emmisive, material.emissive, sizeof(FLOAT)*3); 
			if(material.tex_index < 0) {
				buf_part.tex_enable = 0; 
			} else {
				buf_part.tex_enable = 1; 

				ID3D11ShaderResourceView* srv[1]; 
				xobj->GetTexture(material.tex_index, &(srv[0])); 
				context->PSSetShaderResources(
					0,              // �ݒ肷��ŏ��̃X���b�g�ԍ�
					1,              // �ݒ肷��V�F�[�_�E���\�[�X�E�r���[�̐�
					srv);			// �ݒ肷��V�F�[�_�E���\�[�X�E�r���[�̔z��
			}

			hr = context->Map(
							  const_buf4part,              // �}�b�v���郊�\�[�X
							  0,                       // �T�u���\�[�X�̃C���f�b�N�X�ԍ�
							  D3D11_MAP_WRITE_DISCARD, // �������݃A�N�Z�X
							  0,                       //
							  &MappedResource);        // �f�[�^�̏������ݐ�|�C���^
			if (FAILED(hr)) {
				return DXTRACE_ERR(L"InitBackBuffer  g_pImmediateContext->Map", hr);  // ���s
			}

			// �f�[�^��������
			CopyMemory(MappedResource.pData, &buf_part, sizeof(Dx11XMaterial));
			// �}�b�v����
			context->Unmap(const_buf4part, 0);



//			context->DrawIndexed( in, 0, 0); 
			context->DrawIndexed(submesh.size*3, submesh.start_index*3, 0); 
		}

	}

	// �u�����h�E�X�e�[�g�E�I�u�W�F�N�g�𕜌�
	context->OMSetBlendState(old_pBlendState, oldBlendFactor, old_mask);
	// �[�x/�X�e���V���E�X�e�[�g�E�I�u�W�F�N�g�𕜌�
	context->OMSetDepthStencilState(old_pDepthStencilState, old_stencilref);
	return S_OK; 
}

HRESULT Dx11XEffect::Term(Dx11Context* context)
{
	SAFE_RELEASE(pBlendState); 
	SAFE_RELEASE(pDepthStencilState); 
	return S_OK; 
}
