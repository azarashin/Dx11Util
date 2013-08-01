//-----------------------------------------------------------------------
// File : AsuraModelImpl.cpp
// Desc : Asura Model Implemenation
// Copyright(c) Project Asura All right reserved.
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// History
//-----------------------------------------------------------------------
// <---> 2010/11/13 [Pocol] �V�K�쐬.
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
///�R���X�g���N�^
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
///�f�X�g���N�^
///</summary>
///---------------------------------------------------------------------
Bone::~Bone()
{
    /* DO NOTHING */
}

///---------------------------------------------------------------------
///<summary>
///�������������
///</summary>
///---------------------------------------------------------------------
void Bone::Release()
{
    mName.clear();
}

///---------------------------------------------------------------------
///<summary>
///���O���擾����
///</summary>
///<returns>�{�[������ԋp����</returns>
///---------------------------------------------------------------------
std::string Bone::GetName()
{ return mName; }

///---------------------------------------------------------------------
///<summary>
///�e�̃{�[���C���f�b�N�X���擾����
///</summary>
///<returns>
///�e�̃{�[���C���f�b�N�X��ԋp����.
///�e�����Ȃ����[�g�̏ꍇ��-1��ԋp����.
///</returns>
///---------------------------------------------------------------------
int32_t Bone::GetParentIndex()
{ return mParentIndex; }

///---------------------------------------------------------------------
///<summary>
///�{�[�����W�n�֕ϊ�����s����擾����
///</summary>
///<returns>�{�[�����W�n�ւƕϊ�����s���ԋp����</returns>
///---------------------------------------------------------------------
Matrix Bone::GetBindMatrix()
{ return mBindMatrix; }

///---------------------------------------------------------------------
///<summary>
///�p���s����擾����
///</summary>
///<returns>�p�������肷��I�t�Z�b�g�s���ԋp����</returns>
///---------------------------------------------------------------------
Matrix Bone::GetPoseMatrix()
{ return mPoseMatrix; }

///---------------------------------------------------------------------
///<summary>
///���O��ݒ肷��
///</summary>
///<param name="name">�ݒ肷�閼�O</param>
///---------------------------------------------------------------------
void Bone::SetName( const std::string &name )
{ mName = name; }

///---------------------------------------------------------------------
///<summary>
///�e�̃{�[���C���f�b�N�X��ݒ肷��
///</summary>
///<param name="index">�ݒ肷��e�̃{�[���C���f�b�N�X</param>
///---------------------------------------------------------------------
void Bone::SetParentIndex( const int32_t &index )
{ mParentIndex = index; }

///---------------------------------------------------------------------
///<summary>
///�{�[�����W�n�ւƕϊ�����s���ݒ肷��
///</summary>
///<param name="matrix">�ݒ肷��{�[�����W�n�ւƕϊ�����s��</param>
///---------------------------------------------------------------------
void Bone::SetBindMatrix( const Matrix &matrix )
{ mBindMatrix = matrix; }

///---------------------------------------------------------------------
///<summary>
///�p���s���ݒ肷��
///</summary>
///<param name="matrix">�ݒ肷��p���s��</param>
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
///�R���X�g���N�^
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
///�R�s�[�R���X�g���N�^
///</summary>
///<param name="material">�R�s�[���̃C���X�^���X</param>
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
///�f�X�g���N�^
///</summary>
///---------------------------------------------------------------------
Material::~Material()
{
	Release();
}

///---------------------------------------------------------------------
///<summary>
///�������������
///</summary>
///---------------------------------------------------------------------
void Material::Release()
{
    mTextureName.clear();
    mName.clear();
}

///---------------------------------------------------------------------
///<summary>
///�g�U���ˍގ����擾����
///</summary>
///<returns>�g�U���ˍގ���ԋp����</returns>
///---------------------------------------------------------------------
Vector4 Material::GetDiffuse()
{ return mDiffuse; }

///---------------------------------------------------------------------
///<summary>
///���ʔ��ˍގ����擾����
///</summary>
///<returns>���ʔ��ˍގ���ԋp����</returns>
///---------------------------------------------------------------------
Vector3 Material::GetSpecular()
{ return mSpecular; }

///---------------------------------------------------------------------
///<summary>
///���ˍގ����擾����
///</summary>
///<returns>���ˍގ���ԋp����</returns>
///---------------------------------------------------------------------
Vector3 Material::GetEmissive()
{ return mEmissive; }

///---------------------------------------------------------------------
///<summary>
///���ʔ��ˎw�����擾����
///</summary>
///<returns>���ʔ��ˎw����ԋp����</returns>
///---------------------------------------------------------------------
float Material::GetPower()
{ return mPower; }

///---------------------------------------------------------------------
///<summary>
///�e�N�X�`���t�@�C�������擾����
///</summary>
///<returns>�e�N�X�`���t�@�C������ԋp����</returns>
///---------------------------------------------------------------------
std::string Material::GetTextureName()
{
    return mTextureName;
}

///---------------------------------------------------------------------
///<summary>
///�}�e���A�������擾����
///</summary>
///<returns>�}�e���A������ԋp����</returns>
///---------------------------------------------------------------------
std::string Material::GetName()
{ return mName; }

///---------------------------------------------------------------------
///<summary>
///�}�e���A������ݒ肷��
///</summary>
///<param name="name">�ݒ肷��}�e���A����</param>
///---------------------------------------------------------------------
void Material::SetName( const std::string &name )
{ mName = name; }

///---------------------------------------------------------------------
///<summary>
///�g�U���ˍގ���ݒ肷��
///</summary>
///<param name="diffuse">�ݒ肷��g�U���ˍގ�</param>
///---------------------------------------------------------------------
void Material::SetDiffuse( const Vector4 &diffuse )
{ mDiffuse = diffuse; }

