#include <DX11Util/Core/Dx11Context.h>


#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }	// ����}�N��

Dx11Context::Dx11Context(void)
{
}

Dx11Context::~Dx11Context(void)
{
}

/*-------------------------------------------
	Direct3D������
--------------------------------------------*/
HRESULT Dx11Context::Setup(HWND hwnd, Dx11Scene* first_scene)
{
	pFeatureLevels = new D3D_FEATURE_LEVEL[3]; 
	pFeatureLevels[0] = D3D_FEATURE_LEVEL_11_0; 
	pFeatureLevels[1] = D3D_FEATURE_LEVEL_10_1; 
	pFeatureLevels[2] = D3D_FEATURE_LEVEL_10_0; 

	FeatureLevels    = 3;   // �z��̗v�f��
	FeatureLevelsSupported; // �f�o�C�X�쐬���ɕԂ����@�\���x��

// �C���^�[�t�F�C�X
	pD3DDevice = NULL;        // �f�o�C�X
	pImmediateContext = NULL; // �f�o�C�X�E�R���e�L�X�g
	pSwapChain = NULL;        // �X���b�v�E�`�F�C��

	pRenderTargetView = NULL; // �`��^�[�Q�b�g�E�r���[

	pBlendState = NULL;			// �u�����h�E�X�e�[�g�E�I�u�W�F�N�g
	pRasterizerState = NULL;	// ���X�^���C�U�E�X�e�[�g�E�I�u�W�F�N�g
	pDepthStencilState = NULL;	// �[�x/�X�e���V���E�X�e�[�g�E�I�u�W�F�N�g

	pTextureSamplerWrap = NULL; // �T���v���[

	// �E�C���h�E�̃N���C�A���g �G���A
	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

    // �f�o�C�X�ƃX���b�v�E�`�F�C���̍쐬
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));    // �\���̂̒l��������(�K�v�ȏꍇ)
    sd.BufferCount       = 1;       // �o�b�N�E�o�b�t�@��
    sd.BufferDesc.Width  = 640;     // �o�b�N�E�o�b�t�@�̕�
    sd.BufferDesc.Height = 480;     // �o�b�N�E�o�b�t�@�̍���
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // �t�H�[�}�b�g
    sd.BufferDesc.RefreshRate.Numerator = 60;  // ���t���b�V���E���[�g(���q)
    sd.BufferDesc.RefreshRate.Denominator = 1; // ���t���b�V���E���[�g(����)
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // �o�b�N�E�o�b�t�@�̎g�p�@
    sd.OutputWindow = hwnd;    // �֘A�t����E�C���h�E
    sd.SampleDesc.Count = 1;        // �}���`�E�T���v���̐�
    sd.SampleDesc.Quality = 0;      // �}���`�E�T���v���̃N�I���e�B
    sd.Windowed = TRUE;             // �E�C���h�E�E���[�h
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // ���[�h�����؂�ւ�

#if defined(DEBUG) || defined(_DEBUG)
    UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#else
    UINT createDeviceFlags = 0;
