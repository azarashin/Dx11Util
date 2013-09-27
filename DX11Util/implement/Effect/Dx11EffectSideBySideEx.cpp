#include <DX11Util/Effect/Dx11EffectSideBySideEx.h>
#include <DX11Util/Object/Dx11ObjectTargetTexture.h>
#include <d3dcompiler.h>
#include <DX11Util/Util/ShaderCompilerWrapper.h>


#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }	// 解放マクロ

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

	pTextureSamplerWrap = new ID3D11SamplerState*[mipmap_max]; 
	pOldTextureSamplerWrap = new ID3D11SamplerState*[mipmap_max]; 
	for(i=0;i<mipmap_max;i++) {
		pOldTextureSamplerWrap[i] = 0; 

		// サンプラーの作成
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

	// 左右それぞれレンダリング
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
	ID3D11BlendState*         old_pBlendState;			// ブレンド・ステート・オブジェクト
	ID3D11DepthStencilState*  old_pDepthStencilState;	// 深度/ステンシル・ステート・オブジェクト

	UINT old_mask, old_stencilref; 
	// ブレンド・ステート・オブジェクトを退避
	context->OMGetBlendState(&old_pBlendState, oldBlendFactor, &old_mask);
	// 深度/ステンシル・ステート・オブジェクトを退避
	context->OMGetDepthStencilState(&old_pDepthStencilState, &old_stencilref);

	// OMにブレンド・ステート・オブジェクトを設定
	context->OMSetBlendState(pBlendState, BlendFactor, 0xffffffff);
	// OMに深度/ステンシル・ステート・オブジェクトを設定
	context->OMSetDepthStencilState(pDepthStencilState, 0);


#if 1
	// left

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
	param->left = -0.5f; 
	CopyMemory(MappedResource.pData, param, sizeof(EffectSideBySideExInfo ));
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

		// PSにシェーダ・リソース・ビューを設定
		ID3D11ShaderResourceView* srv[1]; 
		left_buf[j]->GetTexture(i, &(srv[0])); 
		context->PSSetShaderResources(
	        0,              // 設定する最初のスロット番号
	        1,              // 設定するシェーダ・リソース・ビューの数
	        srv);			// 設定するシェーダ・リソース・ビューの配列

		context->DrawIndexed( in, 0, 0); 

//		pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVertices[i], 0, m_dwIndecies[i]/3);

//		pd3dDevice->SetTexture(0, NULL);
	}

#endif

	// right
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
	param->left = 0.5f; 
	CopyMemory(MappedResource.pData, param, sizeof(EffectSideBySideExInfo ));
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

		// PSにシェーダ・リソース・ビューを設定
		ID3D11ShaderResourceView* srv[1]; 
		right_buf[j]->GetTexture(i, &(srv[0])); 
		context->PSSetShaderResources(
	        0,              // 設定する最初のスロット番号
	        1,              // 設定するシェーダ・リソース・ビューの数
	        srv);			// 設定するシェーダ・リソース・ビューの配列

		context->DrawIndexed( in, 0, 0); 

//		pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVertices[i], 0, m_dwIndecies[i]/3);

//		pd3dDevice->SetTexture(0, NULL);
	}

	context->PSSetSamplers(0, 1/*mipmap_max*/, pOldTextureSamplerWrap);

	// ブレンド・ステート・オブジェクトを復元
	context->OMSetBlendState(old_pBlendState, oldBlendFactor, old_mask);
	// 深度/ステンシル・ステート・オブジェクトを復元
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

