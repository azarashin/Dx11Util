//------------------------------------------------------------------------------------
// File : asdxModelUtil.cpp
// Desc : Asura Model Utility for DirectX 11
// Copyright(c) Project Asura. All right reserved.
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------
#include "asdxModelUtil.h"
#include <d3dcompiler.h>
#include <d3dx11.h>


//------------------------------------------------------------------------------------
// Using Statements
//------------------------------------------------------------------------------------
using namespace Asura;
using namespace Asura::Math;
using namespace Asura::Graphics;


//------------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------------
#ifndef AS_ALIGN16
#define AS_ALIGN16 __declspec( align( 16 ) )
#endif

namespace Asura {
namespace Util {

//////////////////////////////////////////////////////////////////////////////////////
// MeshVertex structure
//////////////////////////////////////////////////////////////////////////////////////
typedef struct _MeshVertex
{
    Asura::Math::Vector3      Position;
    Asura::Math::Vector3      Normal;
    Asura::Math::Vector2      TexCoord;
    Asura::Graphics::Short4   SkinIndex;
    Asura::Math::Vector4      SkinWeight;
} MeshVertex;


//////////////////////////////////////////////////////////////////////////////////////
// MeshMaterial structure
//////////////////////////////////////////////////////////////////////////////////////
typedef struct AS_ALIGN16 _MeshMaterial
{
    Asura::Math::Vector4      Diffuse;
    Asura::Math::Vector4      Specular;
    Asura::Math::Vector3      Emissive;
} MeshMaterial;


/////////////////////////////////////////////////////////////////////////////////////
// MaterialBuffer structure
/////////////////////////////////////////////////////////////////////////////////////
typedef struct AS_ALIGN16 _MatrixBuffer
{
    D3DXMATRIX World;
    D3DXMATRIX View;
    D3DXMATRIX Projection;
} MatrixBuffer;


/////////////////////////////////////////////////////////////////////////////////////
// VectorBuffer structure
/////////////////////////////////////////////////////////////////////////////////////
typedef struct AS_ALIGN16 _VectorBuffer
{
    D3DXVECTOR3 LightDir;
    D3DXVECTOR3 ViewDir;
} VectorBuffer;


////////////////////////////////////////////////////////////////////////////////////
// ModelUnitGlobal structure
////////////////////////////////////////////////////////////////////////////////////
struct ModelUtilGlobal
{
    ID3D11InputLayout*  pInputLayout;   // MeshInputLayout
    ID3D11VertexShader* pVS;            // Default Vertex Shader
    ID3D11PixelShader*  pPS;            // Default Pixel Shader
    ID3D11Buffer*       pMatrixBuffer;  // Matrix Buffer ( Constant Buffer Slot 0 )
    ID3D11Buffer*       pVectorBuffer;  // Vector Buffer ( Constant Buffer Slot 2 )
} gGlobal;


////////////////////////////////////////////////////////////////////////////////////
// ModelUtil class
////////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------
//! @brief 初期化処理
//----------------------------------------------------------------------------------
bool ModelUtil::Initialize( ID3D11Device* pDevice )
{
    HRESULT hr = S_OK;

	memset( &gGlobal, 0, sizeof( ModelUtilGlobal ) );
  
    const char *pShaderCode = 
    "struct VSInput\n"
    "{\n"
    "    float3 Position   : POSITION;\n"
    "    float3 Normal     : NORMAL;\n"
    "    float2 TexCoord   : TEXCOORD;\n"
    "    uint4  SkinIndex  : SKIN_INDEX;\n"
    "    float4 SkinWeight : SKIN_WEIGHT;\n"
    "};\n"
    "struct VSOutput\n"
    "{\n"
    "    float4 Position : SV_POSITION;\n"
    "    float2 TexCoord : TEXCOORD0;\n"
    "    float3 Normal   : TEXCOORD1;\n"
    "};\n"
    "cbuffer VSMatrix : register( b0 )\n"
    "{\n"
    "    float4x4 World  : packoffset( c0 );\n"
    "    float4x4 View   : packoffset( c4 );\n"
    "    float4x4 Proj   : packoffset( c8 );\n"
    "};\n"
    "cbuffer PSMaterial : register( b1 )\n"
    "{\n"
    "    float3 Diffuse  : packoffset( c0 );\n"
    "    float  Alpha    : packoffset( c0.w );\n"
    "    float3 Specular : packoffset( c1 );\n"
	"    float  Power    : packoffset( c1.w );\n"
    "    float3 Emissive : packoffset( c2 );\n"
    "};\n"
    "cbuffer PSVector : register( b2 )\n"
    "{\n"
    "    float3 LightDir : packoffset( c0 );\n"
    "    float3 ViewDir  : packoffset( c1 );\n"
    "};\n"
    "VSOutput VSFunc( VSInput input )\n"
    "{\n"
    "    VSOutput output = (VSOutput)0;\n"
    "    float4 localPos = float4( input.Position, 1.0f );\n"
    "    float4 worldPos = mul( World, localPos );\n"
    "    float4 viewPos  = mul( View, worldPos );\n"
    "    float4 projPos  = mul( Proj, viewPos );\n"
    "    output.Position = projPos;\n"
    "    output.TexCoord = input.TexCoord;\n"
    "    output.Normal   = input.Normal;\n"
    "    return output;"
    "}\n"
    "float4 PSFunc( VSOutput input ) : SV_TARGET\n"
    "{\n"
    "    float3 L = normalize( LightDir );\n"
    "    float3 N = normalize( input.Normal );\n"
    "    float3 E = normalize( ViewDir );\n"
    "    float3 H = normalize( L + E );\n"
    "    float3 diffuse  = Diffuse  * 0.85f * max( dot( L, N ), 0 );\n"
    "    float3 specular = Specular * 0.75f * pow( max( dot( N, H ), 0 ), Power );\n"
	"    float3 emissive = Emissive * 0.25f;\n"
    "    float4 output   = float4( ( diffuse + specular + emissive ), Alpha );\n"
    "    return output;\n"
    "}\n";


	// フラグの設定
    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob = NULL;
    ID3DBlob* pVSBlob = NULL;
	ID3D11VertexShader* pVS = NULL;

    // 頂点シェーダのコードをコンパイル
    hr = D3DX11CompileFromMemory(
        pShaderCode,
        strlen( pShaderCode ),
        NULL,
        NULL,
        NULL,
        "VSFunc",
        "vs_4_0",
        dwShaderFlags,
        0,
        NULL,
        &pVSBlob,
        &pErrorBlob,
        NULL );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : D3DX11CompileFromMeory() Failed. retcode = %d", hr );
        if ( pErrorBlob != NULL )
        {
            ELOG( "Error : %s", (char*)pErrorBlob->GetBufferPointer() );
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        }
        SAFE_RELEASE( pErrorBlob );
        return false;
    }
    SAFE_RELEASE( pErrorBlob );

