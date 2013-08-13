//-----------------------------------------------------------------------
// File : AsdxShaderUtil.h
// Desc : Asura DirectX Shader Utility
// Copyright(c) Project Asura. All right resreved.
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// History
//-----------------------------------------------------------------------
// <---> 2011/07/23 [Pocol] 新規作成
// <End>


//-----------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------
#include "asdxShaderUtil.h"

#include <d3dcompiler.h>

namespace Asura {
namespace Graphics {

/////////////////////////////////////////////////////////////////////////
// ShaderUtil class
/////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
//! @brief ファイルからシェーダをコンパイルする（MB版)
//! @param [in] filename シェーダのファイル名
//! @param [in] entryPoint エントリーポイント名
//! @param [in] shader シェーダプロファイル名
//! @param [out] ppBlobOut バイトコードを格納するインタフェースへのポインタ
//-----------------------------------------------------------------------
bool ShaderUtil::CompileFromFileA
(
    const char* filename,
    const char* entryPoint,
    const char* shaderModel,
    ID3DBlob** ppBlobOut 
)
{
    HRESULT hr = S_OK;
    bool result = true;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;

    // ファイルからコンパイル
    hr = D3DX11CompileFromFileA( 
        filename, 
        NULL,
        NULL,
        entryPoint,
        shaderModel,
        dwShaderFlags,
        0,
        NULL,
        ppBlobOut,
        &pErrorBlob,
        NULL );
    if ( FAILED( hr ) )
    {
        result = false;
        ELOG( "Error : D3DX11CompileFromFileA() Failed. filename = %s", filename );
        if ( pErrorBlob != NULL )
        {
            // エラーメッセージ出力
            ELOG( "Error : %s", (char*)pErrorBlob->GetBufferPointer() );
         	OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        }
    }

    SAFE_RELEASE( pErrorBlob );

    return result;
}

//-----------------------------------------------------------------------
//! @brief ファイルからシェーダをコンパイルする(Unicode版)
//! @param [in] filename シェーダのファイル名
//! @param [in] entryPoint エントリーポイント名
//! @param [in] shader シェーダプロファイル名
//! @param [out] ppBlobOut バイトコードを格納するインタフェースへのポインタ
//-----------------------------------------------------------------------
bool ShaderUtil::CompileFromFileW
(
    const wchar_t* filename,
    const char* entryPoint,
    const char* shaderModel,
    ID3DBlob** ppBlobOut 
)
{
    HRESULT hr = S_OK;
    bool result = true;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;

    // ファイルからコンパイル
    hr = D3DX11CompileFromFileW( 
        filename, 
        NULL,
        NULL,
        entryPoint,
        shaderModel,
        dwShaderFlags,
        0,
        NULL,
        ppBlobOut,
        &pErrorBlob,
        NULL );
    if ( FAILED( hr ) )
    {
        result = false;
		printf( "[File: %s, Line: %d] ", __FILE__, __LINE__ );
		wprintf( L"Error : D3DX11CompileFromFileW() Failed. filename = %s\n", filename );
        if ( pErrorBlob != NULL )
        {
            // エラーメッセージ出力
			ELOG( "Error : %s\n", (char*)pErrorBlob->GetBufferPointer() );
         	OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        }
    }

    SAFE_RELEASE( pErrorBlob );

    return result;
}

//-----------------------------------------------------------------------
//! @brief ファイルからシェーダをコンパイルする
//! @param [in] filename シェーダのファイル名
//! @param [in] entryPoint エントリーポイント名
//! @param [in] shader シェーダプロファイル名
//! @param [out] ppBlobOut バイトコードを格納するインタフェースへのポインタ
//-----------------------------------------------------------------------
bool ShaderUtil::CompileFromFile
(
    const tchar* filename,
    const char* entryPoint,
    const char* shaderModel,
    ID3DBlob** ppBlobOut 
)
{
    HRESULT hr = S_OK;
    bool result = true;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;

    // ファイルからコンパイル
    hr = D3DX11CompileFromFile( 
        filename, 
        NULL,
        NULL,
        entryPoint,
        shaderModel,
        dwShaderFlags,
        0,
        NULL,
        ppBlobOut,
        &pErrorBlob,
        NULL );
    if ( FAILED( hr ) )
    {
        result = false;
#ifdef UNICODE
		printf( "[File: %s, Line: %d] ", __FILE__, __LINE__ );
		wprintf( L"Error : D3DX11CompileFromFile() Failed. filename = %s\n",filename );
#else
        ELOG( "Error : D3DX11CompileFromFile() Failed. filename = %s", filename );
#endif

        if ( pErrorBlob != NULL )
        {
            // エラーメッセージ出力
            ELOG( "Error : %s", (char*)pErrorBlob->GetBufferPointer() );
         	OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        }
    }

    SAFE_RELEASE( pErrorBlob );

    return result;
}

//-----------------------------------------------------------------------
//! @brief ファイルから頂点シェーダをコンパイルして，生成する
//! @param [in] filename シェーダのファイル名
//! @param [in] entryPoint エントリーポイント名
//! @param [in] shader シェーダプロファイル名
//! @param [in] pDevice D3D11デバイスインタフェース
//! @param [out] ppBlobOut バイトコードを格納する変数へのポインタ
//! @param [out] ppShader 生成されたシェーダを格納する変数へのポインタ
//-----------------------------------------------------------------------
bool ShaderUtil::CreteVertexShaderFromFileA
(
    const char* filename,
    const char* entryPoint,
    const char* shaderModel,
    ID3D11Device* pDevice,
    ID3DBlob** ppBlob,
    ID3D11VertexShader** ppShader 
)
{
    // ファイルからコンパイル
    if ( !CompileFromFileA( filename, entryPoint, shaderModel, ppBlob ) )
    {
        ELOG( "Error : CompileFromFileA() Failed. filename = %s", filename );
        SAFE_RELEASE( (*ppBlob) );
        return false;
    }

    // シェーダ生成
    HRESULT hr = pDevice->CreateVertexShader( (*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateVertexShader() Failed. filename = %s", filename );
        SAFE_RELEASE( (*ppBlob) );
        return false;
    }

    return true;   
}


//-----------------------------------------------------------------------
//! @brief ファイルから頂点シェーダをコンパイルして，生成する
//! @param [in] filename シェーダのファイル名
//! @param [in] entryPoint エントリーポイント名
//! @param [in] shader シェーダプロファイル名
//! @param [in] pDevice D3D11デバイスインタフェース
//! @param [out] ppBlobOut バイトコードを格納する変数へのポインタ
//! @param [out] ppShader 生成されたシェーダを格納する変数へのポインタ
//-----------------------------------------------------------------------
bool ShaderUtil::CreateVertexShaderFromFileW
(
    const wchar_t* filename,
    const char* entryPoint,
    const char* shaderModel,
    ID3D11Device* pDevice,
    ID3DBlob** ppBlob,
    ID3D11VertexShader** ppShader 
)
{
    // ファイルからコンパイル
    if ( !CompileFromFileW( filename, entryPoint, shaderModel, ppBlob ) )
    {
        ELOG( "Error : CompileFromFileW() Failed. filename = %s", filename );
        SAFE_RELEASE( (*ppBlob) );
        return false;
    }

    // シェーダ生成
    HRESULT hr = pDevice->CreateVertexShader( (*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateVertexShader() Failed. filename = %s", filename );
        SAFE_RELEASE( (*ppBlob) );
        return false;
    }

    return true;   
}


//-----------------------------------------------------------------------
//! @brief ファイルから頂点シェーダをコンパイルして，生成する
//! @param [in] filename シェーダのファイル名
//! @param [in] entryPoint エントリーポイント名
//! @param [in] shader シェーダプロファイル名
//! @param [in] pDevice D3D11デバイスインタフェース
//! @param [out] ppBlobOut バイトコードを格納する変数へのポインタ
//! @param [out] ppShader 生成されたシェーダを格納する変数へのポインタ
//-----------------------------------------------------------------------
bool ShaderUtil::CreateVertexShaderFromFile
(
    const tchar* filename,
    const char* entryPoint,
    const char* shaderModel,
    ID3D11Device* pDevice,
    ID3DBlob** ppBlob,
    ID3D11VertexShader** ppShader 
)
{
    // ファイルからコンパイル
    if ( !CompileFromFile( filename, entryPoint, shaderModel, ppBlob ) )
    {
        ELOG( "Error : CompileFromFile() Failed. filename = %s", filename );
        SAFE_RELEASE( (*ppBlob) );
        return false;
    }

    // シェーダ生成
    HRESULT hr = pDevice->CreateVertexShader( (*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateVertexShader() Failed. filename = %s", filename );
        SAFE_RELEASE( (*ppBlob) );
        return false;
    }

    return true;   
}


//-----------------------------------------------------------------------
//! @brief ファイルからピクセルシェーダをコンパイルして，生成する
//! @param [in] filename シェーダのファイル名
//! @param [in] entryPoint エントリーポイント名
//! @param [in] shader シェーダプロファイル名
//! @param [in] pDevice D3D11デバイスインタフェース
//! @param [out] ppShader 生成されたシェーダを格納する変数へのポインタ
//-----------------------------------------------------------------------
bool ShaderUtil::CreatePixelShaderFromFileA
(
    const char* filename,
    const char* entryPoint,
    const char* shaderModel,
    ID3D11Device* pDevice,
    ID3D11PixelShader** ppShader 
)
{
    ID3DBlob* pBlob;

    // ファイルからコンパイル
    if ( !CompileFromFileA( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFileA() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // シェーダ生成
    HRESULT hr = pDevice->CreatePixelShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreatePixelShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // メモリ解放
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief ファイルからピクセルシェーダをコンパイルして，生成する
//! @param [in] filename シェーダのファイル名
//! @param [in] entryPoint エントリーポイント名
//! @param [in] shader シェーダプロファイル名
//! @param [in] pDevice D3D11デバイスインタフェース
//! @param [out] ppShader 生成されたシェーダを格納する変数へのポインタ
//-----------------------------------------------------------------------
bool ShaderUtil::CreatePixelShaderFromFileW
(
    const wchar_t* filename,
    const char* entryPoint,
    const char* shaderModel,
    ID3D11Device* pDevice,
    ID3D11PixelShader** ppShader 
)
{
    ID3DBlob* pBlob;

    // ファイルからコンパイル
    if ( !CompileFromFileW( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFileW() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // シェーダ生成
    HRESULT hr = pDevice->CreatePixelShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreatePixelShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // メモリ解放
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief ファイルからピクセルシェーダをコンパイルして，生成する
//! @param [in] filename シェーダのファイル名
//! @param [in] entryPoint エントリーポイント名
//! @param [in] shader シェーダプロファイル名
//! @param [in] pDevice D3D11デバイスインタフェース
//! @param [out] ppShader 生成されたシェーダを格納する変数へのポインタ
//-----------------------------------------------------------------------
bool ShaderUtil::CreatePixelShaderFromFile
(
    const tchar* filename,
    const char* entryPoint,
    const char* shaderModel,
    ID3D11Device* pDevice,
    ID3D11PixelShader** ppShader 
)
{
    ID3DBlob* pBlob;

    // ファイルからコンパイル
    if ( !CompileFromFile( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFile() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // シェーダ生成
    HRESULT hr = pDevice->CreatePixelShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreatePixelShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // メモリ解放
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief ファイルからジオメトリシェーダをコンパイルして，生成する
//! @param [in] filename シェーダのファイル名
//! @param [in] entryPoint エントリーポイント名
//! @param [in] shader シェーダプロファイル名
//! @param [in] pDevice D3D11デバイスインタフェース
//! @param [out] ppShader 生成されたシェーダを格納する変数へのポインタ
//-----------------------------------------------------------------------
bool ShaderUtil::CreateGeometryShaderFromFileA
(
    const char* filename,
    const char* entryPoint,
    const char* shaderModel,
    ID3D11Device* pDevice,
    ID3D11GeometryShader** ppShader 
)
{
    ID3DBlob* pBlob;

    // ファイルからコンパイル
    if ( !CompileFromFileA( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFileA() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // シェーダ生成
    HRESULT hr = pDevice->CreateGeometryShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateGeometryShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // メモリ解放
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief ファイルからジオメトリシェーダをコンパイルして，生成する
//! @param [in] filename シェーダのファイル名
//! @param [in] entryPoint エントリーポイント名
//! @param [in] shader シェーダプロファイル名
//! @param [in] pDevice D3D11デバイスインタフェース
//! @param [out] ppShader 生成されたシェーダを格納する変数へのポインタ
//-----------------------------------------------------------------------
bool ShaderUtil::CreateGeometryShaderFromFileW
(
    const wchar_t* filename,
    const char* entryPoint,
    const char* shaderModel,
    ID3D11Device* pDevice,
    ID3D11GeometryShader** ppShader 
)
{
    ID3DBlob* pBlob;

    // ファイルからコンパイル
    if ( !CompileFromFileW( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFileW() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // シェーダ生成
    HRESULT hr = pDevice->CreateGeometryShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateGeometryShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // メモリ解放
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief ファイルからジオメトリシェーダをコンパイルして，生成する
//! @param [in] filename シェーダのファイル名
//! @param [in] entryPoint エントリーポイント名
//! @param [in] shader シェーダプロファイル名
//! @param [in] pDevice D3D11デバイスインタフェース
//! @param [out] ppShader 生成されたシェーダを格納する変数へのポインタ
//-----------------------------------------------------------------------
bool ShaderUtil::CreateGeometryShaderFromFile
(
    const tchar* filename,
    const char* entryPoint,
    const char* shaderModel,
    ID3D11Device* pDevice,
    ID3D11GeometryShader** ppShader 
)
{
    ID3DBlob* pBlob;

    // ファイルからコンパイル
    if ( !CompileFromFile( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFile() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // シェーダ生成
    HRESULT hr = pDevice->CreateGeometryShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateGeometryShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // メモリ解放
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief ファイルからハルシェーダをコンパイルして，生成する
//! @param [in] filename シェーダのファイル名
//! @param [in] entryPoint エントリーポイント名
//! @param [in] shader シェーダプロファイル名
//! @param [in] pDevice D3D11デバイスインタフェース
//! @param [out] ppShader 生成されたシェーダを格納する変数へのポインタ
//-----------------------------------------------------------------------
bool ShaderUtil::CreateHullShaderFromFileA
(
    const char* filename,
    const char* entryPoint,
    const char* shaderModel,
    ID3D11Device* pDevice,
    ID3D11HullShader** ppShader 
)
{
    ID3DBlob* pBlob;

    // ファイルからコンパイル
    if ( !CompileFromFileA( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFileA() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // シェーダ生成
    HRESULT hr = pDevice->CreateHullShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateHullShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // メモリ解放
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief ファイルからハルシェーダをコンパイルして，生成する
//! @param [in] filename シェーダのファイル名
//! @param [in] entryPoint エントリーポイント名
//! @param [in] shader シェーダプロファイル名
//! @param [in] pDevice D3D11デバイスインタフェース
//! @param [out] ppShader 生成されたシェーダを格納する変数へのポインタ
//-----------------------------------------------------------------------
bool ShaderUtil::CreateHullShaderFromFileW
(
    const wchar_t* filename,
    const char* entryPoint,
    const char* shaderModel,
    ID3D11Device* pDevice,
    ID3D11HullShader** ppShader 
)
{
    ID3DBlob* pBlob;

    // ファイルからコンパイル
    if ( !CompileFromFileW( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFileW() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // シェーダ生成
    HRESULT hr = pDevice->CreateHullShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateHullShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // メモリ解放
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief ファイルからハルシェーダをコンパイルして，生成する
//! @param [in] filename シェーダのファイル名
//! @param [in] entryPoint エントリーポイント名
//! @param [in] shader シェーダプロファイル名
//! @param [in] pDevice D3D11デバイスインタフェース
//! @param [out] ppShader 生成されたシェーダを格納する変数へのポインタ
//-----------------------------------------------------------------------
bool ShaderUtil::CreateHullShaderFromFile
(
    const tchar* filename,
    const char* entryPoint,
    const char* shaderModel,
    ID3D11Device* pDevice,
    ID3D11HullShader** ppShader 
)
{
    ID3DBlob* pBlob;

    // ファイルからコンパイル
    if ( !CompileFromFile( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFile() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // シェーダ生成
    HRESULT hr = pDevice->CreateHullShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateHullShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // メモリ解放
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief ファイルから領域シェーダをコンパイルして，生成する
//! @param [in] filename シェーダのファイル名
//! @param [in] entryPoint エントリーポイント名
//! @param [in] shader シェーダプロファイル名
//! @param [in] pDevice D3D11デバイスインタフェース
//! @param [out] ppShader 生成されたシェーダを格納する変数へのポインタ
//-----------------------------------------------------------------------
bool ShaderUtil::CreateDomainShaderFromFileA
(
    const char* filename,
    const char* entryPoint,
    const char* shaderModel,
    ID3D11Device* pDevice,
    ID3D11DomainShader** ppShader 
)
{
    ID3DBlob* pBlob;

    // ファイルからコンパイル
    if ( !CompileFromFileA( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFileA() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // シェーダ生成
    HRESULT hr = pDevice->CreateDomainShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateDomainShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // メモリ解放
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief ファイルから領域シェーダをコンパイルして，生成する
//! @param [in] filename シェーダのファイル名
//! @param [in] entryPoint エントリーポイント名
//! @param [in] shader シェーダプロファイル名
//! @param [in] pDevice D3D11デバイスインタフェース
//! @param [out] ppShader 生成されたシェーダを格納する変数へのポインタ
//-----------------------------------------------------------------------
bool ShaderUtil::CreateDomainShaderFromFileW
(
    const wchar_t* filename,
    const char* entryPoint,
    const char* shaderModel,
    ID3D11Device* pDevice,
    ID3D11DomainShader** ppShader 
)
{
    ID3DBlob* pBlob;

    // ファイルからコンパイル
    if ( !CompileFromFileW( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFileW() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // シェーダ生成
    HRESULT hr = pDevice->CreateDomainShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateDomainShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // メモリ解放
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief ファイルから領域シェーダをコンパイルして，生成する
//! @param [in] filename シェーダのファイル名
//! @param [in] entryPoint エントリーポイント名
//! @param [in] shader シェーダプロファイル名
//! @param [in] pDevice D3D11デバイスインタフェース
//! @param [out] ppShader 生成されたシェーダを格納する変数へのポインタ
//-----------------------------------------------------------------------
bool ShaderUtil::CreateDomainShaderFromFile
(
    const tchar* filename,
    const char* entryPoint,
    const char* shaderModel,
    ID3D11Device* pDevice,
    ID3D11DomainShader** ppShader 
)
{
    ID3DBlob* pBlob;

    // ファイルからコンパイル
    if ( !CompileFromFile( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFile() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // シェーダ生成
    HRESULT hr = pDevice->CreateDomainShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateDomainShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // メモリ解放
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief ファイルから演算シェーダをコンパイルして，生成する
//! @param [in] filename シェーダのファイル名
//! @param [in] entryPoint エントリーポイント名
//! @param [in] shader シェーダプロファイル名
//! @param [in] pDevice D3D11デバイスインタフェース
//! @param [out] ppShader 生成されたシェーダを格納する変数へのポインタ
//-----------------------------------------------------------------------
bool ShaderUtil::CreateComputeShaderFromFileA
(
    const char* filename,
    const char* entryPoint,
    const char* shaderModel,
    ID3D11Device* pDevice,
    ID3D11ComputeShader** ppShader 
)
{
    ID3DBlob* pBlob;

    // ファイルからコンパイル
    if ( !CompileFromFileA( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFileA() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // シェーダ生成
    HRESULT hr = pDevice->CreateComputeShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateComputeShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // メモリ解放
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief ファイルから演算シェーダをコンパイルして，生成する
//! @param [in] filename シェーダのファイル名
//! @param [in] entryPoint エントリーポイント名
//! @param [in] shader シェーダプロファイル名
//! @param [in] pDevice D3D11デバイスインタフェース
//! @param [out] ppShader 生成されたシェーダを格納する変数へのポインタ
//-----------------------------------------------------------------------
bool ShaderUtil::CreateComputeShaderFromFileW
(
    const wchar_t* filename,
    const char* entryPoint,
    const char* shaderModel,
    ID3D11Device* pDevice,
    ID3D11ComputeShader** ppShader 
)
{
    ID3DBlob* pBlob;

    // ファイルからコンパイル
    if ( !CompileFromFileW( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFileW() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // シェーダ生成
    HRESULT hr = pDevice->CreateComputeShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateComputeShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // メモリ解放
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief ファイルから演算シェーダをコンパイルして，生成する
//! @param [in] filename シェーダのファイル名
//! @param [in] entryPoint エントリーポイント名
//! @param [in] shader シェーダプロファイル名
//! @param [in] pDevice D3D11デバイスインタフェース
//! @param [out] ppShader 生成されたシェーダを格納する変数へのポインタ
//-----------------------------------------------------------------------
bool ShaderUtil::CreateComputeShaderFromFile
(
    const tchar* filename,
    const char* entryPoint,
    const char* shaderModel,
    ID3D11Device* pDevice,
    ID3D11ComputeShader** ppShader 
)
{
    ID3DBlob* pBlob;

    // ファイルからコンパイル
    if ( !CompileFromFile( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFile() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // シェーダ生成
    HRESULT hr = pDevice->CreateComputeShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateComputeShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // メモリ解放
    SAFE_RELEASE( pBlob );

    return true;   
}



} // namespace Graphics
} // namespace Asura