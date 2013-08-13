#include <DX11Util/Core/Dx11Context.h>


#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }	// 解放マクロ

Dx11Context::Dx11Context(void)
{
}

Dx11Context::~Dx11Context(void)
{
}

/*-------------------------------------------
	Direct3D初期化
--------------------------------------------*/
HRESULT Dx11Context::Setup(HWND hwnd, Dx11Scene* first_scene)
{
	pFeatureLevels = new D3D_FEATURE_LEVEL[3]; 
	pFeatureLevels[0] = D3D_FEATURE_LEVEL_11_0; 
	pFeatureLevels[1] = D3D_FEATURE_LEVEL_10_1; 
	pFeatureLevels[2] = D3D_FEATURE_LEVEL_10_0; 

	FeatureLevels    = 3;   // 配列の要素数
	FeatureLevelsSupported; // デバイス作成時に返される機能レベル

// インターフェイス
	pD3DDevice = NULL;        // デバイス
	pImmediateContext = NULL; // デバイス・コンテキスト
	pSwapChain = NULL;        // スワップ・チェイン

	pRenderTargetView = NULL; // 描画ターゲット・ビュー

	pBlendState = NULL;			// ブレンド・ステート・オブジェクト
	pRasterizerState = NULL;	// ラスタライザ・ステート・オブジェクト
	pDepthStencilState = NULL;	// 深度/ステンシル・ステート・オブジェクト

	pTextureSamplerWrap = NULL; // サンプラー

	// ウインドウのクライアント エリア
	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

    // デバイスとスワップ・チェインの作成
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));    // 構造体の値を初期化(必要な場合)
    sd.BufferCount       = 1;       // バック・バッファ数
    sd.BufferDesc.Width  = 640;     // バック・バッファの幅
    sd.BufferDesc.Height = 480;     // バック・バッファの高さ
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // フォーマット
    sd.BufferDesc.RefreshRate.Numerator = 60;  // リフレッシュ・レート(分子)
    sd.BufferDesc.RefreshRate.Denominator = 1; // リフレッシュ・レート(分母)
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // バック・バッファの使用法
    sd.OutputWindow = hwnd;    // 関連付けるウインドウ
    sd.SampleDesc.Count = 1;        // マルチ・サンプルの数
    sd.SampleDesc.Quality = 0;      // マルチ・サンプルのクオリティ
    sd.Windowed = TRUE;             // ウインドウ・モード
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // モード自動切り替え

#if defined(DEBUG) || defined(_DEBUG)
    UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#else
    UINT createDeviceFlags = 0;