///---------------------------------------------------------------------
///<summary>
///���ʔ��ˍގ���ݒ肷��
///</summary>
///<param name="specular">�ݒ肷�鋾�ʔ��ˍގ�</param>
///---------------------------------------------------------------------
void Material::SetSpecular( const Vector3 &specular )
{ mSpecular = specular; }

///---------------------------------------------------------------------
///<summary>
///���ˍގ���ݒ肷��
///</summary>
///<param name="emissive">�ݒ肷����ˍގ�</param>
///---------------------------------------------------------------------
void Material::SetEmissive( const Vector3 &emissive )
{ mEmissive = emissive; }

///---------------------------------------------------------------------
///<summary>
///���ʔ��ˎw����ݒ肷��
///</summary>
///<param name="power">�ݒ肷�鋾�ʔ��ˎw��</param>
///---------------------------------------------------------------------
void Material::SetPower( const float &power )
{ mPower = power; }

///---------------------------------------------------------------------
///<summary>
///�e�N�X�`���t�@�C������ݒ肷��
///</summary>
///<param name="texture_name">�ݒ肷��e�N�X�`���t�@�C����</param>
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
///�R���X�g���N�^
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
///�f�X�g���N�^
///</summary>
///---------------------------------------------------------------------
MeshSubset::~MeshSubset()
{
    /* DO NOTHING */
}

///---------------------------------------------------------------------
///<summary>
///�}�e���A���C���f�b�N�X���擾����
///</summary>
///<returns>�}�e���A���C���f�b�N�X��ԋp����</returns>
///---------------------------------------------------------------------
uint32_t MeshSubset::GetMaterialIndex()
{ return mMaterialIndex; }

///---------------------------------------------------------------------
///<summary>
///�ʂ̃C���f�b�N�X���擾����
///</summary>
///<returns>�J�n����ʂ̃C���f�b�N�X�̒l��ԋp����</returns>
///---------------------------------------------------------------------
uint32_t MeshSubset::GetFaceIndex()
{ return mFaceIndex; }

///---------------------------------------------------------------------
///<summary>
///�ʐ����擾����
///</summary>
///<returns>�`�悷��ʐ���ԋp����</returns>
///---------------------------------------------------------------------
uint32_t MeshSubset::GetNumFace()
{ return mNumFace; }

///---------------------------------------------------------------------
///<summary>
///�}�e���A���C���f�b�N�X��ݒ肷��
///</summary>
///<param name="material_index">�ݒ肷��}�e���A���C���f�b�N�X</param>
///---------------------------------------------------------------------
void MeshSubset::SetMaterialIndex( const uint32_t &material_index )
{ mMaterialIndex = material_index; }

///---------------------------------------------------------------------
///<summary>
///�ʂ̃C���f�b�N�X��ݒ肷��
///</summary>
///<param name="face_index">�ݒ肷��ʂ̃C���f�b�N�X</param>
///---------------------------------------------------------------------
void MeshSubset::SetFaceIndex( const uint32_t &face_index )
{ mFaceIndex = face_index; }

///---------------------------------------------------------------------
///<summary>
///�ʐ���ݒ肷��
///</summary>
///<param name="num_face">�ݒ肷��`��ʐ�</param>
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
///�R���X�g���N�^
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
///�R�s�[�R���X�g���N�^
///</summary>
///<param name="mesh">�R�s�[���̃C���X�^���X</param>
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
///�f�X�g���N�^
///</summary>
///---------------------------------------------------------------------
Mesh::~Mesh()
{
    /* DO_NOTHING */
}

///---------------------------------------------------------------------
///<summary>
///�������������
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
///���b�V�������擾����
///</summary>
///<returns>���b�V������ԋp����</returns>
///---------------------------------------------------------------------
std::string Mesh::GetName()
{ return mName; }

///---------------------------------------------------------------------
///<summary>
///�ʒu���W�R���e�i���擾����
///</summary>
///<returns>�ʒu���W�R���e�i��ԋp����</returns>
///---------------------------------------------------------------------
std::vector< Vector3 > Mesh::GetPositions()
{
    return mPositions;
}

///---------------------------------------------------------------------
///<summary>
///�@���x�N�g���R���e�i���擾����
///</summary>
///<returns>�@���x�N�g���R���e�i��ԋp����</returns>
///---------------------------------------------------------------------
std::vector< Vector3 > Mesh::GetNormals()
{
    return mNormals;
}

///---------------------------------------------------------------------
///<summary>
///���_�J���[�R���e�i���擾����
///</summary>
///<returns>���_�J���[�R���e�i��ԋp����</returns>
///---------------------------------------------------------------------
std::vector< UByte4 > Mesh::GetColors()
{
    return mColors;
}

///---------------------------------------------------------------------
///<summary>
///�e�N�X�`�����W�R���e�i���擾����
///</summary>
///<returns>�e�N�X�`�����W�R���e�i��ԋp����</returns>
///---------------------------------------------------------------------
std::vector< Vector2 > Mesh::GetTexCoords()
{
    return mTexCoords;
}

///---------------------------------------------------------------------
///<summary>
///�ڃx�N�g���R���e�i���擾����
///</summary>
///<returns>�ڃx�N�g���R���e�i��ԋp����</returns>
///---------------------------------------------------------------------
std::vector< Vector3 > Mesh::GetTangents()
{
    return mTangents;
}

///---------------------------------------------------------------------
///<summary>
///�]�@���x�N�g���R���e�i���擾����
///</summary>
///<returns>�]�@���x�N�g���R���e�i��ԋp����</returns>
///---------------------------------------------------------------------
std::vector< Vector3 > Mesh::GetBinormals()
{
    return mBinormals;
}

///---------------------------------------------------------------------
///<summary>
///�X�L���C���f�b�N�X�R���e�i���擾����
///</summary>
///<returns>�X�L���C���f�b�N�X�R���e�i��ԋp����</returns>
///---------------------------------------------------------------------
std::vector< Short4 > Mesh::GetSkinIndices()
{
    return mSkinIndices;
}

