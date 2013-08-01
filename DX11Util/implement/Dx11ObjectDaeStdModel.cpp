//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                               for PIT 
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

#include <DX11Util/Dx11ObjectDaeStdModel.h>


#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

Dx11ObjectDaeStdModel::Dx11ObjectDaeStdModel(ID3D11Device* _pd3dDevice, const char* _filename)
	: Dx11ObjectDae()
{
	pd3dDevice = _pd3dDevice; 
	filename = _filename; 
}


Dx11ObjectDaeStdModel::~Dx11ObjectDaeStdModel(void)
{
}





unsigned int Dx11ObjectDaeStdModel::getMaxOffset( domInput_local_offset_Array &input_array ) {
	unsigned int maxOffset = 0;
	for ( unsigned int i = 0; i < input_array.getCount(); i++ ) {
		if ( input_array[i]->getOffset() > maxOffset ) {
			maxOffset = (unsigned int)input_array[i]->getOffset();
		}
	}
	return maxOffset;
}

void Dx11ObjectDaeStdModel::Setup()
{
	LoadDAE(filename.c_str(), pd3dDevice); 

}

void Dx11ObjectDaeStdModel::Term()
{
	m_dwIndecies.clear();
	m_dwVertices.clear();

	for(int i=0;i<(int)m_pTexture.size();i++)
	{
		if(m_pTexture[i])
			m_pTexture[i]->Release();
	}
	m_pTexture.clear();

	for(int i=0;i<(int)m_pIB.size();i++)
	{
		m_pIB[i]->Release();
	}
	m_pIB.clear();
	
	for(int i=0;i<(int)m_pVB.size();i++)
	{
		m_pVB[i]->Release();
	}
	m_pVB.clear();

}

HRESULT Dx11ObjectDaeStdModel::LoadModel(DAE* dae, ID3D11Device* pd3dDevice) 
{
	HRESULT hr = S_OK;
	int currentGeometry; 
	int currentTriangle; 
	int currentPolylist; 
	int currentPolygones; 
	int currentImage; 

	int geometryElementCount = (int)(dae->getDatabase()->getElementCount(NULL, "geometry", NULL));
	for(currentGeometry=0;currentGeometry<geometryElementCount;currentGeometry++) {
		domGeometry *thisGeometry;
		dae->getDatabase()->getElement((daeElement**)&thisGeometry,currentGeometry, NULL, "geometry");

		// get mesh
		domMesh *thisMesh = thisGeometry->getMesh();

		// Triangle
		int triangleElementCounr = (int)(thisMesh->getTriangles_array().getCount());
		for(currentTriangle = 0;currentTriangle < triangleElementCounr; currentTriangle++)
		{
			domTriangles* thisTriangles = thisMesh->getTriangles_array().get(0);

			ConstructTriangles(thisMesh, thisTriangles, pd3dDevice);
		}

		// Plylist
		int polylistElementCount = (int)(thisMesh->getPolylist_array().getCount());
		for(currentPolylist = 0; currentPolylist < polylistElementCount; currentPolylist++) {		
			domPolylist *thisPolylist = thisMesh->getPolylist_array().get(0);

			ConstructPolylist( thisMesh, thisPolylist, pd3dDevice );
		}

		//Polygons
		int polygonesElementCount = (int)(thisMesh->getPolygons_array().getCount());
		for(currentPolygones = 0; currentPolygones < polygonesElementCount; currentPolygones++) {
			domPolygons* thisPolygons = thisMesh->getPolygons_array().get(0);

			ConstructPolygon( thisMesh, thisPolygons, pd3dDevice );
		}
	}

	// texture
	int imageElementCount = (int)(dae->getDatabase()->getElementCount(NULL, "image", NULL));
	for(currentImage=0;currentImage<imageElementCount;currentImage++)
	{
		domImage* thisImage;
		dae->getDatabase()->getElement((daeElement**)&thisImage,currentImage, NULL, "image");
		ID3D11ShaderResourceView* pTexture = NULL;

//		daeString name =  thisImage->getInit_from()->getValue().getFile();
		daeString name =  thisImage->getInit_from()->getRef()->getValue().getPath();

		WCHAR filepath[256];
		MultiByteToWideChar(CP_UTF8, 0, name, -1, filepath, (int)sizeof(filepath));
		D3DX11CreateShaderResourceViewFromFile(pd3dDevice, filepath, NULL, NULL, &pTexture, &hr);

		m_pTexture.push_back(pTexture);
	}

	return hr;
}