    // 頂点シェーダ生成
    hr = pDevice->CreateVertexShader( 
        pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(),
        NULL,
        &pVS );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateVertexShader() Failed." );
        SAFE_RELEASE( pVSBlob );
        return false;
    }
	assert( pVS != NULL );


    ID3DBlob* pPSBlob = NULL;
	ID3D11PixelShader* pPS = NULL;

    // ピクセルシェーダのコードをコンパイル
    hr = D3DX11CompileFromMemory(
        pShaderCode,
        strlen( pShaderCode ),
        NULL,
        NULL,
        NULL,
        "PSFunc",
        "ps_4_0",
        dwShaderFlags,
        0,
        NULL,
        &pPSBlob,
        &pErrorBlob,
        NULL );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : D3DX11CompileFromMeory() Failed. retcode = %d", hr );
        if ( pErrorBlob != NULL )
        {
            ELOG( "Error : %s", (char*)pErrorBlob->GetBufferPointer() );
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        }
        SAFE_RELEASE( pVSBlob );
        SAFE_RELEASE( pErrorBlob );
		SAFE_RELEASE( pVS );
        return false;
    }
    SAFE_RELEASE( pErrorBlob );

    // ピクセルシェーダ生成
     hr = pDevice->CreatePixelShader( 
        pPSBlob->GetBufferPointer(),
        pPSBlob->GetBufferSize(),
        NULL,
        &pPS );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreatePixelShader() Failed." );
        SAFE_RELEASE( pVSBlob );
        SAFE_RELEASE( pPSBlob );
		SAFE_RELEASE( pVS );
        return false;
    }
	assert( pPS != NULL );

	gGlobal.pVS = pVS;
	gGlobal.pPS = pPS;

    // メッシュ用入力レイアウトの生成
    D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SKIN_INDEX",  0, DXGI_FORMAT_R16G16B16A16_UINT,  0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SKIN_WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	uint32_t numElements = sizeof( layout ) / sizeof( layout[ 0 ] );

    // 入力レイアウト生成
    hr = pDevice->CreateInputLayout(
        layout,
        numElements,
        pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(),
        &gGlobal.pInputLayout );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateInputLayout() Failed." );
        SAFE_RELEASE( pVSBlob );
        SAFE_RELEASE( pPSBlob );
        return false;
    }

	// メモリ解放
    SAFE_RELEASE( pVSBlob );
    SAFE_RELEASE( pPSBlob );

	// 行列バッファの設定
    D3D11_BUFFER_DESC bufDesc;
    memset( &bufDesc, 0, sizeof( bufDesc ) );
    bufDesc.Usage = D3D11_USAGE_DEFAULT;
    bufDesc.ByteWidth = sizeof( MatrixBuffer );
    bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufDesc.CPUAccessFlags = 0;

	// 行列バッファの生成
    hr = pDevice->CreateBuffer( &bufDesc, NULL, &gGlobal.pMatrixBuffer );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateBuffer() Failed." );
        ELOG( "Error : Constant Buffer for Matrix Create Failed" );
        /* NOT_RETURN */
    }

	// ベクトルバッファの設定
    memset( &bufDesc, 0, sizeof( bufDesc ) );
    bufDesc.Usage = D3D11_USAGE_DEFAULT;
    bufDesc.ByteWidth = sizeof( VectorBuffer );
    bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufDesc.CPUAccessFlags = 0;

	// ベクトルバッファの生成
    hr = pDevice->CreateBuffer( &bufDesc, NULL, &gGlobal.pVectorBuffer );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateBuffer() Failed." );
        ELOG( "Error : Constant Buffer for Vector Create Failed" );
        /* NOT_RETURN */
    }

    return true;
}

