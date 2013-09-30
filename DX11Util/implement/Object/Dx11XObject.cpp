#include <DX11Util/Object/Dx11XObject.h>
#include <asModel.h>

#include <map>

#include <DX11Util/Util/DDSTextureLoader.h>


using namespace Asura::Graphics; 

Dx11XObject::Dx11XObject(const char* _filename)
{
	filename = _filename; 
}

Dx11XObject::~Dx11XObject()
{
}

struct comparer
{
    public:
    bool operator()(const std::string x, const std::string y)
    {
         return (x.compare(y)<0);
    }
};

void Dx11XObject::Setup(Dx11Context* _context)
{
	IModelLoader* pLoader = null;
    Asura::Graphics::IModel*    mpInterface;   //!< インターフェース
	ID3D11Device* pd3dDevice = _context->GetDXDevice(); 
	unsigned int i, j, k; 

	// ローダインタフェース生成
	try 
	{
		pLoader = ModelLoaderFactory::Create( ModelLoaderFactory::MODEL_TYPE_X );
	}
	catch( std::bad_alloc ba )
	{
		ELOG( "Error : ModelLoaderFactory::Create() Failed. reason = %s", ba.what() );
		throw;
	}

	// ロード処理
	if ( !pLoader->Load( filename.c_str() ) )
	{
		ELOG( "Error : IModelLoader::Load() Failed. filename = %s", filename );
		// ローダー破棄
		pLoader->Release();
		delete pLoader;
		pLoader = null;

		throw;
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
				throw;
			}
			break;
		}
	}
	assert( mpInterface != null );


	std::vector< IMesh* > mesh_list = mpInterface->GetMeshes();
    std::vector< IAnimationClip* > animation_list = mpInterface->GetClips(); 

	buffer_info.assign(mesh_list.size(), Dx11XBufferInfo()); 
	material.assign(mesh_list.size(), std::vector<Dx11XMaterialStandard>()); 
	sub_mesh.assign(mesh_list.size(), std::vector<Dx11XSubMesh>()); 


	std::map<std::string, int, comparer> tex_map; 

	tex_map.clear(); 

	for(i=0;i<mesh_list.size();i++) {
		IMesh* mesh = mesh_list[i]; 

		std::vector< Asura::Math::Vector3 > pos = mesh->GetPositions(); 
		std::vector< Asura::Math::Vector3 > nor = mesh->GetNormals(); 
		std::vector< Asura::Math::Vector2 > uv = mesh->GetTexCoords(); 

		if(pos.size() == 0) {
			buffer_info[i].vb = 0; 
			buffer_info[i].ib = 0; 
			buffer_info[i].vtx_num = 0; 
			buffer_info[i].idx_num = 0; 
			continue; 
		}

		if(pos.size() != nor.size()) {
			throw; 
		}

		buffer_info[i].ref_frame = mesh->GetRefBone(); 

		tDAEVERTEX* v = new tDAEVERTEX[pos.size()]; 

		for(j=0;j<pos.size();j++) {
			v[j].vPos = XMFLOAT3(pos[j].x, pos[j].y, pos[j].z); 
		}
		if(pos.size() == nor.size()) {
			for(j=0;j<pos.size();j++) {
				v[j].vNormal = XMFLOAT3(nor[j].x, nor[j].y, nor[j].z); 
			}
		}
		if(pos.size() == uv.size()) {
			for(j=0;j<pos.size();j++) {
				v[j].tu = uv[j].x; 
				v[j].tv = uv[j].y; 
			}
		}


		// **********************************************************
		// Weight 

		std::vector< Asura::Math::Vector4 > wt = mesh->GetSkinWeights(); 
		std::vector< Short4 > wtidx = mesh->GetSkinIndices(); 
		if((pos.size() == wt.size()) && (pos.size() == wtidx.size())) {
			for(j=0;j<wt.size();j++) {
				v[j].mat_weight.x = wt[j].x; 
				v[j].mat_weight.y = wt[j].y; 
				v[j].mat_weight.z = wt[j].z; 
				v[j].mat_weight.w = wt[j].w; 

				v[j].mat_index[0] = wtidx[j].val[0]; 
				v[j].mat_index[1] = wtidx[j].val[1]; 
				v[j].mat_index[2] = wtidx[j].val[2]; 
				v[j].mat_index[3] = wtidx[j].val[3]; 
			}
		} else {
			for(j=0;j<pos.size();j++) {
				v[j].mat_weight.x = 1.0f; 
				v[j].mat_weight.y = 0.0f; 
				v[j].mat_weight.z = 0.0f; 
				v[j].mat_weight.w = 0.0f; 

				v[j].mat_index[0] = 0; 
				v[j].mat_index[1] = 0; 
				v[j].mat_index[2] = 0; 
				v[j].mat_index[3] = 0; 

			}
		}


		/*
		D3D11_BUFFER_DESC idxBufferDesc;
		idxBufferDesc.Usage          = D3D11_USAGE_DEFAULT;    
		idxBufferDesc.ByteWidth      = sizeof(tDAEWEIGHT) * idx.size();  
		idxBufferDesc.BindFlags      = D3D11_BIND_INDEX_BUFFER; 
		idxBufferDesc.CPUAccessFlags = 0;
		idxBufferDesc.MiscFlags      = 0;
		idxBufferDesc.StructureByteStride = 0;


		D3D11_SUBRESOURCE_DATA idxSubData;
		idxSubData.pSysMem          = idxVertexID;  // バッファ・データの初期値
		idxSubData.SysMemPitch      = 0;
		idxSubData.SysMemSlicePitch = 0;

		hr = pd3dDevice->CreateBuffer(&idxBufferDesc, &idxSubData, &buffer_info[i].ib);
		if (FAILED(hr)) {
			DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateBuffer", hr);
			return; 
		}

		buffer_info[i].idx_num = idx.size(); 

		delete[] idxVertexID; 
		*/


		HRESULT hr; 
		ID3D11Device* pd3dDevice = _context->GetDXDevice(); 

		D3D11_BUFFER_DESC vBufferDesc;
		vBufferDesc.Usage          = D3D11_USAGE_DEFAULT; 
		vBufferDesc.ByteWidth      = sizeof(tDAEVERTEX) * pos.size(); 
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
		hr = pd3dDevice->CreateBuffer(&vBufferDesc, &vSubData, &buffer_info[i].vb);
		if (FAILED(hr)) {
			DXTRACE_ERR(L"InitDirect3D pd3dDevice->CreateBuffer", hr);
			return; 
		}
		buffer_info[i].vtx_num = pos.size(); 




		// **********************************************************
		// インデックス・バッファの定義

		std::vector< uint32_t > idx = mesh->GetIndices(); 
		if(idx.size() % 3 != 0) {
			throw; 
		}

		UINT* idxVertexID = new UINT[idx.size()]; 
		for(j=0;j<idx.size();j++) {
			idxVertexID[j] = idx[j]; 
		}


		D3D11_BUFFER_DESC idxBufferDesc;
		idxBufferDesc.Usage          = D3D11_USAGE_DEFAULT;    
		idxBufferDesc.ByteWidth      = sizeof(UINT) * idx.size();  
		idxBufferDesc.BindFlags      = D3D11_BIND_INDEX_BUFFER; 
		idxBufferDesc.CPUAccessFlags = 0;
		idxBufferDesc.MiscFlags      = 0;
		idxBufferDesc.StructureByteStride = 0;


		D3D11_SUBRESOURCE_DATA idxSubData;
		idxSubData.pSysMem          = idxVertexID;  // バッファ・データの初期値
		idxSubData.SysMemPitch      = 0;
		idxSubData.SysMemSlicePitch = 0;

		hr = pd3dDevice->CreateBuffer(&idxBufferDesc, &idxSubData, &buffer_info[i].ib);
		if (FAILED(hr)) {
			DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateBuffer", hr);
			return; 
		}

		buffer_info[i].idx_num = idx.size(); 






		// Material 
		std::vector< IMaterial* > mtl = mesh->GetMaterials(); 

		material[i].assign(mtl.size(), Dx11XMaterialStandard()); 

		for(j=0;j<mtl.size();j++) {
			Asura::Math::Vector4 diffuse = mtl[j]->GetDiffuse();  
			material[i][j].diffuse[0] = diffuse.x; 
			material[i][j].diffuse[1] = diffuse.y; 
			material[i][j].diffuse[2] = diffuse.z; 
			material[i][j].diffuse[3] = diffuse.w; 

			Asura::Math::Vector3 specular = mtl[j]->GetSpecular();  
			material[i][j].specular[0] = specular.x; 
			material[i][j].specular[1] = specular.y; 
			material[i][j].specular[2] = specular.z; 

			Asura::Math::Vector3 emmisive = mtl[j]->GetEmissive();  
			material[i][j].emissive[0] = emmisive.x; 
			material[i][j].emissive[1] = emmisive.y; 
			material[i][j].emissive[2] = emmisive.z; 

			material[i][j].power = mtl[j]->GetPower(); 

			std::string tname = mtl[j]->GetTextureName(); 

			if(tname.size() == 0) {
				material[i][j].tex_index = -1; 
			} else {
				if(tex_map.find(tname) == tex_map.end()) {
					int id = tex_map.size(); 
					tex_map[tname] = id; 
				}

				material[i][j].tex_index = tex_map[tname]; 
			}
		}

		//// サブメッシュの情報をここで保持する
		// Material 
		std::vector< IMeshSubset* > sbm = mesh->GetSubsets(); 

		sub_mesh[i].assign(sbm.size(), Dx11XSubMesh()); 
		for(j=0;j<sbm.size();j++) {
			sub_mesh[i][j].mat_index = sbm[j]->GetMaterialIndex(); 
			sub_mesh[i][j].start_index = sbm[j]->GetFaceIndex(); 
			sub_mesh[i][j].size = sbm[j]->GetNumFace(); 

			if(sub_mesh[i][j].size >= 1) {
				sub_mesh[i][j].bounding_box[0][0] = sub_mesh[i][j].bounding_box[0][1] = v[idxVertexID[sub_mesh[i][j].start_index]].vPos.x; 
				sub_mesh[i][j].bounding_box[1][0] = sub_mesh[i][j].bounding_box[1][1] = v[idxVertexID[sub_mesh[i][j].start_index]].vPos.y; 
				sub_mesh[i][j].bounding_box[2][0] = sub_mesh[i][j].bounding_box[2][1] = v[idxVertexID[sub_mesh[i][j].start_index]].vPos.z; 
				int k; 
				for(k=1;k<sub_mesh[i][j].size;k++) {
					int idx = sub_mesh[i][j].start_index + k; 
					if(sub_mesh[i][j].bounding_box[0][0] > v[idxVertexID[idx]].vPos.x) {
						sub_mesh[i][j].bounding_box[0][0] = v[idxVertexID[idx]].vPos.x; 
					} 
					if(sub_mesh[i][j].bounding_box[1][0] > v[idxVertexID[idx]].vPos.y) {
						sub_mesh[i][j].bounding_box[1][0] = v[idxVertexID[idx]].vPos.y; 
					} 
					if(sub_mesh[i][j].bounding_box[2][0] > v[idxVertexID[idx]].vPos.z) {
						sub_mesh[i][j].bounding_box[2][0] = v[idxVertexID[idx]].vPos.z; 
					} 

					if(sub_mesh[i][j].bounding_box[0][1] < v[idxVertexID[idx]].vPos.x) {
						sub_mesh[i][j].bounding_box[0][1] = v[idxVertexID[idx]].vPos.x; 
					} 
					if(sub_mesh[i][j].bounding_box[1][1] < v[idxVertexID[idx]].vPos.y) {
						sub_mesh[i][j].bounding_box[1][1] = v[idxVertexID[idx]].vPos.y; 
					} 
					if(sub_mesh[i][j].bounding_box[2][1] < v[idxVertexID[idx]].vPos.z) {
						sub_mesh[i][j].bounding_box[2][1] = v[idxVertexID[idx]].vPos.z; 
					} 
				}
			}
		}

		delete[] v; 
		delete[] idxVertexID; 
	}

	std::map<std::string, int, comparer>::iterator it; 
	m_pTexture.assign(tex_map.size(), 0); 

	i=0; 
	for(it=tex_map.begin();it!=tex_map.end();it++) {
		WCHAR filepath[4096];
		MultiByteToWideChar(CP_UTF8, 0, it->first.c_str(), -1, filepath, (int)sizeof(filepath));
		HRESULT hr; 
		hr = CreateDDSTextureFromFile( pd3dDevice, filepath, NULL, &m_pTexture[i] ); 
		if (FAILED(hr)) {
			DXTRACE_ERR(L"D3DX11CreateShaderResourceViewFromFile", hr);
			return; 
		}
		i++; 
	}


	// animation. 
	unsigned int num_bones = mpInterface->GetNumBones(); 
	bones.assign(num_bones, Dx11XBone()); 
	for(i=0;i<num_bones;i++) {
		IBone* bone = mpInterface->GetBone(i); 
		std::string bname = bone->GetName(); 
		int parent = bone->GetParentIndex(); 
		Asura::Math::Matrix bmat = bone->GetBindMatrix(); 
		Asura::Math::Matrix pmat = bone->GetPoseMatrix(); 
		XMFLOAT4X4 xmbmat, xmpmat; 
		memcpy(&xmbmat.m, &bmat.m, sizeof(float)*16); 
		memcpy(&xmpmat.m, &pmat.m, sizeof(float)*16); 
		XMMATRIX xmmb = XMLoadFloat4x4(&xmbmat); 
		XMMATRIX xmmp = XMLoadFloat4x4(&xmpmat); 
		XMMATRIX xmmbindpose = xmmb * xmmp; 
		bones[i].bind = xmbmat; 
		bones[i].pose = xmpmat; 
		XMStoreFloat4x4(&bones[i].bindpose, xmmbindpose); 
		bones[i].parent = bone->GetParentIndex(); 
		bones[i].name = bone->GetName(); 
	}

	unsigned int num_clips = mpInterface->GetNumClips(); 
	for(i=0;i<num_clips;i++) {
		IAnimationClip* clip = mpInterface->GetClip(i); 
		float duration = clip->GetDuration(); 
		std::string name = clip->GetName(); 
		unsigned int num_frames = clip->GetNumFrames(); 
		for(j=0;j<num_frames;j++) {
			IAnimation* frame = clip->GetFrame(j); 
			int bidx = frame->GetBoneIndex(); 
			std::string bname = frame->GetName(); 
			unsigned int num_keys = frame->GetNumKeys(); 
			for(k=0;k<num_keys;k++) {
				IKeyPose* keypose = frame->GetKey(k); 
				int tm = keypose->GetTime(); 
				Asura::Math::Matrix mat = keypose->GetTransform(); 
			}
		}
	}
}