#endif

	// �n�[�h�E�F�A�E�f�o�C�X���쐬
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
            NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags,
            pFeatureLevels, FeatureLevels, D3D11_SDK_VERSION, &sd,
            &pSwapChain, &pD3DDevice, &FeatureLevelsSupported,
            &pImmediateContext);
    if(FAILED(hr)) {
        // WARP�f�o�C�X���쐬
        hr = D3D11CreateDeviceAndSwapChain(
            NULL, D3D_DRIVER_TYPE_WARP, NULL, createDeviceFlags,
            pFeatureLevels, FeatureLevels, D3D11_SDK_VERSION, &sd,
            &pSwapChain, &pD3DDevice, &FeatureLevelsSupported,
            &pImmediateContext);
        if(FAILED(hr)) {
            // ���t�@�����X�E�f�o�C�X���쐬
            hr = D3D11CreateDeviceAndSwapChain(
                NULL, D3D_DRIVER_TYPE_REFERENCE, NULL, createDeviceFlags,
                pFeatureLevels, FeatureLevels, D3D11_SDK_VERSION, &sd,
                &pSwapChain, &pD3DDevice, &FeatureLevelsSupported,
                &pImmediateContext);
            if(FAILED(hr)) {
                return DXTRACE_ERR(L"InitDirect3D D3D11CreateDeviceAndSwapChain", hr);
            }
        }
    }


	// �o�b�N �o�b�t�@�̏�����

	// �X���b�v�E�`�F�C������ŏ��̃o�b�N�E�o�b�t�@���擾����
    ID3D11Texture2D *pBackBuffer;  // �o�b�t�@�̃A�N�Z�X�Ɏg���C���^�[�t�F�C�X
    hr = pSwapChain->GetBuffer(
            0,                         // �o�b�N�E�o�b�t�@�̔ԍ�
            __uuidof(ID3D11Texture2D), // �o�b�t�@�ɃA�N�Z�X����C���^�[�t�F�C�X
            (LPVOID*)&pBackBuffer);    // �o�b�t�@���󂯎��ϐ�
    if(FAILED(hr)) {
		return DXTRACE_ERR(L"InitBackBuffer s_pSwapChain->GetBuffer", hr);  // ���s
	}

	// �o�b�N�E�o�b�t�@�̏��
	D3D11_TEXTURE2D_DESC descBackBuffer;
	pBackBuffer->GetDesc(&descBackBuffer);

	// �o�b�N�E�o�b�t�@�̕`��^�[�Q�b�g�E�r���[�����
    hr = pD3DDevice->CreateRenderTargetView(
            pBackBuffer,           // �r���[�ŃA�N�Z�X���郊�\�[�X
            NULL,                  // �`��^�[�Q�b�g�E�r���[�̒�`
            &pRenderTargetView); // �`��^�[�Q�b�g�E�r���[���󂯎��ϐ�

    SAFE_RELEASE(pBackBuffer);  // �ȍ~�A�o�b�N�E�o�b�t�@�͒��ڎg��Ȃ��̂ŉ��
    if(FAILED(hr)) {
		return DXTRACE_ERR(L"InitBackBuffer s_pD3DDevice->CreateRenderTargetView", hr);  // ���s
	}

	// �[�x/�X�e���V���E�e�N�X�`���̍쐬
	D3D11_TEXTURE2D_DESC descDepth = descBackBuffer;
//	descDepth.Width     = descBackBuffer.Width;   // ��
//	descDepth.Height    = descBackBuffer.Height;  // ����
	descDepth.MipLevels = 1;       // �~�b�v�}�b�v�E���x����
	descDepth.ArraySize = 1;       // �z��T�C�Y
	descDepth.Format    = DXGI_FORMAT_D32_FLOAT;  // �t�H�[�}�b�g(�[�x�̂�)
//	descDepth.SampleDesc.Count   = 1;  // �}���`�T���v�����O�̐ݒ�
//	descDepth.SampleDesc.Quality = 0;  // �}���`�T���v�����O�̕i��
	descDepth.Usage          = D3D11_USAGE_DEFAULT;      // �f�t�H���g�g�p�@
	descDepth.BindFlags      = D3D11_BIND_DEPTH_STENCIL; // �[�x/�X�e���V���Ƃ��Ďg�p
	descDepth.CPUAccessFlags = 0;   // CPU����̓A�N�Z�X���Ȃ�
	descDepth.MiscFlags      = 0;   // ���̑��̐ݒ�Ȃ�
	hr = pD3DDevice->CreateTexture2D(
	        &descDepth,         // �쐬����2D�e�N�X�`���̐ݒ�
	        NULL,               // 
	        &pDepthStencil);  // �쐬�����e�N�X�`�����󂯎��ϐ�
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitBackBuffer s_pD3DDevice->CreateTexture2D", hr);  // ���s
	}

	// �[�x/�X�e���V���E�r���[�̍쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format             = descDepth.Format;            // �r���[�̃t�H�[�}�b�g
	descDSV.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags              = 0;
	descDSV.Texture2D.MipSlice = 0;
	hr = pD3DDevice->CreateDepthStencilView(
	        pDepthStencil,       // �[�x/�X�e���V���E�r���[�����e�N�X�`��
	        &descDSV,              // �[�x/�X�e���V���E�r���[�̐ݒ�
	        &pDepthStencilView); // �쐬�����r���[���󂯎��ϐ�
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitBackBuffer s_pD3DDevice->CreateDepthStencilView", hr);  // ���s
	}

    // �r���[�|�[�g�̐ݒ�
    ViewPort[0].TopLeftX = 0.0f;    // �r���[�|�[�g�̈�̍���X���W�B
    ViewPort[0].TopLeftY = 0.0f;    // �r���[�|�[�g�̈�̍���Y���W�B
    ViewPort[0].Width    = 640.0f;  // �r���[�|�[�g�̈�̕�
    ViewPort[0].Height   = 480.0f;  // �r���[�|�[�g�̈�̍���
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
	hr = pD3DDevice->CreateBlendState(&BlendState, &pBlendState);
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateBlendState", hr);
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
	hr = pD3DDevice->CreateRasterizerState(&RSDesc, &pRasterizerState);
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateRasterizerState", hr);
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
	hr = pD3DDevice->CreateDepthStencilState(&DepthStencil,
											   &pDepthStencilState);
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateDepthStencilState", hr);
	}


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
	hr = pD3DDevice->CreateSamplerState(&descSampler, &pTextureSamplerWrap);
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateSamplerState", hr);
	}

	current_scene = first_scene; 

	if(first_scene == 0) {
		return S_FALSE; 
	}

	current_scene->Setup(this); 

	QueryPerformanceFrequency(&freq); 
	QueryPerformanceCounter(&now);

	return S_OK;
}


