#include <DX11Util/Object/Dx11ObjectViewAround.h>
#include <DX11Util/Util/DDSTextureLoader.h>

#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }


Dx11ObjectViewAround::Dx11ObjectViewAround(const char* _front, const char* _back, const char* _top, const char* _bottom, const char* _left, const char* _right)
{
	top = _top; 
	bottom = _bottom; 
	right = _right; 
	left = _left; 
	front = _front; 
	back = _back; 
}


Dx11ObjectViewAround::~Dx11ObjectViewAround(void)
{
}



void Dx11ObjectViewAround::Setup(Dx11Context* _context)
{
	HRESULT hr; 
	ID3D11Device* pd3dDevice = _context->GetDXDevice(); 
	tDAEVERTEX v[4] = {
		{XMFLOAT3( 0.0f,  1.0f, 0.0f) , XMFLOAT3(0.0f, 0.0f, -1.0f) , XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0), {0, 0, 0, 0}, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0), 0.0f, 1.0f}, 
		{XMFLOAT3( 1.0f,  1.0f, 0.0f) , XMFLOAT3(0.0f, 0.0f, -1.0f) , XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0), {0, 0, 0, 0}, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0), 1.0f, 1.0f}, 
		{XMFLOAT3( 1.0f,  0.0f, 0.0f) , XMFLOAT3(0.0f, 0.0f, -1.0f) , XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0), {0, 0, 0, 0}, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0), 1.0f, 0.0f}, 
		{XMFLOAT3( 0.0f,  0.0f, 0.0f) , XMFLOAT3(0.0f, 0.0f, -1.0f) , XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0), {0, 0, 0, 0}, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0), 0.0f, 0.0f}, 
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

	WCHAR filepath[2048];

	MultiByteToWideChar(CP_UTF8, 0, front.c_str(), -1, filepath, (int)sizeof(filepath));
	hr = CreateDDSTextureFromFile( pd3dDevice, filepath, NULL, &m_pTexture[0] ); 
	if (FAILED(hr)) {
		DXTRACE_ERR(L"D3DX11CreateShaderResourceViewFromFile", hr);
		return; 
	}

	MultiByteToWideChar(CP_UTF8, 0, back.c_str(), -1, filepath, (int)sizeof(filepath));
	hr = CreateDDSTextureFromFile( pd3dDevice, filepath, NULL, &m_pTexture[1] ); 
	if (FAILED(hr)) {
		DXTRACE_ERR(L"D3DX11CreateShaderResourceViewFromFile", hr);
		return; 
	}

	MultiByteToWideChar(CP_UTF8, 0, top.c_str(), -1, filepath, (int)sizeof(filepath));
	hr = CreateDDSTextureFromFile( pd3dDevice, filepath, NULL, &m_pTexture[2] ); 
	if (FAILED(hr)) {
		DXTRACE_ERR(L"D3DX11CreateShaderResourceViewFromFile", hr);
		return; 
	}

	MultiByteToWideChar(CP_UTF8, 0, bottom.c_str(), -1, filepath, (int)sizeof(filepath));
	hr = CreateDDSTextureFromFile( pd3dDevice, filepath, NULL, &m_pTexture[3] ); 
	if (FAILED(hr)) {
		DXTRACE_ERR(L"D3DX11CreateShaderResourceViewFromFile", hr);
		return; 
	}

	MultiByteToWideChar(CP_UTF8, 0, left.c_str(), -1, filepath, (int)sizeof(filepath));
	hr = CreateDDSTextureFromFile( pd3dDevice, filepath, NULL, &m_pTexture[4] ); 
	if (FAILED(hr)) {
		DXTRACE_ERR(L"D3DX11CreateShaderResourceViewFromFile", hr);
		return; 
	}

	MultiByteToWideChar(CP_UTF8, 0, right.c_str(), -1, filepath, (int)sizeof(filepath));
	hr = CreateDDSTextureFromFile( pd3dDevice, filepath, NULL, &m_pTexture[5] ); 
	if (FAILED(hr)) {
		DXTRACE_ERR(L"D3DX11CreateShaderResourceViewFromFile", hr);
		return; 
	}
}

void Dx11ObjectViewAround::Term()
{
	int i; 
	for(i=0;i<6;i++) {
		SAFE_RELEASE(m_pTexture[i]); 
	}
	SAFE_RELEASE(m_pIB); 
	SAFE_RELEASE(m_pVB); 
}

BOOL Dx11ObjectViewAround::GetBufNum(unsigned int* num)
{
	*num = 1; 
	return true; 
}

BOOL Dx11ObjectViewAround::GetVertexBuf(unsigned int num, ID3D11Buffer** buf)
{
	*buf = m_pVB; 
	return true; 
}

BOOL Dx11ObjectViewAround::GetWeightBuf(unsigned int num, ID3D11Buffer** buf)
{
	*buf = 0; 
	return false; 
}

BOOL Dx11ObjectViewAround::GetIndexBuf(unsigned int num, ID3D11Buffer** buf)
{
	*buf = m_pIB; 
	return true; 
}

BOOL Dx11ObjectViewAround::GetTexture(unsigned int num, ID3D11ShaderResourceView** pTexture)
{
	if(num < 6) {
		*pTexture = m_pTexture[num]; 
	} else {
		*pTexture = 0; 
	}
	return true; 
}

BOOL Dx11ObjectViewAround::GetVertexNum(unsigned int idx, unsigned int* num)
{
	*num = 4; 
	return true; 
}

BOOL Dx11ObjectViewAround::GetIndexNum(unsigned int idx, unsigned int* num)
{
	*num = 6; 
	return true; 
}


BOOL Dx11ObjectViewAround::GetTextureNum(unsigned int* num)
{
	*num = 1; 
	return true; 
}
