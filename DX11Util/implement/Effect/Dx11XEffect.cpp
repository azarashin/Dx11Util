#include <DX11Util/Effect/Dx11XEffect.h>


typedef struct  { // Warning: XMMATRIX ではなくXMFLOAT4X4 を使うように修正すること
	XMMATRIX World;		// ワールド変換行列
	XMMATRIX View;		// ビュー変換行列
	XMMATRIX Projection;	// 透視変換行列
} Dx11XEffectCBuffer;

typedef struct {
	XMFLOAT4X4 bind_pose; 
	FLOAT diffuse[4]; // r, g, b, a
	FLOAT specular[3]; // r, g, b
	FLOAT emmisive[3]; // r, g, b
	FLOAT power; 
	UINT tex_enable; 
} Dx11XMaterial; 

#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }	// 解放マクロ


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
	// 深度/ステンシル・ステート・オブジェクトの作成
	D3D11_DEPTH_STENCIL_DESC DepthStencil;
	DepthStencil.DepthEnable      = TRUE; 
	DepthStencil.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ALL; 
	DepthStencil.DepthFunc        = D3D11_COMPARISON_ALWAYS; // 手前の物体を描画
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


	cam->GetMatrix(&(buf.View)); 

	lens->GetMatrix(&(buf.Projection)); 

	mot->GetMatrix(&(buf.World)); 


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
	CopyMemory(MappedResource.pData, &buf, sizeof(Dx11XEffectCBuffer));
	// マップ解除
	context->Unmap(const_buf, 0);

	// ***************************************
	// IAに頂点バッファを設定
	// IAに入力レイアウト・オブジェクトを設定(頂点バッファなし)
	context->IASetInputLayout(NULL);
	// IAにプリミティブの種類を設定
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VSに頂点シェーダを設定
	context->VSSetShader(pVertexShader, NULL, 0);
	// VSに定数バッファを設定
	context->VSSetConstantBuffers(0, 1, &const_buf);
	context->VSSetConstantBuffers(2, 1, &const_buf4part);

	// PSにピクセル・シェーダを設定
	context->PSSetShader(pPixelShader, NULL, 0);
	// PSに定数バッファを設定
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
					0,              // 設定する最初のスロット番号
					1,              // 設定するシェーダ・リソース・ビューの数
					srv);			// 設定するシェーダ・リソース・ビューの配列
			}

			hr = context->Map(
							  const_buf4part,              // マップするリソース
							  0,                       // サブリソースのインデックス番号
							  D3D11_MAP_WRITE_DISCARD, // 書き込みアクセス
							  0,                       //
							  &MappedResource);        // データの書き込み先ポインタ
			if (FAILED(hr)) {
				return DXTRACE_ERR(L"InitBackBuffer  g_pImmediateContext->Map", hr);  // 失敗
			}

			// データ書き込み
			CopyMemory(MappedResource.pData, &buf_part, sizeof(Dx11XMaterial));
			// マップ解除
			context->Unmap(const_buf4part, 0);



//			context->DrawIndexed( in, 0, 0); 
			context->DrawIndexed(submesh.size*3, submesh.start_index*3, 0); 
		}

	}

	// ブレンド・ステート・オブジェクトを復元
	context->OMSetBlendState(old_pBlendState, oldBlendFactor, old_mask);
	// 深度/ステンシル・ステート・オブジェクトを復元
	context->OMSetDepthStencilState(old_pDepthStencilState, old_stencilref);
	return S_OK; 
}

HRESULT Dx11XEffect::Term(Dx11Context* context)
{
	SAFE_RELEASE(pBlendState); 
	SAFE_RELEASE(pDepthStencilState); 
	return S_OK; 
}
