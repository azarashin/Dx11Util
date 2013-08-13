//-----------------------------------------------------------------------
// File : AsdxShaderUtil.h
// Desc : Asura DirectX Shader Utility
// Copyright(c) Project Asura. All right resreved.
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// History
//-----------------------------------------------------------------------
// <---> 2011/07/23 [Pocol] �V�K�쐬
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
//! @brief �t�@�C������V�F�[�_���R���p�C������iMB��)
//! @param [in] filename �V�F�[�_�̃t�@�C����
//! @param [in] entryPoint �G���g���[�|�C���g��
//! @param [in] shader �V�F�[�_�v���t�@�C����
//! @param [out] ppBlobOut �o�C�g�R�[�h���i�[����C���^�t�F�[�X�ւ̃|�C���^
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

    // �t�@�C������R���p�C��
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
            // �G���[���b�Z�[�W�o��
            ELOG( "Error : %s", (char*)pErrorBlob->GetBufferPointer() );
         	OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        }
    }

    SAFE_RELEASE( pErrorBlob );

    return result;
}

//-----------------------------------------------------------------------
//! @brief �t�@�C������V�F�[�_���R���p�C������(Unicode��)
//! @param [in] filename �V�F�[�_�̃t�@�C����
//! @param [in] entryPoint �G���g���[�|�C���g��
//! @param [in] shader �V�F�[�_�v���t�@�C����
//! @param [out] ppBlobOut �o�C�g�R�[�h���i�[����C���^�t�F�[�X�ւ̃|�C���^
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

    // �t�@�C������R���p�C��
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
            // �G���[���b�Z�[�W�o��
			ELOG( "Error : %s\n", (char*)pErrorBlob->GetBufferPointer() );
         	OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        }
    }

    SAFE_RELEASE( pErrorBlob );

    return result;
}

//-----------------------------------------------------------------------
//! @brief �t�@�C������V�F�[�_���R���p�C������
//! @param [in] filename �V�F�[�_�̃t�@�C����
//! @param [in] entryPoint �G���g���[�|�C���g��
//! @param [in] shader �V�F�[�_�v���t�@�C����
//! @param [out] ppBlobOut �o�C�g�R�[�h���i�[����C���^�t�F�[�X�ւ̃|�C���^
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

    // �t�@�C������R���p�C��
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
            // �G���[���b�Z�[�W�o��
            ELOG( "Error : %s", (char*)pErrorBlob->GetBufferPointer() );
         	OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        }
    }

    SAFE_RELEASE( pErrorBlob );

    return result;
}