///---------------------------------------------------------------------
///<summary>
///�X�L���E�F�C�g�R���e�i���擾����
///</summary>
///<returns>�X�L���E�F�C�g�R���e�i��ԋp����</returns>
///---------------------------------------------------------------------
std::vector< Vector4 > Mesh::GetSkinWeights()
{
    return mSkinWeights;
}

///---------------------------------------------------------------------
///<summary>
///���_�C���f�b�N�X�R���e�i���擾����
///</summary>
///<returns>���_�C���f�b�N�X�R���e�i��ԋp����</returns>
///---------------------------------------------------------------------
std::vector< uint32_t > Mesh::GetIndices()
{
    return mIndices;
}

///---------------------------------------------------------------------
///<summary>
///�{�[���R���e�i���擾����
///</summary>
///<returns>IBone*�^�ɕϊ������{�[���R���e�i��ԋp����</returns>
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
///�T�u�Z�b�g�R���e�i���擾����
///</summary>
///<returns>IMeshSubset*�^�ɕϊ������T�u�Z�b�g�R���e�i��ԋp����</returns>
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
///�}�e���A���R���e�i��ԋp����
///</summary>
///<returns>IMaterial*�^�ɕϊ������}�e���A���R���e�i��ԋp����</returns>
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
///�w�肳�ꂽ�C���f�b�N�X�̈ʒu���W���擾����
///</summary>
///<param name="index">�擾����ʒu���W�̃C���f�b�N�X</param>
///<returns>�w�肳�ꂽ�C���f�b�N�X�̈ʒu���W��ԋp����</returns>
///----------------------------------------------------------------------
Vector3 Mesh::GetPosition( uint32_t index )
{
	assert( index < GetNumPositions() );
	return mPositions[index];
}

///----------------------------------------------------------------------
///<summary>
///�w�肳�ꂽ�C���f�b�N�X�̖@���x�N�g�����擾����
///</summary>
///<param name="index">�擾����@���x�N�g���̃C���f�b�N�X</param>
///<returns>�w�肳�ꂽ�C���f�b�N�X�̖@���x�N�g����ԋp����</returns>
///----------------------------------------------------------------------
Vector3 Mesh::GetNormal( uint32_t index )
{
	assert( index < GetNumNormals() );
	return mNormals[index];
}

///----------------------------------------------------------------------
///<summary>
///�w�肳�ꂽ�C���f�b�N�X�̒��_�J���[���擾����
///</summary>
///<param name="index">�擾���钸�_�J���[�̃C���f�b�N�X</param>
///<returns>�w�肳�ꂽ�C���f�b�N�X�̒��_�J���[��ԋp����</returns>
///----------------------------------------------------------------------
UByte4 Mesh::GetColor( uint32_t index )
{
	assert( index < GetNumColors() );
	return mColors[index];
}

///----------------------------------------------------------------------
///<summary>
///�w�肳�ꂽ�C���f�b�N�X�̃e�N�X�`�����W���擾����
///</summary>
///<param name="index">�擾����e�N�X�`�����W�̃C���f�b�N�X</param>
///<returns>�w�肳�ꂽ�C���f�b�N�X�̃e�N�X�`�����W��ԋp����</returns>
///----------------------------------------------------------------------
Vector2 Mesh::GetTexCoord( uint32_t index )
{
	assert( index < GetNumTexCoords() );
	return mTexCoords[index];
}

///----------------------------------------------------------------------
///<summary>
///�w�肳�ꂽ�C���f�b�N�X�̐ڃx�N�g�����擾����
///</summary>
///<param name="index">�擾����ڃx�N�g���̃C���f�b�N�X</param>
///<returns>�w�肳�ꂽ�C���f�b�N�X�̐ڃx�N�g����ԋp����</returns>
///----------------------------------------------------------------------
Vector3 Mesh::GetTangent( uint32_t index )
{
	assert( index < GetNumTangents() );
	return mTangents[index];
}

///----------------------------------------------------------------------
///<summary>
///�w�肳�ꂽ�C���f�b�N�X�̏]�@���x�N�g�����擾����
///</summary>
///<param name="index">�擾����ڃx�N�g���̃C���f�b�N�X</param>
///<returns>�w�肳�ꂽ�C���f�b�N�X�̐ڃx�N�g����ԋp����</returns>
///----------------------------------------------------------------------
Vector3 Mesh::GetBinormal( uint32_t index )
{
	assert( index < GetNumBinormals() );
	return mBinormals[index];
}

///----------------------------------------------------------------------
///<summary>
///�w�肳�ꂽ�C���f�b�N�X�̃X�L���C���f�b�N�X���擾����
///</summary>
///<param name="index">�擾����X�L���C���f�b�N�X�̃C���f�b�N�X</param>
///<returns>�w�肳�ꂽ�C���f�b�N�X�̃X�L���C���f�b�N�X��ԋp����</returns>
///----------------------------------------------------------------------
Short4 Mesh::GetSkinIndex( uint32_t index )
{
	assert( index < GetNumSkinIndices() );
	return mSkinIndices[index];
}

///----------------------------------------------------------------------
///<summary>
///�w�肳�ꂽ�C���f�b�N�X�̃X�L���E�F�C�g���擾����
///</summary>
///<param name="index">�擾����X�L���E�F�C�g�̃C���f�b�N�X</param>
///<returns>�w�肳�ꂽ�C���f�b�N�X�̃X�L���E�F�C�g��ԋp����</returns>
///----------------------------------------------------------------------
Vector4 Mesh::GetSkinWeight( uint32_t index )
{
	assert( index < GetNumSkinWeights() );
	return mSkinWeights[index];
}