HRESULT Dx11Context::Update()
{
	const int frames = 60; 
	if(current_scene == 0) {
		return S_FALSE; 
	}
	LARGE_INTEGER latest, diff, li; 
	QueryPerformanceCounter(&latest);
	diff.QuadPart = latest.QuadPart - now.QuadPart; 

//	for(li.QuadPart = 0;li.QuadPart < diff / (freq/frames); li.QuadPart += freq/frame) {
	for(li.QuadPart = freq.QuadPart;li.QuadPart < diff.QuadPart * frames; li.QuadPart += freq.QuadPart) {
		current_scene->Update(); 
		now.QuadPart += freq.QuadPart / frames; 
	}

	Start(); 
	current_scene->Render(this, 0, 0 ); 
	Flush(); 

	Dx11Scene* next = current_scene->GetNext(this); 
	if(next == 0) {
		current_scene->Term(this); 
		current_scene = next; 
	} else if(next != current_scene) {
		current_scene->Term(this); 
		current_scene = next; 
		current_scene->Setup(this); 
	}

	if(current_scene == 0) {
		return S_FALSE; 
	}

	return S_OK; 
}



HRESULT Dx11Context::Start()
{
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    // �`��^�[�Q�b�g�̃N���A
    pImmediateContext->ClearRenderTargetView(
                       pRenderTargetView, // �N���A����`��^�[�Q�b�g
                       ClearColor);         // �N���A����l

	// �[�x/�X�e���V���̃N���A
	pImmediateContext->ClearDepthStencilView(
			pDepthStencilView, // �N���A����[�x/�X�e���V���E�r���[
			D3D11_CLEAR_DEPTH,   // �[�x�l�������N���A����
			1.0f,                // �[�x�o�b�t�@���N���A����l
			0);                  // �X�e���V���E�o�b�t�@���N���A����l(���̏ꍇ�A���֌W)


	return Activate(); 
}

HRESULT Dx11Context::Activate()
{
	// OM�ɕ`��^�[�Q�b�g �r���[�Ɛ[�x/�X�e���V���E�r���[��ݒ�
	pImmediateContext->OMSetRenderTargets(1, &pRenderTargetView,pDepthStencilView);
	// OM�Ƀu�����h�E�X�e�[�g�E�I�u�W�F�N�g��ݒ�
	FLOAT BlendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	pImmediateContext->OMSetBlendState(pBlendState, BlendFactor, 0xffffffff);
	// OM�ɐ[�x/�X�e���V���E�X�e�[�g�E�I�u�W�F�N�g��ݒ�
	pImmediateContext->OMSetDepthStencilState(pDepthStencilState, 0);

	// ���X�^���C�U�Ƀr���[�|�[�g��ݒ�
    pImmediateContext->RSSetViewports(1, ViewPort);

    // �`��^�[�Q�b�g�E�r���[���o�̓}�[�W���[�̕`��^�[�Q�b�g�Ƃ��Đݒ�
    pImmediateContext->OMSetRenderTargets(
            1,                    // �`��^�[�Q�b�g�̐�
            &pRenderTargetView, // �`��^�[�Q�b�g�E�r���[�̔z��
	        pDepthStencilView); // �ݒ肷��[�x/�X�e���V���E�r���[

	// RS�Ƀ��X�^���C�U�E�X�e�[�g�E�I�u�W�F�N�g��ݒ�
	pImmediateContext->RSSetState(pRasterizerState);

	// PS�ɃT���v���[��ݒ�
	pImmediateContext->PSSetSamplers(0, 1, &pTextureSamplerWrap);

	return S_OK; 
}

