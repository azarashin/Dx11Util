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

	// 頂点バッファのサブリソースの定義
	D3D11_SUBRESOURCE_DATA vSubData;
	vSubData.pSysMem          = v;  // バッファ・データの初期値
	vSubData.SysMemPitch      = 0;
	vSubData.SysMemSlicePitch = 0;

	// 頂点バッファの作成
	hr = pd3dDevice->CreateBuffer(&vBufferDesc, &vSubData, &m_pVB);
	if (FAILED(hr)) {
		DXTRACE_ERR(L"InitDirect3D pd3dDevice->CreateBuffer", hr);
		return; 
	}

	// **********************************************************
	// インデックス・バッファの定義
	D3D11_BUFFER_DESC idxBufferDesc;
	idxBufferDesc.Usage          = D3D11_USAGE_DEFAULT;     // デフォルト使用法
	idxBufferDesc.ByteWidth      = sizeof(UINT) * 6;       // 2×3頂点
	idxBufferDesc.BindFlags      = D3D11_BIND_INDEX_BUFFER; // インデックス・バッファ
	idxBufferDesc.CPUAccessFlags = 0;
	idxBufferDesc.MiscFlags      = 0;
	idxBufferDesc.StructureByteStride = 0;

	// インデックス・バッファのサブリソースの初期値(頂点ID)
	UINT idxVertexID[] = {
		0,1,2,  0,2,3,
	};

	D3D11_SUBRESOURCE_DATA idxSubData;
	idxSubData.pSysMem          = idxVertexID;  // バッファ・データの初期値
	idxSubData.SysMemPitch      = 0;
	idxSubData.SysMemSlicePitch = 0;

	hr = pd3dDevice->CreateBuffer(&idxBufferDesc, &idxSubData, &m_pIB);
	if (FAILED(hr)) {
		DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateBuffer", hr);
		return; 
	}

	// **********************************************************
	// 描画可能テクスチャの作成
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width     = width;  // 幅
	desc.Height    = height;  // 高さ
	desc.MipLevels = mipmap_level;    // ミップマップ レベル数
	desc.ArraySize = 1;    // テクスチャの配列サイズ
	desc.Format    = DXGI_FORMAT_R32G32B32A32_FLOAT; // フォーマット
	desc.SampleDesc.Count = 1; // マルチサンプリングの設定
	desc.Usage            = D3D11_USAGE_DEFAULT;  
	desc.BindFlags        = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; // シェーダ リソース
	if(readable) {
//		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE|D3D11_CPU_ACCESS_READ; // impossible to set this flag as output texture. 
	}
	hr = pd3dDevice->CreateTexture2D(&desc, NULL, &m_pTex);
	if (FAILED(hr)) {
		DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateTexture2D", hr);
		return; 
	}

	// 描画可能テクスチャの描画ターゲット ビューの作成
	D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
	rtDesc.Format             = desc.Format; // テクスチャのフォーマットを指定
	rtDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtDesc.Texture2D.MipSlice = 0;
	hr = pd3dDevice->CreateRenderTargetView(
			m_pTex,      // ビューを作るテクスチャ リソース
			&rtDesc,           // ビューの設定
			&pRenderTargetView); // ビューを受け取る変数
	if (FAILED(hr)) {
		DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateRenderTargetView", hr);
		return; 
	}

	// 2Dテクスチャにアクセスするシェーダ リソース ビューの設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format        = desc.Format;		// フォーマット
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2Dテクスチャ
	srDesc.Texture2D.MostDetailedMip = 0; 
	srDesc.Texture2D.MipLevels       = mipmap_level;

	// シェーダ リソース ビューの作成
	hr = pd3dDevice->CreateShaderResourceView(
				m_pTex,       // アクセスするテクスチャ リソース
				&srDesc,          // シェーダ リソース ビューの設定
				&m_pTexture);  // 受け取る変数
	if (FAILED(hr)) {
		DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateShaderResourceView", hr);
		return; 
	}

	// 深度/ステンシル・テクスチャの作成
	D3D11_TEXTURE2D_DESC descDepth = desc;
	descDepth.MipLevels = 1;       // ミップマップ・レベル数
	descDepth.ArraySize = 1;       // 配列サイズ
	descDepth.Format    = DXGI_FORMAT_D32_FLOAT;  // フォーマット(深度のみ)
	descDepth.Usage          = D3D11_USAGE_DEFAULT;      // デフォルト使用法
	descDepth.BindFlags      = D3D11_BIND_DEPTH_STENCIL; // 深度/ステンシルとして使用
	descDepth.CPUAccessFlags = 0;   // CPUからはアクセスしない
	descDepth.MiscFlags      = 0;   // その他の設定なし
	hr = pd3dDevice->CreateTexture2D(
	        &descDepth,         // 作成する2Dテクスチャの設定
	        NULL,               // 
	        &pDepthStencil);  // 作成したテクスチャを受け取る変数
	if (FAILED(hr)) {
		DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateTexture2D", hr);
		return; 
	}
	
	// 深度/ステンシル・ビューの作成
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format             = descDepth.Format;            // ビューのフォーマット
	descDSV.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags              = 0;
	descDSV.Texture2D.MipSlice = 0;
	hr = pd3dDevice->CreateDepthStencilView(
	        pDepthStencil,       // 深度/ステンシル・ビューを作るテクスチャ
	        &descDSV,              // 深度/ステンシル・ビューの設定
	        &pDepthStencilView); // 作成したビューを受け取る変数
	if (FAILED(hr)) {
		DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateDepthStencilView", hr);
		return; 
	}


    // ビューポートの設定
    ViewPort[0].TopLeftX = 0.0f;    // ビューポート領域の左上X座標。
    ViewPort[0].TopLeftY = 0.0f;    // ビューポート領域の左上Y座標。
    ViewPort[0].Width    = (FLOAT)width;  // ビューポート領域の幅
    ViewPort[0].Height   = (FLOAT)height;  // ビューポート領域の高さ
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
	hr = pd3dDevice->CreateBlendState(&BlendState, &pBlendState);
	if (FAILED(hr)) {
		DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateBlendState", hr);
		return; 
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
	hr = pd3dDevice->CreateRasterizerState(&RSDesc, &pRasterizerState);
	if (FAILED(hr)) {
		DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateRasterizerState", hr);
		return; 
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
	hr = pd3dDevice->CreateDepthStencilState(&DepthStencil,
											   &pDepthStencilState);
	if (FAILED(hr)) {
		DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateDepthStencilState", hr);
		return; 
	}


#if 0
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
	                  m_pTex,				// マップするリソース
	                  0,						// サブリソースのインデックス番号
	                  D3D11_MAP_WRITE_DISCARD,	// 書き込みアクセス
	                  0,						//
	                  &resource);			// データの書き込み先ポインタ
	if (FAILED(hr)) {
		return; 
	}
	memcpy(buffer, resource.pData, width * height * sizeof(float)*4); 
	context->Unmap(m_pTex, 0);
}