///----------------------------------------------------------------------
///<summary>
///�w�肳�ꂽ�C���f�b�N�X�̒��_�C���f�b�N�X���擾����
///</summary>
///<param name="index">�擾���钸�_�C���f�b�N�X�̃C���f�b�N�X<param>
///<returns>�w�肳�ꂽ�C���f�b�N�X�̒��_�C���f�b�N�X��ԋp����</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetIndex( uint32_t index )
{
	assert( index < GetNumIndices() );
	return mIndices[index];
}

///----------------------------------------------------------------------
///<summary>
///�w�肳�ꂽ�C���f�b�N�X�̃{�[�����擾����
///</summary>
///<param name="index">�擾����{�[���̃C���f�b�N�X</param>
///<returns>
///�w�肳�ꂽ�C���f�b�N�X�̃{�[����IBone*�^�ɕϊ����ĕԋp����
///</returns>
///----------------------------------------------------------------------
IBone* Mesh::GetBone( uint32_t index )
{
	assert( index < GetNumBones() );
	return &mBones[index];
}

///----------------------------------------------------------------------
///<summary>
///�w�肳�ꂽ�C���f�b�N�X�̃T�u�Z�b�g���擾����
///</summary>
///<param name="index">�擾����T�u�Z�b�g�̃C���f�b�N�X</param>
///</returns>
///�w�肳�ꂽ�C���f�b�N�X�̃T�u�Z�b�g��IMeshSubset*�^�ɕϊ����ĕԋp����
///</returns>
///----------------------------------------------------------------------
IMeshSubset* Mesh::GetSubset( uint32_t index )
{
	assert( index < GetNumSubsets() );
	return &mSubsets[index];
}

///----------------------------------------------------------------------
///<summary>
///�w�肳�ꂽ�C���f�b�N�X�̃}�e���A�����擾����
///</summary>
///<param name="index">�擾����T�u�Z�b�g�̃C���f�b�N�X</param>
///<returns>
///�w�肳�ꂽ�C���f�b�N�X�̃}�e���A����IMaterial*�^�ɕϊ����ĕԋp����
///</returns>
///----------------------------------------------------------------------
IMaterial* Mesh::GetMaterial( uint32_t index )
{
	assert( index < GetNumMaterials() );
	return &mMaterials[index];
}

///----------------------------------------------------------------------
///<summary>
///�ʒu���W�����擾����
///</summary>
///<returns>�ʒu���W����ԋp����</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetNumPositions()
{
	return static_cast< uint32_t >( mPositions.size() );
}

///----------------------------------------------------------------------
///<summary>
///�@���x�N�g�������擾����
///</summary>
///<returns>�@���x�N�g������ԋp����</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetNumNormals()
{
	return static_cast< uint32_t >( mNormals.size() );
}

///----------------------------------------------------------------------
///<summary>
///���_�J���[�����擾����
///</summary>
///<returns>���_�J���[����ԋp����</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetNumColors()
{
	return static_cast< uint32_t >( mColors.size() );
}

///----------------------------------------------------------------------
///<summary>
///�e�N�X�`�����W�����擾����
///</summary>
///<returns>�e�N�X�`�����W����ԋp����</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetNumTexCoords()
{
	return static_cast< uint32_t >( mTexCoords.size() );
}

///----------------------------------------------------------------------
///<summary>
///�ڃx�N�g�������擾����
///</summary>
///<returns>�ڃx�N�g������ԋp����</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetNumTangents()
{
	return static_cast< uint32_t >( mTangents.size() );
}

///----------------------------------------------------------------------
///<summary>
///�]�@���x�N�g�������擾����
///</summary>
///<returns>�]�@���x�N�g������ԋp����</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetNumBinormals()
{
	return static_cast< uint32_t >( mBinormals.size() );
}

///----------------------------------------------------------------------
///<summary>
///�X�L���C���f�b�N�X�����擾����
///</summary>
///<returns>�X�L���C���f�b�N�X����ԋp����</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetNumSkinIndices()
{
	return static_cast< uint32_t >( mSkinIndices.size() );
}

///----------------------------------------------------------------------
///<summary>
///�X�L���E�F�C�g�����擾����
///</summary>
///<returns>�X�L���E�F�C�g����ԋp����</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetNumSkinWeights()
{
	return static_cast< uint32_t >( mSkinWeights.size() );
}

///----------------------------------------------------------------------
///<summary>
///���_�C���f�b�N�X�����擾����
///</summary>
///<returns>���_�C���f�b�N�X����ԋp����</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetNumIndices()
{
	return static_cast< uint32_t >( mIndices.size() );
}

///----------------------------------------------------------------------
///<summary>
///�{�[�������擾����
///</summary>
///<returns>�{�[������ԋp����</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetNumBones()
{
	return static_cast< uint32_t >( mBones.size() );
}

///----------------------------------------------------------------------
///<summary>
///�T�u�Z�b�g�����擾����
///</summary>
///<returns>�T�u�Z�b�g����ԋp����</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetNumSubsets()
{
	return static_cast< uint32_t >( mSubsets.size() );
}

///----------------------------------------------------------------------
///<summary>
///�}�e���A�������擾����
///</summary>
///<returns>�}�e���A������ԋp����</returns>
///----------------------------------------------------------------------
uint32_t Mesh::GetNumMaterials()
{
	return static_cast< uint32_t >( mMaterials.size() );
}

///----------------------------------------------------------------------
///<summary>
///���b�V������ݒ肷��
///</summary>
///<param name="name">�ݒ肷�郁�b�V����</param>
///----------------------------------------------------------------------
void Mesh::SetName( const std::string &name )
{
	mName = name;
}

