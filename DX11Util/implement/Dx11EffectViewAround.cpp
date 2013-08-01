#include <DX11Util/Dx11EffectViewAround.h>


#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }	// 解放マクロ

Dx11EffectViewAround::Dx11EffectViewAround(void)
{
}


Dx11EffectViewAround::~Dx11EffectViewAround(void)
{
}



HRESULT Dx11EffectViewAround::Setup(Dx11Context* _context)
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
			L"shader\\commet.sh",  // file name
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
		SAFE_RELEASE(pBlobVS);
		return DXTRACE_ERR(L"InitDirect3D pd3dDevice->CreateVertexShader", hr);
	}

	// **********************************************************
	// Layout
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0}, 
		{"TEXTURE",    0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3) + sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	hr = pd3dDevice->CreateInputLayout(
			layout,                            // 定義の配列
			_countof(layout),                  // 定義の要素数
			pBlobVS->GetBufferPointer(),       // バイト・コードへのポインタ
			pBlobVS->GetBufferSize(),          // バイト・コードのサイズ
			&pInputLayout);                  // 受け取る変数のポインタ

	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateInputLayout", hr);
	}
	SAFE_RELEASE(pBlobVS);


	// **********************************************************
	// compile pixel shader
	ID3DBlob* pBlobPS = NULL;
	hr = D3DX11CompileFromFile(
			L"shader\\commet.sh",  // file name
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
	cBufferDesc.ByteWidth      = sizeof(EffectCommetMatInfo);
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
	BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_ONE; 
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
	// 深度/ステンシル・ステート・オブジェクトの作成
	D3D11_DEPTH_STENCIL_DESC DepthStencil;
	DepthStencil.DepthEnable      = FALSE; // 深度テスト無し
	DepthStencil.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ZERO; // 書き込まない
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
		return DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateDepthStencilState", hr);
	}

	return S_OK; 
}

