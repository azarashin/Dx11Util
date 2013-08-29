#include <DX11Util/Object/Dx11ObjectGUICanvas.h>

#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }


Dx11ObjectGUICanvas::Dx11ObjectGUICanvas(unsigned int _width, unsigned int _height)
{
	width = _width; 
	height = _height; 
}


Dx11ObjectGUICanvas::~Dx11ObjectGUICanvas(void)
{
}



void Dx11ObjectGUICanvas::Setup(Dx11Context* _context)
{
	HRESULT hr; 
	ID3D11Device* pd3dDevice = _context->GetDXDevice(); 
	tDAEVERTEX v[4] = {
		{XMFLOAT3( 0.0f,  1.0f, 0.0f) , XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0), {0, 0, 0, 0}, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0), 1.0f, 0.0f}, 
		{XMFLOAT3( 1.0f,  1.0f, 0.0f) , XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) , XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0), {0, 0, 0, 0}, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0), 0.0f, 0.0f}, 
		{XMFLOAT3( 1.0f,  0.0f, 0.0f) , XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) , XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0), {0, 0, 0, 0}, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0), 0.0f, 1.0f}, 
		{XMFLOAT3( 0.0f,  0.0f, 0.0f) , XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) , XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0), {0, 0, 0, 0}, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0), 1.0f, 1.0f}, 
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
	desc.MipLevels = 1;    // ミップマップ レベル数
	desc.ArraySize = 1;    // テクスチャの配列サイズ
	desc.Format    = DXGI_FORMAT_R8G8B8A8_UNORM; // フォーマット
	desc.SampleDesc.Count = 1; // マルチサンプリングの設定
	desc.Usage            = D3D11_USAGE_DYNAMIC;  // 動的(Dynamic)使用法
	desc.BindFlags        = D3D11_BIND_SHADER_RESOURCE; // シェーダ リソース
	desc.CPUAccessFlags   = D3D11_CPU_ACCESS_WRITE;     // CPUで書き込む
	hr = pd3dDevice->CreateTexture2D(&desc, NULL, &m_pTex);
	if (FAILED(hr)) {
		return; 
	}

	// 2Dテクスチャにアクセスするシェーダ リソース ビューの設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format        = desc.Format;		// フォーマット
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2Dテクスチャ
	srDesc.Texture2D.MostDetailedMip = 0;   // 最初のミップマップ レベル
	srDesc.Texture2D.MipLevels       = -1;  // すべてのミップマップ レベル

	// シェーダ リソース ビューの作成
	hr = pd3dDevice->CreateShaderResourceView(
				m_pTex,       // アクセスするテクスチャ リソース
				&srDesc,          // シェーダ リソース ビューの設定
				&m_pTexture);  // 受け取る変数
	if (FAILED(hr)) {
		return; 
	}
}

void Dx11ObjectGUICanvas::Map(Dx11Context* _context, D3D11_MAPPED_SUBRESOURCE* resource)
{
	ID3D11DeviceContext* context = _context->GetDXDC(); 
	HRESULT hr; 
	// リソースをマップする
	hr = context->Map(
	                  m_pTex,				// マップするリソース
	                  0,						// サブリソースのインデックス番号
	                  D3D11_MAP_WRITE_DISCARD,	// 書き込みアクセス
	                  0,						//
	                  resource);			// データの書き込み先ポインタ
	if (FAILED(hr)) {
		return; 
	}
}

void Dx11ObjectGUICanvas::Unmap(Dx11Context* _context)
{
	ID3D11DeviceContext* context = _context->GetDXDC(); 
	context->Unmap(m_pTex, 0);
}


void Dx11ObjectGUICanvas::Term()
{
	SAFE_RELEASE(m_pTexture); 
	SAFE_RELEASE(m_pIB); 
	SAFE_RELEASE(m_pVB); 
}

BOOL Dx11ObjectGUICanvas::GetBufNum(unsigned int* num)
{
	*num = 1; 
	return true; 
}

BOOL Dx11ObjectGUICanvas::GetVertexBuf(unsigned int num, ID3D11Buffer** buf)
{
	*buf = m_pVB; 
	return true; 
}

BOOL Dx11ObjectGUICanvas::GetWeightBuf(unsigned int num, ID3D11Buffer** buf)
{
	*buf = 0; 
	return false; 
}

BOOL Dx11ObjectGUICanvas::GetIndexBuf(unsigned int num, ID3D11Buffer** buf)
{
	*buf = m_pIB; 
	return true; 
}

BOOL Dx11ObjectGUICanvas::GetTexture(unsigned int num, ID3D11ShaderResourceView** pTexture)
{
	*pTexture = m_pTexture; 
	return true; 
}

BOOL Dx11ObjectGUICanvas::GetVertexNum(unsigned int idx, unsigned int* num)
{
	*num = 4; 
	return true; 
}

BOOL Dx11ObjectGUICanvas::GetIndexNum(unsigned int idx, unsigned int* num)
{
	*num = 6; 
	return true; 
}

BOOL Dx11ObjectGUICanvas::GetTextureNum(unsigned int* num)
{
	*num = 1; 
	return true; 
}
