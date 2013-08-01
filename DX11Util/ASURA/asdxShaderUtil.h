//-----------------------------------------------------------------------
// File : AsdxShaderUtil.h
// Desc : Asura DirectX Shader Utility
// Copyright(c) Project Asura. All right resreved.
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// History
//-----------------------------------------------------------------------
// <---> 2011/07/23 [Pocol] êVãKçÏê¨
// <End>

#ifndef __ASURA_DIRECTX_SHADER_UTIL_H__
#define __ASURA_DIRECTX_SHADER_UTIL_H__

//-----------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------
#include "asDef.h"
#include <d3d11.h>

namespace Asura {
namespace Graphics {

/////////////////////////////////////////////////////////////////////////
// ShaderUtil class
/////////////////////////////////////////////////////////////////////////
class ShaderUtil
{
    //-------------------------------------------------------------------
    // list of friend classes and methods
    //-------------------------------------------------------------------
    /* NOTHING */

private:
    //-------------------------------------------------------------------
    // private variables
    //-------------------------------------------------------------------
    /* NOTHING */

    //-------------------------------------------------------------------
    // private methods
    //-------------------------------------------------------------------
    /* NOTHING */

protected:
    //-------------------------------------------------------------------
    // protected variables
    //-------------------------------------------------------------------
    /* NOTHING */

    //-------------------------------------------------------------------
    // protected methods
    //-------------------------------------------------------------------
    /* NOTHING */

public:
    //-------------------------------------------------------------------
    // public variables
    //-------------------------------------------------------------------
    /* NOTHING */

    //-------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------

    // Compile From File
	static bool CompileFromFileA( 
		const char* filename, 
		const char* entryPoint,
		const char* shaderModel,
		ID3DBlob** ppBlobOut );
	static bool CompileFromFileW(
		const wchar_t* filename,
		const char* entryPoint,
		const char* shaderModel,
		ID3DBlob** ppBlobOut );
	static bool CompileFromFile(
		const tchar* filename,
		const char* entryPoint,
		const char* shaderModel,
		ID3DBlob** ppBlobOut );

    // Create Vertex Shader From File
    static bool CreteVertexShaderFromFileA(
        const char* filename,
        const char* entryPoint,
        const char* shaderModel,
        ID3D11Device* pDevice,
        ID3DBlob** ppBlob,
        ID3D11VertexShader** ppShader );
    static bool CreateVertexShaderFromFileW(
        const wchar_t* filename,
        const char* entryPoint,
        const char* shaderModel,
        ID3D11Device* pDevice,
        ID3DBlob** ppBlob,
        ID3D11VertexShader** ppShader );
    static bool CreateVertexShaderFromFile(
        const tchar* filename,
        const char* entryPoint,
        const char* shaderModel,
        ID3D11Device* pDevice,
        ID3DBlob** ppBlob,
        ID3D11VertexShader** ppShader );

    // Create Pixel Shader From File
    static bool CreatePixelShaderFromFileA(
        const char* filename,
        const char* entryPoint,
        const char* shaderModel,
        ID3D11Device* pDevice,
        ID3D11PixelShader** ppShader );
    static bool CreatePixelShaderFromFileW(
        const wchar_t* filename,
        const char* entryPoint,
        const char* shaderModel,
        ID3D11Device* pDevice,
        ID3D11PixelShader** ppShader );
    static bool CreatePixelShaderFromFile(
        const tchar* filename,
        const char* entryPoint,
        const char* shaderModel,
        ID3D11Device* pDevice,
        ID3D11PixelShader** ppShader );

    // Create Geometry Shader From File
    static bool CreateGeometryShaderFromFileA(
        const char* filename,
        const char* entryPoint,
        const char* shaderModel,
        ID3D11Device* pDevice,
        ID3D11GeometryShader** ppShader );
    static bool CreateGeometryShaderFromFileW(
        const wchar_t* filename,
        const char* entryPoint,
        const char* shaderModel,
        ID3D11Device* pDevice,
        ID3D11GeometryShader** ppShader );
   static bool CreateGeometryShaderFromFile(
        const tchar* filename,
        const char* entryPoint,
        const char* shaderModel,
        ID3D11Device* pDevice,
        ID3D11GeometryShader** ppShader );

   // Create Hull Shader From File
   static bool CreateHullShaderFromFileA(
        const char* filename,
        const char* entryPoint,
        const char* shaderModel,
        ID3D11Device* pDevice,
        ID3D11HullShader** ppShader );
   static bool CreateHullShaderFromFileW(
        const wchar_t* filename,
        const char* entryPoint,
        const char* shaderModel,
        ID3D11Device* pDevice,
        ID3D11HullShader** ppShader );
   static bool CreateHullShaderFromFile(
        const tchar* filename,
        const char* entryPoint,
        const char* shaderModel,
        ID3D11Device* pDevice,
        ID3D11HullShader** ppShader );

   // Create Domain Shader From File
   static bool CreateDomainShaderFromFileA(
        const char* filename,
        const char* entryPoint,
        const char* shaderModel,
        ID3D11Device* pDevice,
        ID3D11DomainShader** ppShader );
   static bool CreateDomainShaderFromFileW(
        const wchar_t* filename,
        const char* entryPoint,
        const char* shaderModel,
        ID3D11Device* pDevice,
        ID3D11DomainShader** ppShader );
   static bool CreateDomainShaderFromFile(
        const tchar* filename,
        const char* entryPoint,
        const char* shaderModel,
        ID3D11Device* pDevice,
        ID3D11DomainShader** ppShader );

   // Create Compute Shader From File
   static bool CreateComputeShaderFromFileA(
      const char* filename,
      const char* entryPoint,
      const char* shaderModel,
      ID3D11Device* pDevice,
      ID3D11ComputeShader** ppShader );
   static bool CreateComputeShaderFromFileW(
      const wchar_t* filename,
      const char* entryPoint,
      const char* shaderModel,
      ID3D11Device* pDevice,
      ID3D11ComputeShader** ppShader );
   static bool CreateComputeShaderFromFile(
      const tchar* filename,
      const char* entryPoint,
      const char* shaderModel,
      ID3D11Device* pDevice,
      ID3D11ComputeShader** ppShader );
};


} // namespace Graphics
} // namespace Asura

#endif//__ASURA_DIRECTX_SHADER_UTIL_H__