#endif

	// ハードウェア・デバイスを作成
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
            NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags,
            pFeatureLevels, FeatureLevels, D3D11_SDK_VERSION, &sd,
            &pSwapChain, &pD3DDevice, &FeatureLevelsSupported,
            &pImmediateContext);
    if(FAILED(hr)) {
        // WARPデバイスを作成
        hr = D3D11CreateDeviceAndSwapChain(
            NULL, D3D_DRIVER_TYPE_WARP, NULL, createDeviceFlags,
            pFeatureLevels, FeatureLevels, D3D11_SDK_VERSION, &sd,
            &pSwapChain, &pD3DDevice, &FeatureLevelsSupported,
            &pImmediateContext);
        if(FAILED(hr)) {
            // リファレンス・デバイスを作成
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


	// バック バッファの初期化

	// スワップ・チェインから最初のバック・バッファを取得する
    ID3D11Texture2D *pBackBuffer;  // バッファのアクセスに使うインターフェイス
    hr = pSwapChain->GetBuffer(
            0,                         // バック・バッファの番号
            __uuidof(ID3D11Texture2D), // バッファにアクセスするインターフェイス
            (LPVOID*)&pBackBuffer);    // バッファを受け取る変数
    if(FAILED(hr)) {
		return DXTRACE_ERR(L"InitBackBuffer s_pSwapChain->GetBuffer", hr);  // 失敗
	}

	// バック・バッファの情報
	D3D11_TEXTURE2D_DESC descBackBuffer;
	pBackBuffer->GetDesc(&descBackBuffer);

	// バック・バッファの描画ターゲット・ビューを作る
    hr = pD3DDevice->CreateRenderTargetView(
            pBackBuffer,           // ビューでアクセスするリソース
            NULL,                  // 描画ターゲット・ビューの定義
            &pRenderTargetView); // 描画ターゲット・ビューを受け取る変数

    SAFE_RELEASE(pBackBuffer);  // 以降、バック・バッファは直接使わないので解放
    if(FAILED(hr)) {
		return DXTRACE_ERR(L"InitBackBuffer s_pD3DDevice->CreateRenderTargetView", hr);  // 失敗
	}

	// 深度/ステンシル・テクスチャの作成
	D3D11_TEXTURE2D_DESC descDepth = descBackBuffer;
//	descDepth.Width     = descBackBuffer.Width;   // 幅
//	descDepth.Height    = descBackBuffer.Height;  // 高さ
	descDepth.MipLevels = 1;       // ミップマップ・レベル数
	descDepth.ArraySize = 1;       // 配列サイズ
	descDepth.Format    = DXGI_FORMAT_D32_FLOAT;  // フォーマット(深度のみ)
//	descDepth.SampleDesc.Count   = 1;  // マルチサンプリングの設定
//	descDepth.SampleDesc.Quality = 0;  // マルチサンプリングの品質
	descDepth.Usage          = D3D11_USAGE_DEFAULT;      // デフォルト使用法
	descDepth.BindFlags      = D3D11_BIND_DEPTH_STENCIL; // 深度/ステンシルとして使用
	descDepth.CPUAccessFlags = 0;   // CPUからはアクセスしない
	descDepth.MiscFlags      = 0;   // その他の設定なし
	hr = pD3DDevice->CreateTexture2D(
	        &descDepth,         // 作成する2Dテクスチャの設定
	        NULL,               // 
	        &pDepthStencil);  // 作成したテクスチャを受け取る変数
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitBackBuffer s_pD3DDevice->CreateTexture2D", hr);  // 失敗
	}

	// 深度/ステンシル・ビューの作成
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format             = descDepth.Format;            // ビューのフォーマット
	descDSV.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags              = 0;
	descDSV.Texture2D.MipSlice = 0;
	hr = pD3DDevice->CreateDepthStencilView(
	        pDepthStencil,       // 深度/ステンシル・ビューを作るテクスチャ
	        &descDSV,              // 深度/ステンシル・ビューの設定
	        &pDepthStencilView); // 作成したビューを受け取る変数
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitBackBuffer s_pD3DDevice->CreateDepthStencilView", hr);  // 失敗
	}

    // ビューポートの設定
    ViewPort[0].TopLeftX = 0.0f;    // ビューポート領域の左上X座標。
    ViewPort[0].TopLeftY = 0.0f;    // ビューポート領域の左上Y座標。
    ViewPort[0].Width    = 640.0f;  // ビューポート領域の幅
    ViewPort[0].Height   = 480.0f;  // ビューポート領域の高さ
    ViewPort[0].MinDepth = 0.0f; // ビューポート領域の深度値の最小値
    ViewPort[0].MaxDepth = 1.0f; // ビューポート領域の深度値の最大値

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

	// ラスタライザ・ステート・オブジェクトの作成
	D3D11_RASTERIZER_DESC RSDesc;
	RSDesc.FillMode = D3D11_FILL_SOLID;   // 普通に描画する
	RSDesc.CullMode = D3D11_CULL_NONE;    // 両面を描画する
	RSDesc.FrontCounterClockwise = FALSE; // 時計回りが表面
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
	// 深度/ステンシル・ステート・オブジェクトの作成
	D3D11_DEPTH_STENCIL_DESC DepthStencil;
	DepthStencil.DepthEnable      = TRUE; // 深度テストあり
	DepthStencil.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ALL; // 書き込む
	DepthStencil.DepthFunc        = D3D11_COMPARISON_LESS; // 手前の物体を描画
	DepthStencil.StencilEnable    = FALSE; // ステンシル・テストなし
	DepthStencil.StencilReadMask  = 0;     // ステンシル読み込みマスク。
	DepthStencil.StencilWriteMask = 0;     // ステンシル書き込みマスク。
			// 面が表を向いている場合のステンシル・テストの設定
	DepthStencil.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;  // 維持
	DepthStencil.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;  // 維持
	DepthStencil.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;  // 維持
	DepthStencil.FrontFace.StencilFunc        = D3D11_COMPARISON_NEVER; // 常に失敗
			// 面が裏を向いている場合のステンシル・テストの設定
	DepthStencil.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;   // 維持
	DepthStencil.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;   // 維持
	DepthStencil.BackFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;   // 維持
	DepthStencil.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS; // 常に成功
	hr = pD3DDevice->CreateDepthStencilState(&DepthStencil,
											   &pDepthStencilState);
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateDepthStencilState", hr);
	}


	// サンプラーの作成
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
    // 描画ターゲットのクリア
    pImmediateContext->ClearRenderTargetView(
                       pRenderTargetView, // クリアする描画ターゲット
                       ClearColor);         // クリアする値

	// 深度/ステンシルのクリア
	pImmediateContext->ClearDepthStencilView(
			pDepthStencilView, // クリアする深度/ステンシル・ビュー
			D3D11_CLEAR_DEPTH,   // 深度値だけをクリアする
			1.0f,                // 深度バッファをクリアする値
			0);                  // ステンシル・バッファをクリアする値(この場合、無関係)


	return Activate(); 
}