HRESULT Dx11ObjectDaeStdModel::ConstructTriangles(domMesh *thisMesh, domTriangles *thisTriangles, ID3D11Device* pd3dDevice)
{
	HRESULT hr = S_OK;

	ID3D11Buffer* pVB = NULL; 
	ID3D11Buffer* pIB = NULL;
	tDAEVERTEX* pV;

	// number of vertex
	m_dwVertices.push_back( (DWORD)thisMesh->getSource_array()[0]->getFloat_array()->getCount()/3 );
	DWORD dwSourceElements = thisMesh->getSource_array().getCount();


	pV = new tDAEVERTEX[dwSourceElements]; 

	for(int i=0;i<m_dwVertices.back();i++) {
		pV[i].vPos = XMFLOAT3(
			thisMesh->getSource_array()[0]->getFloat_array()->getValue().get(i*3),
			thisMesh->getSource_array()[0]->getFloat_array()->getValue().get(i*3+1),
			thisMesh->getSource_array()[0]->getFloat_array()->getValue().get(i*3+2)
			);

		if(dwSourceElements>=2) {
			pV[i].vNormal = XMFLOAT3(
				thisMesh->getSource_array()[1]->getFloat_array()->getValue().get(i*3),
				thisMesh->getSource_array()[1]->getFloat_array()->getValue().get(i*3+1),
				thisMesh->getSource_array()[1]->getFloat_array()->getValue().get(i*3+2)
				);
		} else {
			pV[i].vNormal = XMFLOAT3(0,0,0);	// no source of normal vector. 
		}

		// dummy uv-pos. 
		pV[i].tu = 0.0f;
		pV[i].tv = 0.0f;
	}

	int numberOfInputs = (int)getMaxOffset(thisTriangles->getInput_array()) +1;	// number of offset
	int numberOfTriangles = (int)(thisTriangles->getP()->getValue().getCount());		// number of triangle
 	m_dwIndecies.push_back( numberOfTriangles / (numberOfInputs) );
	
	// offset of index
	DWORD offset = 0;
	int texoffset = -255;
	for(int i=0;i<thisTriangles->getInput_array().getCount();i++)
	{
		if(strcmpi(thisTriangles->getInput_array()[i]->getSemantic(), "VERTEX")==0)
			offset = thisTriangles->getInput_array()[i]->getOffset();
		if(strcmpi(thisTriangles->getInput_array()[i]->getSemantic(), "TEXCOORD")==0)
			texoffset = thisTriangles->getInput_array()[i]->getOffset();
	}

	UINT* pI = new UINT[m_dwIndecies.back()];

	for(int i=0;i<m_dwIndecies.back();i++)
	{
		pI[i] = thisTriangles->getP()->getValue().get(i*numberOfInputs+offset);

		if(texoffset!=-255)
		{
			// when texture is enable. 
			pV[pI[i]].tu =  thisMesh->getSource_array()[2]->getFloat_array()->getValue().get(
				thisTriangles->getP()->getValue().get(i*numberOfInputs+texoffset)*2
				);
			pV[pI[i]].tv =  thisMesh->getSource_array()[2]->getFloat_array()->getValue().get(
				thisTriangles->getP()->getValue().get(i*numberOfInputs+texoffset)*2+1
				);
		}
	}

	D3D11_BUFFER_DESC xyzBufferDesc;
	xyzBufferDesc.Usage          = D3D11_USAGE_DEFAULT; 
	xyzBufferDesc.ByteWidth      = sizeof(tDAEVERTEX) * dwSourceElements; 
	xyzBufferDesc.BindFlags      = D3D11_BIND_VERTEX_BUFFER; 
	xyzBufferDesc.CPUAccessFlags = 0;
	xyzBufferDesc.MiscFlags      = 0;
	xyzBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA xyzSubData;
	xyzSubData.pSysMem          = pV; 
	xyzSubData.SysMemPitch      = 0;
	xyzSubData.SysMemSlicePitch = 0;

	hr = pd3dDevice->CreateBuffer(&xyzBufferDesc, &xyzSubData, &pVB);
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateBuffer", hr);
	}



	D3D11_BUFFER_DESC idxBufferDesc;
	idxBufferDesc.Usage          = D3D11_USAGE_DEFAULT; 
	idxBufferDesc.ByteWidth      = sizeof(UINT)*m_dwIndecies.back(); 
	idxBufferDesc.BindFlags      = D3D11_BIND_INDEX_BUFFER; 
	idxBufferDesc.CPUAccessFlags = 0;
	idxBufferDesc.MiscFlags      = 0;
	idxBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA idxSubData;
	idxSubData.pSysMem          = pI; 
	idxSubData.SysMemPitch      = 0;
	idxSubData.SysMemSlicePitch = 0;

	hr = pd3dDevice->CreateBuffer(&idxBufferDesc, &idxSubData, &pIB);
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateBuffer", hr);
	}

	delete[] pI; 
	delete[] pV; 


	m_pVB.push_back( pVB );

	m_pIB.push_back( pIB );

	return hr;
}