//---------------------------------------------------------------------------------
//! @brief メモリ解放処理
//---------------------------------------------------------------------------------
bool ModelUtil::Release()
{
	// メモリ解放
    SAFE_RELEASE( gGlobal.pVS );
    SAFE_RELEASE( gGlobal.pPS );
    SAFE_RELEASE( gGlobal.pInputLayout );
    SAFE_RELEASE( gGlobal.pMatrixBuffer );
    SAFE_RELEASE( gGlobal.pVectorBuffer );
    return true;
}

//----------------------------------------------------------------------------------
//! @brief デフォルトの頂点シェーダを取得する
//----------------------------------------------------------------------------------
ID3D11VertexShader* ModelUtil::GetDefaultVertexShader()
{ return gGlobal.pVS; }

//----------------------------------------------------------------------------------
//! @brief デフォルトのピクセルシェーダを取得する
//----------------------------------------------------------------------------------
ID3D11PixelShader* ModelUtil::GetDefaultPixelShader()
{ return gGlobal.pPS; }

//----------------------------------------------------------------------------------
//! @brief デフォルトの入力レイアウトを取得する
//----------------------------------------------------------------------------------
ID3D11InputLayout* ModelUtil::GetDefaultInputLayout()
{ return gGlobal.pInputLayout; }

//----------------------------------------------------------------------------------
//! @brief 定数バッファに行列を設定する
//----------------------------------------------------------------------------------
void ModelUtil::SetMatrix
(
    ID3D11DeviceContext* pImmediateContext,
    const D3DXMATRIX &world,
    const D3DXMATRIX &view,
    const D3DXMATRIX &proj
)
{
    MatrixBuffer buffer;
	memset( &buffer, 0, sizeof( MatrixBuffer ) );
    buffer.World = world;
    buffer.View = view;
    buffer.Projection = proj;

	// slot:0の定数バッファに行列を設定
    pImmediateContext->UpdateSubresource( gGlobal.pMatrixBuffer, 0, NULL, &buffer, 0, 0 );
    pImmediateContext->VSSetConstantBuffers( 0, 1, &gGlobal.pMatrixBuffer );
}

