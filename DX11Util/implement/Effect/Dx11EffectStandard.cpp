#include <DX11Util/Dx11EffectStandard.h>

typedef struct  {
	XMMATRIX World;		// ���[���h�ϊ��s��
	XMMATRIX View;		// �r���[�ϊ��s��
	XMMATRIX Projection;	// �����ϊ��s��
	FLOAT      dummy;		// �_�~�[
} Dx11EffectStandardCBuffer;


#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }	// ����}�N��


Dx11EffectStandard::Dx11EffectStandard(Dx11Camera* _camera, Dx11Lens* _lens, Dx11DirectionalLight* _light, int num_light)
{
	camera = _camera; 
	lens = _lens; 
	light = _light; 
}
Dx11EffectStandard::~Dx11EffectStandard(void)
{
}

HRESULT Dx11EffectStandard::Setup(Dx11Context* _context)
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
			L"shader\\standard.sh",  // file name
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

	SAFE_RELEASE(pBlobVS);
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitDirect3D pd3dDevice->CreateVertexShader", hr);
	}
	// **********************************************************
	// compile geometory shader. 
	ID3DBlob* pBlobGS = NULL;
	hr = D3DX11CompileFromFile(
			L"shader\\standard.sh",  // file name
			NULL,          // macro definition
			NULL,          // include file
			"GS",          // run GS function
			"gs_4_0",      // geometory shader
			flagCompile, // compile option
			0,             // compile option for effect
			NULL,          // finish function after compiling
			&pBlobGS,      // compiled byte code 
			NULL,          // no error message
			NULL);         // return
	if (FAILED(hr))
		return DXTRACE_ERR(L"InitDirect3D D3DX11CompileShaderFromFile", hr);

	hr = pd3dDevice->CreateGeometryShader(
			pBlobGS->GetBufferPointer(), 
			pBlobGS->GetBufferSize(),    
			NULL,
			&pGeometryShader); 
	SAFE_RELEASE(pBlobGS);  
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitDirect3D pd3dDevice->CreateGeometryShader", hr);
	}

	// **********************************************************
	// compile pixel shader
	ID3DBlob* pBlobPS = NULL;
	hr = D3DX11CompileFromFile(
			L"shader\\standard.sh",  // file name
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
	// define constant buffer. 
	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.Usage          = D3D11_USAGE_DYNAMIC;    
	cBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER; 
	cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;     
	cBufferDesc.MiscFlags      = 0;
	cBufferDesc.StructureByteStride = 0;

	// for view
	cBufferDesc.ByteWidth      = sizeof(Dx11EffectStandardCBuffer);
	hr = pd3dDevice->CreateBuffer(&cBufferDesc, NULL, &const_buf);
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitDirect3D pd3dDevice->CreateBuffer", hr);
	}

	return S_OK; 
}

HRESULT Dx11EffectStandard::Update(Dx11Context* _context, Dx11Object* obj, Dx11Motion* mot)
{
	HRESULT hr; 
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	Dx11EffectStandardCBuffer buf; 
	ID3D11DeviceContext* context = _context->GetDXDC(); 

	camera->GetMatrix(&(buf.View)); 

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
	CopyMemory(MappedResource.pData, &buf, sizeof(Dx11EffectStandardCBuffer));
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

	// GS�ɃW�I���g���E�V�F�[�_��ݒ�
	context->GSSetShader(pGeometryShader, NULL, 0);
	// GS�ɒ萔�o�b�t�@��ݒ�
	context->GSSetConstantBuffers(0, 1, &const_buf);

	// PS�Ƀs�N�Z���E�V�F�[�_��ݒ�
	context->PSSetShader(pPixelShader, NULL, 0);
	// PS�ɒ萔�o�b�t�@��ݒ�
	context->PSSetConstantBuffers(0, 1, &const_buf);

#if 0
	// PS�ɃV�F�[�_�E���\�[�X�E�r���[��ݒ�
	context->PSSetShaderResources(
        0,                // �ݒ肷��ŏ��̃X���b�g�ԍ�
        1,                // �ݒ肷��V�F�[�_�E���\�[�X�E�r���[�̐�
        &g_pTextureSRV);  // �ݒ肷��V�F�[�_�E���\�[�X�E�r���[�̔z��

#endif



	return S_OK; 
}

HRESULT Dx11EffectStandard::Term(Dx11Context* _context)
{
	return S_OK; 
}