HRESULT Dx11ObjectDaeStdModel::ConstructPolylist(domMesh *thisMesh, domPolylist *thisPolylist, ID3D11Device *pd3dDevice)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* pVB = NULL; 
	ID3D11Buffer* pIB = NULL;
	tDAEVERTEX* pV;

	// number of vertex
	m_dwVertices.push_back( (DWORD)thisMesh->getSource_array()[0]->getFloat_array()->getCount()/3 );
	DWORD dwSourceElements = thisMesh->getSource_array().getCount();

	pV = new tDAEVERTEX[dwSourceElements]; 

	for(int i=0;i<m_dwVertices.back();i++)
	{
		pV[i].vPos = XMFLOAT3(
			thisMesh->getSource_array()[0]->getFloat_array()->getValue().get(i*3),
			thisMesh->getSource_array()[0]->getFloat_array()->getValue().get(i*3+1),
			thisMesh->getSource_array()[0]->getFloat_array()->getValue().get(i*3+2)
			);

		if(dwSourceElements>=2)
		{
			pV[i].vNormal = XMFLOAT3(
				thisMesh->getSource_array()[1]->getFloat_array()->getValue().get(i*3),
				thisMesh->getSource_array()[1]->getFloat_array()->getValue().get(i*3+1),
				thisMesh->getSource_array()[1]->getFloat_array()->getValue().get(i*3+2)
				);
		}
		else {
			pV[i].vNormal = XMFLOAT3(0,0,0);
		}

		pV[i].tu = 0.0f;
		pV[i].tv = 0.0f;
	}

	domTriangles *thisTriangles = (domTriangles *)thisMesh->createAndPlace("triangles");
	unsigned int triangles = 0;
	thisTriangles->setMaterial(thisPolylist->getMaterial());
	domP* p_triangles = (domP*)thisTriangles->createAndPlace("p");

	for(int i=0; i<(int)(thisPolylist->getInput_array().getCount()); i++)
	{
		thisTriangles->placeElement( thisPolylist->getInput_array()[i]->clone() );
	}

	// Get the number of inputs and primitives for the polygons array
	int numberOfInputs = (int)getMaxOffset(thisPolylist->getInput_array()) + 1;
	int numberOfPrimitives = (int)(thisPolylist->getVcount()->getValue().getCount());

	unsigned int offset = 0;

	for(int j = 0; j < numberOfPrimitives; j++)
	{	
		int triangleCount = thisPolylist->getVcount()->getValue()[j] -2;
		// Write out the primitives as triangles, just fan using the first element as the base
		int idx = numberOfInputs;
		for(int k = 0; k < triangleCount; k++)
		{
			// First vertex
			for(int l = 0; l < numberOfInputs; l++)
			{
				int a = thisPolylist->getP()->getValue()[offset + l];
				float b;
				memcpy(&b, &a, sizeof(float));
				p_triangles->getValue().append(thisPolylist->getP()->getValue()[offset + l]);
			}
			// Second vertex
			for(int l = 0; l < numberOfInputs; l++)
			{
				p_triangles->getValue().append(thisPolylist->getP()->getValue()[offset + idx + l]);
			}
			// Third vertex
			idx += numberOfInputs;
			for(int l = 0; l < numberOfInputs; l++)
			{
				p_triangles->getValue().append(thisPolylist->getP()->getValue()[offset + idx + l]);
			}
			//thisTriangles->setCount(thisTriangles->getCount()+1);
			triangles++;
		}
		offset += thisPolylist->getVcount()->getValue()[j] * numberOfInputs;
	}

	thisTriangles->setCount( triangles );
	m_dwIndecies.push_back( p_triangles->getValue().getCount() / numberOfInputs);

	DWORD veroffset = 0;
	int texoffset = -255, noroffset = -255;
	for(int i=0;i<thisTriangles->getInput_array().getCount();i++)
	{
		if(strcmpi(thisTriangles->getInput_array()[i]->getSemantic(), "VERTEX")==0)
			veroffset = thisTriangles->getInput_array()[i]->getOffset();
		if(strcmpi(thisTriangles->getInput_array()[i]->getSemantic(), "TEXCOORD")==0)
			texoffset = thisTriangles->getInput_array()[i]->getOffset();
		if(strcmpi(thisTriangles->getInput_array()[i]->getSemantic(), "NORMAL")==0)
			noroffset = thisTriangles->getInput_array()[i]->getOffset();
	}


	UINT* pI = new UINT[m_dwIndecies.back()];

	for(DWORD i =0;i<m_dwIndecies.back();i++)
	{
		pI[i] = (UINT)p_triangles->getValue()[i*numberOfInputs+veroffset];

		if(texoffset!=-255)
		{
			pV[pI[i]].tu =  thisMesh->getSource_array()[2]->getFloat_array()->getValue().get(
				thisTriangles->getP()->getValue().get(i*numberOfInputs+texoffset)*2
				);
			pV[pI[i]].tv =  thisMesh->getSource_array()[2]->getFloat_array()->getValue().get(
				thisTriangles->getP()->getValue().get(i*numberOfInputs+texoffset)*2+1
				);
		}

		if(noroffset!=-255)
		{
			pV[pI[i]].vNormal = XMFLOAT3(
				thisMesh->getSource_array()[1]->getFloat_array()->getValue().get(
				thisTriangles->getP()->getValue().get(i*numberOfInputs+noroffset)*3
				),
				thisMesh->getSource_array()[1]->getFloat_array()->getValue().get(
				thisTriangles->getP()->getValue().get(i*numberOfInputs+noroffset)*3+1
				),
				thisMesh->getSource_array()[1]->getFloat_array()->getValue().get(
				thisTriangles->getP()->getValue().get(i*numberOfInputs+noroffset)*3+2
				)
			);
		}
	}



	D3D11_BUFFER_DESC xyzBufferDesc;
	xyzBufferDesc.Usage          = D3D11_USAGE_DEFAULT; 
	xyzBufferDesc.ByteWidth      = sizeof(tDAEVERTEX) * dwSourceElements; 
	xyzBufferDesc.BindFlags      = D3D11_BIND_VERTEX_BUFFER; 
	xyzBufferDesc.CPUAccessFlags = 0;
	xyzBufferDesc.MiscFlags      = 0;
	xyzBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA xyzSubData;
	xyzSubData.pSysMem          = pV; 
	xyzSubData.SysMemPitch      = 0;
	xyzSubData.SysMemSlicePitch = 0;

	hr = pd3dDevice->CreateBuffer(&xyzBufferDesc, &xyzSubData, &pVB);
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateBuffer", hr);
	}


	D3D11_BUFFER_DESC idxBufferDesc;
	idxBufferDesc.Usage          = D3D11_USAGE_DEFAULT; 
	idxBufferDesc.ByteWidth      = sizeof(UINT)*m_dwIndecies.back(); 
	idxBufferDesc.BindFlags      = D3D11_BIND_INDEX_BUFFER; 
	idxBufferDesc.CPUAccessFlags = 0;
	idxBufferDesc.MiscFlags      = 0;
	idxBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA idxSubData;
	idxSubData.pSysMem          = pI; 
	idxSubData.SysMemPitch      = 0;
	idxSubData.SysMemSlicePitch = 0;

	hr = pd3dDevice->CreateBuffer(&idxBufferDesc, &idxSubData, &pIB);
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateBuffer", hr);
	}

	delete[] pI; 
	delete[] pV; 


	m_pVB.push_back( pVB );

	m_pIB.push_back( pIB );


	return hr;
}

