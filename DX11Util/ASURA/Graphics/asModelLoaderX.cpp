//-----------------------------------------------------------------------
// File : AsuraModelLoaderX.cpp
// Desc : Asura X File Parser
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
#include "asModelLoaderX.h"


//-----------------------------------------------------------------------
// Using Statements
//-----------------------------------------------------------------------
using namespace Asura::Math;


//-----------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------
#if 0
#define DEBUG_LOG DLOG
#else
#define DEBUG_LOG( x, ... )
#endif

namespace Asura {
namespace Graphics {


/////////////////////////////////////////////////////////////////////////
// SkinWeights class
/////////////////////////////////////////////////////////////////////////

///---------------------------------------------------------------------
///<summary>
///�R���X�g���N�^
///</summary>
///---------------------------------------------------------------------
SkinWeightCache::SkinWeightCache( uint32_t mesh_index )
{
    mName.clear();
    mWeights.clear();
    mIndices.clear();
    mMeshIndex = mesh_index;
    mMatrix.Identity();
}

///---------------------------------------------------------------------
///<summary>
///�f�X�g���N�^
///</summary>
///---------------------------------------------------------------------
SkinWeightCache::~SkinWeightCache()
{
    mName.clear();
    mWeights.clear();
    mIndices.clear();
}

///---------------------------------------------------------------------
///<summary>
///�������������
///</summary>
///---------------------------------------------------------------------
void SkinWeightCache::Release()
{
    mName.clear();
    mWeights.clear();
    mIndices.clear();
    mMeshIndex = 0;
    mMatrix.Identity();
}

///---------------------------------------------------------------------
///<summary>
///�E�F�C�g�̃��������������
///</summary>
///---------------------------------------------------------------------
void SkinWeightCache::ClearWeights()
{
    mWeights.clear();
}

///---------------------------------------------------------------------
///<summary>
///�C���f�b�N�X�̃��������������
///</summary>
///---------------------------------------------------------------------
void SkinWeightCache::ClearIndices()
{
    mIndices.clear();
}

///---------------------------------------------------------------------
///<summary>
///�E�F�C�g�̃������T�C�Y���Đݒ肷��
///</summary>
///<param name="size">�ݒ肷�郁�����T�C�Y</param>
///---------------------------------------------------------------------
void SkinWeightCache::ResizeWeights( int size )
{
    mWeights.resize( size );
}

///---------------------------------------------------------------------
///<summary>
///�C���f�b�N�X�̃������T�C�Y���Đݒ肷��
///</summary>
///<param name="size">�ݒ肷�郁�����T�C�Y</param>
///---------------------------------------------------------------------
void SkinWeightCache::ResizeIndices( int size )
{
    mIndices.resize( size );
}

///---------------------------------------------------------------------
///<summary>
///���O��ݒ肷��
///</summary>
///<param name="name">�ݒ肷�閼�O</param>
///---------------------------------------------------------------------
void SkinWeightCache::SetName( std::string name )
{
    mName = name;
}

///---------------------------------------------------------------------
///<summary>
///�E�F�C�g��ݒ肷��
///</summary>
///<param name="index">�ݒ肷��C���f�b�N�X</param>
///<param name="valeu">�ݒ肷��l</param>
///---------------------------------------------------------------------
void SkinWeightCache::SetWeight( int index, float value )
{
    assert( 0 <= index && index < static_cast< int >( mWeights.size() ) );
    mWeights[index] = value;
}

///---------------------------------------------------------------------
///<summary>
///�C���f�b�N�X��ݒ肷��
///</summary>
///<param name="index">�ݒ肷��C���f�b�N�X</param>
///<param name="value">�ݒ肷��l</param>
///---------------------------------------------------------------------
void SkinWeightCache::SetIndex( int index, uint32_t value )
{
    assert( 0 <= index && index < static_cast< int >( mIndices.size() ) );
    mIndices[index] = value;
}

///---------------------------------------------------------------------
///<summary>
///�s���ݒ肷��
///</summary>
///<param name="index">�ݒ肷��C���f�b�N�X</param>
///<param name="value">�ݒ肷��l</param>
///---------------------------------------------------------------------
void SkinWeightCache::SetMatrix( int index, float value )
{
    switch( index )
    {
        case 0 : mMatrix._11 = value; break;
        case 1 : mMatrix._12 = value; break;
        case 2 : mMatrix._13 = value; break;
        case 3 : mMatrix._14 = value; break;

        case 4 : mMatrix._21 = value; break;
        case 5 : mMatrix._22 = value; break;
        case 6 : mMatrix._23 = value; break;
        case 7 : mMatrix._24 = value; break;

        case 8 : mMatrix._31 = value; break;
        case 9 : mMatrix._32 = value; break;
        case 10: mMatrix._33 = value; break;
        case 11: mMatrix._34 = value; break;

        case 12: mMatrix._41 = value; break;
        case 13: mMatrix._42 = value; break;
        case 14: mMatrix._43 = value; break;
        case 15: mMatrix._44 = value; break;
        default: break;
    }
}

///--------------------------------------------------------------------
///<summary>
///���O���擾����
///</summary>
///<returns>�{�[������ԋp����</returns>
///--------------------------------------------------------------------
std::string SkinWeightCache::GetName()
{
    return mName;
}

///--------------------------------------------------------------------
///<summary>
///�E�F�C�g�����擾����
///</summary>
///<returns>�E�F�C�g����ԋp����</returns>
///--------------------------------------------------------------------
uint32_t SkinWeightCache::GetNumWeights()
{
    return static_cast< uint32_t >( mWeights.size() );
}

///--------------------------------------------------------------------
///<summary>
///�C���f�b�N�X�����擾����
///</summary>
///<returns>�C���f�b�N�X����ԋp����</returns>
///--------------------------------------------------------------------
uint32_t SkinWeightCache::GetNumIndices()
{
    return static_cast< uint32_t >( mIndices.size() );
}

///--------------------------------------------------------------------
///<summary>
///�E�F�C�g���擾����
///</summary>
///<param name="index">�擾����C���f�b�N�X</param>
///<returns>�E�F�C�g��ԋp����</returns>
///--------------------------------------------------------------------
float SkinWeightCache::GetWeight( uint32_t index )
{
    assert( 0 <= index && index < GetNumWeights() );
    return mWeights[index];
}

///--------------------------------------------------------------------
///<summary>
///�C���f�b�N�X���擾����
///</summary>
///<param name="index">�擾����C���f�b�N�X</param>
///<returns>�C���f�b�N�X��ԋp����</returns>
///--------------------------------------------------------------------
uint32_t SkinWeightCache::GetIndex( uint32_t index )
{
    assert( 0 <= index && index < GetNumIndices() );
    return mIndices[index];
}

///--------------------------------------------------------------------
///<summary>
///�s����擾����
///</summary>
///<returns>�s���ԋp����</returns>
///--------------------------------------------------------------------
Matrix SkinWeightCache::GetMatrix()
{
    return mMatrix;
}

///--------------------------------------------------------------------
///<summary>
///���b�V���̃C���f�b�N�X���擾����
///</summary>
///<returns>���b�V���̃C���f�b�N�X��ԋp����</returns>
///--------------------------------------------------------------------
uint32_t SkinWeightCache::GetMeshIndex()
{
    return mMeshIndex;
}

SkinWeightCache& SkinWeightCache::operator = ( const SkinWeightCache &cache )
{
    mName = cache.mName;
    mWeights.clear();
    mIndices.clear();
    mWeights.resize( cache.mWeights.size() );
    mIndices.resize( cache.mIndices.size() );

    for( size_t i=0; i<cache.mWeights.size(); ++i )
    {
        mWeights[i] = cache.mWeights[i];
    }
    for( size_t i=0; i<cache.mIndices.size(); ++i )
    {
        mIndices[i] = cache.mIndices[i];
    }
    mMatrix = cache.mMatrix;
    mMeshIndex = cache.mMeshIndex;
    return (*this);
}


/////////////////////////////////////////////////////////////////////////
// ModelLoaderX class
/////////////////////////////////////////////////////////////////////////

///----------------------------------------------------------------------
///<summary>
///�R���X�g���N�^
///</summary>
///----------------------------------------------------------------------
ModelLoaderX::ModelLoaderX( uint32_t token_size ) 
: ModelLoaderBase( token_size )
{
	mBoneCaches.clear();
	mFaceElement.clear();
	mParentIndex = -1;
    mSkinWeightCaches.clear();
    mAnimationCaches.clear();
    mMaxTime = 0;
}

///----------------------------------------------------------------------
///<summary>
///�f�X�g���N�^
///</summary>
///----------------------------------------------------------------------
ModelLoaderX::~ModelLoaderX()
{
	mBoneCaches.clear();
	mFaceElement.clear();
    mSkinWeightCaches.clear();
    mAnimationCaches.clear();
    Release();
}

///----------------------------------------------------------------------
///<summary>
///�m�[�h���`�F�b�N����
///</summary>
///----------------------------------------------------------------------
void ModelLoaderX::CheckNode(int parent)
{
    if ( mToken.Check( "template" ) )
    {
        SkipNode();
    }
    else if ( mToken.Check( "Mesh" ) )
    {
        ParseMesh(parent);
    }
    else if ( mToken.Check( "MeshNormals" ) )
    {
        ParseMeshNormals(parent);
    }
    else if ( mToken.Check( "MeshTextureCoords" ) )
    {
        ParseMeshTextureCoords(parent);
    }
	else if ( mToken.Check( "MeshVertexColors" ) )
	{
		ParseMeshVertexColors(parent);
	}
    else if ( mToken.Check( "MeshMaterialList" ) )
    {
        ParseMeshMaterialList(parent);
    }
    else if ( mToken.Check( "Frame" ) )
    {
	    ParseFrame(parent);
    }
    else if ( mToken.Check( "SkinWeights" ) )
    {
        ParseSkinWeights(parent);
    }
    else if ( mToken.Check( "AnimationSet" ) )
    {
        ParseAnimationSet(parent);
    }
    else if ( mToken.Check( "Animation" ) )
    {
        ParseAnimation(parent);
#if 1 // azarashin modified...
    } else {
		mToken.Next();
#endif 
	}
}

///----------------------------------------------------------------------
///<summary>
///�\����͂��s��
///</summary>
///----------------------------------------------------------------------
void ModelLoaderX::Parse()
{
	//�@�g�[�J�i�C�U�[�̐ݒ�
	mToken.SetBuffer( mpBuf );

	// --> azarashin modified to remove comment
	char* cp = mpBuf; 
	char* lt = cp; 
	char bkp; 
	while(*cp != '\0') {
		if((*cp != 0x0d) && (*cp != 0x0a)) {
			cp++; 
			continue; 
		}

		bkp = *cp; 
		*cp = 0; 
		char* cm = strstr(lt, "//"); 
		char* lft = strstr(lt, "{"); 
		if((cm != 0) && (lft != 0) && (lft < cm)) { // find comment
			char* cm2 = cm+2; 
			int ps = strspn(cm2, " \t\r\n"); 
			char* cm3 = cm2 + ps; 
			int ps2 = strcspn(cm3, " \t\r\n"); 
			char* cm4 = cm3 + ps2; // key word appear from cm3 to (cm4 - 1)

			while(cm3 != cm4) {
				*lft = *cm3; 
				lft++; 
				cm3++; 
			}
			*lft = '{'; 
			lft++; 
			while(lft != cp) {
				*lft = ' '; 
				lft++; 
			}
		} else if(cm != 0) {
			while(cm != cp) {
				*cm = ' '; 
				cm++; 
			}
		}

		*cp = bkp; 

		cp++; 
		lt = cp; 
	}
	// <-- azarashin modified to remove comment

	mToken.SetSeparator( " \t\r\n,;\"" );
	mToken.SetCutOff( "{}" );

	//�@�t�@�C���I�[�܂Ń��[�v
	while( !mToken.IsEnd() )
	{
#if 0 // azarashin modified...
		mToken.Next();
#endif 
        DEBUG_LOG( "Check Token : %s", mToken.GetAsChar() );

        CheckNode(-1);
	}

	if ( mModel.GetNumMeshes() > 0 )
	{
        if ( mBoneCaches.size() > 0 )
        {
            //�@�X�L���E�F�C�g�ƃX�L���C���f�b�N�X�̐ݒ�
            SetSkinDataFromCache(); 

           	//�@�Ō�̃{�[����ǉ�
#if 0 // azarashin modified...
    		uint32_t index = mModel.GetNumMeshes() -1;
    		mModel.mMeshes[index].SetBones( mBoneCaches );
#endif
			mModel.mBaseBone = mBoneCaches; 
        }

        if ( mAnimationCaches.size() > 0 )
        {
            uint32_t index = mModel.GetNumClips() - 1;
            mModel.mClips[index].SetFrames( mAnimationCaches );
            mModel.mClips[index].SetDuration( mMaxTime );
        }
	}

#if 0
	//�@�f�o�b�O���O

    //�@���O�t�@�C�����I�[�v��
	FILE *pFile = NULL;
	pFile = fopen( "model.log", "w" );
	if ( pFile == NULL )
	{
		return;
	}

    //�@���O�t�@�C���ɏ�������
	fprintf( pFile, "Num Meshes: %d\n", mModel.GetNumMeshes() );
	fprintf( pFile, "Num Clips: %d\n", mModel.GetNumClips() );
	for( uint32_t i=0; i<mModel.GetNumMeshes(); i++ )
	{
		fprintf( pFile, "\n" );
		fprintf( pFile, "Mesh[%d] Num Positions: %d\n", i, mModel.mMeshes[i].GetNumPositions() );
		fprintf( pFile, "Mesh[%d] Num Indices: %d\n", i, mModel.mMeshes[i].GetNumIndices() );
		fprintf( pFile, "Mesh[%d] Num Faces: %d\n", i, mModel.mMeshes[i].GetNumIndices()/3 );
		fprintf( pFile, "Mesh[%d] Num Normals: %d\n", i, mModel.mMeshes[i].GetNumNormals() );
		fprintf( pFile, "Mesh[%d] Num Colors: %d\n", i, mModel.mMeshes[i].GetNumColors () );
		fprintf( pFile, "Mesh[%d] Num TexCoords: %d\n", i, mModel.mMeshes[i].GetNumTexCoords() );
		fprintf( pFile, "Mesh[%d] Num Tangents: %d\n", i, mModel.mMeshes[i].GetNumTangents() );
		fprintf( pFile, "Mesh[%d] Num Binormals: %d\n", i, mModel.mMeshes[i].GetNumBinormals() );
		fprintf( pFile, "Mesh[%d] Num SkinIndices: %d\n", i, mModel.mMeshes[i].GetNumSkinIndices() );
		fprintf( pFile, "Mesh[%d] Num SkinWeights: %d\n", i, mModel.mMeshes[i].GetNumSkinWeights() );
		fprintf( pFile, "Mesh[%d] Num Subsets: %d\n", i, mModel.mMeshes[i].GetNumSubsets() );
		fprintf( pFile, "Mesh[%d] Num Materails: %d\n", i, mModel.mMeshes[i].GetNumMaterials() );
		fprintf( pFile, "Mesh[%d] Num Bones: %d\n", i, mModel.mMeshes[i].GetNumBones() );
		fprintf( pFile, "\n" );

		for( uint32_t j=0; j<mModel.mMeshes[i].GetNumPositions(); j++ )
		{
			fprintf( pFile, "Mesh[%d]: Positions[%d] = (%3f, %3f, %3f)\n", i, j, mModel.mMeshes[i].mPositions[j].x, mModel.mMeshes[i].mPositions[j].y, mModel.mMeshes[i].mPositions[j].z );
		}
		for( uint32_t j=0; j<mModel.mMeshes[i].GetNumIndices(); j++ )
		{
			fprintf( pFile, "Mesh[%d]: Indices[%d] = %d\n", i, j, mModel.mMeshes[i].mIndices[j] );
		}
		for( uint32_t j=0; j<mModel.mMeshes[i].GetNumNormals(); j++ )
		{
			fprintf( pFile, "Mesh[%d]: Normals[%d] = (%3f, %3f, %3f)\n", i, j, mModel.mMeshes[i].mNormals[j].x, mModel.mMeshes[i].mNormals[j].y, mModel.mMeshes[i].mNormals[j].z );
		}
		for( uint32_t j=0; j<mModel.mMeshes[i].GetNumColors(); j++ )
		{
			fprintf( pFile, "Mesh[%d]: Colors[%d] = (%d, %d, %d, %d)\n", i, j, mModel.mMeshes[i].mColors[j].val[0], mModel.mMeshes[i].mColors[j].val[1], mModel.mMeshes[i].mColors[j].val[2], mModel.mMeshes[i].mColors[j].val[3] );
		}
		for( uint32_t j=0; j<mModel.mMeshes[i].GetNumTexCoords(); j++ )
		{
			fprintf( pFile, "Mesh[%d]: TexCoords[%d] = (%3f, %3f)\n", i, j, mModel.mMeshes[i].mTexCoords[j].x, mModel.mMeshes[i].mTexCoords[j].y );
		}
		for( uint32_t j=0; j<mModel.mMeshes[i].GetNumTangents(); j++ )
		{
			fprintf( pFile, "Mesh[%d]: Tangents[%d] = (%3f, %3f, %3f)\n", i, j, mModel.mMeshes[i].mTangents[j].x, mModel.mMeshes[i].mTangents[j].y, mModel.mMeshes[i].mTangents[j].z );
		}
		for( uint32_t j=0; j<mModel.mMeshes[i].GetNumBinormals(); j++ )
		{
			fprintf( pFile, "Mesh[%d]: Binormals[%d] = (%3f, %3f, %3f)\n", i, j, mModel.mMeshes[i].mBinormals[j].x, mModel.mMeshes[i].mBinormals[j].y, mModel.mMeshes[i].mBinormals[j].z );
		}
		for( uint32_t j=0; j<mModel.mMeshes[i].GetNumSkinIndices(); j++ )
		{
			fprintf( pFile, "Mesh[%d]: Skin Index[%d] = (%d, %d, %d, %d)\n", i, j, mModel.mMeshes[i].mSkinIndices[j].val[0], mModel.mMeshes[i].mSkinIndices[j].val[1], mModel.mMeshes[i].mSkinIndices[j].val[2], mModel.mMeshes[i].mSkinIndices[j].val[3] );
		}
		for( uint32_t j=0; j<mModel.mMeshes[i].GetNumSkinWeights(); j++ )
		{
			fprintf( pFile, "Mesh[%d]: Skin Weight[%d] = (%f, %f, %f, %f)\n", i, j, mModel.mMeshes[i].mSkinWeights[j].x, mModel.mMeshes[i].mSkinWeights[j].y, mModel.mMeshes[i].mSkinWeights[j].z, mModel.mMeshes[i].mSkinWeights[j].w );
		}
		for( uint32_t j=0; j<mModel.mMeshes[i].GetNumSubsets(); j++ )
		{
			fprintf( pFile, "\n" );
			fprintf( pFile, "Mesh[%d]: Subset[%d] Material Index = %d\n", i, j, mModel.mMeshes[i].mSubsets[j].GetMaterialIndex() );
			fprintf( pFile, "Mesh[%d]: Subset[%d] Face Index = %d\n", i, j, mModel.mMeshes[i].mSubsets[j].GetFaceIndex() );
			fprintf( pFile, "Mesh[%d]: Subset[%d] Num Faces = %d\n", i, j, mModel.mMeshes[i].mSubsets[j].GetNumFace() );
		}
		for( uint32_t j=0; j<mModel.mMeshes[i].GetNumMaterials(); j++ )
		{
			fprintf( pFile, "\n" );
			fprintf( pFile, "Mesh[%d]: Material[%d] Diffuse = (%f, %f, %f, %f)\n", i, j, mModel.mMeshes[i].mMaterials[j].mDiffuse.x, mModel.mMeshes[i].mMaterials[j].mDiffuse.y, mModel.mMeshes[i].mMaterials[j].mDiffuse.z, mModel.mMeshes[i].mMaterials[j].mDiffuse.w );
			fprintf( pFile, "Mesh[%d]: Material[%d] Specular = (%f, %f, %f)\n", i, j, mModel.mMeshes[i].mMaterials[j].mSpecular.x, mModel.mMeshes[i].mMaterials[j].mSpecular.y, mModel.mMeshes[i].mMaterials[j].mSpecular.z );
			fprintf( pFile, "Mesh[%d]: Material[%d] Power = %f\n", i, j, mModel.mMeshes[i].mMaterials[j].mPower );
			fprintf( pFile, "Mesh[%d]: Material[%d] Emissive = (%f, %f, %f)\n", i, j, mModel.mMeshes[i].mMaterials[j].mEmissive.x, mModel.mMeshes[i].mMaterials[j].mEmissive.y, mModel.mMeshes[i].mMaterials[j].mEmissive.z );
		}
		for( uint32_t j=0; j<mModel.mMeshes[i].GetNumBones(); j++ )
		{
			fprintf( pFile, "\n" );
			fprintf( pFile, "Mesh[%d]: Bone[%d] Name = %s\n", i, j, mModel.mMeshes[i].mBones[j].GetName().c_str() );
			fprintf( pFile, "Mesh[%d]: Bone[%d] Parent Index = %d\n", i, j, mModel.mMeshes[i].mBones[j].GetParentIndex() );
			fprintf( pFile, "Mesh[%d]: Bone[%d] Bind Matrix[0] = (%f, %f, %f, %f)\n", i, j, mModel.mMeshes[i].mBones[j].mBindMatrix._11, mModel.mMeshes[i].mBones[j].mBindMatrix._12, mModel.mMeshes[i].mBones[j].mBindMatrix._13, mModel.mMeshes[i].mBones[j].mBindMatrix._14 );
			fprintf( pFile, "Mesh[%d]: Bone[%d] Bind Matrix[1] = (%f, %f, %f, %f)\n", i, j, mModel.mMeshes[i].mBones[j].mBindMatrix._21, mModel.mMeshes[i].mBones[j].mBindMatrix._22, mModel.mMeshes[i].mBones[j].mBindMatrix._23, mModel.mMeshes[i].mBones[j].mBindMatrix._24 );
			fprintf( pFile, "Mesh[%d]: Bone[%d] Bind Matrix[2] = (%f, %f, %f, %f)\n", i, j, mModel.mMeshes[i].mBones[j].mBindMatrix._31, mModel.mMeshes[i].mBones[j].mBindMatrix._32, mModel.mMeshes[i].mBones[j].mBindMatrix._33, mModel.mMeshes[i].mBones[j].mBindMatrix._34 );
			fprintf( pFile, "Mesh[%d]: Bone[%d] Bind Matrix[3] = (%f, %f, %f, %f)\n", i, j, mModel.mMeshes[i].mBones[j].mBindMatrix._41, mModel.mMeshes[i].mBones[j].mBindMatrix._42, mModel.mMeshes[i].mBones[j].mBindMatrix._43, mModel.mMeshes[i].mBones[j].mBindMatrix._44 );
			fprintf( pFile, "Mesh[%d]: Bone[%d] Pose Matrix[0] = (%f, %f, %f, %f)\n", i, j, mModel.mMeshes[i].mBones[j].mPoseMatrix._11, mModel.mMeshes[i].mBones[j].mPoseMatrix._12, mModel.mMeshes[i].mBones[j].mPoseMatrix._13, mModel.mMeshes[i].mBones[j].mPoseMatrix._14 );
			fprintf( pFile, "Mesh[%d]: Bone[%d] Pose Matrix[1] = (%f, %f, %f, %f)\n", i, j, mModel.mMeshes[i].mBones[j].mPoseMatrix._21, mModel.mMeshes[i].mBones[j].mPoseMatrix._22, mModel.mMeshes[i].mBones[j].mPoseMatrix._23, mModel.mMeshes[i].mBones[j].mPoseMatrix._24 );
			fprintf( pFile, "Mesh[%d]: Bone[%d] Pose Matrix[2] = (%f, %f, %f, %f)\n", i, j, mModel.mMeshes[i].mBones[j].mPoseMatrix._31, mModel.mMeshes[i].mBones[j].mPoseMatrix._32, mModel.mMeshes[i].mBones[j].mPoseMatrix._33, mModel.mMeshes[i].mBones[j].mPoseMatrix._34 );
			fprintf( pFile, "Mesh[%d]: Bone[%d] Pose Matrix[3] = (%f, %f, %f, %f)\n", i, j, mModel.mMeshes[i].mBones[j].mPoseMatrix._41, mModel.mMeshes[i].mBones[j].mPoseMatrix._42, mModel.mMeshes[i].mBones[j].mPoseMatrix._43, mModel.mMeshes[i].mBones[j].mPoseMatrix._44 );
		}
	}
	for( uint32_t i=0; i<mModel.GetNumClips(); ++i )
	{
		fprintf(pFile, "\n" );
		fprintf( pFile, "Clip[%d] Name: %s\n", i, mModel.mClips[i].GetName().c_str() );
	    fprintf( pFile, "Clip[%d] Num Frames: %d\n", i, mModel.mClips[i].GetNumFrames() );
        fprintf( pFile, "Clip[%d] Duration : %f\n", mModel.mClips[i].GetDuration() );
		for( uint32_t j=0; j<mModel.mClips[i].GetNumFrames(); ++j )
		{
			fprintf( pFile, "\n" );
			fprintf( pFile, "Clip[%d] Frame[%d]: Bone Index = %d\n", i, j, mModel.mClips[i].mFrames[j].GetBoneIndex() );
            fprintf( pFile, "Clip[%d] Frame[%d]: Num KeyPoses = %d\n", i, j, mModel.mClips[i].mFrames[j].GetNumKeys() );
            for( uint32_t k=0; k<mModel.mClips[i].mFrames[j].GetNumKeys(); ++k )
            {
                fprintf( pFile, "\n" );
                fprintf( pFile, "Clip[%d] Frame[%d] Key[%d]: Time = %d\n", i, j, k, mModel.mClips[i].mFrames[j].mKeys[k].GetTime() );
                fprintf( pFile, "Clip[%d] Frame[%d] Key[%d]: Transform[0] = (%f, %f, %f, %f)\n", i, j, k, mModel.mClips[i].mFrames[j].mKeys[k].mTransform._11,  mModel.mClips[i].mFrames[j].mKeys[k].mTransform._12,  mModel.mClips[i].mFrames[j].mKeys[k].mTransform._13,  mModel.mClips[i].mFrames[j].mKeys[k].mTransform._14 );
                fprintf( pFile, "Clip[%d] Frame[%d] Key[%d]: Transform[1] = (%f, %f, %f, %f)\n", i, j, k, mModel.mClips[i].mFrames[j].mKeys[k].mTransform._21,  mModel.mClips[i].mFrames[j].mKeys[k].mTransform._22,  mModel.mClips[i].mFrames[j].mKeys[k].mTransform._23,  mModel.mClips[i].mFrames[j].mKeys[k].mTransform._24 );
                fprintf( pFile, "Clip[%d] Frame[%d] Key[%d]: Transform[2] = (%f, %f, %f, %f)\n", i, j, k, mModel.mClips[i].mFrames[j].mKeys[k].mTransform._31,  mModel.mClips[i].mFrames[j].mKeys[k].mTransform._32,  mModel.mClips[i].mFrames[j].mKeys[k].mTransform._33,  mModel.mClips[i].mFrames[j].mKeys[k].mTransform._34 );
                fprintf( pFile, "Clip[%d] Frame[%d] Key[%d]: Transform[3] = (%f, %f, %f, %f)\n", i, j, k, mModel.mClips[i].mFrames[j].mKeys[k].mTransform._41,  mModel.mClips[i].mFrames[j].mKeys[k].mTransform._42,  mModel.mClips[i].mFrames[j].mKeys[k].mTransform._43,  mModel.mClips[i].mFrames[j].mKeys[k].mTransform._44 );
            }
		}
	}
    fprintf( pFile, "\nEnd Of File.\n" );
	fclose( pFile );
#endif

	//�@���������
	mBoneCaches.clear();
	mFaceElement.clear();
    mSkinWeightCaches.clear();
    mAnimationCaches.clear();
}

///----------------------------------------------------------------------
///<summary>
///�m�[�h��ǂݔ�΂�
///</summary>
///----------------------------------------------------------------------
void ModelLoaderX::SkipNode()
{
	// '{'���o��܂ŃX�L�b�v
	while( !mToken.IsEnd() )
	{
		mToken.Next();
        DEBUG_LOG( "Skip Token : %s", mToken.GetAsChar() );
		if ( strchr( mToken.GetAsChar(), '{' ) )
			break;
	}

	// �u���b�N��
	int count = 1;

	//�@�I�[�܂��̓u���b�N�����I���܂Ń��[�v
	while( !mToken.IsEnd() && count > 0 )
	{
		//�@�g�[�N�����擾
		mToken.Next();
        DEBUG_LOG( "Skip Token : %s", mToken.GetAsChar() );

		//�@�u���b�N�J�n
		if ( strchr( mToken.GetAsChar(), '{' ) )
		{
			count++;
		}
		//�@�u���b�N�I��
		else if ( strchr( mToken.GetAsChar(), '}' ) )
		{
			count--;
		}		
	}

	//�@�I�[�̏ꍇ������̂Ń`�F�b�N
	if ( count > 0 )
	{
		std::cerr << "Error : �J�b�R����v���Ă��܂���\n";
	}
}

///----------------------------------------------------------------------
///<summary>
///���b�V���̍\����͂��s��
///</summary>
///----------------------------------------------------------------------
void ModelLoaderX::ParseMesh(int parent)
{
	Mesh mesh;

	//�@���b�V���f�[�^��ǂݎ������Ń{�[����ǉ�����
	if ( mModel.GetNumMeshes() > 0 )
	{
#if 0 // azarashin modified...
		uint32_t index = mModel.GetNumMeshes() -1;
		mModel.mMeshes[index].SetBones( mBoneCaches );
		mBoneCaches.clear();
#endif
	}

	mToken.Next();
	if ( !mToken.Check( "{" ) )
	{
		//���O���Z�b�g
#if 0 // azarashin modified...
		std::string name = std::string( mToken.GetNextAsChar() );
#else
		std::string name = std::string( mToken.GetAsChar() );
#endif
		DEBUG_LOG( "Mesh Name : %s", name.c_str() );
		mesh.SetName( name );

		//'{'���o��܂ŃX�L�b�v
		while( !mToken.IsEnd() )
		{
			mToken.Next();
			if ( strchr( mToken.GetAsChar(), '{' ) )
				break;
		}
	}
	else
	{
		//�K���Ȗ��O���Z�b�g
		char temp[255];
		std::sprintf( temp, "mesh_%03d", mModel.GetNumMeshes() );
		std::string name = std::string( temp );
		DEBUG_LOG( "Mesh Name : %s", name.c_str() );
		mesh.SetName( name );
	}

	//�@���_�����擾
	int vertices_size = 0;
	vertices_size = mToken.GetNextAsInt();
	DEBUG_LOG( "Number Of Vertices : %d", vertices_size );

	//�@���_���W���擾
	std::vector< Vector3 > vertices;
	vertices.resize( vertices_size );
	for( int i=0; i<vertices_size; i++ )
	{
		vertices[i].x = mToken.GetNextAsFloat();
 		vertices[i].y = mToken.GetNextAsFloat();
 		vertices[i].z = mToken.GetNextAsFloat();
 		DEBUG_LOG( "Vertex[%d] : %f,  %f, %f", i, vertices[i].x, vertices[i].y, vertices[i].z );
	}

	//�@�C���f�b�N�X�����擾
	int indices_size = 0;
	indices_size = mToken.GetNextAsInt();
	DEBUG_LOG( "Number Of Vertex Indices : %d", indices_size );

	mFaceElement.clear();
	mFaceElement.resize( indices_size );

	//�@�O�p�`�Ǝl�p�`���������Ă���ꍇ������̂Ń��T�C�Y�͂��Ȃ�
	std::vector< uint32_t > indices;
	for( int i=0; i<indices_size; i++ )
	{
		mToken.Next();
		if ( mToken.Check( "3" ) )
		{
			mFaceElement[i] = 3;

			int idx[3] = { -1 };
			idx[0] = mToken.GetNextAsInt();
			idx[1] = mToken.GetNextAsInt();
			idx[2] = mToken.GetNextAsInt();

			//�@�C���f�b�N�X���i�[
			for( int j=0; j<3; ++j )
			{
				DEBUG_LOG( "Vertex Index[%d] : %d", i, idx[j] );
				indices.push_back( idx[j] );
			}
		}
		else if ( mToken.Check( "4" ) )
		{
			mFaceElement[i] = 4;

			//�@�l�p�`���O�p�`2�����ɕϊ�
			int idx[6] = { -1 };
			idx[0] = mToken.GetNextAsInt();
			idx[1] = mToken.GetNextAsInt();
			idx[2] = mToken.GetNextAsInt();

            idx[3] = idx[2];
   			idx[4] = mToken.GetNextAsInt();
   			idx[5] = idx[0];

			for( int j=0; j<6; j++ )
			{
				DEBUG_LOG( "Vertex Index[%d] : %d", i, idx[j] );
				indices.push_back( idx[j] );
			}
		}
	}

	//�@���_���W���Z�b�g
	mesh.SetPositions( vertices );

	//�@���_�C���f�b�N�X���Z�b�g
	mesh.SetIndices( indices );

	mesh.SetRefBone(parent); 

	//�@���b�V����ǉ�
	mModel.mMeshes.push_back( mesh );

	//�@���������
	vertices.clear();
	indices.clear();


#if 1 // azarashin modified...
	while(!mToken.Check( "}" ) ) 
	{
		//�@�m�[�h�`�F�b�N
    	CheckNode(parent);
	}
	mToken.Next();
#endif 

}


///----------------------------------------------------------------------
///<summary>
///�@���x�N�g���̍\����͂��s��
///</summary>
///----------------------------------------------------------------------
void ModelLoaderX::ParseMeshNormals(int parent)
{
	//�@'{'���o��܂ŃX�L�b�v
	while ( !mToken.IsEnd() )
	{
		mToken.Next();
		if ( mToken.Check( "{" ) )
			break;
	}

	//�@�@���x�N�g�������擾
	int normal_size = 0;
	normal_size = mToken.GetNextAsInt();
	DEBUG_LOG( "Number Of Normals : %d", normal_size );

	//�@�@���x�N�g�����擾
	std::vector< Vector3 > normals;
	normals.resize( normal_size );
	for( int i=0; i<normal_size; i++ )
	{
		normals[i].x = mToken.GetNextAsFloat();
		normals[i].y = mToken.GetNextAsFloat();
		normals[i].z = mToken.GetNextAsFloat();
		DEBUG_LOG( "Normal[%d] : %f,  %f, %f", i, normals[i].x, normals[i].y, normals[i].z );
	}

	//�@�@���C���f�b�N�X�����擾
	int indices_size = 0;
	indices_size = mToken.GetNextAsInt();
	DEBUG_LOG( "Number Of Normal Indices : %d", indices_size );

	//�@�@���C���f�b�N�X���擾
	std::vector< uint32_t > indices;
	for( int i=0; i<indices_size; i++ )
	{
		mToken.Next();

		if ( mToken.Check( "3" ) )
		{
			int idx[3] = { -1 };
			idx[0] = mToken.GetNextAsInt();
			idx[1] = mToken.GetNextAsInt();
			idx[2] = mToken.GetNextAsInt();

			//�@�C���f�b�N�X���i�[
			for( int j=0; j<3; j++ )
			{
				DEBUG_LOG( "Normal Index[%d] : %d", i, idx[j] );
				indices.push_back( idx[j] );
			}
		}
		else if ( mToken.Check( "4" ) )
		{
			//�@�l�p�`���O�p�`2�����ɕϊ�
			int idx[6] = { -1 };
			idx[0] = mToken.GetNextAsInt();
			idx[1] = mToken.GetNextAsInt();
			idx[2] = mToken.GetNextAsInt();
			
            idx[3] = idx[2];
            idx[4] = mToken.GetNextAsInt();
          	idx[5] = idx[0];
       
			for( int j=0; j<6; j++ )
			{
				DEBUG_LOG( "Normal Index[%d] : %d", i, idx[j] );
				indices.push_back( idx[j] );
			}
		}
	}

	uint32_t index = mModel.GetNumMeshes() -1;
    bool isFixed = false;

    if ( mModel.mMeshes[index].GetNumPositions() != normals.size() )
    {
        // ���_�C���f�b�N�X�Ɩ@���C���f�b�N�X���قȂ�ꍇ�C
        // �������`�悳��Ȃ��̂ŁC�@���x�N�g���̊i�[���Ԃ�ύX����
        
        std::vector< Vector3 > sorted_normals;
        sorted_normals.resize( mModel.mMeshes[index].GetNumPositions() );

        for( size_t i=0; i<indices.size(); ++i )
        {
            //�@�@���C���f�b�N�X�ƒ��_�C���f�b�N�X����v����悤�Ɋi�[����
            sorted_normals[ mModel.mMeshes[index].GetIndex( i ) ] = normals[ indices[ i ] ];
        }

      	//�@�@���x�N�g�����i�[
        mModel.mMeshes[index].SetNormals( sorted_normals );
        sorted_normals.clear();

        //�@�C�������t���O�𗧂Ă�
        isFixed = true;
    }

    if ( !isFixed )
    {
        //�@���f������ǂݍ��񂾖@�������̂܂܊i�[
    	mModel.mMeshes[index].SetNormals( normals );
    }

	//�@���������
	normals.clear();
	indices.clear();
#if 1 // azarashin modified...
	while(!mToken.Check( "}" ) ) 
	{
		//�@�m�[�h�`�F�b�N
    	CheckNode(parent);
	}
	mToken.Next();
#endif 
}

///-----------------------------------------------------------------------
///<summary>
///�e�N�X�`�����W�̍\����͂��s��
///</summary>
///-----------------------------------------------------------------------
void ModelLoaderX::ParseMeshTextureCoords(int parent)
{
	//�@'{'���o��܂ŃX�L�b�v
	while ( !mToken.IsEnd() )
	{
		mToken.Next();
		if ( mToken.Check( "{" ) )
			break;
	}

	//�@�e�N�X�`�����W�����擾
	int texcoord_size = 0;
	texcoord_size = mToken.GetNextAsInt();
	DEBUG_LOG( "Number Of TexCoords : %d", texcoord_size );
	
	//�@�e�N�X�`�����W���擾
	std::vector< Vector2 > texcoords;
	texcoords.resize( texcoord_size );
	for( int i=0; i<texcoord_size; i++ )
	{
		texcoords[i].x = mToken.GetNextAsFloat();
		texcoords[i].y = mToken.GetNextAsFloat();
		DEBUG_LOG( "TexCoord[%d] : %f, %f", i, texcoords[i].x, texcoords[i].y );
	}

	//�@�e�N�X�`�����W�����̂܂܊i�[
	uint32_t index = mModel.GetNumMeshes() -1;
	mModel.mMeshes[index].SetTexCoords( texcoords );

	//�@���������
	texcoords.clear();
#if 1 // azarashin modified...
	while(!mToken.Check( "}" ) ) 
	{
		//�@�m�[�h�`�F�b�N
    	CheckNode(parent);
	}
	mToken.Next();
#endif 
}

///----------------------------------------------------------------------
///<summary>
///���_�J���[���\����͂���
///</summary>
///----------------------------------------------------------------------
void ModelLoaderX::ParseMeshVertexColors(int parent)
{
	while( !mToken.IsEnd() )
	{
		mToken.Next();
		if ( mToken.Check( "{" ) )
			break;
	}

	//�@�J���[�����擾
	int color_size = 0;
	color_size = mToken.GetNextAsInt();
	DEBUG_LOG( "Number Of Vertex Colors: %d", color_size );

	//�@�������m��
	std::vector< uint32_t > indices;
	std::vector< Vector4 > colors;
	indices.resize( color_size );
	colors.resize( color_size );

	for( int i=0; i<color_size; i++ )
	{
		//�@�C���f�b�N�X�ƐF�f�[�^���擾
		indices[i] = mToken.GetNextAsInt();
		colors[i].x = mToken.GetNextAsFloat();
		colors[i].y = mToken.GetNextAsFloat();
		colors[i].z = mToken.GetNextAsFloat();
		colors[i].w = mToken.GetNextAsFloat();

		DEBUG_LOG( "Index: %d, Color = (%f, %f, %f, %f)", indices[i], colors[i].x, colors[i].y, colors[i].z, colors[i].w );
	}

	//�@�i�[�`���ɕϊ�����
	std::vector< UByte4 > vcolors;
	vcolors.resize( color_size );
	for( int i=0; i<color_size; i++ )
	{
		vcolors[ indices[i] ].val[0] = static_cast< uint8_t >( colors[i].x * 255.0f );
		vcolors[ indices[i] ].val[1] = static_cast< uint8_t >( colors[i].y * 255.0f );
		vcolors[ indices[i] ].val[2] = static_cast< uint8_t >( colors[i].z * 255.0f );
		vcolors[ indices[i] ].val[3] = static_cast< uint8_t >( colors[i].w * 255.0f );

		DEBUG_LOG( "Vertex Colors = (%d, %d, %d, %d)", vcolors[ indices[i] ].val[0], vcolors[ indices[i] ].val[1], vcolors[ indices[i] ].val[2], vcolors[ indices[i] ].val[3] );
	}

	//�@���_�J���[���i�[
	uint32_t mesh_index = mModel.GetNumMeshes() -1;
	mModel.mMeshes[mesh_index].SetColors( vcolors );

	//�@���������
	indices.clear();
	colors.clear();
	vcolors.clear();

}

///-----------------------------------------------------------------------
///<summary>
///�}�e���A�����X�g�̍\����͂��s��
///</summary>
///-----------------------------------------------------------------------
void ModelLoaderX::ParseMeshMaterialList(int parent)
{
	// '{'���o��܂ŃX�L�b�v
	while ( !mToken.IsEnd() )
	{
		mToken.Next();
		if ( mToken.Check( "{" ) )
			break;
	}

	//�@�}�e���A�������擾
	int material_size = 0;
	material_size = mToken.GetNextAsInt();
	std::vector< Material > materials;
	materials.resize( material_size );
	DEBUG_LOG( "Number Of Materials : %d", material_size );

	//�@�}�e���A���C���f�b�N�X�����擾
    int indices_size = 0;
    indices_size = mToken.GetNextAsInt();

	int prev_index = -1;
	int face_index = 0;
	int face_count = 0;
    for( int i=0; i<indices_size; i++ )
    {
		//�@�}�e���A���C���f�b�N�X�擾
        int index  = mToken.GetNextAsInt();
        DEBUG_LOG( "Material Index[%d] : %d", i, index );

		//�@�ȑO�̃}�e���A���C���f�b�N�X�ƈقȂ�ꍇ
		if ( prev_index != index )
		{
			//�@�X�V
			prev_index = index;

			//�@�ʐ���������
			face_count = 1;
			if ( mFaceElement[i] == 4 )
			{
				face_count++;
			}

       		//�@�T�u�Z�b�g��ǉ�
			MeshSubset subset;
			subset.SetFaceIndex( face_index );
			subset.SetMaterialIndex( index );
			subset.SetNumFace( face_count );
			uint32_t mesh_index = mModel.GetNumMeshes() -1;
			mModel.mMeshes[mesh_index].mSubsets.push_back( subset );
		}
		else
		{
			//�@�ʐ����J�E���g
			face_count++;
			if ( mFaceElement[i] == 4 )
			{
				face_count++;
			}

			//�@�ʐ����X�V
			uint32_t mesh_index = mModel.GetNumMeshes() -1;
			uint32_t subset_index = mModel.mMeshes[mesh_index].GetNumSubsets() -1;
			mModel.mMeshes[mesh_index].mSubsets[subset_index].SetNumFace( face_count );
		}

        //�@�������ʂ̃C���f�b�N�X���Z�o
		face_index++;
		if ( mFaceElement[i] == 4 )
		{
			face_index++;
		}
    }

	for( int i=0; i<material_size; i++ )
	{
		//�@"Material"���o��܂ŃX�L�b�v
		while( !mToken.IsEnd() )
		{
			if ( mToken.Check( "Material" ) )
				break;

			mToken.Next();
		}

		//�@�}�e���A�����擾
		mToken.Next();
		if ( !mToken.Check( "{" ) )
		{
			//�@���O���Z�b�g
#if 0 // azarashin modified...
			std::string name = std::string( mToken.GetNextAsChar() );
#else
			std::string name = std::string( mToken.GetAsChar() );
#endif 
			DEBUG_LOG( "Material Name : %s", name.c_str() );
			materials[i].SetName( name );

			//�@'{'���o��܂ŃX�L�b�v
			while( !mToken.IsEnd() )
			{
				mToken.Next();
				if ( mToken.Check( "{" ) )
					break;
			}
		}
		else
		{
			//�@�K���Ȗ��O���Z�b�g
			char temp[255];
			std::sprintf( temp, "material_%03d", i );
			std::string name = std::string( temp );
			DEBUG_LOG( "Material Name : %s", name.c_str() );
			materials[i].SetName( name );
		}

		// Diffuse�擾
		float diffuse[4];
		diffuse[0] = mToken.GetNextAsFloat();
		diffuse[1] = mToken.GetNextAsFloat();
		diffuse[2] = mToken.GetNextAsFloat();
		diffuse[3] = mToken.GetNextAsFloat();
		DEBUG_LOG( "Diffuse : %f, %f, %f, %f", diffuse[0], diffuse[1], diffuse[2], diffuse[3] );
		Vector4 dif( diffuse );
		materials[i].SetDiffuse( dif );

		// Power�擾
		float power;
		power = mToken.GetNextAsFloat();
		DEBUG_LOG( "Power : %f", power );
		materials[i].SetPower( power );

		// Specular�擾
		float specular[3];
		specular[0] = mToken.GetNextAsFloat();
		specular[1] = mToken.GetNextAsFloat();
		specular[2] = mToken.GetNextAsFloat();
		DEBUG_LOG( "Specular : %f, %f, %f", specular[0], specular[1], specular[2] );
		Vector3 spe( specular );
		materials[i].SetSpecular( spe );

		// Emissive�擾
		float emissive[3];
		emissive[0] = mToken.GetNextAsFloat();
		emissive[1] = mToken.GetNextAsFloat();
		emissive[2] = mToken.GetNextAsFloat();
		DEBUG_LOG( "Emissive : %f, %f, %f", emissive[0], emissive[1], emissive[2] );
		Vector3 emi( emissive );
		materials[i].SetEmissive( emi );

		// ���̃g�[�N�����擾���`�F�b�N����
		mToken.Next();
		if ( mToken.Check( "}" ) )
		{
			continue;
		}
		else if ( mToken.Check( "TextureFilename" ) )
		{
			// '{'���o��܂ŃX�L�b�v
			while( !mToken.IsEnd() )
			{
				mToken.Next();
				if ( mToken.Check( "{" ) )
					break;
			}

			//�@�e�N�X�`���t�@�C�����擾
			std::string texture_filename = std::string( mToken.GetNextAsChar() );
			DEBUG_LOG( "Texture File Name : %s", texture_filename.c_str() );
			materials[i].SetTextureName( texture_filename );

			// '}'���o��܂ŃX�L�b�v
			while( !mToken.IsEnd() )
			{
				mToken.Next();
				if ( mToken.Check( "}" ) )
					break;
			}			
		}
	}
#if 1 // azarashin
	mToken.Next();
#endif

	while( !mToken.IsEnd() )
	{
		if ( mToken.Check( "}" ) )
			break;

		mToken.Next();
	}
#if 1 // azarashin
	mToken.Next();
#endif

	//�@�}�e���A�����Z�b�g
	uint32_t index = mModel.GetNumMeshes() -1;
	mModel.mMeshes[index].SetMaterials( materials );

	//�@���������
	materials.clear();
	mFaceElement.clear();
}

///-----------------------------------------------------------------------
///<summary>
///�t���[���̍\����͂��s��
///</summary>
///-----------------------------------------------------------------------
void ModelLoaderX::ParseFrame(int parent)
{
	Bone bone;
#if 0 // azarashin modified...
	bone.SetParentIndex( mParentIndex );

	mParentIndex++;
#else 
	bone.SetParentIndex( parent );
#endif

    mToken.Next();
    if ( mToken.Check( "{" ) )
    {
        //�@�K���ȃt���[������ݒ�
		char temp[255];
		uint32_t frame_index = mBoneCaches.size();
		std::sprintf( temp, "frame_%03d", frame_index );
		std::string name = std::string( temp );
		DEBUG_LOG( "Frame Name : %s", name.c_str() );
		bone.SetName( name );
    }
    else
    {
    	//�@�t���[�������擾
    	std::string name = std::string( mToken.GetAsChar() );
    	DEBUG_LOG( "Frame Name : %s", name.c_str() );
		bone.SetName( name );
    
        // '{'���o��܂ŃX�L�b�v
    	while( !mToken.IsEnd() )
    	{
    		mToken.Next();
    		if ( mToken.Check( "{"  ) )
    			break;
    	}
    }


	//�@���̃g�[�N����"FrameTransformMatrix"�ł��邱�Ƃ��m�F
	mToken.Next();
	if( mToken.Check( "FrameTransformMatrix" ) )
	{
		// '{'���o��܂ŃX�L�b�v
		while( !mToken.IsEnd() )
		{
			mToken.Next();
			if ( mToken.Check( "{" ) )
				break;
		}

		// �s����擾
		float matrix[16] = { 0 };
		for( int i=0; i<16; i++ )
		{
			matrix[i] = mToken.GetNextAsFloat();
			DEBUG_LOG( "FrameTransformMatrix[%d] : %f", i, matrix[i] );
		}

        // �L���b�V���ɒǉ�
		Matrix mat( matrix );
		bone.SetPoseMatrix( mat );
		mBoneCaches.push_back( bone );

		// '}'���o��܂ŃX�L�b�v
		while( !mToken.IsEnd() )
		{		
			if ( mToken.Check( "}" ) )
				break;
			mToken.Next();
		}		
	}
	//�@�����̂킩����̓X�L�b�v
	else
	{
		SkipNode();
	}

	//�@�q�t���[�������邩�ǂ����`�F�b�N
	mToken.Next();

	int this_node = static_cast< int >( mBoneCaches.size() ) -1; 

#if 1 // azarashin modified...
	while(!mToken.Check( "}" ) ) {

		if ( mToken.Check( "Frame" ) )
		{
			//�@�q�t���[��������Ȃ�΍ċA�Ăяo��
	#if 0 // azarashin modified...
			mParentIndex = static_cast< int >( mBoneCaches.size() ) -1;
			ParseFrame();
	#else 
			ParseFrame(this_node);
	#endif 
		}
		//�@Frame�m�[�h���I���̏ꍇ
		else if ( mToken.Check( "}" ) )
		{
	#if 0 // azarashin modified...
			uint32_t index = mBoneCaches.size() -1;
			mParentIndex = mBoneCaches[index].GetParentIndex();
	#else 
			mParentIndex = parent; 
	#endif
			while( !mToken.IsEnd() )
			{
				//�@Frame�m�[�h�̏I�����A�����Ă��邩�`�F�b�N
				mToken.Next();
				if ( mToken.Check( "}" ) )
				{
					//�@�e�̃C���f�b�N�X��1�O�̐[���ɖ߂�
	#if 0 //azarashin modified...
					index = mParentIndex;
					mParentIndex = mBoneCaches[index].GetParentIndex();
	#endif 
				}
				else
				{
					//�@�m�[�h�`�F�b�N
					CheckNode(parent);
					bone.Release();
					return;
				}
			}
		}
		else
		{
			//�@�m�[�h�`�F�b�N
    		CheckNode(this_node);
		}
	}

	mToken.Next();
#endif 

	bone.Release();
}

///----------------------------------------------------------------------
///<summary>
///�X�L���E�F�C�g�̍\����͂��s��
///</summary>
///----------------------------------------------------------------------
void ModelLoaderX::ParseSkinWeights(int parent)
{
	//�@'{'���o��܂ŃX�L�b�v
	while( !mToken.IsEnd() )
	{
		mToken.Next();
		if ( mToken.Check( "{" ) )
			break;
	}

    uint32_t mesh_index = mModel.GetNumMeshes() -1;
    SkinWeightCache cache( mesh_index );

	//�@�t���[�������擾����
	std::string name = std::string( mToken.GetNextAsChar() );
    cache.SetName( name );
	DEBUG_LOG( "Skin Weight Frame Name : %s", name.c_str() );

	//�@�E�F�C�g�����擾����
	int weight_size = 0;
	weight_size = mToken.GetNextAsInt();
    cache.ResizeWeights( weight_size );
    cache.ResizeIndices( weight_size );
	DEBUG_LOG( "Number Of Skin Weights : %d", weight_size );

	//�@���_�ԍ����擾����
	for( int i=0; i<weight_size; i++ )
	{
		uint32_t index = static_cast< uint32_t >( mToken.GetNextAsInt() );
		DEBUG_LOG( "Skin Index[%d] : %d", i, index );
        cache.SetIndex( i, index );
	}

	//�@�E�F�C�g���擾����
	for( int i=0; i<weight_size; i++ )
	{
		float weight = mToken.GetNextAsFloat();
		DEBUG_LOG( "Skin Weight[%d] : %f", i, weight );
        cache.SetWeight( i, weight );
	}

	//�@�I�t�Z�b�g�s����擾����
	for( int i=0; i<16; i++ )
	{
		float value = mToken.GetNextAsFloat();
		DEBUG_LOG( "Offset Matrix[%d] : %f", i, value );
        cache.SetMatrix( i, value );
	}

	// '}'���o��܂ŃX�L�b�v
	while( !mToken.IsEnd() )
	{
		mToken.Next();
		if ( mToken.Check( "}" ) )
			break;
	}

    //�@�i�[
    mSkinWeightCaches.push_back( cache );

	//�@���������
    cache.Release();

	//�@Frame�m�[�h�̏I�����ǂ�������
	mToken.Next();
	if ( mToken.Check( "}" ) )
	{
		//�@���̃g�[�N����Frame�m�[�h�̏I�����ǂ�������
		while( !mToken.IsEnd() )
		{
			//�@�A�����邩�`�F�b�N
			mToken.Next();
			if ( mToken.Check( "}" ) )
			{
				//�@�e�̃C���f�b�N�X��1�O�̐[���ɂ���
				uint32_t index = mParentIndex;
				mParentIndex = mBoneCaches[index].GetParentIndex();
			}
			else
			{
				//�@�m�[�h�`�F�b�N
				CheckNode(parent);
				return;
			}
		}
	}
	else
	{
		//�@�m�[�h�`�F�b�N
		CheckNode(parent);
	}
}



///----------------------------------------------------------------------
///<summary>
///�A�j���[�V�����Z�b�g�̍\����͂��s��
///</summary>
///----------------------------------------------------------------------
void ModelLoaderX::ParseAnimationSet(int parent)
{
	AnimationClip clip;

    if ( mModel.GetNumClips() > 0 )
    {
        uint32_t clip_index = mModel.GetNumClips() - 1;
        mModel.mClips[clip_index].SetFrames( mAnimationCaches );
        mModel.mClips[clip_index].SetDuration( mMaxTime );
        mAnimationCaches.clear();
        mMaxTime = 0.0f;
    }

    mToken.Next();
    if ( mToken.Check( "{" ) )
    {
        //�@�K���ɃA�j���[�V�����Z�b�g����t����
        char temp[255];
		std::sprintf( temp, "clip_%03d", mModel.GetNumClips() );
		std::string name = std::string( temp );
		DEBUG_LOG( "AnimationSet Name : %s", name.c_str() );
		clip.SetName( name );
    }
    else
    {
		//�@�A�j���[�V�����Z�b�g�����擾
        std::string name = std::string( mToken.GetAsChar() );
        DEBUG_LOG( "AnimationSet Name : %s", name.c_str() );
		clip.SetName( name );

        while( !mToken.IsEnd() )
        {
            mToken.Next();
            if ( mToken.Check( "{" ) )
                break;
        }
    }

	//�@�N���b�v��ǉ�
	mModel.mClips.push_back( clip );
}

///-----------------------------------------------------------------------
///<summary>
///�A�j���[�V�����̍\����͂��s��
///</summary>
///-----------------------------------------------------------------------
void ModelLoaderX::ParseAnimation(int parent)
{
    Animation anime;

    //�@�N���b�v�C���f�b�N�X�����߂�
	uint32_t clip_index = mModel.GetNumClips() -1;

	//�@�A�j���[�V��������ǂݔ�΂�
    mToken.Next();
    if ( mToken.Check( "{" ) )
    {
        char temp[255];
		std::sprintf( temp, "animation_%03d", mModel.GetNumClips() );
		std::string name = std::string( temp );
		DEBUG_LOG( "AnimationSet Name : %s", name.c_str() );
        anime.SetName( name );
    }
    else
    {
		//�@�A�j���[�V���������擾
        std::string anim_name = std::string( mToken.GetAsChar() );
        DEBUG_LOG( "Animation Name : %s", anim_name.c_str() );
        anime.SetName( anim_name );

        while( !mToken.IsEnd() )
        {
            mToken.Next();
            if ( mToken.Check( "{" ) )
                break;
        }
    }
  
	int frame_index = -1;

	//�@�t���[�������ŏ��ɂ���p�^�[���̏ꍇ
    mToken.Next();
    if ( mToken.Check( "{" ) )
    {
		//�@�t���[�������擾
        std::string frame_name = std::string( mToken.GetNextAsChar() );
        DEBUG_LOG( "Animation Frame Name : %s", frame_name.c_str() );

		frame_index = FindFrame( frame_name );
        while( !mToken.IsEnd() )
        {
            mToken.Next();
            if ( mToken.Check( "}" ) )
                break;
        }
    }
    else if ( mToken.Check( "AnimationKey" ) )
    {
        while( !mToken.IsEnd() )
        {
            mToken.Next();
            if ( mToken.Check( "{" ) )
                break;
        }
	
		//�@�^�C�v���擾
        int type = mToken.GetNextAsInt();
        DEBUG_LOG( "Type : %d", type );

		//�@�L�[�T�C�Y���擾
        int key_size = mToken.GetNextAsInt();
        DEBUG_LOG( "Key Size : %d", key_size );
        std::vector< KeyPose > keyPoses;
        keyPoses.resize( key_size );

        
		//�@�L�[�T�C�Y�����[�v
        for ( int i=0; i<key_size; i++ )
        {
			//�@���Ԃƃp�����[�^�T�C�Y���擾
            uint32_t time = static_cast< uint32_t >( mToken.GetNextAsInt() );
            int param_size = mToken.GetNextAsInt();
            DEBUG_LOG( "Time : %d, ParamSize : %d", time, param_size );

			//�@���Ԃ�ݒ�
            keyPoses[i].SetTime( time );

			//�@�t���[�����ŏ��ɂ���p�^�[���̏ꍇ
			if ( frame_index > -1 )
			{
				//�@�t���[���ԍ����i�[
                anime.SetBoneIndex( frame_index );
			}

			std::vector< float > params;
			params.resize( param_size );
			//�@�p�����[�^�T�C�Y�����[�v
            for( int j=0; j<param_size; j++ )
            {
				//�@�p�����[�^�擾
                float param = mToken.GetNextAsFloat();
                DEBUG_LOG( "Param[%d][%d] : %f", i, j, param );
				params[j] = param;
            }

			//�@�^�C�v�ɂ�菈���𕪊�
            switch( type )
            {
                //�@��]
                case 0:
                    {
                        assert( param_size == 4 );
                        Quaternion rotate( params[0], params[1], params[2], params[3] );
                        keyPoses[i].SetRotate( rotate );
                    }
                    break;

                //�@�g��k��
                case 1:
                    {
                        assert( param_size == 3 );
                        Vector3 scale( params[0], params[1], params[2] );
                        keyPoses[i].SetScale( scale );
                    }
                    break;

                //�@���s�ړ�
                case 2:
                    {
                        assert( param_size == 3 );
                        Vector3 translate( params[0], params[1], params[2] );
                        keyPoses[i].SetTranslate( translate );
                    }
                    break;

                //�@�s��
                case 3:
                case 4: // Tiny��p�[�u
                    {
                        assert( param_size == 16 );
						Matrix matrix;
						matrix._11 = params[0];		matrix._12 = params[1];		matrix._13 = params[2];		matrix._14 = params[3];
						matrix._21 = params[4];		matrix._22 = params[5];		matrix._23 = params[6];		matrix._24 = params[7];
						matrix._31 = params[8];		matrix._32 = params[9];		matrix._33 = params[10];	matrix._34 = params[11];
						matrix._41 = params[12];	matrix._42 = params[13];	matrix._43 = params[14];	matrix._44 = params[15];
                        keyPoses[i].SetTransform( matrix );
                    }
                    break;
            }

			//�@���������
			params.clear();
        }

		//�@�p�����Ԃ�ݒ�
        assert( key_size >= 1 );
        float duration = static_cast< float >( keyPoses[ key_size -1 ].GetTime() );
        mMaxTime = Max( duration, mMaxTime );
        anime.SetKeys( keyPoses );

        //�@���������
        keyPoses.clear();

        while( !mToken.IsEnd() )
        {
            mToken.Next();
            if ( mToken.Check( "}" ) )
                break;
        }

		//�@�t���[�������Ō�ɂ���p�^�[���̏ꍇ
        mToken.Next();
        if ( mToken.Check( "{" ) )
        {
			//�@�t���[�����擾
            std::string frame_name = std::string( mToken.GetNextAsChar() );
            DEBUG_LOG( "Animation Frame Name : %s", frame_name.c_str() );

			//�@���O����t���[���ԍ�������
			frame_index = FindFrame( frame_name );
            DEBUG_LOG( "Frame Index = %d", frame_index );
			if ( frame_index > -1 )
			{
                anime.SetBoneIndex( frame_index );
			}

            mAnimationCaches.push_back( anime );

            while( !mToken.IsEnd() )
            {
                mToken.Next();
                if ( mToken.Check( "}" ) )
                    break;
            }
        }
        else if ( mToken.Check( "}" ) )
        {
            mAnimationCaches.push_back( anime );

			//�@�I��
            return;
        }
        else
        {
            mAnimationCaches.push_back( anime );
			//�@�m�[�h���`�F�b�N
            CheckNode(parent);
        }
    }

}

///----------------------------------------------------------------------
///<summary>
///���O����t���[���ԍ�����������
///</summary>
///<param name="name">�����������t���[����</param>
///<returns>
///�t���[�������������ꍇ�́C�t���[���ԍ���ԋp.
///������Ȃ������ꍇ�́C-1��ԋp����.
///</returns>
///----------------------------------------------------------------------
short ModelLoaderX::FindFrame( std::string name )
{
	// ���݂̓ǂݍ���ł���{�[�������猟������
	uint32_t num_bones= static_cast< uint32_t >( mBoneCaches.size() );
  	for( uint32_t i=0; i<num_bones; i++ )
	{
        if ( mBoneCaches[i].GetName() == name )
		{
			//�@������Hit.
			int result = static_cast< short >( i );
            return result;
		}
	}
	
	//�@�����Ɉ���������Ȃ�����.
	DEBUG_LOG( "FindFrame() Not Find Bone. -%s", name.c_str() );
	return -1;
}

///----------------------------------------------------------------------
///<summary>
///�X�L���E�F�C�g�ƃX�L���C���f�b�N�X�̐ݒ�
///</summary>
///----------------------------------------------------------------------
void ModelLoaderX::SetSkinDataFromCache()
{
    //�@�t���[��������
    for( size_t i=0; i<mSkinWeightCaches.size(); ++i )
    {
	    short frame_index = FindFrame( mSkinWeightCaches[i].GetName() );
        uint32_t mesh_index = mSkinWeightCaches[i].GetMeshIndex();

        //�@���������ꍇ
	    if ( frame_index > -1 )
	    {
		    uint32_t num_vertices = mModel.mMeshes[mesh_index].GetNumPositions();

		    //�@�������m�ۂ��Ă��Ȃ��ꍇ
		    if( (mModel.mMeshes[mesh_index].GetNumSkinWeights() != num_vertices )
	         || (mModel.mMeshes[mesh_index].GetNumSkinIndices() != num_vertices ) )
		    {
			    //�@�������m��
			    mModel.mMeshes[mesh_index].mSkinWeights.resize( num_vertices );
			    mModel.mMeshes[mesh_index].mSkinIndices.resize( num_vertices );

			    //�@�����l���
			    for( uint32_t i=0; i<num_vertices; i++ )
			    {
				    mModel.mMeshes[mesh_index].mSkinIndices[i].val[0] = 0;
				    mModel.mMeshes[mesh_index].mSkinIndices[i].val[1] = 0;
				    mModel.mMeshes[mesh_index].mSkinIndices[i].val[2] = 0;
				    mModel.mMeshes[mesh_index].mSkinIndices[i].val[3] = 0;

				    mModel.mMeshes[mesh_index].mSkinWeights[i].x = 0.0f;
				    mModel.mMeshes[mesh_index].mSkinWeights[i].y = 0.0f;
				    mModel.mMeshes[mesh_index].mSkinWeights[i].z = 0.0f;
				    mModel.mMeshes[mesh_index].mSkinWeights[i].w = 0.0f;
			    }
		    }

		    //�@�E�F�C�g�������[�v
            for( uint32_t j=0; j<mSkinWeightCaches[i].GetNumWeights(); ++j )
		    {
			    //�@���_�ԍ�
			    uint32_t vertex_index = mSkinWeightCaches[i].GetIndex( j );
                float weight = mSkinWeightCaches[i].GetWeight( j );

			    //�@index 0 ���ݒ�
			    if ( mModel.mMeshes[mesh_index].mSkinIndices[vertex_index].val[0] <= 0 )
			    {
				    mModel.mMeshes[mesh_index].mSkinIndices[vertex_index].val[0] = frame_index;
				    mModel.mMeshes[mesh_index].mSkinWeights[vertex_index].x = weight;
				    DEBUG_LOG( "mMesh[%d] - VertexIndex=%d, Skin Index[0] =%d, SkinWeight[0] = %f", mesh_index, vertex_index, frame_index, weight );
			    }
			    //�@index 1 ���ݒ�
			    else if ( mModel.mMeshes[mesh_index].mSkinIndices[vertex_index].val[1] <= 0 )
			    {
				    mModel.mMeshes[mesh_index].mSkinIndices[vertex_index].val[1] = frame_index;
				    mModel.mMeshes[mesh_index].mSkinWeights[vertex_index].y = weight;
				    DEBUG_LOG( "mMesh[%d] - VertexIndex=%d, Skin Index[1] =%d, SkinWeight[1] = %f", mesh_index, vertex_index, frame_index, weight );
			    }
			    //�@index 2 ���ݒ�
			    else if ( mModel.mMeshes[mesh_index].mSkinIndices[vertex_index].val[2] <= 0 )
			    {
				    mModel.mMeshes[mesh_index].mSkinIndices[vertex_index].val[2] = frame_index;
				    mModel.mMeshes[mesh_index].mSkinWeights[vertex_index].z = weight;
				    DEBUG_LOG( "mMesh[%d] - VertexIndex=%d, Skin Index[2] =%d, SkinWeight[2] = %f", mesh_index, vertex_index, frame_index, weight );
			    }
			    //�@index 3 ���ݒ�
			    else if ( mModel.mMeshes[mesh_index].mSkinIndices[vertex_index].val[3] <= 0 )
			    {
				    mModel.mMeshes[mesh_index].mSkinIndices[vertex_index].val[3] = frame_index;
				    mModel.mMeshes[mesh_index].mSkinWeights[vertex_index].w = weight;
				    DEBUG_LOG( "mMesh[%d] - VertexIndex=%d, Skin Index[3] =%d, SkinWeight[3] = %f", mesh_index, vertex_index, frame_index, weight );
			    }
		    }

		    //�@�p���s����i�[
		    mBoneCaches[frame_index].SetBindMatrix( mSkinWeightCaches[i].GetMatrix() );

		    DEBUG_LOG( "mBones[%d] _11=%f, _12=%f, _13=%f, _14=%f", frame_index, mBones[frame_index].mPoseMatrix._11, mBones[frame_index].mPoseMatrix._12, mBones[frame_index].mPoseMatrix._13, mBones[frame_index].mPoseMatrix._14 );
		    DEBUG_LOG( "mBones[%d] _21=%f, _22=%f, _23=%f, _24=%f", frame_index, mBones[frame_index].mPoseMatrix._21, mBones[frame_index].mPoseMatrix._22, mBones[frame_index].mPoseMatrix._23, mBones[frame_index].mPoseMatrix._24 );
		    DEBUG_LOG( "mBones[%d] _31=%f, _32=%f, _33=%f, _34=%f", frame_index, mBones[frame_index].mPoseMatrix._31, mBones[frame_index].mPoseMatrix._32, mBones[frame_index].mPoseMatrix._33, mBones[frame_index].mPoseMatrix._34 );
		    DEBUG_LOG( "mBones[%d] _41=%f, _42=%f, _43=%f, _44=%f", frame_index, mBones[frame_index].mPoseMatrix._41, mBones[frame_index].mPoseMatrix._42, mBones[frame_index].mPoseMatrix._43, mBones[frame_index].mPoseMatrix._44 );
	    }

        //�@�i�[�������̂̓������������
        mSkinWeightCaches[i].Release();
    }

    //�@���������
    mSkinWeightCaches.clear();

}



} // namespace Graphics
} // namespace Asura