///----------------------------------------------------------------------
///<summary>
///�ʒu���W��ݒ肷��
///</summary>
///<param name="positions">�ݒ肷��ʒu���W</param>
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
///�@���x�N�g����ݒ肷��
///</summary>
///<param name="normals">�ݒ肷��@���x�N�g��</param>
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
///���_�J���[��ݒ肷��
///</summary>
///<param name="colors">�ݒ肷�钸�_�J���[</param>
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
///�e�N�X�`�����W��ݒ肷��
///</summary>
///<param name="texcoords">�ݒ肷��e�N�X�`�����W</param>
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
///�ڃx�N�g����ݒ肷��
///</summary>
///<param name="tangents">�ݒ肷��ڃx�N�g��</param>
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
///�]�@���x�N�g����ݒ肷��
///</summary>
///<param name="binormals">�ݒ肷��]�@���x�N�g��</param>
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
///�X�L���C���f�b�N�X��ݒ肷��
///</summary>
///<param name="skin_indices">�ݒ肷��X�L���C���f�b�N�X</param>
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
///�X�L���E�F�C�g��ݒ肷��
///</summary>
///<param name="skin_weights">�ݒ肷��X�L���E�F�C�g</param>
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
///���_�C���f�b�N�X��ݒ肷��
///</summary>
///<param name="indices">�ݒ肷�钸�_�C���f�b�N�X</param>
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
///�{�[����ݒ肷��
///</summary>
///<param name="bones">�ݒ肷��{�[��</param>
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
///�T�u�Z�b�g��ݒ肷��
///</summary>
///<param name="subsets">�ݒ肷��T�u�Z�b�g</param>
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
///�}�e���A����ݒ肷��
///</summary>
///<param name="materials">�ݒ肷��}�e���A��</param>
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
///�@���x�N�g�����v�Z����
///</summary>
///---------------------------------------------------------------------
void Mesh::ComputeNormals()
{
	/* ������ */
}

///---------------------------------------------------------------------
///<summary>
///�ڃx�N�g�����v�Z����
///</summary>
///---------------------------------------------------------------------
void Mesh::ComputeTangents()
{
	/* ������ */
}