HRESULT Dx11ObjectDaeStdModel::ConstructPolygon(domMesh *thisMesh, domPolygons *thisPolygons, ID3D11Device *pd3dDevice)
{
	HRESULT hr = S_OK;

	ID3D11Buffer* pVB = NULL; 
	ID3D11Buffer* pIB = NULL;
	tDAEVERTEX* pV;

	// number of vertex
	m_dwVertices.push_back( thisMesh->getSource_array()[0]->getFloat_array()->getCount()/3 );
	DWORD dwSourceElements = thisMesh->getSource_array().getCount();

	pV = new tDAEVERTEX[dwSourceElements]; 

	for(int i=0;i<m_dwVertices.back();i++)
	{
		pV[i].vPos = XMFLOAT3(
			thisMesh->getSource_array()[0]->getFloat_array()->getValue().get(i*3),
			thisMesh->getSource_array()[0]->getFloat_array()->getValue().get(i*3+1),
			thisMesh->getSource_array()[0]->getFloat_array()->getValue().get(i*3+2)
			);

		if(dwSourceElements>=2)
		{
			pV[i].vNormal = XMFLOAT3(
				thisMesh->getSource_array()[1]->getFloat_array()->getValue().get(i*3),
				thisMesh->getSource_array()[1]->getFloat_array()->getValue().get(i*3+1),
				thisMesh->getSource_array()[1]->getFloat_array()->getValue().get(i*3+2)
				);
		}
		else {
			pV[i].vNormal = XMFLOAT3(0,0,0);	// no normal vector
		}

		pV[i].tu = 0.0f;
		pV[i].tv = 0.0f;
	}

	// make triangles
	domTriangles *thisTriangles = (domTriangles *)thisMesh->createAndPlace("triangles");
	thisTriangles->setCount( 0 );
	thisTriangles->setMaterial(thisPolygons->getMaterial());
	domP* p_triangles = (domP*)thisTriangles->createAndPlace("p");

	// Give the new <triangles> the same <input> and <parameters> as the old <polygons>
	for(int i=0; i<(int)(thisPolygons->getInput_array().getCount()); i++)
	{
		thisTriangles->placeElement( thisPolygons->getInput_array()[i]->clone() );
	}
	// Get the number of inputs and primitives for the polygons array
	int numberOfInputs = (int)getMaxOffset(thisPolygons->getInput_array()) +1;
	int numberOfPrimitives = (int)(thisPolygons->getP_array().getCount());

	// Triangulate all the primitives, this generates all the triangles in a single <p> element
	for(int j = 0; j < numberOfPrimitives; j++)
	{
		// Check the polygons for consistancy (some exported files have had the wrong number of indices)
		domP * thisPrimitive = thisPolygons->getP_array()[j];
		int elementCount = (int)(thisPrimitive->getValue().getCount());
		if((elementCount%numberOfInputs) != 0)
		{
//			cerr<<"Primitive "<<j<<" has an element count "<<elementCount<<" not divisible by the number of inputs "<<numberOfInputs<<"\n";
			continue;
		}
		else
		{
			int triangleCount = (elementCount/numberOfInputs)-2;
			// Write out the primitives as triangles, just fan using the first element as the base
			int idx = numberOfInputs;
			for(int k = 0; k < triangleCount; k++)
			{
				// First vertex
				for(int l = 0; l < numberOfInputs; l++)
				{
					p_triangles->getValue().append(thisPrimitive->getValue()[l]);
				}
				// Second vertex
				for(int l = 0; l < numberOfInputs; l++)
				{
					p_triangles->getValue().append(thisPrimitive->getValue()[idx + l]);
				}
				// Third vertex
				idx += numberOfInputs;
				for(int l = 0; l < numberOfInputs; l++)
				{
					p_triangles->getValue().append(thisPrimitive->getValue()[idx + l]);
				}
				thisTriangles->setCount(thisTriangles->getCount()+1);
			}
		}
	}
	
	m_dwIndecies.push_back( p_triangles->getValue().getCount() / numberOfInputs);

	DWORD veroffset = 0;
	int texoffset = -255, noroffset = -255;
	for(int i=0;i<thisTriangles->getInput_array().getCount();i++)
	{
		if(strcmpi(thisTriangles->getInput_array()[i]->getSemantic(), "VERTEX")==0)
			veroffset = thisTriangles->getInput_array()[i]->getOffset();
		if(strcmpi(thisTriangles->getInput_array()[i]->getSemantic(), "TEXCOORD")==0)
			texoffset = thisTriangles->getInput_array()[i]->getOffset();
		if(strcmpi(thisTriangles->getInput_array()[i]->getSemantic(), "NORMAL")==0)
			noroffset = thisTriangles->getInput_array()[i]->getOffset();
	}


	UINT* pI = new UINT[m_dwIndecies.back()];

	for(DWORD i =0;i<m_dwIndecies.back();i++)
	{
		pI[i] = (UINT)p_triangles->getValue()[i*numberOfInputs+veroffset];

		if(texoffset!=-255)
		{
			pV[pI[i]].tu =  thisMesh->getSource_array()[2]->getFloat_array()->getValue().get(
				thisTriangles->getP()->getValue().get(i*numberOfInputs+texoffset)*2
				);
			pV[pI[i]].tv =  thisMesh->getSource_array()[2]->getFloat_array()->getValue().get(
				thisTriangles->getP()->getValue().get(i*numberOfInputs+texoffset)*2+1
				);
		}

		if(noroffset!=-255)
		{
			pV[pI[i]].vNormal = XMFLOAT3(
				thisMesh->getSource_array()[1]->getFloat_array()->getValue().get(
				thisTriangles->getP()->getValue().get(i*numberOfInputs+noroffset)*3
				),
				thisMesh->getSource_array()[1]->getFloat_array()->getValue().get(
				thisTriangles->getP()->getValue().get(i*numberOfInputs+noroffset)*3+1
				),
				thisMesh->getSource_array()[1]->getFloat_array()->getValue().get(
				thisTriangles->getP()->getValue().get(i*numberOfInputs+noroffset)*3+2
				)
			);
		}
	}

	D3D11_BUFFER_DESC xyzBufferDesc;
	xyzBufferDesc.Usage          = D3D11_USAGE_DEFAULT; 
	xyzBufferDesc.ByteWidth      = sizeof(tDAEVERTEX) * dwSourceElements; 
	xyzBufferDesc.BindFlags      = D3D11_BIND_VERTEX_BUFFER; 
	xyzBufferDesc.CPUAccessFlags = 0;
	xyzBufferDesc.MiscFlags      = 0;
	xyzBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA xyzSubData;
	xyzSubData.pSysMem          = pV; 
	xyzSubData.SysMemPitch      = 0;
	xyzSubData.SysMemSlicePitch = 0;

	hr = pd3dDevice->CreateBuffer(&xyzBufferDesc, &xyzSubData, &pVB);
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateBuffer", hr);
	}


	D3D11_BUFFER_DESC idxBufferDesc;
	idxBufferDesc.Usage          = D3D11_USAGE_DEFAULT; 
	idxBufferDesc.ByteWidth      = sizeof(UINT)*m_dwIndecies.back(); 
	idxBufferDesc.BindFlags      = D3D11_BIND_INDEX_BUFFER; 
	idxBufferDesc.CPUAccessFlags = 0;
	idxBufferDesc.MiscFlags      = 0;
	idxBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA idxSubData;
	idxSubData.pSysMem          = pI; 
	idxSubData.SysMemPitch      = 0;
	idxSubData.SysMemSlicePitch = 0;

	hr = pd3dDevice->CreateBuffer(&idxBufferDesc, &idxSubData, &pIB);
	if (FAILED(hr)) {
		return DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateBuffer", hr);
	}

	delete[] pI; 
	delete[] pV; 

	m_pVB.push_back( pVB );


	m_pIB.push_back( pIB );

	return hr;
}