XMFLOAT4X4 Dx11XObject::GetBindPose(unsigned int idx)
{
	return bones[buffer_info[idx].ref_frame].bindpose; 
}


XMFLOAT4X4 Dx11XObject::GetBindPoseFromParent(unsigned int idx)
{
	if(bones[buffer_info[idx].ref_frame].parent >= 0) {
		XMFLOAT4X4  parmat = GetBindPoseFromParent_Bone( bones[buffer_info[idx].ref_frame].parent); 
		XMMATRIX xm_parmat = XMLoadFloat4x4(&parmat); 
		XMMATRIX xm_this = XMLoadFloat4x4(&bones[buffer_info[idx].ref_frame].bindpose); 
		XMMATRIX xm_ret = xm_this * xm_parmat; 
		XMFLOAT4X4 ret; 
		XMStoreFloat4x4(&ret, xm_ret); 
		return ret; 
	}
	return bones[buffer_info[idx].ref_frame].bindpose; 
}

XMFLOAT4X4 Dx11XObject::GetBindPoseFromParent_Bone(unsigned int idx)
{
	if(bones[idx].parent >= 0) {
		XMFLOAT4X4  parmat = GetBindPoseFromParent_Bone( bones[idx].parent); 
		XMMATRIX xm_parmat = XMLoadFloat4x4(&parmat); 
		XMMATRIX xm_this = XMLoadFloat4x4(&bones[idx].bindpose); 
		XMMATRIX xm_ret = xm_this * xm_parmat; 
		XMFLOAT4X4 ret; 
		XMStoreFloat4x4(&ret, xm_ret); 
		return ret; 
	}
	return bones[idx].bindpose; 
}