//----------------------------------------------------------------------------------
//! @brief 定数バッファにベクトルを設定
//----------------------------------------------------------------------------------
void ModelUtil::SetVector
(
    ID3D11DeviceContext* pImmediateContext,
    const D3DXVECTOR3 &lightDir,
    const D3DXVECTOR3 &viewDir
)
{
    VectorBuffer buffer;
	memset( &buffer, 0, sizeof( VectorBuffer ) );
    buffer.LightDir = lightDir;
    buffer.ViewDir = viewDir;

	// slot:2の定数バッファにベクトルを設定
    pImmediateContext->UpdateSubresource( gGlobal.pVectorBuffer, 0, NULL, &buffer, 0, 0 );
    pImmediateContext->PSSetConstantBuffers( 2, 1, &gGlobal.pVectorBuffer );
}


//////////////////////////////////////////////////////////////////////////////////////
// MeshData class
//////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------
//! @brief 初期値設定
//------------------------------------------------------------------------------------
MeshData::MeshData()
{
    mSubsets.clear();
    mpVB = null;
    mpMB = null;
	mpIB = null;
}

//-----------------------------------------------------------------------------------
//! @brief 破棄処理
//-----------------------------------------------------------------------------------
MeshData::~MeshData()
{
    Release();
}

//-----------------------------------------------------------------------------------
//! @brief メモリ解放処理
//-----------------------------------------------------------------------------------
void MeshData::Release()
{
    for( uint32_t i=0; i<mSubsets.size(); ++i )
    {
		SAFE_RELEASE( mSubsets[ i ].pSRV );
        memset( &mSubsets[ i ], 0, sizeof( SubsetData ) );
    }
    mSubsets.clear();

    SAFE_RELEASE( mpMB );
    SAFE_RELEASE( mpVB );
	SAFE_RELEASE( mpIB );
}

//-----------------------------------------------------------------------------------
//! @brief バッファを生成する
//-----------------------------------------------------------------------------------
bool MeshData::CreateBuffer( ID3D11Device* pDevice, IMesh* pMesh )
{
    // 頂点データが無い場合は即終了
    if ( pMesh->GetNumPositions() == 0 )
    {
        ELOG( "Error : Position data is empty." );
        return false;
    }

    // 頂点データのメモリを確保
    MeshVertex *pVertex = null;
    try
    {
        pVertex = new MeshVertex[ pMesh->GetNumPositions() ];
    }
    catch( std::bad_alloc ba )
    {
        ELOG( "Error : Memory Allocate Faield. reason = %s", ba.what() );
        return false;
    }

    // データがあるかチェックしておく
    bool isEmpty_N = ( pMesh->GetNumNormals() == 0 );
    bool isEmpty_U = ( pMesh->GetNumTexCoords() == 0 );
    bool isEmpty_I = ( pMesh->GetNumSkinIndices() == 0 );
    bool isEmpty_W = ( pMesh->GetNumSkinWeights() == 0 );

    // 頂点データ構築
    for( uint32_t i=0; i<pMesh->GetNumPositions(); ++i )
    {
		// データが無い場合はゼロで埋める
        pVertex[ i ].Position   = pMesh->GetPosition( i );
        pVertex[ i ].Normal     = ( !isEmpty_N ) ? pMesh->GetNormal( i )     : Vector3( 0.0f, 0.0f, 0.0f );
        pVertex[ i ].TexCoord   = ( !isEmpty_U ) ? pMesh->GetTexCoord( i )   : Vector2( 0.0f, 0.0f );
        pVertex[ i ].SkinIndex  = ( !isEmpty_I ) ? pMesh->GetSkinIndex( i )  : Short4( 0, 0, 0, 0 );
        pVertex[ i ].SkinWeight = ( !isEmpty_W ) ? pMesh->GetSkinWeight( i ) : Vector4( 0.0f, 0.0f, 0.0f, 0.0f );       
    }

    // 頂点バッファの設定
    D3D11_BUFFER_DESC bufDesc;
    memset( &bufDesc, 0, sizeof( bufDesc ) );
    bufDesc.Usage = D3D11_USAGE_DEFAULT;
    bufDesc.ByteWidth = sizeof( MeshVertex ) * pMesh->GetNumPositions();
    bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufDesc.CPUAccessFlags = 0;

    // ストライドとオフセットの設定
    mStride = sizeof( MeshVertex );
    mOffset = 0;

    // サブリソースデータの設定
    D3D11_SUBRESOURCE_DATA initData;
    memset( &initData, 0, sizeof( initData ) );
    initData.pSysMem = pVertex;

    // 頂点バッファ作成
    HRESULT hr = pDevice->CreateBuffer( &bufDesc, &initData, &mpVB );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateBuffer() Failed." );
        SAFE_DELETE_ARRAY( pVertex );
        return false;
    }

    // メモリ解放
    SAFE_DELETE_ARRAY( pVertex );

    // マテリアル用定数バッファの作成
    memset( &bufDesc, 0, sizeof( bufDesc ) );
    bufDesc.Usage = D3D11_USAGE_DEFAULT;
    bufDesc.ByteWidth = sizeof( MeshMaterial );
    bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufDesc.CPUAccessFlags = 0;
    hr = pDevice->CreateBuffer( &bufDesc, NULL, &mpMB );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateBuffer() Failed." );
        return false;
    }
  
	uint32_t *pIndices = null; 
	try
	{
		pIndices = new uint32_t [ pMesh->GetNumIndices() ];
	}
	catch( std::bad_alloc ba )
	{
		ELOG( "Error : Memory Allocate failed. reason = %s", ba.what() );
		return false;
	}
	for( uint32_t i=0; i<pMesh->GetNumIndices(); ++i )
	{
		pIndices[ i ] = pMesh->GetIndex( i );
	}

    // インデックスバッファの設定
    memset( &bufDesc, 0, sizeof( bufDesc ) );
    memset( &initData, 0, sizeof( initData ) );
    bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.ByteWidth = sizeof( uint32_t ) * pMesh->GetNumIndices();
    bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufDesc.CPUAccessFlags = 0;
    initData.pSysMem = pIndices;

	// インデックスバッファ生成
    hr = pDevice->CreateBuffer( &bufDesc, &initData, &mpIB );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateBuffer() Failed." );
        SAFE_DELETE_ARRAY( pIndices );
        return false;
    }
	SAFE_DELETE_ARRAY( pIndices );

    // サブセットの構築
    mSubsets.resize( pMesh->GetNumSubsets() );
    for( uint32_t i=0; i<pMesh->GetNumSubsets(); ++i )
    {
		memset( &mSubsets[ i ], 0, sizeof( SubsetData ) );
   
		// 頂点カウント数設定
		mSubsets[ i ].count = pMesh->GetSubset( i )->GetNumFace() * 3;
		mSubsets[ i ].startIndex = pMesh->GetSubset( i )->GetFaceIndex() * 3;

		// マテリアルを格納
        IMaterial* pMat = pMesh->GetMaterial( pMesh->GetSubset( i )->GetMaterialIndex() );
        mSubsets[ i ].diffuse  = pMat->GetDiffuse();
        mSubsets[ i ].specular = pMat->GetSpecular();
        mSubsets[ i ].emissive = pMat->GetEmissive();
        mSubsets[ i ].power    = pMat->GetPower();
    }
  
    return true;
}