HRESULT Dx11ObjectDaeStdModel::Render(ID3D11DeviceContext* pd3d_dc)
{
	HRESULT hr = S_OK;

	for(int i=0;i<(int)m_pVB.size();i++)
	{
		if(!m_pVB[i] || !m_pIB[i]) {
			continue ;
		}

		UINT strides[1] = {sizeof(tDAEVERTEX)}; 
		UINT offsets[1] = {0}; 
		ID3D11Buffer* vbs[1] = {m_pVB[i]}; 

		pd3d_dc->IASetVertexBuffers(0, m_dwVertices[i], vbs, strides, offsets); 
		pd3d_dc->IASetIndexBuffer(m_pIB[i], DXGI_FORMAT_R32_UINT, 0); 
		pd3d_dc->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); 
		pd3d_dc->DrawIndexed( m_dwIndecies[i]/3, 0, 0); 

//		pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVertices[i], 0, m_dwIndecies[i]/3);

//		pd3dDevice->SetTexture(0, NULL);
	}

	return hr;
}

BOOL Dx11ObjectDaeStdModel::GetVertexNum(unsigned int idx, unsigned int* num)
{
	*num = m_dwVertices[idx]; 
	return true; 
}

BOOL Dx11ObjectDaeStdModel::GetIndexNum(unsigned int idx, unsigned int* num)
{
	*num = m_dwIndecies[idx]; 
	return true; 
}