HRESULT Dx11Context::Flush()
{
	HRESULT hr; 
	// ***************************************
	// �o�b�N �o�b�t�@�̕\��
	hr = pSwapChain->Present(	0,	// ��ʂ𒼂��ɍX�V����
								0);	// ��ʂ����ۂɍX�V����
	return hr; 
}

HRESULT Dx11Context::Term()
{
	if(current_scene != 0) {
		current_scene->Term(this); 
		current_scene = 0; 
	}

	SAFE_RELEASE(pTextureSamplerWrap); 
	SAFE_RELEASE(pDepthStencilState); 
	SAFE_RELEASE(pRasterizerState); 
	SAFE_RELEASE(pBlendState); 
	SAFE_RELEASE(pDepthStencilView); 
	SAFE_RELEASE(pDepthStencil); 
	SAFE_RELEASE(pRenderTargetView); 
	SAFE_RELEASE(pSwapChain); 
	SAFE_RELEASE(pImmediateContext); 
	SAFE_RELEASE(pD3DDevice); 
	return S_OK; 
}

ID3D11DeviceContext* Dx11Context::GetDXDC()
{
	return pImmediateContext; 
}

ID3D11Device* Dx11Context::GetDXDevice()
{
	return pD3DDevice; 
}

HRESULT Dx11Context::WindowResize(int width, int height)
{
	pImmediateContext->OMSetRenderTargets(0, NULL, NULL);	// �`��^�[�Q�b�g�̉���
	SAFE_RELEASE(pRenderTargetView);					    // �`��^�[�Q�b�g �r���[�̉��

	// �o�b�t�@�̕ύX
	pSwapChain->ResizeBuffers(1, 0, 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	// �o�b�N �o�b�t�@�̏�����
	InitBackBuffer(width, height);
	return S_OK; 
}

HRESULT Dx11Context::ToggleFullScreen()
{
	BOOL fullscreen; 
	if (pSwapChain != NULL) {
		pSwapChain->GetFullscreenState(&fullscreen, NULL);
		pSwapChain->SetFullscreenState(!fullscreen, NULL);
	}
	return S_OK; 
}

HRESULT Dx11Context::InitBackBuffer(int width, int height)
{
	HRESULT hr;

    // �X���b�v�E�`�F�C������ŏ��̃o�b�N�E�o�b�t�@���擾����
    ID3D11Texture2D *pBackBuffer;  // �o�b�t�@�̃A�N�Z�X�Ɏg���C���^�[�t�F�C�X
    hr = pSwapChain->GetBuffer(
            0,                         // �o�b�N�E�o�b�t�@�̔ԍ�
            __uuidof(ID3D11Texture2D), // �o�b�t�@�ɃA�N�Z�X����C���^�[�t�F�C�X
            (LPVOID*)&pBackBuffer);    // �o�b�t�@���󂯎��ϐ�
    if(FAILED(hr)) {
		return DXTRACE_ERR(L"InitBackBuffer g_pSwapChain->GetBuffer", hr);  // ���s
	}

    // �o�b�N�E�o�b�t�@�̕`��^�[�Q�b�g�E�r���[�����
    hr = pD3DDevice->CreateRenderTargetView(
            pBackBuffer,           // �r���[�ŃA�N�Z�X���郊�\�[�X
            NULL,                  // �`��^�[�Q�b�g�E�r���[�̒�`
            &pRenderTargetView); // �`��^�[�Q�b�g�E�r���[���󂯎��ϐ�
    SAFE_RELEASE(pBackBuffer);  // �ȍ~�A�o�b�N�E�o�b�t�@�͒��ڎg��Ȃ��̂ŉ��
    if(FAILED(hr)) {
		return DXTRACE_ERR(L"InitBackBuffer g_pD3DDevice->CreateRenderTargetView", hr);  // ���s
	}

    // �r���[�|�[�g�̐ݒ�
    ViewPort[0].TopLeftX = 0.0f;    // �r���[�|�[�g�̈�̍���X���W�B
    ViewPort[0].TopLeftY = 0.0f;    // �r���[�|�[�g�̈�̍���Y���W�B
    ViewPort[0].Width    = (float)width;  // �r���[�|�[�g�̈�̕�
    ViewPort[0].Height   = (float)height;  // �r���[�|�[�g�̈�̍���
    ViewPort[0].MinDepth = 0.0f; // �r���[�|�[�g�̈�̐[�x�l�̍ŏ��l
    ViewPort[0].MaxDepth = 1.0f; // �r���[�|�[�g�̈�̐[�x�l�̍ő�l

	return S_OK;
}