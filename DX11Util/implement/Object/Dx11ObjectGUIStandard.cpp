#include <DX11Util/Object/Dx11ObjectGUIStandard.h>
#include "DDSTextureLoader.h"

#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }


Dx11ObjectGUIStandard::Dx11ObjectGUIStandard(const char* _filename)
{
	filename = _filename; 
}


Dx11ObjectGUIStandard::~Dx11ObjectGUIStandard(void)
{
}



void Dx11ObjectGUIStandard::Setup(Dx11Context* _context)
{
	HRESULT hr; 
	ID3D11Device* pd3dDevice = _context->GetDXDevice(); 
	tDAEVERTEX v[4] = {
		{XMFLOAT3( 0.0f,  1.0f, 0.0f) , XMFLOAT3(0.0f, 0.0f, 1.0f) , XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0), {0, 0, 0, 0}, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0), 1.0f, 0.0f}, 
		{XMFLOAT3( 1.0f,  1.0f, 0.0f) , XMFLOAT3(0.0f, 0.0f, 1.0f) , XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0), {0, 0, 0, 0}, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0), 0.0f, 0.0f}, 
		{XMFLOAT3( 1.0f,  0.0f, 0.0f) , XMFLOAT3(0.0f, 0.0f, 1.0f) , XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0), {0, 0, 0, 0}, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0), 0.0f, 1.0f}, 
		{XMFLOAT3( 0.0f,  0.0f, 0.0f) , XMFLOAT3(0.0f, 0.0f, 1.0f) , XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0), {0, 0, 0, 0}, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0), 1.0f, 1.0f}, 
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
	MultiByteToWideChar(CP_UTF8, 0, filename.c_str(), -1, filepath, (int)sizeof(filepath));
	hr = CreateDDSTextureFromFile( pd3dDevice, filepath, NULL, &m_pTexture ); 
	if (FAILED(hr)) {
		DXTRACE_ERR(L"D3DX11CreateShaderResourceViewFromFile", hr);
		return; 
	}

}

void Dx11ObjectGUIStandard::Term()
{
	SAFE_RELEASE(m_pTexture); 
	SAFE_RELEASE(m_pIB); 
	SAFE_RELEASE(m_pVB); 
}

BOOL Dx11ObjectGUIStandard::GetBufNum(unsigned int* num)
{
	*num = 1; 
	return true; 
}

BOOL Dx11ObjectGUIStandard::GetVertexBuf(unsigned int num, ID3D11Buffer** buf)
{
	*buf = m_pVB; 
	return true; 
}

BOOL Dx11ObjectGUIStandard::GetWeightBuf(unsigned int num, ID3D11Buffer** buf)
{
	*buf = 0; 
	return false; 
}

BOOL Dx11ObjectGUIStandard::GetIndexBuf(unsigned int num, ID3D11Buffer** buf)
{
	*buf = m_pIB; 
	return true; 
}

BOOL Dx11ObjectGUIStandard::GetTexture(unsigned int num, ID3D11ShaderResourceView** pTexture)
{
	*pTexture = m_pTexture; 
	return true; 
}

BOOL Dx11ObjectGUIStandard::GetVertexNum(unsigned int idx, unsigned int* num)
{
	*num = 4; 
	return true; 
}

BOOL Dx11ObjectGUIStandard::GetIndexNum(unsigned int idx, unsigned int* num)
{
	*num = 6; 
	return true; 
}

BOOL Dx11ObjectGUIStandard::GetTextureNum(unsigned int* num)
{
	*num = 1; 
	return true; 
}
