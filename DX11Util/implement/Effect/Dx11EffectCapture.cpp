#include <DX11Util/Effect/Dx11EffectCapture.h>
#include <d3dcompiler.h>
#include <DX11Util/Util/ShaderCompilerWrapper.h>
#include <DX11Util/Camera/Dx11CameraStandard.h>

typedef struct  {
	float trans; 
	float inv; 
	float dummy1, dummy2; 
	XMFLOAT4X4 World;		// ���[���h�ϊ��s��
	XMFLOAT4X4 View;		// �r���[�ϊ��s��
	XMFLOAT4X4 Projection;	// �����ϊ��s��
} Dx11EffectCaptureCBuffer;


#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }	// ����}�N��

class Dx11MotionScale : public Dx11Motion
{
public:
	Dx11MotionScale(float _sw, float _sh, float _px, float _py, float _pz) {
		sw = _sw; 
		sh = _sh; 
		px = _px; 
		py = _py; 
		pz = _pz; 
	}
	virtual ~Dx11MotionScale(void) {}

	virtual HRESULT Setup(){return S_OK;} 
	virtual HRESULT Update(){return S_OK;}
	virtual HRESULT GetMatrix(XMFLOAT4X4* mat) {
		mat->m[0][0] = sw; 
		mat->m[1][1] = sh; 
		mat->m[2][2] = 1.0; 
		mat->m[3][3] = 1.0f; 
		mat->m[0][1] = mat->m[0][2] = mat->m[0][3] = 0.0f; 
		mat->m[1][0] = mat->m[1][2] = mat->m[1][3] = 0.0f; 
		mat->m[2][0] = mat->m[2][1] = mat->m[2][3] = 0.0f; 
		mat->m[3][0] = mat->m[3][1] = mat->m[3][2] = 0.0f; 

		mat->m[3][0] = px; 
		mat->m[3][1] = py; 
		mat->m[3][2] = pz; 
		return S_OK; 
	}
	virtual HRESULT GetNumberOfMatrix(int* num) {
		*num = 1; 
		return S_OK;
	}
	virtual HRESULT Term(){return S_OK;} 

private: 
	float sw, sh, px, py, pz; 

};


Dx11EffectCapture::Dx11EffectCapture()
{
} 
Dx11EffectCapture::~Dx11EffectCapture(void)
{
}

HRESULT Dx11EffectCapture::Setup(Dx11Context* _context)
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
	hr = D3DCompileFromFileWrapper(
			L"shader\\capture.sh",  // file name
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
			L"shader\\capture.sh",  // file name
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
	cBufferDesc.ByteWidth      = sizeof(Dx11EffectCaptureCBuffer);
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

	return S_OK; 
}

HRESULT Dx11EffectCapture::Update(Dx11Context* _context, Dx11Object* obj, float px, float py, float width, float height, float depth, Dx11Lens* lens, float trans, float inv)
{
	Dx11CameraStandard cam; 
	float z_near, z_far, nwidth, nheight; 
	float scale; 

	lens->GetDistanceNear(&z_near); 
	lens->GetDistanceFar(&z_far); 
	lens->GetNearWidth(&nwidth); 
	lens->GetNearHeight(&nheight); 

	scale = depth / z_near; 

	Dx11MotionScale mot(nwidth * width*scale, nheight * height*scale, ((px*2.0f)-1.0f) * nwidth*0.5f*scale, ((py*2.0f)-1.0f) * nheight*0.5f*scale, depth); 

	cam.Setup(); 
	cam.SetCameraDirection(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 1000), XMFLOAT3(0, 1, 0)); 

	return Update(_context, obj, &mot, &cam, lens, trans, inv); 
}

HRESULT Dx11EffectCapture::Update(Dx11Context* _context, Dx11Object* obj, Dx11Motion* mot, Dx11Camera* cam, Dx11Lens* lens, float trans, float inv)
{
	HRESULT hr; 
	unsigned int i, max; 
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	Dx11EffectCaptureCBuffer buf; 
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
	buf.trans = trans; 
	buf.inv = inv; 

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
	CopyMemory(MappedResource.pData, &buf, sizeof(Dx11EffectCaptureCBuffer));
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

	// PS�Ƀs�N�Z���E�V�F�[�_��ݒ�
	context->PSSetShader(pPixelShader, NULL, 0);
	// PS�ɒ萔�o�b�t�@��ݒ�
	context->PSSetConstantBuffers(0, 1, &const_buf);


	obj->GetBufNum(&max); 

	for(i=0;i<max;i++)
	{
		unsigned int vn, in; 
		ID3D11Buffer* vb; 
		ID3D11Buffer* ib; 

		obj->GetVertexBuf(i, &vb); 
		obj->GetIndexBuf(i, &ib); 

		obj->GetVertexNum(i, &vn); 
		obj->GetIndexNum(i, &in); 

		UINT strides[1] = {sizeof(tDAEVERTEX)}; 
		UINT offsets[1] = {0}; 
		ID3D11Buffer* vbs[1] = {vb}; 

		context->IASetVertexBuffers(0, 1, vbs, strides, offsets); 

		context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0); 
		context->IASetInputLayout(pInputLayout);
		context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// PS�ɃV�F�[�_�E���\�[�X�E�r���[��ݒ�
		ID3D11ShaderResourceView* srv[1]; 
		obj->GetTexture(i, &(srv[0])); 
		context->PSSetShaderResources(
	        0,              // �ݒ肷��ŏ��̃X���b�g�ԍ�
	        1,              // �ݒ肷��V�F�[�_�E���\�[�X�E�r���[�̐�
	        srv);			// �ݒ肷��V�F�[�_�E���\�[�X�E�r���[�̔z��

		context->DrawIndexed( in, 0, 0); 

//		pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVertices[i], 0, m_dwIndecies[i]/3);

//		pd3dDevice->SetTexture(0, NULL);
	}

	// �u�����h�E�X�e�[�g�E�I�u�W�F�N�g�𕜌�
	context->OMSetBlendState(old_pBlendState, oldBlendFactor, old_mask);
	// �[�x/�X�e���V���E�X�e�[�g�E�I�u�W�F�N�g�𕜌�
	context->OMSetDepthStencilState(old_pDepthStencilState, old_stencilref);
	return S_OK; 
}

HRESULT Dx11EffectCapture::Term(Dx11Context* _context)
{
	SAFE_RELEASE(pBlendState); 
	SAFE_RELEASE(pDepthStencilState); 
	return S_OK; 
}