//-----------------------------------------------------------------------
//! @brief �t�@�C�����璸�_�V�F�[�_���R���p�C�����āC��������
//! @param [in] filename �V�F�[�_�̃t�@�C����
//! @param [in] entryPoint �G���g���[�|�C���g��
//! @param [in] shader �V�F�[�_�v���t�@�C����
//! @param [in] pDevice D3D11�f�o�C�X�C���^�t�F�[�X
//! @param [out] ppBlobOut �o�C�g�R�[�h���i�[����ϐ��ւ̃|�C���^
//! @param [out] ppShader �������ꂽ�V�F�[�_���i�[����ϐ��ւ̃|�C���^
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
    // �t�@�C������R���p�C��
    if ( !CompileFromFileA( filename, entryPoint, shaderModel, ppBlob ) )
    {
        ELOG( "Error : CompileFromFileA() Failed. filename = %s", filename );
        SAFE_RELEASE( (*ppBlob) );
        return false;
    }

    // �V�F�[�_����
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
//! @brief �t�@�C�����璸�_�V�F�[�_���R���p�C�����āC��������
//! @param [in] filename �V�F�[�_�̃t�@�C����
//! @param [in] entryPoint �G���g���[�|�C���g��
//! @param [in] shader �V�F�[�_�v���t�@�C����
//! @param [in] pDevice D3D11�f�o�C�X�C���^�t�F�[�X
//! @param [out] ppBlobOut �o�C�g�R�[�h���i�[����ϐ��ւ̃|�C���^
//! @param [out] ppShader �������ꂽ�V�F�[�_���i�[����ϐ��ւ̃|�C���^
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
    // �t�@�C������R���p�C��
    if ( !CompileFromFileW( filename, entryPoint, shaderModel, ppBlob ) )
    {
        ELOG( "Error : CompileFromFileW() Failed. filename = %s", filename );
        SAFE_RELEASE( (*ppBlob) );
        return false;
    }

    // �V�F�[�_����
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
//! @brief �t�@�C�����璸�_�V�F�[�_���R���p�C�����āC��������
//! @param [in] filename �V�F�[�_�̃t�@�C����
//! @param [in] entryPoint �G���g���[�|�C���g��
//! @param [in] shader �V�F�[�_�v���t�@�C����
//! @param [in] pDevice D3D11�f�o�C�X�C���^�t�F�[�X
//! @param [out] ppBlobOut �o�C�g�R�[�h���i�[����ϐ��ւ̃|�C���^
//! @param [out] ppShader �������ꂽ�V�F�[�_���i�[����ϐ��ւ̃|�C���^
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
    // �t�@�C������R���p�C��
    if ( !CompileFromFile( filename, entryPoint, shaderModel, ppBlob ) )
    {
        ELOG( "Error : CompileFromFile() Failed. filename = %s", filename );
        SAFE_RELEASE( (*ppBlob) );
        return false;
    }

    // �V�F�[�_����
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
//! @brief �t�@�C������s�N�Z���V�F�[�_���R���p�C�����āC��������
//! @param [in] filename �V�F�[�_�̃t�@�C����
//! @param [in] entryPoint �G���g���[�|�C���g��
//! @param [in] shader �V�F�[�_�v���t�@�C����
//! @param [in] pDevice D3D11�f�o�C�X�C���^�t�F�[�X
//! @param [out] ppShader �������ꂽ�V�F�[�_���i�[����ϐ��ւ̃|�C���^
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

    // �t�@�C������R���p�C��
    if ( !CompileFromFileA( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFileA() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // �V�F�[�_����
    HRESULT hr = pDevice->CreatePixelShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreatePixelShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // ���������
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief �t�@�C������s�N�Z���V�F�[�_���R���p�C�����āC��������
//! @param [in] filename �V�F�[�_�̃t�@�C����
//! @param [in] entryPoint �G���g���[�|�C���g��
//! @param [in] shader �V�F�[�_�v���t�@�C����
//! @param [in] pDevice D3D11�f�o�C�X�C���^�t�F�[�X
//! @param [out] ppShader �������ꂽ�V�F�[�_���i�[����ϐ��ւ̃|�C���^
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

    // �t�@�C������R���p�C��
    if ( !CompileFromFileW( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFileW() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // �V�F�[�_����
    HRESULT hr = pDevice->CreatePixelShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreatePixelShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // ���������
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief �t�@�C������s�N�Z���V�F�[�_���R���p�C�����āC��������
//! @param [in] filename �V�F�[�_�̃t�@�C����
//! @param [in] entryPoint �G���g���[�|�C���g��
//! @param [in] shader �V�F�[�_�v���t�@�C����
//! @param [in] pDevice D3D11�f�o�C�X�C���^�t�F�[�X
//! @param [out] ppShader �������ꂽ�V�F�[�_���i�[����ϐ��ւ̃|�C���^
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

    // �t�@�C������R���p�C��
    if ( !CompileFromFile( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFile() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // �V�F�[�_����
    HRESULT hr = pDevice->CreatePixelShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreatePixelShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // ���������
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief �t�@�C������W�I���g���V�F�[�_���R���p�C�����āC��������
//! @param [in] filename �V�F�[�_�̃t�@�C����
//! @param [in] entryPoint �G���g���[�|�C���g��
//! @param [in] shader �V�F�[�_�v���t�@�C����
//! @param [in] pDevice D3D11�f�o�C�X�C���^�t�F�[�X
//! @param [out] ppShader �������ꂽ�V�F�[�_���i�[����ϐ��ւ̃|�C���^
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

    // �t�@�C������R���p�C��
    if ( !CompileFromFileA( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFileA() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // �V�F�[�_����
    HRESULT hr = pDevice->CreateGeometryShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateGeometryShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // ���������
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief �t�@�C������W�I���g���V�F�[�_���R���p�C�����āC��������
//! @param [in] filename �V�F�[�_�̃t�@�C����
//! @param [in] entryPoint �G���g���[�|�C���g��
//! @param [in] shader �V�F�[�_�v���t�@�C����
//! @param [in] pDevice D3D11�f�o�C�X�C���^�t�F�[�X
//! @param [out] ppShader �������ꂽ�V�F�[�_���i�[����ϐ��ւ̃|�C���^
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

    // �t�@�C������R���p�C��
    if ( !CompileFromFileW( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFileW() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // �V�F�[�_����
    HRESULT hr = pDevice->CreateGeometryShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateGeometryShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // ���������
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief �t�@�C������W�I���g���V�F�[�_���R���p�C�����āC��������
//! @param [in] filename �V�F�[�_�̃t�@�C����
//! @param [in] entryPoint �G���g���[�|�C���g��
//! @param [in] shader �V�F�[�_�v���t�@�C����
//! @param [in] pDevice D3D11�f�o�C�X�C���^�t�F�[�X
//! @param [out] ppShader �������ꂽ�V�F�[�_���i�[����ϐ��ւ̃|�C���^
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

    // �t�@�C������R���p�C��
    if ( !CompileFromFile( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFile() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // �V�F�[�_����
    HRESULT hr = pDevice->CreateGeometryShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateGeometryShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // ���������
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief �t�@�C������n���V�F�[�_���R���p�C�����āC��������
//! @param [in] filename �V�F�[�_�̃t�@�C����
//! @param [in] entryPoint �G���g���[�|�C���g��
//! @param [in] shader �V�F�[�_�v���t�@�C����
//! @param [in] pDevice D3D11�f�o�C�X�C���^�t�F�[�X
//! @param [out] ppShader �������ꂽ�V�F�[�_���i�[����ϐ��ւ̃|�C���^
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

    // �t�@�C������R���p�C��
    if ( !CompileFromFileA( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFileA() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // �V�F�[�_����
    HRESULT hr = pDevice->CreateHullShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateHullShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // ���������
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief �t�@�C������n���V�F�[�_���R���p�C�����āC��������
//! @param [in] filename �V�F�[�_�̃t�@�C����
//! @param [in] entryPoint �G���g���[�|�C���g��
//! @param [in] shader �V�F�[�_�v���t�@�C����
//! @param [in] pDevice D3D11�f�o�C�X�C���^�t�F�[�X
//! @param [out] ppShader �������ꂽ�V�F�[�_���i�[����ϐ��ւ̃|�C���^
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

    // �t�@�C������R���p�C��
    if ( !CompileFromFileW( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFileW() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // �V�F�[�_����
    HRESULT hr = pDevice->CreateHullShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateHullShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // ���������
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief �t�@�C������n���V�F�[�_���R���p�C�����āC��������
//! @param [in] filename �V�F�[�_�̃t�@�C����
//! @param [in] entryPoint �G���g���[�|�C���g��
//! @param [in] shader �V�F�[�_�v���t�@�C����
//! @param [in] pDevice D3D11�f�o�C�X�C���^�t�F�[�X
//! @param [out] ppShader �������ꂽ�V�F�[�_���i�[����ϐ��ւ̃|�C���^
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

    // �t�@�C������R���p�C��
    if ( !CompileFromFile( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFile() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // �V�F�[�_����
    HRESULT hr = pDevice->CreateHullShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateHullShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // ���������
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief �t�@�C������̈�V�F�[�_���R���p�C�����āC��������
//! @param [in] filename �V�F�[�_�̃t�@�C����
//! @param [in] entryPoint �G���g���[�|�C���g��
//! @param [in] shader �V�F�[�_�v���t�@�C����
//! @param [in] pDevice D3D11�f�o�C�X�C���^�t�F�[�X
//! @param [out] ppShader �������ꂽ�V�F�[�_���i�[����ϐ��ւ̃|�C���^
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

    // �t�@�C������R���p�C��
    if ( !CompileFromFileA( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFileA() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // �V�F�[�_����
    HRESULT hr = pDevice->CreateDomainShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateDomainShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // ���������
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief �t�@�C������̈�V�F�[�_���R���p�C�����āC��������
//! @param [in] filename �V�F�[�_�̃t�@�C����
//! @param [in] entryPoint �G���g���[�|�C���g��
//! @param [in] shader �V�F�[�_�v���t�@�C����
//! @param [in] pDevice D3D11�f�o�C�X�C���^�t�F�[�X
//! @param [out] ppShader �������ꂽ�V�F�[�_���i�[����ϐ��ւ̃|�C���^
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

    // �t�@�C������R���p�C��
    if ( !CompileFromFileW( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFileW() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // �V�F�[�_����
    HRESULT hr = pDevice->CreateDomainShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateDomainShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // ���������
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief �t�@�C������̈�V�F�[�_���R���p�C�����āC��������
//! @param [in] filename �V�F�[�_�̃t�@�C����
//! @param [in] entryPoint �G���g���[�|�C���g��
//! @param [in] shader �V�F�[�_�v���t�@�C����
//! @param [in] pDevice D3D11�f�o�C�X�C���^�t�F�[�X
//! @param [out] ppShader �������ꂽ�V�F�[�_���i�[����ϐ��ւ̃|�C���^
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

    // �t�@�C������R���p�C��
    if ( !CompileFromFile( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFile() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // �V�F�[�_����
    HRESULT hr = pDevice->CreateDomainShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateDomainShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // ���������
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief �t�@�C�����牉�Z�V�F�[�_���R���p�C�����āC��������
//! @param [in] filename �V�F�[�_�̃t�@�C����
//! @param [in] entryPoint �G���g���[�|�C���g��
//! @param [in] shader �V�F�[�_�v���t�@�C����
//! @param [in] pDevice D3D11�f�o�C�X�C���^�t�F�[�X
//! @param [out] ppShader �������ꂽ�V�F�[�_���i�[����ϐ��ւ̃|�C���^
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

    // �t�@�C������R���p�C��
    if ( !CompileFromFileA( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFileA() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // �V�F�[�_����
    HRESULT hr = pDevice->CreateComputeShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateComputeShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // ���������
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief �t�@�C�����牉�Z�V�F�[�_���R���p�C�����āC��������
//! @param [in] filename �V�F�[�_�̃t�@�C����
//! @param [in] entryPoint �G���g���[�|�C���g��
//! @param [in] shader �V�F�[�_�v���t�@�C����
//! @param [in] pDevice D3D11�f�o�C�X�C���^�t�F�[�X
//! @param [out] ppShader �������ꂽ�V�F�[�_���i�[����ϐ��ւ̃|�C���^
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

    // �t�@�C������R���p�C��
    if ( !CompileFromFileW( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFileW() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // �V�F�[�_����
    HRESULT hr = pDevice->CreateComputeShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateComputeShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // ���������
    SAFE_RELEASE( pBlob );

    return true;   
}

//-----------------------------------------------------------------------
//! @brief �t�@�C�����牉�Z�V�F�[�_���R���p�C�����āC��������
//! @param [in] filename �V�F�[�_�̃t�@�C����
//! @param [in] entryPoint �G���g���[�|�C���g��
//! @param [in] shader �V�F�[�_�v���t�@�C����
//! @param [in] pDevice D3D11�f�o�C�X�C���^�t�F�[�X
//! @param [out] ppShader �������ꂽ�V�F�[�_���i�[����ϐ��ւ̃|�C���^
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

    // �t�@�C������R���p�C��
    if ( !CompileFromFile( filename, entryPoint, shaderModel, &pBlob ) )
    {
        ELOG( "Error : CompileFromFile() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // �V�F�[�_����
    HRESULT hr = pDevice->CreateComputeShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, ppShader );
    if ( FAILED( hr ) )
    {
        ELOG( "Error : ID3D11Device::CreateComputeShader() Failed. filename = %s", filename );
        SAFE_RELEASE( pBlob );
        return false;
    }

    // ���������
    SAFE_RELEASE( pBlob );

    return true;   
}



} // namespace Graphics
} // namespace Asura