///---------------------------------------------------------------------
///<summary>
///�]�@���x�N�g�����v�Z����
///</summary>
///---------------------------------------------------------------------
void Mesh::ComputeBinormals()
{
	/* ������ */
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
///�R���X�g���N�^
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
///�f�X�g���N�^
///</summary>
///----------------------------------------------------------------------
KeyPose::~KeyPose()
{
    /* DO_NOTHING */
}

///----------------------------------------------------------------------
///<summary>
///���Ԃ��擾����
///</summary>
///<returns>���Ԃ�ԋp����</returns>
///----------------------------------------------------------------------
uint32_t KeyPose::GetTime()
{
    return mTime;
}

///----------------------------------------------------------------------
///<summary>
///�g��ɗp����x�N�g�����擾����
///</summary>
///<returns>�g��ɗp����x�N�g����ԋp����</returns>
///----------------------------------------------------------------------
Vector3 KeyPose::GetScale()
{
    return mScale;
}

///----------------------------------------------------------------------
///<summary>
///��]�ɗp����l�������擾����
///</summary>
///<returns>��]�ɗp����l������ԋp����</returns>
///----------------------------------------------------------------------
Quaternion KeyPose::GetRotate()
{
    return mRotate;
}

///----------------------------------------------------------------------
///<summary>
///���s�ړ��ɗp����x�N�g�����擾����
///</summary>
///<returns>���s�ړ��ɗp����x�N�g����ԋp����</returns>
///----------------------------------------------------------------------
Vector3 KeyPose::GetTranslate()
{
    return mTranslate;
}

///----------------------------------------------------------------------
///<summary>
///�A�j���[�V�����ɗp����ϊ��s����擾����
///</summary>
///<returns>�A�j���[�V�����ɗp����ϊ��s���ԋp����</returns>
///----------------------------------------------------------------------
Matrix KeyPose::GetTransform()
{
    return mTransform;
}

///----------------------------------------------------------------------
///<summary>
///���Ԃ�ݒ肷��
///</summary>
///<param name="time">�ݒ肷�鎞��</param>
///----------------------------------------------------------------------
void KeyPose::SetTime( const uint32_t &time )
{
    mTime = time;
}

///----------------------------------------------------------------------
///<summary>
///�g��ɗp����x�N�g����ݒ肷��
///</summary>
///<param name="scale">�ݒ肷��x�N�g��</param>
///----------------------------------------------------------------------
void KeyPose::SetScale( const Vector3 &scale )
{
    mScale = scale;
}

///----------------------------------------------------------------------
///<summary>
///��]�ɗp����l������ݒ肷��
///</summary>
///<param name="rotate">�ݒ肷��l����</param>
///----------------------------------------------------------------------
void KeyPose::SetRotate( const Quaternion &rotate )
{
    mRotate = rotate;
}

///----------------------------------------------------------------------
///<summary>
///���s�ړ��ɗp����x�N�g����ݒ肷��
///</summary>
///<param name="translate">�ݒ肷��x�N�g��</param>
///----------------------------------------------------------------------
void KeyPose::SetTranslate( const Vector3 &translate )
{
    mTranslate = translate;
}

///----------------------------------------------------------------------
///<summary>
///�A�j���[�V�����ɗp����ϊ��s���ݒ肷��
///</summary>
///<param name="transform">�ݒ肷��ϊ��s��</param>
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
///�R���X�g���N�^
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
///�f�X�g���N�^
///</summary>
///----------------------------------------------------------------------
Animation::~Animation()
{
    Release();
}

///----------------------------------------------------------------------
///<summary>
///�������������
///</summary>
///----------------------------------------------------------------------
void Animation::Release()
{
    mName.clear();
    mKeys.clear();
}

///----------------------------------------------------------------------
///<summary>
///���O���擾����
///</summary>
///<returns>���O��ԋp����</returns>
///----------------------------------------------------------------------
std::string Animation::GetName()
{
    return mName;
}

///----------------------------------------------------------------------
///<summary>
///���̃C���f�b�N�X���擾����
///</summary>
///<returns>���̃C���f�b�N�X��ԋp����</returns>
///----------------------------------------------------------------------
uint32_t Animation::GetBoneIndex()
{
    return mBoneIndex;
}

///----------------------------------------------------------------------
///<summary>
///�L�[�|�[�Y�����擾����
///</summary>
///<returns>�L�[�|�[�Y����ԋp����</returns>
///----------------------------------------------------------------------
uint32_t Animation::GetNumKeys()
{
    return static_cast< uint32_t >( mKeys.size() );
}

///----------------------------------------------------------------------
///<summary>
///�w�肳�ꂽ�L�[�|�[�Y���擾����
///</summary>
///<param name="index">�擾����L�[�|�[�Y�̃C���f�b�N�X</param>
///<returns>�w�肳�ꂽ�C���f�b�N�X�̃L�[�|�[�Y�̒l��ԋp����</returns>
///----------------------------------------------------------------------
IKeyPose* Animation::GetKey( uint32_t index )
{
    assert( index < GetNumKeys() );
    return &mKeys[index];
}

///----------------------------------------------------------------------
///<summary>
///�L�[�|�[�Y���擾����
///</summary>
///<returns>�L�[�|�[�Y��ԋp����</returns>
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
///���O��ݒ肷��
///</summary>
///<param name="name">�ݒ肷�閼�O</param>
///----------------------------------------------------------------------
void Animation::SetName( const std::string &name )
{
    mName = name;
}

///----------------------------------------------------------------------
///<summary>
///���̃C���f�b�N�X��ݒ肷��
///</summary>
///<param name="index"></param>
///----------------------------------------------------------------------
void Animation::SetBoneIndex( const uint32_t &index )
{
    mBoneIndex = index;
}

///----------------------------------------------------------------------
///<summary>
///�L�[�|�[�Y��ݒ肷��
///</summary>
///<param name="keyposes">�ݒ肷��L�[�|�[�Y</param>
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
///�w�肳�ꂽ�C���f�b�N�X�̃L�[�|�[�Y��ݒ肷��
///</summary>
///<param name="index">�ݒ肷��C���f�b�N�X</param>
///<param name="pose">�ݒ肷��L�[�|�[�Y</param>
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
///�R���X�g���N�^
///</summary>
///----------------------------------------------------------------------
AnimationClip::AnimationClip()
{
    mName.clear();
    mFrames.clear();
}

///----------------------------------------------------------------------
///<summary>
///�f�X�g���N�^
///</summary>
///----------------------------------------------------------------------
AnimationClip::~AnimationClip()
{
   Release();
}

///---------------------------------------------------------------------
///<summary>
///�������������
///</summary>
///---------------------------------------------------------------------
void AnimationClip::Release()
{ 
    mName.clear();
    mFrames.clear();
}

///---------------------------------------------------------------------
///<summary>
///�A�j���[�V�����N���b�v�����擾����
///</summary>
///<returns>�A�j���[�V�����N���b�v����ԋp����</returns>
///---------------------------------------------------------------------
std::string AnimationClip::GetName()
{ return mName; }

///---------------------------------------------------------------------
///<summary>
///�w�肳�ꂽ�C���f�b�N�X�̃{�[���A�j���[�V�����f�[�^���擾����
///</summary>
///<param name="index">�擾����C���f�b�N�X</param>
///<returns>
///�w�肳�ꂽ�C���f�b�N�X�̃{�[���A�j���[�V�����f�[�^��ԋp����
///</returns>
///---------------------------------------------------------------------
IAnimation* AnimationClip::GetFrame( uint32_t index )
{
    assert( index < GetNumFrames() );
    return &mFrames[index];
}

///----------------------------------------------------------------------
///<summary>
///�{�[���A�j���[�V�����f�[�^���擾����
///</summary>
///<returns>�{�[���A�j���[�V�����f�[�^��ԋp����</returns>
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
///�{�[���A�j���[�V�����f�[�^�����擾����
///</summary>
///<returns>�{�[���A�j���[�V�����f�[�^����ԋp����</returns>
///----------------------------------------------------------------------
uint32_t AnimationClip::GetNumFrames()
{
    return static_cast< uint32_t >( mFrames.size() );
}

///----------------------------------------------------------------------
///<summary>
///�A�j���[�V�����p�����Ԃ��擾����
///</summary>
///<returns>�A�j���[�V�����p�����Ԃ�ԋp����</returns>
///----------------------------------------------------------------------
float AnimationClip::GetDuration()
{
    return mDuration;
}

///---------------------------------------------------------------------
///<summary>
///�A�j���[�V�����N���b�v����ݒ肷��
///</summary>
///<param name="name">�ݒ肷��A�j���[�V�����N���b�v��</param>
///---------------------------------------------------------------------
void AnimationClip::SetName( const std::string &name )
{ mName = name; }


///---------------------------------------------------------------------
///<summary>
///�w�肳�ꂽ�C���f�b�N�X�̃{�[���A�j���[�V�����f�[�^��ݒ肷��
///</summary>
///<param name="index">�ݒ肷��C���f�b�N�X</param>
///<param name="frame">�ݒ肷��f�[�^</param>
///---------------------------------------------------------------------
void AnimationClip::SetFrame( uint32_t index, Animation &frame )
{
    assert( index < GetNumFrames() );
    mFrames[index] = frame;
}

///---------------------------------------------------------------------
///<summary>
///�{�[���A�j���[�V�����f�[�^��ݒ肷��
///</summary>
///<param name="frames">�ݒ肷��f�[�^</param>
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
///�A�j���[�V�����p�����Ԃ�ݒ肷��
///</summary>
///<param name="duration">�ݒ肷�鎞�ԕ�</param>
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
///�R���X�g���N�^
///</summary>
///<param name="bones">�A�j���[�V���������郁�b�V���̃{�[��</param>
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
///�f�X�g���N�^
///</summary>
///---------------------------------------------------------------------
AnimationPlayer::~AnimationPlayer()
{
    /* DO NOTHING */
}

///---------------------------------------------------------------------
///<summary>
///�������������
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
///�A�j���[�V�����N���b�v��ݒ肷��
///</summary>
///<param name="pclip">�ݒ肷��A�j���[�V�����N���b�v</param>
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
///�{�[���ϊ��s����X�V����
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
        //�@���Ԃ�i�߂�
        time += mCurTime;

        //�@�p�����Ԃ����傫���Ȃ�����C���߂ɖ߂�
        while( time >= mpClip->GetDuration() )
        {
            time -= mpClip->GetDuration();
        }
    }
    //�@���Ԃ͈̔̓`�F�b�N
    if ( (time < 0.0f) || (time >= mpClip->GetDuration()) )
    {
        DLOG( "Error : AnimationPlayer::UpdateBoneTransform() time value range is invalid." );
        assert( false );
    }

    //�@���ݎ��Ԃ����������Ȃ�C������
    if ( time < mCurTime )
    {
        for( size_t i=0; i<mBones.size(); i++ )
        {
            mBoneTransforms[i] = mBones[i]->GetPoseMatrix();
        }
    }

    //�@���ݎ��Ԃ��X�V
    mCurTime = time;

    uint32_t num_frames = mpClip->GetNumFrames();
    for( uint32_t i=0; i<num_frames; ++i )
    {
        //�@�A�j���[�V�����f�[�^�擾
        IAnimation* animation = mpClip->GetFrame( i );

        uint32_t index1 = 0;
        uint32_t index2 = 0;
        //�@���ݎ��Ԃɋ߂��t���[��������
        for( uint32_t j=0; j<animation->GetNumKeys(); ++j )
        {
            if ( animation->GetKey( j )->GetTime() > mCurTime )
            {
                index1 = j;
                index2 = ( j > 0 ) ? ( j - 1 ) : j;
                break;
            }
        }

		//�@���̃C���f�b�N�X���擾����
        uint32_t bone_index = animation->GetBoneIndex();
		assert( bone_index >= 0 );

		//�@���̃C���f�b�N�X����{�[���A�j���[�V�����ɗp����ϊ��s����擾����
		Matrix mat1 = animation->GetKey( index1 )->GetTransform();
		Matrix mat2 = animation->GetKey( index2 )->GetTransform();

		//�@���̕ϊ��s������߂�
		mBoneTransforms[ bone_index ] = mat1 + mat2;
    }
}