HRESULT Dx11Context::Activate()
{
	// OMに描画ターゲット ビューと深度/ステンシル・ビューを設定
	pImmediateContext->OMSetRenderTargets(1, &pRenderTargetView,pDepthStencilView);
	// OMにブレンド・ステート・オブジェクトを設定
	FLOAT BlendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	pImmediateContext->OMSetBlendState(pBlendState, BlendFactor, 0xffffffff);
	// OMに深度/ステンシル・ステート・オブジェクトを設定
	pImmediateContext->OMSetDepthStencilState(pDepthStencilState, 0);

	// ラスタライザにビューポートを設定
    pImmediateContext->RSSetViewports(1, ViewPort);

    // 描画ターゲット・ビューを出力マージャーの描画ターゲットとして設定
    pImmediateContext->OMSetRenderTargets(
            1,                    // 描画ターゲットの数
            &pRenderTargetView, // 描画ターゲット・ビューの配列
	        pDepthStencilView); // 設定する深度/ステンシル・ビュー

	// RSにラスタライザ・ステート・オブジェクトを設定
	pImmediateContext->RSSetState(pRasterizerState);

	// PSにサンプラーを設定
	pImmediateContext->PSSetSamplers(0, 1, &pTextureSamplerWrap);

	return S_OK; 
}

HRESULT Dx11Context::Flush()
{
	HRESULT hr; 
	// ***************************************
	// バック バッファの表示
	hr = pSwapChain->Present(	0,	// 画面を直ぐに更新する
								0);	// 画面を実際に更新する
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
	pImmediateContext->OMSetRenderTargets(0, NULL, NULL);	// 描画ターゲットの解除
	SAFE_RELEASE(pRenderTargetView);					    // 描画ターゲット ビューの解放

	// バッファの変更
	pSwapChain->ResizeBuffers(1, 0, 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	// バック バッファの初期化
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

    // スワップ・チェインから最初のバック・バッファを取得する
    ID3D11Texture2D *pBackBuffer;  // バッファのアクセスに使うインターフェイス
    hr = pSwapChain->GetBuffer(
            0,                         // バック・バッファの番号
            __uuidof(ID3D11Texture2D), // バッファにアクセスするインターフェイス
            (LPVOID*)&pBackBuffer);    // バッファを受け取る変数
    if(FAILED(hr)) {
		return DXTRACE_ERR(L"InitBackBuffer g_pSwapChain->GetBuffer", hr);  // 失敗
	}

    // バック・バッファの描画ターゲット・ビューを作る
    hr = pD3DDevice->CreateRenderTargetView(
            pBackBuffer,           // ビューでアクセスするリソース
            NULL,                  // 描画ターゲット・ビューの定義
            &pRenderTargetView); // 描画ターゲット・ビューを受け取る変数
    SAFE_RELEASE(pBackBuffer);  // 以降、バック・バッファは直接使わないので解放
    if(FAILED(hr)) {
		return DXTRACE_ERR(L"InitBackBuffer g_pD3DDevice->CreateRenderTargetView", hr);  // 失敗
	}

    // ビューポートの設定
    ViewPort[0].TopLeftX = 0.0f;    // ビューポート領域の左上X座標。
    ViewPort[0].TopLeftY = 0.0f;    // ビューポート領域の左上Y座標。
    ViewPort[0].Width    = (float)width;  // ビューポート領域の幅
    ViewPort[0].Height   = (float)height;  // ビューポート領域の高さ
    ViewPort[0].MinDepth = 0.0f; // ビューポート領域の深度値の最小値
    ViewPort[0].MaxDepth = 1.0f; // ビューポート領域の深度値の最大値

	return S_OK;
}