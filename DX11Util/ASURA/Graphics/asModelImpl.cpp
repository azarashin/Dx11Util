//-----------------------------------------------------------------------
// File : AsuraModelImpl.cpp
// Desc : Asura Model Implemenation
// Copyright(c) Project Asura All right reserved.
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// History
//-----------------------------------------------------------------------
// <---> 2010/11/13 [Pocol] 新規作成.
// <End>

//-----------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------
#include "asModelImpl.h"
#include "asModelLoaderX.h"


//-----------------------------------------------------------------------
// Using Statements
//-----------------------------------------------------------------------
using namespace Asura::Math;


namespace Asura {
namespace Graphics {


/////////////////////////////////////////////////////////////////////////
// UShort4 structure
/////////////////////////////////////////////////////////////////////////
UShort4::UShort4()
{
    memset( val, 0, sizeof(uint16_t) * 4 );
};

UShort4::UShort4(const uint16_t *pValues)
{
    assert( pValues != NULL );
    memcpy( val, pValues, sizeof(uint16_t) * 4 );
}

UShort4::UShort4( uint16_t val0, uint16_t val1, uint16_t val2, uint16_t val3 )
{
    val[0] = val0;
    val[1] = val1;
    val[2] = val2;
    val[3] = val3;
}

UShort4::operator uint16_t* ()
{
    return (uint16_t *)&val[0];
}

UShort4::operator const uint16_t *() const
{
    return (const uint16_t *)&val[0];
}

bool UShort4::operator == ( const UShort4 &value )
{
    return ( ( val[0] == value.val[0] ) 
          && ( val[1] == value.val[1] )
          && ( val[2] == value.val[2] )
          && ( val[3] == value.val[3] ) );
}

bool UShort4::operator != ( const UShort4 &value )
{
    return ( ( val[0] != value.val[0] )
          || ( val[1] != value.val[1] )
          || ( val[2] != value.val[2] )
          || ( val[3] != value.val[3] ) );
}

/////////////////////////////////////////////////////////////////////////
// Short4 structure
/////////////////////////////////////////////////////////////////////////
Short4::Short4()
{
    memset( val, 0, sizeof(int16_t) * 4 );
};

Short4::Short4(const int16_t *pValues)
{
    assert( pValues != NULL );
    memcpy( val, pValues, sizeof(int16_t) * 4 );
}

Short4::Short4( int16_t val0, int16_t val1, int16_t val2, int16_t val3 )
{
    val[0] = val0;
    val[1] = val1;
    val[2] = val2;
    val[3] = val3;
}

Short4::operator int16_t* ()
{
    return (int16_t *)&val[0];
}

Short4::operator const int16_t *() const
{
    return (const int16_t *)&val[0];
}

bool Short4::operator == ( const Short4 &value )
{
    return ( ( val[0] == value.val[0] ) 
          && ( val[1] == value.val[1] )
          && ( val[2] == value.val[2] )
          && ( val[3] == value.val[3] ) );
}

bool Short4::operator != ( const Short4 &value )
{
    return ( ( val[0] != value.val[0] )
          || ( val[1] != value.val[1] )
          || ( val[2] != value.val[2] )
          || ( val[3] != value.val[3] ) );
}

/////////////////////////////////////////////////////////////////////////
// UByte4 structure
/////////////////////////////////////////////////////////////////////////
UByte4::UByte4()
{
    memset( val, 0, sizeof(uint8_t) * 4 );
};

UByte4::UByte4(const uint8_t *pValues)
{
    assert( pValues != NULL );
    memcpy( val, pValues, sizeof(uint8_t) * 4 );
}

UByte4::UByte4( uint8_t val0, uint8_t val1, uint8_t val2, uint8_t val3 )
{
    val[0] = val0;
    val[1] = val1;
    val[2] = val2;
    val[3] = val3;
}

UByte4::operator uint8_t* ()
{
    return (uint8_t *)&val[0];
}

UByte4::operator const uint8_t *() const
{
    return (const uint8_t *)&val[0];
}

bool UByte4::operator == ( const UByte4 &value )
{
    return ( ( val[0] == value.val[0] ) 
          && ( val[1] == value.val[1] )
          && ( val[2] == value.val[2] )
          && ( val[3] == value.val[3] ) );
}

bool UByte4::operator != ( const UByte4 &value )
{
    return ( ( val[0] != value.val[0] )
          || ( val[1] != value.val[1] )
          || ( val[2] != value.val[2] )
          || ( val[3] != value.val[3] ) );
}

/////////////////////////////////////////////////////////////////////////
// Byte4 structure
/////////////////////////////////////////////////////////////////////////
Byte4::Byte4()
{
    memset( val, 0, sizeof(int8_t) * 4 );
};

Byte4::Byte4(const int8_t *pValues)
{
    assert( pValues != NULL );
    memcpy( val, pValues, sizeof(int8_t) * 4 );
}

Byte4::Byte4( int8_t val0, int8_t val1, int8_t val2, int8_t val3 )
{
    val[0] = val0;
    val[1] = val1;
    val[2] = val2;
    val[3] = val3;
}

Byte4::operator int8_t* ()
{
    return (int8_t *)&val[0];
}

Byte4::operator const int8_t *() const
{
    return (const int8_t *)&val[0];
}

bool Byte4::operator == ( const Byte4 &value )
{
    return ( ( val[0] == value.val[0] ) 
          && ( val[1] == value.val[1] )
          && ( val[2] == value.val[2] )
          && ( val[3] == value.val[3] ) );
}

bool Byte4::operator != ( const Byte4 &value )
{
    return ( ( val[0] != value.val[0] )
          || ( val[1] != value.val[1] )
          || ( val[2] != value.val[2] )
          || ( val[3] != value.val[3] ) );
}


////////////////////////////////////////////////////////////////////////
// Bone class
////////////////////////////////////////////////////////////////////////

///---------------------------------------------------------------------
///<summary>
///コンストラクタ
///</summary>
///---------------------------------------------------------------------
Bone::Bone()
{
    mName.clear();
    mParentIndex = -1;
    mBindMatrix.Identity();
    mPoseMatrix.Identity();
}

///---------------------------------------------------------------------
///<summary>
///デストラクタ
///</summary>
///---------------------------------------------------------------------
Bone::~Bone()
{
    /* DO NOTHING */
}

///---------------------------------------------------------------------
///<summary>
///メモリ解放処理
///</summary>
///---------------------------------------------------------------------
void Bone::Release()
{
    mName.clear();
}

///---------------------------------------------------------------------
///<summary>
///名前を取得する
///</summary>
///<returns>ボーン名を返却する</returns>
///---------------------------------------------------------------------
std::string Bone::GetName()
{ return mName; }

///---------------------------------------------------------------------
///<summary>
///親のボーンインデックスを取得する
///</summary>
///<returns>
///親のボーンインデックスを返却する.
///親がいないルートの場合は-1を返却する.
///</returns>
///---------------------------------------------------------------------
int32_t Bone::GetParentIndex()
{ return mParentIndex; }

///---------------------------------------------------------------------
///<summary>
///ボーン座標系へ変換する行列を取得する
///</summary>
///<returns>ボーン座標系へと変換する行列を返却する</returns>
///---------------------------------------------------------------------
Matrix Bone::GetBindMatrix()
{ return mBindMatrix; }

///---------------------------------------------------------------------
///<summary>
///姿勢行列を取得する
///</summary>
///<returns>姿勢を決定するオフセット行列を返却する</returns>
///---------------------------------------------------------------------
Matrix Bone::GetPoseMatrix()
{ return mPoseMatrix; }

///---------------------------------------------------------------------
///<summary>
///名前を設定する
///</summary>
///<param name="name">設定する名前</param>
///---------------------------------------------------------------------
void Bone::SetName( const std::string &name )
{ mName = name; }

///---------------------------------------------------------------------
///<summary>
///親のボーンインデックスを設定する
///</summary>
///<param name="index">設定する親のボーンインデックス</param>
///---------------------------------------------------------------------
void Bone::SetParentIndex( const int32_t &index )
{ mParentIndex = index; }

///---------------------------------------------------------------------
///<summary>
///ボーン座標系へと変換する行列を設定する
///</summary>
///<param name="matrix">設定するボーン座標系へと変換する行列</param>
///---------------------------------------------------------------------
void Bone::SetBindMatrix( const Matrix &matrix )
{ mBindMatrix = matrix; }

///---------------------------------------------------------------------
///<summary>
///姿勢行列を設定する
///</summary>
///<param name="matrix">設定する姿勢行列</param>
///---------------------------------------------------------------------
void Bone::SetPoseMatrix( const Matrix &matrix )
{ mPoseMatrix = matrix; }

Bone& Bone::operator =( const Bone &bone )
{
    mName = bone.mName;
    mParentIndex = bone.mParentIndex;
    mBindMatrix = bone.mBindMatrix;
    mPoseMatrix = bone.mPoseMatrix;
    return (*this);
}

////////////////////////////////////////////////////////////////////////
// Material class
////////////////////////////////////////////////////////////////////////

///---------------------------------------------------------------------
///<summary>
///コンストラクタ
///</summary>
///---------------------------------------------------------------------
Material::Material()
{
    mDiffuse = Vector4( 0.0f, 0.0f, 0.0f, 0.0f );
    mSpecular = Vector3( 0.0f, 0.0f, 0.0f );
    mEmissive = Vector3( 0.0f, 0.0f, 0.0f );
    mPower = 0.0f;
    mTextureName.clear();
    mName.clear();
}

///---------------------------------------------------------------------
///<summary>
///コピーコンストラクタ
///</summary>
///<param name="material">コピー元のインスタンス</param>
///---------------------------------------------------------------------
Material::Material( const Material &material )
{
    mDiffuse = material.mDiffuse;
    mSpecular = material.mSpecular;
    mEmissive = material.mEmissive;
    mPower = material.mPower;
	mTextureName = material.mTextureName;
    mName = material.mName;
}

///---------------------------------------------------------------------
///<summary>
///デストラクタ
///</summary>
///---------------------------------------------------------------------
Material::~Material()
{
	Release();
}

///---------------------------------------------------------------------
///<summary>
///メモリ解放処理
///</summary>
///---------------------------------------------------------------------
void Material::Release()
{
    mTextureName.clear();
    mName.clear();
}

///---------------------------------------------------------------------
///<summary>
///拡散反射材質を取得する
///</summary>
///<returns>拡散反射材質を返却する</returns>
///---------------------------------------------------------------------
Vector4 Material::GetDiffuse()
{ return mDiffuse; }

///---------------------------------------------------------------------
///<summary>
///鏡面反射材質を取得する
///</summary>
///<returns>鏡面反射材質を返却する</returns>
///---------------------------------------------------------------------
Vector3 Material::GetSpecular()
{ return mSpecular; }

///---------------------------------------------------------------------
///<summary>
///放射材質を取得する
///</summary>
///<returns>放射材質を返却する</returns>
///---------------------------------------------------------------------
Vector3 Material::GetEmissive()
{ return mEmissive; }

///---------------------------------------------------------------------
///<summary>
///鏡面反射指数を取得する
///</summary>
///<returns>鏡面反射指数を返却する</returns>
///---------------------------------------------------------------------
float Material::GetPower()
{ return mPower; }

///---------------------------------------------------------------------
///<summary>
///テクスチャファイル名を取得する
///</summary>
///<returns>テクスチャファイル名を返却する</returns>
///---------------------------------------------------------------------
std::string Material::GetTextureName()
{
    return mTextureName;
}

///---------------------------------------------------------------------
///<summary>
///マテリアル名を取得する
///</summary>
///<returns>マテリアル名を返却する</returns>
///---------------------------------------------------------------------
std::string Material::GetName()
{ return mName; }

///---------------------------------------------------------------------
///<summary>
///マテリアル名を設定する
///</summary>
///<param name="name">設定するマテリアル名</param>
///---------------------------------------------------------------------
void Material::SetName( const std::string &name )
{ mName = name; }

///---------------------------------------------------------------------
///<summary>
///拡散反射材質を設定する
///</summary>
///<param name="diffuse">設定する拡散反射材質</param>
///---------------------------------------------------------------------
void Material::SetDiffuse( const Vector4 &diffuse )
{ mDiffuse = diffuse; }

///---------------------------------------------------------------------
///<summary>
///鏡面反射材質を設定する
///</summary>
///<param name="specular">設定する鏡面反射材質</param>
///---------------------------------------------------------------------
void Material::SetSpecular( const Vector3 &specular )
{ mSpecular = specular; }

///---------------------------------------------------------------------
///<summary>
///放射材質を設定する
///</summary>
///<param name="emissive">設定する放射材質</param>
///---------------------------------------------------------------------
void Material::SetEmissive( const Vector3 &emissive )
{ mEmissive = emissive; }

///---------------------------------------------------------------------
///<summary>
///鏡面反射指数を設定する
///</summary>
///<param name="power">設定する鏡面反射指数</param>
///---------------------------------------------------------------------
void Material::SetPower( const float &power )
{ mPower = power; }

///---------------------------------------------------------------------
///<summary>
///テクスチャファイル名を設定する
///</summary>
///<param name="texture_name">設定するテクスチャファイル名</param>
///---------------------------------------------------------------------
void Material::SetTextureName( const std::string &texture_name )
{ mTextureName = texture_name; }

Material& Material::operator =( const Material &material )
{
    mDiffuse = material.mDiffuse;
    mSpecular = material.mSpecular;
    mEmissive = material.mEmissive;
    mPower = material.mPower;
	mTextureName = material.mTextureName;
    mName = material.mName;
    return (*this);
}

////////////////////////////////////////////////////////////////////////
// MeshSubset class
////////////////////////////////////////////////////////////////////////

///---------------------------------------------------------------------
///<summary>
///コンストラクタ
///</summary>
///---------------------------------------------------------------------
MeshSubset::MeshSubset()
{
    mMaterialIndex = 0;
    mFaceIndex = 0;
    mNumFace = 0;
}

///---------------------------------------------------------------------
///<summary>
///デストラクタ
///</summary>
///---------------------------------------------------------------------
MeshSubset::~MeshSubset()
{
    /* DO NOTHING */
}

///---------------------------------------------------------------------
///<summary>
///マテリアルインデックスを取得する
///</summary>
///<returns>マテリアルインデックスを返却する</returns>
///---------------------------------------------------------------------
uint32_t MeshSubset::GetMaterialIndex()
{ return mMaterialIndex; }

///---------------------------------------------------------------------
///<summary>
///面のインデックスを取得する
///</summary>
///<returns>開始する面のインデックスの値を返却する</returns>
///---------------------------------------------------------------------
uint32_t MeshSubset::GetFaceIndex()
{ return mFaceIndex; }

///---------------------------------------------------------------------
///<summary>
///面数を取得する
///</summary>
///<returns>描画する面数を返却する</returns>
///---------------------------------------------------------------------
uint32_t MeshSubset::GetNumFace()
{ return mNumFace; }

///---------------------------------------------------------------------
///<summary>
///マテリアルインデックスを設定する
///</summary>
///<param name="material_index">設定するマテリアルインデックス</param>
///---------------------------------------------------------------------
void MeshSubset::SetMaterialIndex( const uint32_t &material_index )
{ mMaterialIndex = material_index; }

///---------------------------------------------------------------------
///<summary>
///面のインデックスを設定する
///</summary>
///<param name="face_index">設定する面のインデックス</param>
///---------------------------------------------------------------------
void MeshSubset::SetFaceIndex( const uint32_t &face_index )
{ mFaceIndex = face_index; }

///---------------------------------------------------------------------
///<summary>
///面数を設定する
///</summary>
///<param name="num_face">設定する描画面数</param>
///---------------------------------------------------------------------
void MeshSubset::SetNumFace( const uint32_t &num_face )
{ mNumFace = num_face; }

MeshSubset& MeshSubset::operator = ( const MeshSubset &subset )
{
    mMaterialIndex = subset.mMaterialIndex;
    mFaceIndex = subset.mFaceIndex;
    mNumFace = subset.mNumFace;
    return (*this);
}

////////////////////////////////////////////////////////////////////////
// Mesh class
////////////////////////////////////////////////////////////////////////

///---------------------------------------------------------------------
///<summary>
///コンストラクタ
///</summary>
///---------------------------------------------------------------------
Mesh::Mesh()
{
	mPositions.clear();
	mNormals.clear();
	mColors.clear();
	mTexCoords.clear();
    mTangents.clear();
    mBinormals.clear();
	mSkinIndices.clear();
	mSkinWeights.clear();
	mIndices.clear();
    mBones.clear();
	mSubsets.clear();
	mMaterials.clear();
}

///---------------------------------------------------------------------
///<summary>
///コピーコンストラクタ
///</summary>
///<param name="mesh">コピー元のインスタンス</param>
///---------------------------------------------------------------------
Mesh::Mesh( const Mesh &mesh )
{
	SetName( mesh.mName );
	SetPositions( mesh.mPositions );
	SetNormals( mesh.mNormals );
	SetColors( mesh.mColors );
	SetTexCoords( mesh.mTexCoords );
	SetTangents( mesh.mTangents );
	SetBinormals( mesh.mBinormals );
	SetSkinIndices( mesh.mSkinIndices );
	SetSkinWeights( mesh.mSkinWeights );
    SetIndices( mesh.mIndices );
    SetBones( mesh.mBones );
	SetSubsets( mesh.mSubsets );
	SetMaterials( mesh.mMaterials );
}

///---------------------------------------------------------------------
///<summary>
///デストラクタ
///</summary>
///---------------------------------------------------------------------
Mesh::~Mesh()
{
    /* DO_NOTHING */
}

///---------------------------------------------------------------------
///<summary>
///メモリ解放処理
///</summary>
///---------------------------------------------------------------------
void Mesh::Release()
{
	for( size_t i=0; i<mBones.size(); i++ )
	{
		mBones[i].Release();
	}
	for( size_t i=0; i<mMaterials.size(); i++ )
	{
		mMaterials[i].Release();
	}
	mPositions.clear();
	mNormals.clear();
	mColors.clear();
	mTexCoords.clear();
    mTangents.clear();
    mBinormals.clear();
	mSkinIndices.clear();
	mSkinWeights.clear();
	mIndices.clear();
    mBones.clear();
	mSubsets.clear();
	mMaterials.clear();
}

///---------------------------------------------------------------------
///<summary>
///メッシュ名を取得する
///</summary>
///<returns>メッシュ名を返却する</returns>
///---------------------------------------------------------------------
std::string Mesh::GetName()
{ return mName; }

///---------------------------------------------------------------------
///<summary>
///位置座標コンテナを取得する
///</summary>
///<returns>位置座標コンテナを返却する</returns>
///---------------------------------------------------------------------
std::vector< Vector3 > Mesh::GetPositions()
{
    return mPositions;
}

///---------------------------------------------------------------------
///<summary>
///法線ベクトルコンテナを取得する
///</summary>
///<returns>法線ベクトルコンテナを返却する</returns>
///---------------------------------------------------------------------
std::vector< Vector3 > Mesh::GetNormals()
{
    return mNormals;
}

///---------------------------------------------------------------------
///<summary>
///頂点カラーコンテナを取得する
///</summary>
///<returns>頂点カラーコンテナを返却する</returns>
///---------------------------------------------------------------------
std::vector< UByte4 > Mesh::GetColors()
{
    return mColors;
}

///---------------------------------------------------------------------
///<summary>
///テクスチャ座標コンテナを取得する
///</summary>
///<returns>テクスチャ座標コンテナを返却する</returns>
///---------------------------------------------------------------------
std::vector< Vector2 > Mesh::GetTexCoords()
{
    return mTexCoords;
}

///---------------------------------------------------------------------
///<summary>
///接ベクトルコンテナを取得する
///</summary>
///<returns>接ベクトルコンテナを返却する</returns>
///---------------------------------------------------------------------
std::vector< Vector3 > Mesh::GetTangents()
{
    return mTangents;
}

///---------------------------------------------------------------------
///<summary>
///従法線ベクトルコンテナを取得する
///</summary>
///<returns>従法線ベクトルコンテナを返却する</returns>
///---------------------------------------------------------------------
std::vector< Vector3 > Mesh::GetBinormals()
{
    return mBinormals;
}

///---------------------------------------------------------------------
///<summary>
///スキンインデックスコンテナを取得する
///</summary>
///<returns>スキンインデックスコンテナを返却する</returns>
///---------------------------------------------------------------------
std::vector< Short4 > Mesh::GetSkinIndices()
{
    return mSkinIndices;
}

///---------------------------------------------------------------------
///<summary>
///スキンウェイトコンテナを取得する
///</summary>
///<returns>スキンウェイトコンテナを返却する</returns>
///---------------------------------------------------------------------
std::vector< Vector4 > Mesh::GetSkinWeights()
{
    return mSkinWeights;
}

///---------------------------------------------------------------------
///<summary>
///頂点インデックスコンテナを取得する
///</summary>
///<returns>頂点インデックスコンテナを返却する</returns>
///---------------------------------------------------------------------
std::vector< uint32_t > Mesh::GetIndices()
{
    return mIndices;
}

///---------------------------------------------------------------------
///<summary>
///ボーンコンテナを取得する
///</summary>
///<returns>IBone*型に変換したボーンコンテナを返却する</returns>
///---------------------------------------------------------------------
std::vector< IBone* > Mesh::GetBones()
{
    std::vector< IBone* > result;
    result.resize( mBones.size() );
    for( size_t i=0; i<mBones.size(); i++ )
    {
        result[i] = &mBones[i];
    }
    return result;
}

///--------------------------------------------------------------------
///<summary>
///サブセットコンテナを取得する
///</summary>
///<returns>IMeshSubset*型に変換したサブセットコンテナを返却する</returns>
///--------------------------------------------------------------------
std::vector< IMeshSubset* > Mesh::GetSubsets()
{
    std::vector< IMeshSubset* > result;
    result.resize( mSubsets.size() );
    for( size_t i=0; i<mSubsets.size(); i++ )
    {
        result[i] = &mSubsets[i];
    }
    return result;
}

///---------------------------------------------------------------------
///<summary>
///マテリアルコンテナを返却する
///</summary>
///<returns>IMaterial*型に変換したマテリアルコンテナを返却する</returns>
///---------------------------------------------------------------------
std::vector< IMaterial* > Mesh::GetMaterials()
{
    std::vector< IMaterial* > result;
    result.resize( mMaterials.size() );
    for( size_t i=0; i<mMaterials.size(); i++ )
    {
        result[i] = &mMaterials[i];
    }
    return result;
}

///----------------------------------------------------------------------
///<summary>
///指定されたインデックスの位置座標を取得する
///</summary>
///<param name="index">取得する位置座標のインデックス</param>
///<returns>指定されたインデックスの位置座標を返却する</returns>
///----------------------------------------------------------------------
Vector3 Mesh::GetPosition( uint32_t index )
{
	assert( index < GetNumPositions() );
	return mPositions[index];
}

///----------------------------------------------------------------------
///<summary>
///指定されたインデックスの法線ベクトルを取得する
///</summary>
///<param name="index">取得する法線ベクトルのインデックス</param>
///<returns>指定されたインデックスの法線ベクトルを返却する</returns>
///----------------------------------------------------------------------
Vector3 Mesh::GetNormal( uint32_t index )
{
	assert( index < GetNumNormals() );
	return mNormals[index];
}

///----------------------------------------------------------------------
///<summary>
///指定されたインデックスの頂点カラーを取得する
///</summary>
///<param name="index">取得する頂点カラーのインデックス</param>
///<returns>指定されたインデックスの頂点カラーを返却する</returns>
///----------------------------------------------------------------------
UByte4 Mesh::GetColor( uint32_t index )
{
	assert( index < GetNumColors() );
	return mColors[index];
}

///----------------------------------------------------------------------
///<summary>
///指定されたインデックスのテクスチャ座標を取得する
///</summary>
///<param name="index">取得するテクスチャ座標のインデックス</param>
///<returns>指定されたインデックスのテクスチャ座標を返却する</returns>
///----------------------------------------------------------------------
Vector2 Mesh::GetTexCoord( uint32_t index )
{
	assert( index < GetNumTexCoords() );
	return mTexCoords[index];
}

///----------------------------------------------------------------------
///<summary>
///指定されたインデックスの接ベクトルを取得する
///</summary>
///<param name="index">取得する接ベクトルのインデックス</param>
///<returns>指定されたインデックスの接ベクトルを返却する</returns>
///----------------------------------------------------------------------
Vector3 Mesh::GetTangent( uint32_t index )
{
	assert( index < GetNumTangents() );
	return mTangents[index];
}

///----------------------------------------------------------------------
///<summary>
///指定されたインデックスの従法線ベクトルを取得する
///</summary>
///<param name="index">取得する接ベクトルのインデックス</param>
///<returns>指定されたインデックスの接ベクトルを返却する</returns>
///----------------------------------------------------------------------
Vector3 Mesh::GetBinormal( uint32_t index )
{
	assert( index < GetNumBinormals() );
	return mBinormals[index];
}

///----------------------------------------------------------------------
///<summary>
///指定されたインデックスのスキンインデックスを取得する
///</summary>
///<param name="index">取得するスキンインデックスのインデックス</param>
///<returns>指定されたインデックスのスキンインデックスを返却する</returns>
///----------------------------------------------------------------------
Short4 Mesh::GetSkinIndex( uint32_t index )
{
	assert( index < GetNumSkinIndices() );
	return mSkinIndices[index];
}

///----------------------------------------------------------------------
///<summary>
///指定されたインデックスのスキンウェイトを取得する
///</summary>
///<param name="index">取得するスキンウェイトのインデックス</param>
///<returns>指定されたインデックスのスキンウェイトを返却する</returns>
///----------------------------------------------------------------------
Vector4 Mesh::GetSkinWeight( uint32_t index )
{
	assert( index < GetNumSkinWeights() );
	return mSkinWeights[index];
}

///----------------------------------------------------------------------
///<summary>
///指定されたインデックスの頂点インデックスを取得する
///</summary>
///<param name="index">取得する頂点インデックスのインデックス<param>
///<returns>指定されたインデックスの頂点インデックスを返却する</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetIndex( uint32_t index )
{
	assert( index < GetNumIndices() );
	return mIndices[index];
}

///----------------------------------------------------------------------
///<summary>
///指定されたインデックスのボーンを取得する
///</summary>
///<param name="index">取得するボーンのインデックス</param>
///<returns>
///指定されたインデックスのボーンをIBone*型に変換して返却する
///</returns>
///----------------------------------------------------------------------
IBone* Mesh::GetBone( uint32_t index )
{
	assert( index < GetNumBones() );
	return &mBones[index];
}

///----------------------------------------------------------------------
///<summary>
///指定されたインデックスのサブセットを取得する
///</summary>
///<param name="index">取得するサブセットのインデックス</param>
///</returns>
///指定されたインデックスのサブセットをIMeshSubset*型に変換して返却する
///</returns>
///----------------------------------------------------------------------
IMeshSubset* Mesh::GetSubset( uint32_t index )
{
	assert( index < GetNumSubsets() );
	return &mSubsets[index];
}

///----------------------------------------------------------------------
///<summary>
///指定されたインデックスのマテリアルを取得する
///</summary>
///<param name="index">取得するサブセットのインデックス</param>
///<returns>
///指定されたインデックスのマテリアルをIMaterial*型に変換して返却する
///</returns>
///----------------------------------------------------------------------
IMaterial* Mesh::GetMaterial( uint32_t index )
{
	assert( index < GetNumMaterials() );
	return &mMaterials[index];
}

///----------------------------------------------------------------------
///<summary>
///位置座標数を取得する
///</summary>
///<returns>位置座標数を返却する</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetNumPositions()
{
	return static_cast< uint32_t >( mPositions.size() );
}

///----------------------------------------------------------------------
///<summary>
///法線ベクトル数を取得する
///</summary>
///<returns>法線ベクトル数を返却する</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetNumNormals()
{
	return static_cast< uint32_t >( mNormals.size() );
}

///----------------------------------------------------------------------
///<summary>
///頂点カラー数を取得する
///</summary>
///<returns>頂点カラー数を返却する</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetNumColors()
{
	return static_cast< uint32_t >( mColors.size() );
}

///----------------------------------------------------------------------
///<summary>
///テクスチャ座標数を取得する
///</summary>
///<returns>テクスチャ座標数を返却する</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetNumTexCoords()
{
	return static_cast< uint32_t >( mTexCoords.size() );
}

///----------------------------------------------------------------------
///<summary>
///接ベクトル数を取得する
///</summary>
///<returns>接ベクトル数を返却する</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetNumTangents()
{
	return static_cast< uint32_t >( mTangents.size() );
}

///----------------------------------------------------------------------
///<summary>
///従法線ベクトル数を取得する
///</summary>
///<returns>従法線ベクトル数を返却する</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetNumBinormals()
{
	return static_cast< uint32_t >( mBinormals.size() );
}

///----------------------------------------------------------------------
///<summary>
///スキンインデックス数を取得する
///</summary>
///<returns>スキンインデックス数を返却する</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetNumSkinIndices()
{
	return static_cast< uint32_t >( mSkinIndices.size() );
}

///----------------------------------------------------------------------
///<summary>
///スキンウェイト数を取得する
///</summary>
///<returns>スキンウェイト数を返却する</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetNumSkinWeights()
{
	return static_cast< uint32_t >( mSkinWeights.size() );
}

///----------------------------------------------------------------------
///<summary>
///頂点インデックス数を取得する
///</summary>
///<returns>頂点インデックス数を返却する</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetNumIndices()
{
	return static_cast< uint32_t >( mIndices.size() );
}

///----------------------------------------------------------------------
///<summary>
///ボーン数を取得する
///</summary>
///<returns>ボーン数を返却する</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetNumBones()
{
	return static_cast< uint32_t >( mBones.size() );
}

///----------------------------------------------------------------------
///<summary>
///サブセット数を取得する
///</summary>
///<returns>サブセット数を返却する</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetNumSubsets()
{
	return static_cast< uint32_t >( mSubsets.size() );
}

///----------------------------------------------------------------------
///<summary>
///マテリアル数を取得する
///</summary>
///<returns>マテリアル数を返却する</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetNumMaterials()
{
	return static_cast< uint32_t >( mMaterials.size() );
}

///----------------------------------------------------------------------
///<summary>
///メッシュ名を設定する
///</summary>
///<param name="name">設定するメッシュ名</param>
///----------------------------------------------------------------------
void Mesh::SetName( const std::string &name )
{
	mName = name;
}

///----------------------------------------------------------------------
///<summary>
///位置座標を設定する
///</summary>
///<param name="positions">設定する位置座標</param>
///----------------------------------------------------------------------
void Mesh::SetPositions( const std::vector< Vector3 > &positions )
{
	size_t size = positions.size();
	mPositions.clear();
	mPositions.resize( size );
	for( size_t i=0; i<size; i++ )
	{
		mPositions[i] = positions[i];
	}
}

///----------------------------------------------------------------------
///<summary>
///法線ベクトルを設定する
///</summary>
///<param name="normals">設定する法線ベクトル</param>
///----------------------------------------------------------------------
void Mesh::SetNormals( const std::vector< Vector3 > &normals )
{
	size_t size = normals.size();
	mNormals.clear();
	mNormals.resize( size );
	for( size_t i=0; i<size; i++ )
	{
		mNormals[i] = normals[i];
	}
}

///---------------------------------------------------------------------
///<summary>
///頂点カラーを設定する
///</summary>
///<param name="colors">設定する頂点カラー</param>
///---------------------------------------------------------------------
void Mesh::SetColors( const std::vector< UByte4 > &colors )
{
	size_t size = colors.size();
	mColors.clear();
	mColors.resize( size );
	for( size_t i=0; i<size; i++ )
	{
		mColors[i] = colors[i];
	}
}

///---------------------------------------------------------------------
///<summary>
///テクスチャ座標を設定する
///</summary>
///<param name="texcoords">設定するテクスチャ座標</param>
///---------------------------------------------------------------------
void Mesh::SetTexCoords( const std::vector< Vector2 > &texcoords )
{
	size_t size = texcoords.size();
	mTexCoords.clear();
	mTexCoords.resize( size );
	for( size_t i=0; i<size; i++ )
	{
		mTexCoords[i] = texcoords[i];
	}
}

///---------------------------------------------------------------------
///<summary>
///接ベクトルを設定する
///</summary>
///<param name="tangents">設定する接ベクトル</param>
///---------------------------------------------------------------------
void Mesh::SetTangents( const std::vector< Vector3 > &tangents )
{
	size_t size = tangents.size();
	mTangents.clear();
	mTangents.resize( size );
	for( size_t i=0; i<size; i++ )
	{
		mTangents[i] = tangents[i];
	}
}

///---------------------------------------------------------------------
///<summary>
///従法線ベクトルを設定する
///</summary>
///<param name="binormals">設定する従法線ベクトル</param>
///---------------------------------------------------------------------
void Mesh::SetBinormals( const std::vector< Vector3 > &binormals )
{
	size_t size = binormals.size();
	mBinormals.clear();
	mBinormals.resize( size );
	for( size_t i=0; i<size; i++ )
	{
		mBinormals[i] = binormals[i];
	}
}

///---------------------------------------------------------------------
///<summary>
///スキンインデックスを設定する
///</summary>
///<param name="skin_indices">設定するスキンインデックス</param>
///---------------------------------------------------------------------
void Mesh::SetSkinIndices( const std::vector< Short4 > &skin_indices )
{
	size_t size = skin_indices.size();
	mSkinIndices.clear();
	mSkinIndices.resize( size );
	for( size_t i=0; i<size; i++ )
	{
		mSkinIndices[i] = skin_indices[i];
	}
}

///---------------------------------------------------------------------
///<summary>
///スキンウェイトを設定する
///</summary>
///<param name="skin_weights">設定するスキンウェイト</param>
///---------------------------------------------------------------------
void Mesh::SetSkinWeights( const std::vector< Vector4 > &skin_weights )
{
    size_t size = skin_weights.size();
	mSkinWeights.clear();
	mSkinWeights.resize( size );
	for( size_t i=0; i<size; i++ )
	{
		mSkinWeights[i] = skin_weights[i];
	}
}

///---------------------------------------------------------------------
///<summary>
///頂点インデックスを設定する
///</summary>
///<param name="indices">設定する頂点インデックス</param>
///---------------------------------------------------------------------
void Mesh::SetIndices( const std::vector< uint32_t > &indices )
{
    size_t size = indices.size();
    mIndices.clear();
    mIndices.resize( size );
    for( size_t i=0; i<size; i++ )
    {
        mIndices[i] = indices[i];
    }
}

///---------------------------------------------------------------------
///<summary>
///ボーンを設定する
///</summary>
///<param name="bones">設定するボーン</param>
///---------------------------------------------------------------------
void Mesh::SetBones( const std::vector< Bone > &bones )
{
    size_t size = bones.size();
    mBones.clear();
    mBones.resize( size );
    for( size_t i=0; i<size; i++ )
    {
        mBones[i] = bones[i];
    }
}

///---------------------------------------------------------------------
///<summary>
///サブセットを設定する
///</summary>
///<param name="subsets">設定するサブセット</param>
///---------------------------------------------------------------------
void Mesh::SetSubsets( const std::vector< MeshSubset > &subsets )
{
    size_t size = subsets.size();
    mSubsets.clear();
    mSubsets.resize( size );
    for( size_t i=0; i<size; i++ )
    {
        mSubsets[i] = subsets[i];
    }
}

///---------------------------------------------------------------------
///<summary>
///マテリアルを設定する
///</summary>
///<param name="materials">設定するマテリアル</param>
///---------------------------------------------------------------------
void Mesh::SetMaterials( const std::vector< Material> &materials )
{
    size_t size = materials.size();
    mMaterials.clear();
    mMaterials.resize( size );
    for( size_t i=0; i<size; i++ )
    {
        mMaterials[i] = materials[i];
    }
}

///---------------------------------------------------------------------
///<summary>
///法線ベクトルを計算する
///</summary>
///---------------------------------------------------------------------
void Mesh::ComputeNormals()
{
	/* 未実装 */
}

///---------------------------------------------------------------------
///<summary>
///接ベクトルを計算する
///</summary>
///---------------------------------------------------------------------
void Mesh::ComputeTangents()
{
	/* 未実装 */
}

///---------------------------------------------------------------------
///<summary>
///従法線ベクトルを計算する
///</summary>
///---------------------------------------------------------------------
void Mesh::ComputeBinormals()
{
	/* 未実装 */
}

Mesh& Mesh::operator = ( const Mesh &mesh )
{
	SetName( mesh.mName );
	SetPositions( mesh.mPositions );
	SetNormals( mesh.mNormals );
	SetColors( mesh.mColors );
	SetTexCoords( mesh.mTexCoords );
	SetTangents( mesh.mTangents );
	SetBinormals( mesh.mBinormals );
	SetSkinIndices( mesh.mSkinIndices );
	SetSkinWeights( mesh.mSkinWeights );
    SetIndices( mesh.mIndices );
    SetBones( mesh.mBones );
    SetSubsets( mesh.mSubsets );
    SetMaterials( mesh.mMaterials );
	return (*this);
}


/////////////////////////////////////////////////////////////////////////
// KeyPose class
/////////////////////////////////////////////////////////////////////////

///----------------------------------------------------------------------
///<summary>
///コンストラクタ
///</summary>
///----------------------------------------------------------------------
KeyPose::KeyPose()
{
    mTime = 0;
    mScale = Vector3( 1.0f, 1.0f, 1.0f );
    mRotate = Quaternion();
    mTranslate = Vector3( 0.0f, 0.0f, 0.0f );
    mTransform.Identity();
}

///----------------------------------------------------------------------
///<summary>
///デストラクタ
///</summary>
///----------------------------------------------------------------------
KeyPose::~KeyPose()
{
    /* DO_NOTHING */
}

///----------------------------------------------------------------------
///<summary>
///時間を取得する
///</summary>
///<returns>時間を返却する</returns>
///----------------------------------------------------------------------
uint32_t KeyPose::GetTime()
{
    return mTime;
}

///----------------------------------------------------------------------
///<summary>
///拡大に用いるベクトルを取得する
///</summary>
///<returns>拡大に用いるベクトルを返却する</returns>
///----------------------------------------------------------------------
Vector3 KeyPose::GetScale()
{
    return mScale;
}

///----------------------------------------------------------------------
///<summary>
///回転に用いる四元数を取得する
///</summary>
///<returns>回転に用いる四元数を返却する</returns>
///----------------------------------------------------------------------
Quaternion KeyPose::GetRotate()
{
    return mRotate;
}

///----------------------------------------------------------------------
///<summary>
///平行移動に用いるベクトルを取得する
///</summary>
///<returns>平行移動に用いるベクトルを返却する</returns>
///----------------------------------------------------------------------
Vector3 KeyPose::GetTranslate()
{
    return mTranslate;
}

///----------------------------------------------------------------------
///<summary>
///アニメーションに用いる変換行列を取得する
///</summary>
///<returns>アニメーションに用いる変換行列を返却する</returns>
///----------------------------------------------------------------------
Matrix KeyPose::GetTransform()
{
    return mTransform;
}

///----------------------------------------------------------------------
///<summary>
///時間を設定する
///</summary>
///<param name="time">設定する時間</param>
///----------------------------------------------------------------------
void KeyPose::SetTime( const uint32_t &time )
{
    mTime = time;
}

///----------------------------------------------------------------------
///<summary>
///拡大に用いるベクトルを設定する
///</summary>
///<param name="scale">設定するベクトル</param>
///----------------------------------------------------------------------
void KeyPose::SetScale( const Vector3 &scale )
{
    mScale = scale;
}

///----------------------------------------------------------------------
///<summary>
///回転に用いる四元数を設定する
///</summary>
///<param name="rotate">設定する四元数</param>
///----------------------------------------------------------------------
void KeyPose::SetRotate( const Quaternion &rotate )
{
    mRotate = rotate;
}

///----------------------------------------------------------------------
///<summary>
///平行移動に用いるベクトルを設定する
///</summary>
///<param name="translate">設定するベクトル</param>
///----------------------------------------------------------------------
void KeyPose::SetTranslate( const Vector3 &translate )
{
    mTranslate = translate;
}

///----------------------------------------------------------------------
///<summary>
///アニメーションに用いる変換行列を設定する
///</summary>
///<param name="transform">設定する変換行列</param>
///----------------------------------------------------------------------
void KeyPose::SetTransform( const Matrix &transform )
{
    mTransform = transform;
}

KeyPose& KeyPose::operator = ( const KeyPose &keyPose )
{
    mTime = keyPose.mTime;
    mScale = keyPose.mScale;
    mRotate = keyPose.mRotate;
    mTranslate = keyPose.mTranslate;
    mTransform = keyPose.mTransform;

    return (*this);
}

/////////////////////////////////////////////////////////////////////////
// Animation class
/////////////////////////////////////////////////////////////////////////

///----------------------------------------------------------------------
///<summary>
///コンストラクタ
///</summary>
///----------------------------------------------------------------------
Animation::Animation()
{
    mName.clear();
    mKeys.clear();
    mBoneIndex = 0;
}

///----------------------------------------------------------------------
///<summary>
///デストラクタ
///</summary>
///----------------------------------------------------------------------
Animation::~Animation()
{
    Release();
}

///----------------------------------------------------------------------
///<summary>
///メモリ解放処理
///</summary>
///----------------------------------------------------------------------
void Animation::Release()
{
    mName.clear();
    mKeys.clear();
}

///----------------------------------------------------------------------
///<summary>
///名前を取得する
///</summary>
///<returns>名前を返却する</returns>
///----------------------------------------------------------------------
std::string Animation::GetName()
{
    return mName;
}

///----------------------------------------------------------------------
///<summary>
///骨のインデックスを取得する
///</summary>
///<returns>骨のインデックスを返却する</returns>
///----------------------------------------------------------------------
uint32_t Animation::GetBoneIndex()
{
    return mBoneIndex;
}

///----------------------------------------------------------------------
///<summary>
///キーポーズ数を取得する
///</summary>
///<returns>キーポーズ数を返却する</returns>
///----------------------------------------------------------------------
uint32_t Animation::GetNumKeys()
{
    return static_cast< uint32_t >( mKeys.size() );
}

///----------------------------------------------------------------------
///<summary>
///指定されたキーポーズを取得する
///</summary>
///<param name="index">取得するキーポーズのインデックス</param>
///<returns>指定されたインデックスのキーポーズの値を返却する</returns>
///----------------------------------------------------------------------
IKeyPose* Animation::GetKey( uint32_t index )
{
    assert( index < GetNumKeys() );
    return &mKeys[index];
}

///----------------------------------------------------------------------
///<summary>
///キーポーズを取得する
///</summary>
///<returns>キーポーズを返却する</returns>
///----------------------------------------------------------------------
std::vector< IKeyPose* > Animation::GetKeys()
{
    std::vector< IKeyPose* > result;
    result.resize( GetNumKeys() );
    for( uint32_t i=0; i<GetNumKeys(); ++i )
    {
        result[i] = &mKeys[i];
    }
    return result;
}

///----------------------------------------------------------------------
///<summary>
///名前を設定する
///</summary>
///<param name="name">設定する名前</param>
///----------------------------------------------------------------------
void Animation::SetName( const std::string &name )
{
    mName = name;
}

///----------------------------------------------------------------------
///<summary>
///骨のインデックスを設定する
///</summary>
///<param name="index"></param>
///----------------------------------------------------------------------
void Animation::SetBoneIndex( const uint32_t &index )
{
    mBoneIndex = index;
}

///----------------------------------------------------------------------
///<summary>
///キーポーズを設定する
///</summary>
///<param name="keyposes">設定するキーポーズ</param>
///----------------------------------------------------------------------
void Animation::SetKeys( const std::vector< KeyPose > &keys )
{
    mKeys.resize( keys.size() );
    for( size_t i=0; i<keys.size(); ++i )
    {
        mKeys[i] = keys[i];
    }
}

///----------------------------------------------------------------------
///<summary>
///指定されたインデックスのキーポーズを設定する
///</summary>
///<param name="index">設定するインデックス</param>
///<param name="pose">設定するキーポーズ</param>
///----------------------------------------------------------------------
void Animation::SetKey( uint32_t index, const KeyPose &pose )
{
    assert( index < GetNumKeys() );
    mKeys[index] = pose;
}

Animation& Animation::operator = ( const Animation &animation )
{
    mName = animation.mName;
    mBoneIndex = animation.mBoneIndex;
    mKeys.clear();
    mKeys.resize( animation.mKeys.size() );
    for( size_t i=0; i<animation.mKeys.size(); ++i )
    {
        mKeys[i] = animation.mKeys[i];
    }

    return (*this);
}

/////////////////////////////////////////////////////////////////////////
// AnimationClip class
/////////////////////////////////////////////////////////////////////////

///----------------------------------------------------------------------
///<summary>
///コンストラクタ
///</summary>
///----------------------------------------------------------------------
AnimationClip::AnimationClip()
{
    mName.clear();
    mFrames.clear();
}

///----------------------------------------------------------------------
///<summary>
///デストラクタ
///</summary>
///----------------------------------------------------------------------
AnimationClip::~AnimationClip()
{
   Release();
}

///---------------------------------------------------------------------
///<summary>
///メモリ解放処理
///</summary>
///---------------------------------------------------------------------
void AnimationClip::Release()
{ 
    mName.clear();
    mFrames.clear();
}

///---------------------------------------------------------------------
///<summary>
///アニメーションクリップ名を取得する
///</summary>
///<returns>アニメーションクリップ名を返却する</returns>
///---------------------------------------------------------------------
std::string AnimationClip::GetName()
{ return mName; }

///---------------------------------------------------------------------
///<summary>
///指定されたインデックスのボーンアニメーションデータを取得する
///</summary>
///<param name="index">取得するインデックス</param>
///<returns>
///指定されたインデックスのボーンアニメーションデータを返却する
///</returns>
///---------------------------------------------------------------------
IAnimation* AnimationClip::GetFrame( uint32_t index )
{
    assert( index < GetNumFrames() );
    return &mFrames[index];
}

///----------------------------------------------------------------------
///<summary>
///ボーンアニメーションデータを取得する
///</summary>
///<returns>ボーンアニメーションデータを返却する</returns>
///----------------------------------------------------------------------
std::vector< IAnimation* > AnimationClip::GetFrames()
{
    std::vector< IAnimation* > result;
    result.resize( GetNumFrames() );
    for( uint32_t i=0; i<GetNumFrames(); ++i )
    {
        result[i] = &mFrames[i];
    }
    return result;
}

///----------------------------------------------------------------------
///<summary>
///ボーンアニメーションデータ数を取得する
///</summary>
///<returns>ボーンアニメーションデータ数を返却する</returns>
///----------------------------------------------------------------------
uint32_t AnimationClip::GetNumFrames()
{
    return static_cast< uint32_t >( mFrames.size() );
}

///----------------------------------------------------------------------
///<summary>
///アニメーション継続時間を取得する
///</summary>
///<returns>アニメーション継続時間を返却する</returns>
///----------------------------------------------------------------------
float AnimationClip::GetDuration()
{
    return mDuration;
}

///---------------------------------------------------------------------
///<summary>
///アニメーションクリップ名を設定する
///</summary>
///<param name="name">設定するアニメーションクリップ名</param>
///---------------------------------------------------------------------
void AnimationClip::SetName( const std::string &name )
{ mName = name; }


///---------------------------------------------------------------------
///<summary>
///指定されたインデックスのボーンアニメーションデータを設定する
///</summary>
///<param name="index">設定するインデックス</param>
///<param name="frame">設定するデータ</param>
///---------------------------------------------------------------------
void AnimationClip::SetFrame( uint32_t index, Animation &frame )
{
    assert( index < GetNumFrames() );
    mFrames[index] = frame;
}

///---------------------------------------------------------------------
///<summary>
///ボーンアニメーションデータを設定する
///</summary>
///<param name="frames">設定するデータ</param>
///---------------------------------------------------------------------
void AnimationClip::SetFrames( const std::vector< Animation > &frames )
{
    mFrames.clear();
    mFrames.resize( frames.size() );
    for( size_t i=0; i<frames.size(); ++i )
    {
        mFrames[i] = frames[i];
    }
}

///----------------------------------------------------------------------
///<summary>
///アニメーション継続時間を設定する
///</summary>
///<param name="duration">設定する時間幅</param>
///----------------------------------------------------------------------
void AnimationClip::SetDuration( const float &duration )
{
    mDuration = duration;
}

AnimationClip& AnimationClip::operator = ( const AnimationClip &clip )
{
    mName = clip.mName;
    mDuration = clip.mDuration;
    mFrames.clear();
    mFrames.resize( clip.mFrames.size() );
    for( size_t i=0; i<clip.mFrames.size(); ++i )
    {
        mFrames[i] = clip.mFrames[i];
    }
   
    return (*this);
}

/////////////////////////////////////////////////////////////////////////
// AnimationPlayer class
/////////////////////////////////////////////////////////////////////////

///---------------------------------------------------------------------
///<summary>
///コンストラクタ
///</summary>
///<param name="bones">アニメーションさせるメッシュのボーン</param>
///---------------------------------------------------------------------
AnimationPlayer::AnimationPlayer( const std::vector< IBone* > &bones )
{
    mCurTime = 0.0f;
    mpClip = null;
    
    size_t size = bones.size();
    mBones.clear();
    mBones.resize( size );
    for( size_t i=0; i<size; i++ )
    {
        mBones[i] = bones[i];
    }

    mBoneTransforms.clear();
    mWorldTransforms.clear();
    mSkinTransforms.clear();

    mBoneTransforms.resize( size );
    mWorldTransforms.resize( size );
    mSkinTransforms.resize( size );  
}

///---------------------------------------------------------------------
///<summary>
///デストラクタ
///</summary>
///---------------------------------------------------------------------
AnimationPlayer::~AnimationPlayer()
{
    /* DO NOTHING */
}

///---------------------------------------------------------------------
///<summary>
///メモリ解放処理
///</summary>
///---------------------------------------------------------------------
void AnimationPlayer::Release()
{
    mpClip = null;
    mBones.clear();
    mBoneTransforms.clear();
    mWorldTransforms.clear();
    mSkinTransforms.clear();
}

///---------------------------------------------------------------------
///<summary>
///アニメーションクリップを設定する
///</summary>
///<param name="pclip">設定するアニメーションクリップ</param>
///---------------------------------------------------------------------
void AnimationPlayer::SetClip( IAnimationClip* pclip )
{ 
    mpClip = pclip;
    mCurTime = 0.0f;

    for( size_t i=0; i<mBones.size(); i++ )
    {
        mBoneTransforms[i] = mBones[i]->GetPoseMatrix();
    }
}

///---------------------------------------------------------------------
///<summary>
///ボーン変換行列を更新する
///</summary>
///<param name="time"></param>
///<param name="relativeToCurTime></param>
///---------------------------------------------------------------------
void AnimationPlayer::UpdateBoneTransform( float time, bool relativeToCurTime )
{
    if ( mpClip == null )
    {
        ELOG( "Error : AnimationPlayer::UpdateBoneTransform() mpClip == null" );
        assert( false );
    }

    if ( relativeToCurTime )
    {
        //　時間を進める
        time += mCurTime;

        //　継続時間よりも大きくなったら，初めに戻す
        while( time >= mpClip->GetDuration() )
        {
            time -= mpClip->GetDuration();
        }
    }
    //　時間の範囲チェック
    if ( (time < 0.0f) || (time >= mpClip->GetDuration()) )
    {
        DLOG( "Error : AnimationPlayer::UpdateBoneTransform() time value range is invalid." );
        assert( false );
    }

    //　現在時間よりも小さいなら，初期化
    if ( time < mCurTime )
    {
        for( size_t i=0; i<mBones.size(); i++ )
        {
            mBoneTransforms[i] = mBones[i]->GetPoseMatrix();
        }
    }

    //　現在時間を更新
    mCurTime = time;

    uint32_t num_frames = mpClip->GetNumFrames();
    for( uint32_t i=0; i<num_frames; ++i )
    {
        //　アニメーションデータ取得
        IAnimation* animation = mpClip->GetFrame( i );

        uint32_t index1 = 0;
        uint32_t index2 = 0;
        //　現在時間に近いフレームを検索
        for( uint32_t j=0; j<animation->GetNumKeys(); ++j )
        {
            if ( animation->GetKey( j )->GetTime() > mCurTime )
            {
                index1 = j;
                index2 = ( j > 0 ) ? ( j - 1 ) : j;
                break;
            }
        }

		//　骨のインデックスを取得する
        uint32_t bone_index = animation->GetBoneIndex();
		assert( bone_index >= 0 );

		//　骨のインデックスからボーンアニメーションに用いる変換行列を取得する
		Matrix mat1 = animation->GetKey( index1 )->GetTransform();
		Matrix mat2 = animation->GetKey( index2 )->GetTransform();

		//　骨の変換行列を求める
		mBoneTransforms[ bone_index ] = mat1 + mat2;
    }
}

///---------------------------------------------------------------------
///<summary>
///ワールド行列を更新する
///</summary>
///<param name="rootMatrix">ルートノードのワールド行列</param>
///---------------------------------------------------------------------
void AnimationPlayer::UpdateWorldTransform( const Matrix &rootMatrix )
{
	//　親の変換行列を求める
    mWorldTransforms[0] = mBoneTransforms[0] * rootMatrix;

	//　子供の変換行列を求める
    for( size_t i=1; i<mBones.size(); i++ )
    {
        int parent = mBones[i]->GetParentIndex();
		assert( parent >= 0 );

		mWorldTransforms[i] = mBoneTransforms[i] * mWorldTransforms[parent];
    }
}

///---------------------------------------------------------------------
///<summary>
///スキニング行列を更新する
///</summary>
///---------------------------------------------------------------------
void AnimationPlayer::UpdateSkinTransform()
{
    for( size_t i=0; i<mBones.size(); i++ )
    {
		//　スキニング行列を求める
         mSkinTransforms[i] = mBones[i]->GetBindMatrix() * mWorldTransforms[i];
    }
}

///---------------------------------------------------------------------
///<summary>
///アニメーション更新処理
///</summary>
///---------------------------------------------------------------------
void AnimationPlayer::Update( float time, bool relativeToCurTime, const Matrix &rootTransform )
{
	//　骨を動かす
    UpdateBoneTransform( time, relativeToCurTime );

	//　ワールド変換行列を更新
    UpdateWorldTransform( rootTransform );

	//　スキニング行列を更新
    UpdateSkinTransform();
}

///--------------------------------------------------------------------
///<summary>
///スキニング行列を取得する
///</summary>
///<returns>スキニング行列を返却する</returns>
///--------------------------------------------------------------------
std::vector< Matrix > AnimationPlayer::GetSkinTransform()
{ return mSkinTransforms; }


/////////////////////////////////////////////////////////////////////////
// Model class
/////////////////////////////////////////////////////////////////////////

///---------------------------------------------------------------------
///<summary>
///コンストラクタ
///</summary>
///---------------------------------------------------------------------
Model::Model()
{
	mName.clear();
	mMeshes.clear();
	mClips.clear();
}

///---------------------------------------------------------------------
///<summary>
///デストラクタ
///</summary>
///---------------------------------------------------------------------
Model::~Model()
{
	/* DO_NOTHING */
}

///---------------------------------------------------------------------
///<summary>
///メモリ解放処理
///</summary>
///---------------------------------------------------------------------
void Model::Release()
{
	for( size_t i=0; i<mMeshes.size(); i++ )
	{
		mMeshes[i].Release();
	}
	for( size_t i=0; i<mClips.size(); i++ )
	{
		mClips[i].Release();
	}

	mName.clear();
	mMeshes.clear();
	mClips.clear();
}

///---------------------------------------------------------------------
///<summary>
///モデル名を取得する
///</summary>
///---------------------------------------------------------------------
std::string Model::GetName()
{
	return mName;
}

///---------------------------------------------------------------------
///<summary>
///メッシュを取得する
///</summary>
///<returns>IMesh*型に変換したメッシュコンテナを返却する</returns>
///---------------------------------------------------------------------
std::vector< IMesh* > Model::GetMeshes()
{
	std::vector< IMesh* > result;
	result.resize( mMeshes.size() );
	for( size_t i=0; i<mMeshes.size(); i++ )
	{
		result[i] = &mMeshes[i];
	}
	return result;
}

///---------------------------------------------------------------------
///<summary>
///アニメーションクリップを取得する
///</summary>
///<returns>
///IAnimationClip*型に変換したアニメーションクリップコンテナを返却する
///</returns>
///---------------------------------------------------------------------
std::vector< IAnimationClip* > Model::GetClips()
{
	std::vector< IAnimationClip* > result;
	result.resize( mClips.size() );
	for( size_t i=0; i<mClips.size(); i++ )
	{
		result[i] = &mClips[i];
	}
	return result;
}

///---------------------------------------------------------------------
///<summary>
///指定されたインデックスのメッシュを取得する
///</summary>
///<param name="index">取得するメッシュのインデックス</param>
///<returns>指定されたインデックスのメッシュを返却する</returns>
///---------------------------------------------------------------------
IMesh* Model::GetMesh( uint32_t index )
{
	assert( index < GetNumMeshes() );
	return &mMeshes[index];
}

///---------------------------------------------------------------------
///<summary>
///指定されたインデックスのアニメーションクリップを取得する
///</summary>
///<param name="index">
///取得するアニメーションクリップのインデックス
///</param>
///<returns>
///指定されたインデックスのアニメーションクリップを返却する
///<returns>
///---------------------------------------------------------------------
IAnimationClip* Model::GetClip( uint32_t index )
{
	assert( index < GetNumClips() );
	return &mClips[index];
}

///---------------------------------------------------------------------
///<summary>
///メッシュ数を取得する
///</summary>
///<returns>メッシュ数を返却する</returns>
///---------------------------------------------------------------------
uint32_t Model::GetNumMeshes()
{
	return static_cast< uint32_t >( mMeshes.size() );
}

///---------------------------------------------------------------------
///<summary>
///アニメーションクリップ数を取得する
///</summary>
///<returns>アニメーションクリップ数を返却する</returns>
///---------------------------------------------------------------------
uint32_t Model::GetNumClips()
{
	return static_cast< uint32_t >( mClips.size() );
}

///---------------------------------------------------------------------
///<summary>
///モデル名を設定する
///</summary>
///<param name="name">設定するモデル名</param>
///---------------------------------------------------------------------
void Model::SetName( const std::string &name )
{
	mName = name;
}

///---------------------------------------------------------------------
///<summary>
///メッシュを設定する
///</summary>
///<param name="meshes">設定するメッシュ</param>
///---------------------------------------------------------------------
void Model::SetMeshes( const std::vector< Mesh > &meshes )
{
	mMeshes.clear();
	mMeshes.resize( meshes.size() );
	for( size_t i=0; i<meshes.size(); i++ )
	{
		mMeshes[i] = meshes[i];
	}
}

///--------------------------------------------------------------------
///<summary>
///アニメーションクリップを設定する
///</summary>
///<param name="clips">設定するアニメーションクリップ</param>
///--------------------------------------------------------------------
void Model::SetClips( const std::vector< AnimationClip > &clips )
{
	mClips.clear();
	mClips.resize( clips.size() );
	for( size_t i=0; i<clips.size(); i++ )
	{
		mClips[i] = clips[i];
	}
}

Model& Model::operator = ( const Model &model )
{
	SetName( model.mName );
	SetMeshes( model.mMeshes );
	SetClips( model.mClips );
	return (*this);
}

/////////////////////////////////////////////////////////////////////////
// ModelLoaderBase class
/////////////////////////////////////////////////////////////////////////

///----------------------------------------------------------------------
///<summary>
///コンストラクタ
///</summary>
///----------------------------------------------------------------------
ModelLoaderBase::ModelLoaderBase( uint32_t size ) : mToken( size )
{
    mFileName.clear();
    mpBuf = null;
    mpFile = null;
    mIsLoading = false;
    mBufSize = 0;
    mCurSize = 0;
}

///----------------------------------------------------------------------
///<summary>
///デストラクタ
///</summary>
///----------------------------------------------------------------------
ModelLoaderBase::~ModelLoaderBase()
{
	/* DO NOTHING */
}

///----------------------------------------------------------------------
///<summary>
///メモリ解放処理
///</summary>
///----------------------------------------------------------------------
void ModelLoaderBase::Release()
{
    mFileName.clear();
    SAFE_DELETE_ARRAY( mpBuf );
	mModel.Release();
    mpFile = null;
    mIsLoading = false;
    mBufSize = 0;
    mCurSize = 0;
    mToken.Release();
}

///----------------------------------------------------------------------
///<summary>
///モデルを取得する
///</summary>
///----------------------------------------------------------------------
IModel* ModelLoaderBase::GetModel()
{
	Model* pModel = new Model();
	(*pModel) = mModel;
    return pModel;
}

///----------------------------------------------------------------------
///<summary>
///ファイルからモデルをロードする
///</summary>
///<param name="filename">ロードするファイル名</param>
///----------------------------------------------------------------------
bool ModelLoaderBase::Load( const char *filename )
{
	//　ファイルを開く
    mpFile = fopen( filename, "rb" );
    if ( !mpFile )
    {
        ELOG( "Error : file open failed - %s", filename );
        return false;
    }

	//　ファイル名をコピーする
    mFileName = std::string( filename );

	//　サイズを取得する
    fseek( mpFile, 0, SEEK_END );
    mBufSize = static_cast< uint32_t >( ftell( mpFile ) );

	//　ファイルの先頭に戻す
    fseek( mpFile, 0, SEEK_SET );

	//　メモリを確保する
    try 
    {
        mpBuf = new char [mBufSize+1];
    }
    catch( std::bad_alloc ba )
    {
        ELOG( "Error : memory allocate failed. reason = %s", ba.what() );
        return false;
    }

	//　ゼロクリア
    memset( mpBuf, 0, mBufSize ); 
    mpBuf[mBufSize] = '\0';

	//　サイズを設定
    mCurSize = 0;
    mRestSize = mBufSize;

    DLOG( "Info : Now Loading - %s", filename );

    return true;
}

///----------------------------------------------------------------------
///<summary>
///ロード中かどうか判定する
///</summary>
///<returns>
///ロード中ならtrueを返却.
///ロード終了またはロードできない場合はfalseを返却する
///</returns>
///----------------------------------------------------------------------
bool ModelLoaderBase::IsLoading()
{
    mIsLoading = true;

	//　ファイルが開いてない場合はロードできないので終了
    if ( !mpFile )
    {
        ELOG( "Error : file not opened." );
        mIsLoading = false;
        return mIsLoading;
    }

    const int READ_SIZE = 131072; // 128[byte] * 1024 = 131072[byte]

	//　読み込みサイズを決定
    uint32_t size = ( mRestSize > READ_SIZE ) ? READ_SIZE : mRestSize;

	//　ファイルから読み込み
    fread( &mpBuf[ mCurSize ], sizeof(char), size, mpFile );

	//　読み込んだ分サイズを加算
    mCurSize += size;
    mRestSize -= size;
    
	//　全て読み込み終わった場合
	if ( mRestSize == 0 )
    {
		//　ファイルを閉じる
        fclose( mpFile );
        DLOG( "Info : Load Finished!" );
        mIsLoading = false;

		//　ロード終了
        return mIsLoading;
    }

	//　ロード中
    return mIsLoading;
}

///----------------------------------------------------------------------
///<summary>
///ファイル名から拡張子を取得する
///</summary>
///<return>拡張子名を返却する</returns>
///----------------------------------------------------------------------
std::string ModelLoaderBase::GetExt()
{
    int32_t index = mFileName.rfind( "." );
    if ( index > 0 )
    {
       std::string ext = mFileName.substr( index );
       DLOG( "Info : MeshLoaderBase::GetExt() ret=%s", ext.c_str() );
       return ext;
    }
    ELOG( "Error : MeshLoaderBase::GetExt() not find extention." );
    return std::string("\0");
}

///----------------------------------------------------------------------
///<summary>
///ファイル名からディレクトリ名を取得する
///</summary>
///<returns>ディレクトリ名を返却する</returns>
///----------------------------------------------------------------------
std::string ModelLoaderBase::GetDir()
{
    std::string dir;
    int32_t index = mFileName.rfind( "\\" );
    if ( index > 0 )
    {
        dir.resize( index );
        for( int32_t i=0; i<index; i++ )
        {
            dir[i] = mFileName[i];
        }
        DLOG( "Info : MeshLoaderBase::GetDir() ret=%s", dir.c_str() );
        return dir;
    }

    index = mFileName.rfind( "/" );
    if ( index > 0 )
    {
        dir.resize( index );
        for( int32_t i=0; i<index; i++ )
        {
            dir[i] = mFileName[i];
        }
        DLOG( "Info : MeshLoaderBase::GetDir() ret=%s", dir.c_str() );
        return dir;
    }
    ELOG( "Error : MeshLoaderBase::GetDir() not find \'/\' or \'\\\'" );
    return std::string("\0");
}


///////////////////////////////////////////////////////////////////////////
// ModelLoaderFactory
///////////////////////////////////////////////////////////////////////////

///------------------------------------------------------------------------
///<summary>
///ローダーを作成する
///</summary>
///<param name="type">作成するローダーのタイプ</param>
///<param name="buf_size">トークンのバッファサイズ</param>
///<retrns>指定されたタイプから作成したローダを返却する</returns>
///------------------------------------------------------------------------
IModelLoader* ModelLoaderFactory::Create( int32_t type, uint32_t token_size )
{
    switch( type )
    {
	    case MODEL_TYPE_X:	{ return new ModelLoaderX( token_size ); }
        default:            { return null; }
    }
}

/////////////////////////////////////////////////////////////////////////
// AnimationPlayerFactory class
/////////////////////////////////////////////////////////////////////////

///--------------------------------------------------------------------
///<summary>
///アニメーションプレイヤーを生成する
///</summary>
///<param name="bones">アニメーションさせるモデルの骨データ</param>
///<returns>生成したアニメーションプレーヤーを返却する</returns>
///--------------------------------------------------------------------
IAnimationPlayer* AnimationPlayerFactory::Create( std::vector< IBone* > bones )
{
    AnimationPlayer* pPlayer = null;
    try 
    {
        pPlayer = new AnimationPlayer( bones );
    }
    catch( std::bad_alloc ba )
    {
        ELOG( "Error : Memory Allocate Failed. reason=%s", ba.what() );
    }
    return pPlayer;
}


} // namespace Graphics
} // namespace Asura