//-----------------------------------------------------------------------------------
//! @brief 描画処理
//-----------------------------------------------------------------------------------
void MeshData::Draw( ID3D11DeviceContext* pImmediateContext )
{
	// 頂点バッファと入力レイアウトを設定
    pImmediateContext->IASetVertexBuffers( 0, 1, &mpVB, &mStride, &mOffset );
    pImmediateContext->IASetInputLayout( gGlobal.pInputLayout );

    for( uint32_t i=0; i<mSubsets.size(); ++i )
    {
        DrawSubset( pImmediateContext, i );
    }
}

//----------------------------------------------------------------------------------
//! @brief サブセットの描画処理
//----------------------------------------------------------------------------------
void MeshData::DrawSubset( ID3D11DeviceContext* pImmediateContext, const int index )
{
    assert( pImmediateContext != NULL );
    assert( index >= 0 );
    assert( gGlobal.pInputLayout != NULL );

    MeshMaterial matData;

    for( uint32_t i=0; i<mSubsets.size(); ++i )
    {
		// マテリアルデータ設定
        memset( &matData, 0, sizeof( MeshMaterial ) );
        matData.Diffuse  = mSubsets[ i ].diffuse;
        matData.Specular.x = mSubsets[ i ].specular.x;
		matData.Specular.y = mSubsets[ i ].specular.y;
		matData.Specular.z = mSubsets[ i ].specular.z;
		matData.Specular.w = mSubsets[ i ].power;
        matData.Emissive = mSubsets[ i ].emissive;

		// マテリアルバッファを更新し，slot:1の定数バッファに設定
        pImmediateContext->UpdateSubresource( mpMB, 0, NULL, &matData, 0, 0 );
        pImmediateContext->PSSetConstantBuffers( 1, 1, &mpMB );

		// インデックスバッファとプリミティブを設定
        pImmediateContext->IASetIndexBuffer( mpIB, DXGI_FORMAT_R32_UINT, 0 );
        pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		// 描画
		UINT indexCount = mSubsets[ i ].count;
		UINT startIndex = mSubsets[ i ].startIndex;
        pImmediateContext->DrawIndexed( indexCount, startIndex, 0 );        
    }
}

