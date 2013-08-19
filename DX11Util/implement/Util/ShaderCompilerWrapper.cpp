//
// Copyright (C) 2010-2011, azarashin, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall Azarashin or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#include <DX11Util/Util/ShaderCompilerWrapper.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>

HRESULT WINAPI
D3DCompileFromFileWrapper(_In_ LPCWSTR pFileName,
                   _In_reads_opt_(_Inexpressible_(pDefines->Name != NULL)) CONST D3D_SHADER_MACRO* pDefines,
                   _In_opt_ ID3DInclude* pInclude,
                   _In_ LPCSTR pEntrypoint,
                   _In_ LPCSTR pTarget,
                   _In_ UINT Flags1,
                   _In_ UINT Flags2,
                   _Out_ ID3DBlob** ppCode,
                   _Out_opt_ ID3DBlob** ppErrorMsgs)
{
	char* data; 
	SIZE_T length; 

	{
		std::ifstream ifs(pFileName);
		length = ifs.seekg(0, std::ios::end).tellg();
	}

	FILE* fp; 
	fp = _wfsopen(pFileName, L"rb", _SH_DENYWR); 

	if(fp == 0) {
		return E_FAIL; 
	}

	data = new char[length]; 

	fread(data, 1, length, fp); 
	fclose(fp); 

	HRESULT ret = D3DCompile(data, length, NULL, pDefines, pInclude, pEntrypoint, pTarget, Flags1, Flags2, ppCode, ppErrorMsgs); 

	delete[] data; 

	return ret; 
}