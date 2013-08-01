#include <DX11Util/Dx11EffectStandard.h>

typedef struct  {
	XMMATRIX World;		// ワールド変換行列
	XMMATRIX View;		// ビュー変換行列
	XMMATRIX Projection;	// 透視変換行列
	FLOAT      dummy;		// ダミー
} Dx11EffectStandardCBuffer;


#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }	// 解放マクロ


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
	                  const_buf,              // マップするリソース
	                  0,                       // サブリソースのインデックス番号
	                  D3D11_MAP_WRITE_DISCARD, // 書き込みアクセス
	                  0,                       //
	                  &MappedResource);        // データの書き込み先ポインタ
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitBackBuffer  g_pImmediateContext->Map", hr);  // 失敗
	}

	// データ書き込み
	CopyMemory(MappedResource.pData, &buf, sizeof(Dx11EffectStandardCBuffer));
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

	// GSにジオメトリ・シェーダを設定
	context->GSSetShader(pGeometryShader, NULL, 0);
	// GSに定数バッファを設定
	context->GSSetConstantBuffers(0, 1, &const_buf);

	// PSにピクセル・シェーダを設定
	context->PSSetShader(pPixelShader, NULL, 0);
	// PSに定数バッファを設定
	context->PSSetConstantBuffers(0, 1, &const_buf);

#if 0
	// PSにシェーダ・リソース・ビューを設定
	context->PSSetShaderResources(
        0,                // 設定する最初のスロット番号
        1,                // 設定するシェーダ・リソース・ビューの数
        &g_pTextureSRV);  // 設定するシェーダ・リソース・ビューの配列

#endif



	return S_OK; 
}

HRESULT Dx11EffectStandard::Term(Dx11Context* _context)
{
	return S_OK; 
}