HRESULT Dx11EffectViewAround::Update(Dx11Context* _context, Dx11ObjectCommet* obj, EffectCameraInfo* param)
{
	HRESULT hr; 
	int num; 
	unsigned int i, max; 
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	EffectCommetMatInfo act_param; 
	ID3D11DeviceContext* context = _context->GetDXDC(); 

	act_param.alpha = param->alpha; 

	FLOAT oldBlendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	FLOAT BlendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	ID3D11BlendState*         old_pBlendState;			// ブレンド・ステート・オブジェクト
	ID3D11DepthStencilState*  old_pDepthStencilState;	// 深度/ステンシル・ステート・オブジェクト

	XMMATRIX mat_scale = XMMatrixScaling(param->size, param->size, param->size); 

	XMVECTORF32 eyePosition_i   = { param->cx, param->cy,  param->cz, 1.0f };  // 視点(カメラの位置)
	XMVECTORF32 focusPosition_i = { param->tx, param->ty,  param->tz, 1.0f };  // 注視点
	XMVECTORF32 upDirection_i   = { 0.0f, 1.0f,  0.0f, 1.0f };  // カメラの上方向
	XMMATRIX mat_view_i = XMMatrixLookAtLH(eyePosition_i, focusPosition_i, upDirection_i);
	XMVECTOR  det; 
	mat_view_i = XMMatrixInverse(&det, mat_view_i); 

	XMMATRIX trans = XMMatrixTranslation(param->x, param->y, param->z); 

	// ビュー変換行列
	XMVECTORF32 eyePosition   = { 0.0f, 0.0f,  0.0f, 1.0f };  // 視点(カメラの位置)
	XMVECTORF32 focusPosition = { param->tx, param->ty,  param->tz, 1.0f };  // 注視点
	XMVECTORF32 upDirection   = { 0.0f, 1.0f,  0.0f, 1.0f };  // カメラの上方向
	XMMATRIX mat_view = XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);

	// 射影変換行列(パースペクティブ(透視法)射影)
	XMMATRIX mat_proj = XMMatrixPerspectiveFovLH(
			XMConvertToRadians(60.0f),		// 視野角60°
			1920.0f / 1080.0f,	// アスペクト比
			5.0f,							// 前方投影面までの距離(単位は万キロメートル)
			200.0f);							// 後方投影面までの距離(単位は万キロメートル)
	XMMATRIX mat; 

	mat = XMMatrixMultiply(mat_scale, mat_view_i); 
	mat = XMMatrixMultiply(mat, trans); 
	mat = XMMatrixMultiply(mat, mat_view); 
	mat = XMMatrixMultiply(mat, mat_proj); 
	mat = XMMatrixTranspose(mat);
	XMStoreFloat4x4(&act_param.mat, mat);


	UINT old_mask, old_stencilref; 
	// ブレンド・ステート・オブジェクトを退避
	context->OMGetBlendState(&old_pBlendState, oldBlendFactor, &old_mask);
	// 深度/ステンシル・ステート・オブジェクトを退避
	context->OMGetDepthStencilState(&old_pDepthStencilState, &old_stencilref);

	// OMにブレンド・ステート・オブジェクトを設定
	context->OMSetBlendState(pBlendState, BlendFactor, 0xffffffff);
	// OMに深度/ステンシル・ステート・オブジェクトを設定
	context->OMSetDepthStencilState(pDepthStencilState, 0);


	hr = context->Map(
	                  const_buf,              // マップするリソース
	                  0,                       // サブリソースのインデックス番号
	                  D3D11_MAP_WRITE_DISCARD, // 書き込みアクセス
	                  0,                       //
	                  &MappedResource);        // データの書き込み先ポインタ
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitBackBuffer  g_pImmediateContext->Map", hr);  // 失敗
	}

	// データ書き込み
	CopyMemory(MappedResource.pData, &act_param, sizeof(EffectCommetMatInfo));
	// マップ解除
	context->Unmap(const_buf, 0);


	// VSに頂点シェーダを設定
	context->VSSetShader(pVertexShader, NULL, 0);
	// VSに定数バッファを設定
	context->VSSetConstantBuffers(0, 1, &const_buf);

	// GSにジオメトリ・シェーダを設定
	context->GSSetShader(NULL, NULL, 0);
	// GSに定数バッファを設定
	context->GSSetConstantBuffers(0, 1, &const_buf);

	// PSにピクセル・シェーダを設定
	context->PSSetShader(pPixelShader, NULL, 0);
	// PSに定数バッファを設定
	context->PSSetConstantBuffers(0, 1, &const_buf);

	obj->GetBufNum(&max); 

	for(int i=0;i<max;i++)
	{
		unsigned int vn, in; 
		ID3D11Buffer* vb; 
		ID3D11Buffer* ib; 
		ID3D11Buffer* uv; 

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

		// PSにシェーダ・リソース・ビューを設定
		ID3D11ShaderResourceView* srv[1]; 
		obj->GetTexture(i, &(srv[0])); 
		context->PSSetShaderResources(
	        0,              // 設定する最初のスロット番号
	        1,              // 設定するシェーダ・リソース・ビューの数
	        srv);			// 設定するシェーダ・リソース・ビューの配列

		context->DrawIndexed( in, 0, 0); 

//		pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVertices[i], 0, m_dwIndecies[i]/3);

//		pd3dDevice->SetTexture(0, NULL);
	}

	// ブレンド・ステート・オブジェクトを復元
	context->OMSetBlendState(old_pBlendState, oldBlendFactor, old_mask);
	// 深度/ステンシル・ステート・オブジェクトを復元
	context->OMSetDepthStencilState(old_pDepthStencilState, old_stencilref);
	return S_OK; 
}

HRESULT Dx11EffectViewAround::Term(Dx11Context* _context)
{
	SAFE_RELEASE(pBlendState); 
	SAFE_RELEASE(pDepthStencilState); 
	return S_OK; 
}