//----------------------------------------------------------------------------------
//! @brief サブセット数を取得
//----------------------------------------------------------------------------------
uint32_t MeshData::GetNumSubsets()
{ return mSubsets.size();}

//----------------------------------------------------------------------------------
//! @brief サブセットを取得
//----------------------------------------------------------------------------------
SubsetData& MeshData::GetSubset( const int index )
{ return mSubsets[ index ]; }

//----------------------------------------------------------------------------------
//! @brief 頂点バッファを取得
//----------------------------------------------------------------------------------
ID3D11Buffer* MeshData::GetVertexBuffer()
{ return mpVB; }

//----------------------------------------------------------------------------------
//! @brief マテリアルバッファを取得
//----------------------------------------------------------------------------------
ID3D11Buffer* MeshData::GetMaterialBuffer()
{ return mpMB; }

//----------------------------------------------------------------------------------
//! @brief 頂点バッファのストライドを取得
//----------------------------------------------------------------------------------
uint32_t MeshData::GetStride()
{ return mStride; }

//----------------------------------------------------------------------------------
//! @brief 頂点バッファのオフセットを取得
//----------------------------------------------------------------------------------
uint32_t MeshData::GetOffset()
{ return mOffset; }


/////////////////////////////////////////////////////////////////////////
// ModelData class
/////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
//! @brief 初期値設定
//-----------------------------------------------------------------------
ModelData::ModelData()
{
	mMeshes.clear();
	mpInterface = null;
}

//-----------------------------------------------------------------------
//! @brief 破棄処理
//-----------------------------------------------------------------------
ModelData::~ModelData()
{
	Release();
}

//-----------------------------------------------------------------------
//! @brief メモリ解放処理
//-----------------------------------------------------------------------
void ModelData::Release()
{
	ReleaseBuffer();

	if ( mpInterface )
	{
		mpInterface->Release();
		delete mpInterface;
		mpInterface = null;
	}
}

//-----------------------------------------------------------------------
//! @brief Xファイルからモデルを読み込み
//-----------------------------------------------------------------------
bool ModelData::LoadFromX( const char *filename )
{
	IModelLoader* pLoader = null;

	// ローダインタフェース生成
	try 
	{
		pLoader = ModelLoaderFactory::Create( ModelLoaderFactory::MODEL_TYPE_X );
	}
	catch( std::bad_alloc ba )
	{
		ELOG( "Error : ModelLoaderFactory::Create() Failed. reason = %s", ba.what() );
		return false;
	}

	// ロード処理
	if ( !pLoader->Load( filename ) )
	{
		ELOG( "Error : IModelLoader::Load() Failed. filename = %s", filename );
		// ローダー破棄
		pLoader->Release();
		delete pLoader;
		pLoader = null;

		return false;
	}
	for( uint32_t i=0; i<UINT_MAX; ++i )
	{
		if ( !pLoader->IsLoading() )
		{
			// 読み込み結果を解析
			pLoader->Parse();

			// モデルインタフェースを取得
			mpInterface = pLoader->GetModel();
			
			// ローダー破棄
			pLoader->Release();
			delete pLoader;
			pLoader = null;

			// モデルインタフェースがnullなら終了
			if ( mpInterface == null )
			{
				ELOG( "Error : IModelLoader::GetModel() Failed." );
				return false;
			}
			break;
		}
	}
	assert( mpInterface != null );

	return true;
}

//------------------------------------------------------------------------
//! @brief バッファを生成する
//------------------------------------------------------------------------
bool ModelData::CreateBuffer( ID3D11Device *pDevice )
{
	// メッシュデータのメモリを確保
	mMeshes.resize( mpInterface->GetNumMeshes() );

	// 各メッシュのバッファ生成
	for( uint32_t i=0; i<mpInterface->GetNumMeshes(); ++i )
	{
		if ( !mMeshes[ i ].CreateBuffer( pDevice, mpInterface->GetMesh( i ) ) )
		{
			// エラーログ
			ELOG( "Error : MeshData::CreateBuffer() Failed. index = %d", i );

			// 確保したメモリを解放
			for( uint32_t j=0; j<i; j++ )
			{
				mMeshes[ j ].Release();
			}

			// メッシュデータのメモリを解放
			mMeshes.clear();

			return false;
		}
	}

	return true;
}