///---------------------------------------------------------------------
///<summary>
///���[���h�s����X�V����
///</summary>
///<param name="rootMatrix">���[�g�m�[�h�̃��[���h�s��</param>
///---------------------------------------------------------------------
void AnimationPlayer::UpdateWorldTransform( const Matrix &rootMatrix )
{
	//�@�e�̕ϊ��s������߂�
    mWorldTransforms[0] = mBoneTransforms[0] * rootMatrix;

	//�@�q���̕ϊ��s������߂�
    for( size_t i=1; i<mBones.size(); i++ )
    {
        int parent = mBones[i]->GetParentIndex();
		assert( parent >= 0 );

		mWorldTransforms[i] = mBoneTransforms[i] * mWorldTransforms[parent];
    }
}

///---------------------------------------------------------------------
///<summary>
///�X�L�j���O�s����X�V����
///</summary>
///---------------------------------------------------------------------
void AnimationPlayer::UpdateSkinTransform()
{
    for( size_t i=0; i<mBones.size(); i++ )
    {
		//�@�X�L�j���O�s������߂�
         mSkinTransforms[i] = mBones[i]->GetBindMatrix() * mWorldTransforms[i];
    }
}

///---------------------------------------------------------------------
///<summary>
///�A�j���[�V�����X�V����
///</summary>
///---------------------------------------------------------------------
void AnimationPlayer::Update( float time, bool relativeToCurTime, const Matrix &rootTransform )
{
	//�@���𓮂���
    UpdateBoneTransform( time, relativeToCurTime );

	//�@���[���h�ϊ��s����X�V
    UpdateWorldTransform( rootTransform );

	//�@�X�L�j���O�s����X�V
    UpdateSkinTransform();
}

///--------------------------------------------------------------------
///<summary>
///�X�L�j���O�s����擾����
///</summary>
///<returns>�X�L�j���O�s���ԋp����</returns>
///--------------------------------------------------------------------
std::vector< Matrix > AnimationPlayer::GetSkinTransform()
{ return mSkinTransforms; }


/////////////////////////////////////////////////////////////////////////
// Model class
/////////////////////////////////////////////////////////////////////////

///---------------------------------------------------------------------
///<summary>
///�R���X�g���N�^
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
///�f�X�g���N�^
///</summary>
///---------------------------------------------------------------------
Model::~Model()
{
	/* DO_NOTHING */
}

///---------------------------------------------------------------------
///<summary>
///�������������
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
///���f�������擾����
///</summary>
///---------------------------------------------------------------------
std::string Model::GetName()
{
	return mName;
}

///---------------------------------------------------------------------
///<summary>
///���b�V�����擾����
///</summary>
///<returns>IMesh*�^�ɕϊ��������b�V���R���e�i��ԋp����</returns>
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
///�A�j���[�V�����N���b�v���擾����
///</summary>
///<returns>
///IAnimationClip*�^�ɕϊ������A�j���[�V�����N���b�v�R���e�i��ԋp����
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
///�w�肳�ꂽ�C���f�b�N�X�̃��b�V�����擾����
///</summary>
///<param name="index">�擾���郁�b�V���̃C���f�b�N�X</param>
///<returns>�w�肳�ꂽ�C���f�b�N�X�̃��b�V����ԋp����</returns>
///---------------------------------------------------------------------
IMesh* Model::GetMesh( uint32_t index )
{
	assert( index < GetNumMeshes() );
	return &mMeshes[index];
}