BOOL Dx11ObjectDaeStdModel::GetBufNum(unsigned int* num)
{
	*num = m_pVB.size(); 
	return TRUE; 
}

BOOL Dx11ObjectDaeStdModel::GetTexture(unsigned int num,  ID3D11ShaderResourceView** pTexture)
{
	if(num > m_pTexture.size() || m_pTexture.size()==0)
	{
		*pTexture = NULL;
		return FALSE;
	}

	*pTexture = m_pTexture[num];
	return TRUE;
}

BOOL Dx11ObjectDaeStdModel::GetVertexBuf(unsigned int num, ID3D11Buffer** buf)
{
	if(num > m_pVB.size() || m_pVB.size()==0)
	{
		*buf = NULL;
		return FALSE;
	}

	*buf = m_pVB[num];
	return TRUE;
}


BOOL Dx11ObjectDaeStdModel::GetWeightBuf(unsigned int num, ID3D11Buffer** buf)
{
	*buf = NULL;
	return FALSE;
}

BOOL Dx11ObjectDaeStdModel::GetIndexBuf(unsigned int num, ID3D11Buffer** buf)
{
	if(num > m_pIB.size() || m_pIB.size()==0)
	{
		*buf = NULL;
		return FALSE;
	}

	*buf = m_pIB[num];
	return TRUE;
}