//-----------------------------------------------------------------------
//! @brief バッファを破棄する
//-----------------------------------------------------------------------
void ModelData::ReleaseBuffer()
{
	for( uint32_t i=0; i<mMeshes.size(); ++i )
	{
		mMeshes[ i ].Release();
	}
	mMeshes.clear();
}

//-----------------------------------------------------------------------
//! @brief モデルの描画
//-----------------------------------------------------------------------
void ModelData::Draw( ID3D11DeviceContext* pImmediateContext )
{
	assert( pImmediateContext != null );

	for( uint32_t i=0; i<mMeshes.size(); ++i )
	{
		DrawMesh( pImmediateContext, i );
	}
}

//-----------------------------------------------------------------------
//! @brief メッシュの描画
//! @param [in] index 描画するメッシュのインデックス
//-----------------------------------------------------------------------
void ModelData::DrawMesh( ID3D11DeviceContext* pImmediateContext, const int index )
{
	assert( pImmediateContext != null );
	assert( 0 <= index && index < mMeshes.size() );

	mMeshes[ index ].Draw( pImmediateContext );
}

//-----------------------------------------------------------------------
//! @brief メッシュ数を取得する
//-----------------------------------------------------------------------
uint32_t ModelData::GetNumMeshes()
{ return mMeshes.size(); }

//-----------------------------------------------------------------------
//! @brief メッシュを取得する
//! @param [in] index 取得したメッシュのインデックス
//-----------------------------------------------------------------------
MeshData& ModelData::GetMesh( const int index )
{
	assert( 0 <= index && index < mMeshes.size() );
	return mMeshes[ index ];
}

//-----------------------------------------------------------------------
//! @brief モデルインタフェースを取得する
//-----------------------------------------------------------------------
IModel* ModelData::GetInterface()
{ return mpInterface; }


/////////////////////////////////////////////////////////////////////////
// SkinningModelData class
/////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
//! @brief 初期値設定
//-----------------------------------------------------------------------
SkinningModelData::SkinningModelData()
: ModelData()
{
	mRoot.Identity();
	mTimer.Reset();
	mpPlayer = null;
}

//-----------------------------------------------------------------------
//! @brief 破棄処理
//-----------------------------------------------------------------------
SkinningModelData::~SkinningModelData()
{
	Release();
}

//-----------------------------------------------------------------------
//! @brief メモリ解放処理
//-----------------------------------------------------------------------
void SkinningModelData::Release()
{
	ReleaseBuffer();

	if ( mpInterface )
	{
		mpInterface->Release();
		delete mpInterface;
		mpInterface = null;
	}

	if ( mpPlayer )
	{
		mpPlayer->Release();
		delete mpPlayer;
		mpPlayer = null;
	}

	mTimer.Reset();
}