///---------------------------------------------------------------------
///<summary>
///�w�肳�ꂽ�C���f�b�N�X�̃A�j���[�V�����N���b�v���擾����
///</summary>
///<param name="index">
///�擾����A�j���[�V�����N���b�v�̃C���f�b�N�X
///</param>
///<returns>
///�w�肳�ꂽ�C���f�b�N�X�̃A�j���[�V�����N���b�v��ԋp����
///<returns>
///---------------------------------------------------------------------
IAnimationClip* Model::GetClip( uint32_t index )
{
	assert( index < GetNumClips() );
	return &mClips[index];
}

///---------------------------------------------------------------------
///<summary>
///���b�V�������擾����
///</summary>
///<returns>���b�V������ԋp����</returns>
///---------------------------------------------------------------------
uint32_t Model::GetNumMeshes()
{
	return static_cast< uint32_t >( mMeshes.size() );
}

///---------------------------------------------------------------------
///<summary>
///�A�j���[�V�����N���b�v�����擾����
///</summary>
///<returns>�A�j���[�V�����N���b�v����ԋp����</returns>
///---------------------------------------------------------------------
uint32_t Model::GetNumClips()
{
	return static_cast< uint32_t >( mClips.size() );
}

///---------------------------------------------------------------------
///<summary>
///���f������ݒ肷��
///</summary>
///<param name="name">�ݒ肷�郂�f����</param>
///---------------------------------------------------------------------
void Model::SetName( const std::string &name )
{
	mName = name;
}

///---------------------------------------------------------------------
///<summary>
///���b�V����ݒ肷��
///</summary>
///<param name="meshes">�ݒ肷�郁�b�V��</param>
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
///�A�j���[�V�����N���b�v��ݒ肷��
///</summary>
///<param name="clips">�ݒ肷��A�j���[�V�����N���b�v</param>
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
///�R���X�g���N�^
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
///�f�X�g���N�^
///</summary>
///----------------------------------------------------------------------
ModelLoaderBase::~ModelLoaderBase()
{
	/* DO NOTHING */
}

///----------------------------------------------------------------------
///<summary>
///�������������
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
///���f�����擾����
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
///�t�@�C�����烂�f�������[�h����
///</summary>
///<param name="filename">���[�h����t�@�C����</param>
///----------------------------------------------------------------------
bool ModelLoaderBase::Load( const char *filename )
{
	//�@�t�@�C�����J��
    mpFile = fopen( filename, "rb" );
    if ( !mpFile )
    {
        ELOG( "Error : file open failed - %s", filename );
        return false;
    }

	//�@�t�@�C�������R�s�[����
    mFileName = std::string( filename );

	//�@�T�C�Y���擾����
    fseek( mpFile, 0, SEEK_END );
    mBufSize = static_cast< uint32_t >( ftell( mpFile ) );

	//�@�t�@�C���̐擪�ɖ߂�
    fseek( mpFile, 0, SEEK_SET );

	//�@���������m�ۂ���
    try 
    {
        mpBuf = new char [mBufSize+1];
    }
    catch( std::bad_alloc ba )
    {
        ELOG( "Error : memory allocate failed. reason = %s", ba.what() );
        return false;
    }

	//�@�[���N���A
    memset( mpBuf, 0, mBufSize ); 
    mpBuf[mBufSize] = '\0';

	//�@�T�C�Y��ݒ�
    mCurSize = 0;
    mRestSize = mBufSize;

    DLOG( "Info : Now Loading - %s", filename );

    return true;
}

///----------------------------------------------------------------------
///<summary>
///���[�h�����ǂ������肷��
///</summary>
///<returns>
///���[�h���Ȃ�true��ԋp.
///���[�h�I���܂��̓��[�h�ł��Ȃ��ꍇ��false��ԋp����
///</returns>
///----------------------------------------------------------------------
bool ModelLoaderBase::IsLoading()
{
    mIsLoading = true;

	//�@�t�@�C�����J���ĂȂ��ꍇ�̓��[�h�ł��Ȃ��̂ŏI��
    if ( !mpFile )
    {
        ELOG( "Error : file not opened." );
        mIsLoading = false;
        return mIsLoading;
    }

    const int READ_SIZE = 131072; // 128[byte] * 1024 = 131072[byte]

	//�@�ǂݍ��݃T�C�Y������
    uint32_t size = ( mRestSize > READ_SIZE ) ? READ_SIZE : mRestSize;

	//�@�t�@�C������ǂݍ���
    fread( &mpBuf[ mCurSize ], sizeof(char), size, mpFile );

	//�@�ǂݍ��񂾕��T�C�Y�����Z
    mCurSize += size;
    mRestSize -= size;
    
	//�@�S�ēǂݍ��ݏI������ꍇ
	if ( mRestSize == 0 )
    {
		//�@�t�@�C�������
        fclose( mpFile );
        DLOG( "Info : Load Finished!" );
        mIsLoading = false;

		//�@���[�h�I��
        return mIsLoading;
    }

	//�@���[�h��
    return mIsLoading;
}

///----------------------------------------------------------------------
///<summary>
///�t�@�C��������g���q���擾����
///</summary>
///<return>�g���q����ԋp����</returns>
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
///�t�@�C��������f�B���N�g�������擾����
///</summary>
///<returns>�f�B���N�g������ԋp����</returns>
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
///���[�_�[���쐬����
///</summary>
///<param name="type">�쐬���郍�[�_�[�̃^�C�v</param>
///<param name="buf_size">�g�[�N���̃o�b�t�@�T�C�Y</param>
///<retrns>�w�肳�ꂽ�^�C�v����쐬�������[�_��ԋp����</returns>
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
///�A�j���[�V�����v���C���[�𐶐�����
///</summary>
///<param name="bones">�A�j���[�V���������郂�f���̍��f�[�^</param>
///<returns>���������A�j���[�V�����v���[���[��ԋp����</returns>
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