void Dx11XObject::Term()
{
	unsigned int i; 
	for(i=0;i<buffer_info.size();i++) {
		SAFE_RELEASE(buffer_info[i].vb); 
		SAFE_RELEASE(buffer_info[i].ib); 
	}
	buffer_info.clear(); 

	for(i=0;i<m_pTexture.size();i++) {
		SAFE_RELEASE(m_pTexture[i]); 
	}
	m_pTexture.clear(); 
}

BOOL Dx11XObject::GetBufNum(unsigned int* num)
{
	*num = buffer_info.size(); 
	return true; 
}

BOOL Dx11XObject::GetVertexBuf(unsigned int num, ID3D11Buffer** buf)
{
	*buf = buffer_info[num].vb; 
	return true; 
}

BOOL Dx11XObject::GetWeightBuf(unsigned int num, ID3D11Buffer** buf)
{
	return false; 
}

BOOL Dx11XObject::GetIndexBuf(unsigned int num, ID3D11Buffer** buf)
{
	*buf = buffer_info[num].ib; 
	return true; 
}


BOOL Dx11XObject::GetVertexNum(unsigned int idx, unsigned int* num)
{
	*num = buffer_info[idx].vtx_num; 
	return true; 
}

BOOL Dx11XObject::GetIndexNum(unsigned int idx, unsigned int* num)
{
	*num = buffer_info[idx].idx_num; 
	return true; 
}

BOOL Dx11XObject::GetTexture(unsigned int num, ID3D11ShaderResourceView** pTexture)
{
	*pTexture = m_pTexture[num]; 
	return true; 
}

BOOL Dx11XObject::GetTextureNum(unsigned int* num)
{
	*num = m_pTexture.size(); 
	return true; 
}

void Dx11XObject::GetMaterial(unsigned int id, unsigned int mat_id, Dx11XMaterialStandard* _material)
{
	memcpy(_material, &material[id][mat_id], sizeof(Dx11XMaterialStandard)); 
}

int Dx11XObject::GetNumberOfMaterial(unsigned int id)
{
	return material[id].size(); 
}


void Dx11XObject::GetSubMesh(unsigned int id, unsigned int mat_id, Dx11XSubMesh* _sub_mesh)
{
	memcpy(_sub_mesh, &sub_mesh[id][mat_id], sizeof(Dx11XSubMesh)); 
}

int Dx11XObject::GetNumberOfSubMesh(unsigned int id)
{
	return sub_mesh[id].size(); 
}

std::string Dx11XObject::GetName(unsigned int idx)
{
	return bones[buffer_info[idx].ref_frame].name; 
}