//-----------------------------------------------------------------------
//! @brief Xファイルからスキニングモデルをロードする
//! @param [in] filename ロードするファイル名
//-----------------------------------------------------------------------
bool SkinningModelData::LoadFromX( const char* filename )
{
	IModelLoader* pLoader = null;
	try 
	{
		pLoader = ModelLoaderFactory::Create( ModelLoaderFactory::MODEL_TYPE_X );
	}
	catch( std::bad_alloc ba )
	{
		ELOG( "Error : ModelLoaderFactory::Create() Failed. filename = %s", filename );
		return false;
	}

	// ロード処理
	if ( !pLoader->Load( filename ) )
	{
		ELOG( "Error : IModelLoader::Load() Failed. filename = %s", filename );
		// ローダー破棄
		pLoader->Release();
		delete pLoader;
		pLoader = null;

		return false;
	}
	for( uint32_t i=0; i<UINT_MAX; ++i )
	{
		if ( !pLoader->IsLoading() )
		{
			// 読み込み結果を解析
			pLoader->Parse();

			// モデルインタフェースを取得
			mpInterface = pLoader->GetModel();
			
			// ローダー破棄
			pLoader->Release();
			delete pLoader;
			pLoader = null;

			// モデルインタフェースがnullなら終了
			if ( mpInterface == null )
			{
				ELOG( "Error : IModelLoader::GetModel() Failed." );
				return false;
			}
			break;
		}
	}
	assert( mpInterface != null );

#if 0 // azarashin modified...
	// アニメーションプレイヤーを生成
	if ( mpInterface->GetMesh( 0 )->GetNumBones() > 0 )
	{
		try
		{
			mpPlayer = AnimationPlayerFactory::Create( mpInterface->GetMesh( 0 )->GetBones() );
		}
		catch( std::bad_alloc ba )
		{
			ELOG( "Error : AnimationPlayerFactory::Create() Failed. reason = %s", ba.what() );
			return false;
		}
		assert( mpPlayer != null );

		// 生成されたらアニメーションクリップを設定
		if ( mpInterface->GetNumClips() > 0 )
		{
			mpPlayer->SetClip( mpInterface->GetClip( 0 ) );
			mTimer.Start();
		}
		else
		{
			// クリップが無い場合はメモリ解放して終了
			ELOG( "Error : AnimationClip is not exist." );

			mpPlayer->Release();
			delete mpPlayer;
			mpPlayer = null;

			return false;
		}
	}
#endif

	return true;
}

//-----------------------------------------------------------------------
//! @brief 更新処理
//-----------------------------------------------------------------------
void SkinningModelData::Update( const bool isStop, const float speed )
{
	if ( mpPlayer != null )
	{
		mpPlayer->Update( 
			static_cast< float >( mTimer.GetElapsedTime() * speed ),
			!isStop, 
			mRoot ); 
	}
}

//-----------------------------------------------------------------------
//! @brief 指定された番号のアニメーションクリップを設定
//-----------------------------------------------------------------------
void SkinningModelData::SetClip( const uint32_t index )
{
	if ( mpPlayer != null )
	{
		if ( 0 <= index && index < mpInterface->GetNumClips() )
		{
			mpPlayer->SetClip( mpInterface->GetClip( index ) );
		}
	}
}

//-----------------------------------------------------------------------
//! @brief ルート行列を設定する
//-----------------------------------------------------------------------
void SkinningModelData::SetRootMatrix( const Matrix &matrix )
{ mRoot = matrix; }

//-----------------------------------------------------------------------
//! @brief ルート行列を設定する
//-----------------------------------------------------------------------
void SkinningModelData::SetD3DXRootMatrix( const D3DXMATRIX &matrix )
{ memcpy( &mRoot.m[ 0 ][ 0 ], &matrix.m[ 0 ][ 0 ], sizeof( float ) * 16 ); }

//-----------------------------------------------------------------------
//! @brief ルート行列を取得する
//-----------------------------------------------------------------------
Matrix SkinningModelData::GetRootMatrix()
{ return mRoot; }

//-----------------------------------------------------------------------
//! @brief ルート行列を取得する
//-----------------------------------------------------------------------
D3DXMATRIX SkinningModelData::GetD3DXRootMatrix()
{
	D3DXMATRIX result;
	memcpy(
		&result.m[ 0 ][ 0 ],
		&mRoot.m[ 0 ][ 0 ], 
		sizeof( float ) * 16 );
	return result;
}

//-----------------------------------------------------------------------
//! @brief 行列パレットを取得する
//-----------------------------------------------------------------------
std::vector< Matrix > SkinningModelData::GetSkinTransformMatrix()
{
	if ( mpPlayer )
	{
		return mpPlayer->GetSkinTransform();
	}

	std::vector< Matrix > result;
	result.clear();
	return result;
}

//-----------------------------------------------------------------------
//! @brief 行列パレットを取得する
//-----------------------------------------------------------------------
std::vector< D3DXMATRIX > SkinningModelData::GetD3DXSkinTransformMatrix()
{
	std::vector< D3DXMATRIX > result;
	if ( mpPlayer )
	{
		result.resize( mpPlayer->GetSkinTransform().size() );
		for( uint32_t i=0; i<mpPlayer->GetSkinTransform().size(); ++i )
		{
			memcpy(
				&result[ i ].m[ 0 ][ 0 ],
				&mpPlayer->GetSkinTransform()[ i ].m[ 0 ][ 0 ],
				sizeof( float ) * 16 );
		}
	}
	else
	{
		result.clear();
	}

	return result;
}


} // namespace Util
} // namespace Asura 