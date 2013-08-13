//-----------------------------------------------------------------------
// File : AsuraModelImpl.h
// Desc : Asura Model Implmentation
// Copyright(c) Project Asura All right reserved.
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// History
//-----------------------------------------------------------------------
// <---> 2010/11/13 [Pocol] 新規作成.
// <End>

#ifndef __ASURA_MODEL_IMPL_H__
#define __ASURA_MODEL_IMPL_H__

// Disable Warning C4996
//#ifndef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
//#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
//#endif
//#ifndef _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES
//#define _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES 1
//#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#endif

//-----------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------
#include "../AsModel.h"
#include "../AsTokenizer.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>


//-----------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------
#ifndef DLOG
    #if defined(DEBUG) || defined(_DEBUG)
        #define DLOG( x, ... ) std::printf( "[File: %s, Line: %d]"x"\n", __FILE__, __LINE__, ##__VA_ARGS__ )
   #else
        #define DLOG( x, ... )
    #endif
#endif // DLOG

#ifndef ELOG
    #define ELOG( x, ... ) std::printf( "[File: %s, Line: %d]"x"\n", __FILE__, __LINE__, ##__VA_ARGS__ )
#endif // ELOG


namespace Asura {
namespace Graphics {

//-----------------------------------------------------------------------
// Forward Declarations
//-----------------------------------------------------------------------
class ModelLoaderX;


/////////////////////////////////////////////////////////////////////////
// Bone class
/////////////////////////////////////////////////////////////////////////
class Bone : public IBone
{
	//--------------------------------------------------------------------
	// list of friend classes
	//--------------------------------------------------------------------
	friend class ModelLoaderX;

public:
    //--------------------------------------------------------------------
    // public variables
    //--------------------------------------------------------------------
	/* NOTHING */

    //---------------------------------------------------------------------
    // public methods
    //---------------------------------------------------------------------
    Bone();
    virtual ~Bone();
    void                    Release();
    std::string             GetName();
    int32_t                 GetParentIndex();
    Asura::Math::Matrix		GetBindMatrix();
    Asura::Math::Matrix		GetPoseMatrix();
 
    void SetName( const std::string &name );
    void SetParentIndex( const int32_t &index );
    void SetBindMatrix( const Asura::Math::Matrix &matrix );
    void SetPoseMatrix( const Asura::Math::Matrix &matrix );
    Bone& operator = ( const Bone &bone );

protected:
	//-------------------------------------------------------------------
	// protected variables
	//-------------------------------------------------------------------
    std::string             mName;
    int32_t                 mParentIndex;
    Asura::Math::Matrix		mBindMatrix;
    Asura::Math::Matrix		mPoseMatrix;

	//-------------------------------------------------------------------
	// protected methods
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
 };

/////////////////////////////////////////////////////////////////////////
// Material class
/////////////////////////////////////////////////////////////////////////
class Material : public IMaterial
{
    //-------------------------------------------------------------------
    // list of friend class
    //-------------------------------------------------------------------
    friend class ModelLoaderX;

public:
    //-------------------------------------------------------------------
    // public variables
    //-------------------------------------------------------------------
    /* NOTHING */

    //-------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------
    Material();
    Material( const Material &material );
    virtual ~Material();
    void                    Release();

    std::string             GetName();
    Asura::Math::Vector4	GetDiffuse();
    Asura::Math::Vector3	GetSpecular();
    Asura::Math::Vector3	GetEmissive();
    float                   GetPower();
    std::string             GetTextureName();

    void SetName( const std::string &name );
    void SetDiffuse( const Asura::Math::Vector4 &diffuse );
    void SetSpecular( const Asura::Math::Vector3 &specular );
    void SetEmissive( const Asura::Math::Vector3 &emissive );
    void SetPower( const float &power );
    void SetTextureName( const std::string &texture_name );
   
	Material& operator = ( const Material &material );

protected:
    //-------------------------------------------------------------------
    // protected variables
    //-------------------------------------------------------------------
    std::string             mName;
    Asura::Math::Vector4	mDiffuse;
    Asura::Math::Vector3	mSpecular;
    Asura::Math::Vector3	mEmissive;
    float                   mPower;
    std::string				mTextureName;

    //-------------------------------------------------------------------
    // protected methods
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
};

/////////////////////////////////////////////////////////////////////////
// MeshSubset class
/////////////////////////////////////////////////////////////////////////
class MeshSubset : public IMeshSubset
{
    //-------------------------------------------------------------------
    // list of friend classes
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
    MeshSubset();
    virtual ~MeshSubset();

    uint32_t GetMaterialIndex();
    uint32_t GetFaceIndex();
    uint32_t GetNumFace();

    void     SetMaterialIndex( const uint32_t &material_index );
    void     SetFaceIndex( const uint32_t &face_index );
    void     SetNumFace( const uint32_t &num_face );

    MeshSubset& operator = ( const MeshSubset &subset );

protected:
    //-------------------------------------------------------------------
    // protected variables
    //-------------------------------------------------------------------
    uint32_t mMaterialIndex;
    uint32_t mFaceIndex;
    uint32_t mNumFace;

    //-------------------------------------------------------------------
    // protected methods
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
};

//-----------------------------------------------------------------------
// Mesh class
//-----------------------------------------------------------------------
class Mesh : public IMesh
{
	//-------------------------------------------------------------------
	// list of friend classes
	//-------------------------------------------------------------------
	friend class ModelLoaderX;

public:
	//-------------------------------------------------------------------
	// public variables
	//-------------------------------------------------------------------
	/* NOTHING */

	//-------------------------------------------------------------------
	// public methods
	//-------------------------------------------------------------------
	Mesh();
	Mesh( const Mesh &mesh );
	virtual ~Mesh();
	void Release();

	//　コンテナデータ取得用
    std::vector< Asura::Math::Vector3 >   GetPositions();
    std::vector< Asura::Math::Vector3 >   GetNormals();
    std::vector< UByte4 >                 GetColors();
    std::vector< Asura::Math::Vector2 >   GetTexCoords();
    std::vector< Asura::Math::Vector3 >   GetTangents();
    std::vector< Asura::Math::Vector3 >   GetBinormals();
    std::vector< Short4 >                 GetSkinIndices();
    std::vector< Asura::Math::Vector4 >   GetSkinWeights();
#if 0 // azarashin modified...
    std::vector< IBone* >                 GetBones();
#endif
    std::vector< uint32_t >               GetIndices();
    std::vector< IMeshSubset* >           GetSubsets();
    std::vector< IMaterial* >             GetMaterials();

	//　個別データ取得用
	std::string                           GetName();
	Asura::Math::Vector3                  GetPosition( uint32_t index );
	Asura::Math::Vector3                  GetNormal( uint32_t index );
	UByte4                                GetColor( uint32_t index );
	Asura::Math::Vector2                  GetTexCoord( uint32_t index );
	Asura::Math::Vector3                  GetTangent( uint32_t index );
	Asura::Math::Vector3                  GetBinormal( uint32_t index );
	Short4                                GetSkinIndex( uint32_t index );
	Asura::Math::Vector4                  GetSkinWeight( uint32_t index );
	uint32_t                              GetIndex( uint32_t index );
#if 0 // azarashin modified...
	IBone*                                GetBone( uint32_t index );
#endif
	IMeshSubset*                          GetSubset( uint32_t index );
	IMaterial*                            GetMaterial( uint32_t index );
    
	//　サイズ取得用
	uint32_t  GetNumPositions();
	uint32_t  GetNumNormals();
	uint32_t  GetNumColors();
	uint32_t  GetNumTexCoords();
	uint32_t  GetNumTangents();
	uint32_t  GetNumBinormals();
	uint32_t  GetNumSkinIndices();
	uint32_t  GetNumSkinWeights();
#if 0 // azarashin modified...
	uint32_t  GetNumBones();
#endif
	uint32_t  GetNumIndices();
	uint32_t  GetNumSubsets();
	uint32_t  GetNumMaterials();

	//　データ設定用
	void      SetName( const std::string &name );
    void      SetPositions( const std::vector< Asura::Math::Vector3 > &positions );
    void      SetNormals( const std::vector< Asura::Math::Vector3 > &normals );
	void      SetColors( const std::vector< UByte4 > &colors );
    void      SetTexCoords( const std::vector< Asura::Math::Vector2 > &texcoords );
    void      SetTangents( const std::vector< Asura::Math::Vector3 > &tangents );
    void      SetBinormals( const std::vector< Asura::Math::Vector3 > &binormals );
	void      SetSkinIndices( const std::vector< Short4 > &skin_indices );
    void      SetSkinWeights( const std::vector< Asura::Math::Vector4 > &skin_weights );
    void      SetIndices( const std::vector< uint32_t > &indices );
#if 0 // azarashin modified...
    void      SetBones( const std::vector< Bone > &bones );
#else
	void SetRefBone(int ref); 
	int GetRefBone(); 
#endif
    void      SetSubsets( const std::vector< MeshSubset > &subsets );
    void      SetMaterials( const std::vector< Material> &materials );
	Mesh& operator = ( const Mesh &mesh );

protected:
	//-------------------------------------------------------------------
	// protected variables
	//-------------------------------------------------------------------
    std::string                             mName;
	std::vector< Asura::Math::Vector3 >     mPositions;
	std::vector< Asura::Math::Vector3 >     mNormals;
	std::vector< UByte4 >                   mColors;
    std::vector< Asura::Math::Vector2 >		mTexCoords;
    std::vector< Asura::Math::Vector3 >		mTangents;
    std::vector< Asura::Math::Vector3 >		mBinormals;
	std::vector< Short4 >                   mSkinIndices;
    std::vector< Asura::Math::Vector4 >		mSkinWeights;
	std::vector< uint32_t >                 mIndices;
#if 0 // azarashin modified...
    std::vector< Bone >                     mBones;
#else 
	int ref_bone; 
#endif 
    std::vector< MeshSubset >               mSubsets;
    std::vector< Material >                 mMaterials;

	//-------------------------------------------------------------------
	// protected methods
	//------------------------------------------------------------------
	void ComputeNormals();
	void ComputeTangents();
	void ComputeBinormals();

private:
	//-------------------------------------------------------------------
	// private variables
	//-------------------------------------------------------------------
    /* NOTHING */

	//-------------------------------------------------------------------
	// private methods
	//-------------------------------------------------------------------
	/* NOTHING */
};


/////////////////////////////////////////////////////////////////////////
// KeyPose class
/////////////////////////////////////////////////////////////////////////
class KeyPose : public IKeyPose
{
    //-------------------------------------------------------------------
    // list of friend classes
    //-------------------------------------------------------------------
    friend class ModelLoaderX;

public:
    //-------------------------------------------------------------------
    // public variables
    //-------------------------------------------------------------------
    /* NOTHING */

    //-------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------
    KeyPose();
    virtual ~KeyPose();
    uint32_t                    GetTime();
    Asura::Math::Vector3		GetScale();
    Asura::Math::Quaternion		GetRotate();
    Asura::Math::Vector3		GetTranslate();
    Asura::Math::Matrix			GetTransform();

    void SetTime( const uint32_t &time );
    void SetScale( const Asura::Math::Vector3 &scale );
    void SetRotate( const Asura::Math::Quaternion &rotate );
    void SetTranslate( const Asura::Math::Vector3 &translate );
    void SetTransform( const Asura::Math::Matrix &transform );

    KeyPose& operator = ( const KeyPose &keyPose );

protected:
    //------------------------------------------------------------------
    // protected variables
    //------------------------------------------------------------------
    uint32_t                    mTime;
    Asura::Math::Vector3		mScale;
    Asura::Math::Quaternion		mRotate;
    Asura::Math::Vector3		mTranslate;
    Asura::Math::Matrix			mTransform;

    //------------------------------------------------------------------
    // protected methods
    //------------------------------------------------------------------
    /* NOTHING */

private:
    //------------------------------------------------------------------
    // private variables
    //------------------------------------------------------------------
    /* NOTHING */

    //------------------------------------------------------------------
    // private methods
    //------------------------------------------------------------------
    /* NOTHING */
};


////////////////////////////////////////////////////////////////////////
// Animation class
////////////////////////////////////////////////////////////////////////
class Animation : public IAnimation
{
    //------------------------------------------------------------------
    // list of friend classes
    //------------------------------------------------------------------
    friend class ModelLoaderX;

public:
    //------------------------------------------------------------------
    // public variables
    //------------------------------------------------------------------
    /* NOTHING */

    //------------------------------------------------------------------
    // public methods
    //------------------------------------------------------------------
    Animation();
    virtual ~Animation();
    void        Release();
    std::string GetName();
    uint32_t    GetBoneIndex();
    uint32_t    GetNumKeys();
    IKeyPose*   GetKey( uint32_t index );
    std::vector< IKeyPose* > GetKeys();

    void SetName( const std::string &name );
    void SetBoneIndex( const uint32_t &index );
    void SetKeys( const std::vector< KeyPose > &keyposes );
    void SetKey( uint32_t index, const KeyPose &pose );

    Animation& operator = ( const Animation &animation );

protected:
    //------------------------------------------------------------------
    // protected variables
    //------------------------------------------------------------------
    std::string             mName;
    uint32_t                mBoneIndex;
    std::vector< KeyPose >  mKeys;

    //------------------------------------------------------------------
    // protected methods
    //------------------------------------------------------------------
    /* NOTHING */

private:
    //------------------------------------------------------------------
    // private variables
    //------------------------------------------------------------------
    /* NOTHING */

    //------------------------------------------------------------------
    // private methods
    //------------------------------------------------------------------
    /* NOTHING */
};


//////////////////////////////////////////////////////////////////////////
// AnimationClip class
//////////////////////////////////////////////////////////////////////////
class AnimationClip : public IAnimationClip
{
    //-------------------------------------------------------------------
    // list of friend class
    //-------------------------------------------------------------------
    friend class ModelLoaderX;

public:
    //-------------------------------------------------------------------
    // public variables
    //-------------------------------------------------------------------
    /* NOTHING */

    //-------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------
    AnimationClip();
    virtual ~AnimationClip();
    void                        Release();

    std::string                 GetName();
    IAnimation*                 GetFrame( uint32_t index );
    std::vector< IAnimation* >  GetFrames();
    uint32_t                    GetNumFrames();
    float                       GetDuration();
    
    void SetName( const std::string &name );
    void SetFrame( uint32_t index, Animation &frame );
    void SetFrames( const std::vector< Animation > &frames );
    void SetDuration( const float &duration );

	AnimationClip& operator = ( const AnimationClip &clip );

protected:
    //-------------------------------------------------------------------
    // protected variables
    //-------------------------------------------------------------------
    std::string              mName;
    std::vector< Animation > mFrames;
    float                    mDuration;
  
    //-------------------------------------------------------------------
    // protected methods
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
};

/////////////////////////////////////////////////////////////////////////
// AnimationPlayer class
/////////////////////////////////////////////////////////////////////////
class AnimationPlayer : public IAnimationPlayer
{
    //-------------------------------------------------------------------
    // list of friend class
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
    AnimationPlayer( const std::vector< IBone* > &bones );
    virtual ~AnimationPlayer();

    void	Release();
    void	SetClip( IAnimationClip* pclip );
    void	Update( float time, bool relativeToCurTime, const Asura::Math::Matrix &rootTransform );
    std::vector< Asura::Math::Matrix > GetSkinTransform();

protected:
    //-------------------------------------------------------------------
    // protected variables
    //-------------------------------------------------------------------
    float                 mCurTime;
    IAnimationClip*       mpClip;
    std::vector< IBone* > mBones;
    std::vector< Asura::Math::Matrix > mBoneTransforms;
    std::vector< Asura::Math::Matrix > mWorldTransforms;
    std::vector< Asura::Math::Matrix > mSkinTransforms;

    //-------------------------------------------------------------------
    // protected methods
    //-------------------------------------------------------------------
    void UpdateBoneTransform( float time, bool relativeToCurTime );
    void UpdateWorldTransform( const Asura::Math::Matrix &rootMatrix );
    void UpdateSkinTransform();
 
private:
    //-------------------------------------------------------------------
    // private variables
    //-------------------------------------------------------------------
    /* NOTHING */

    //-------------------------------------------------------------------
    // private methods
    //-------------------------------------------------------------------
    /* NOTHING */
};

/////////////////////////////////////////////////////////////////////////
// Model class
/////////////////////////////////////////////////////////////////////////
class Model : public IModel
{
    //-------------------------------------------------------------------
    // list of friend classes
    //-------------------------------------------------------------------
	friend class ModelLoaderX;

public:
    //-------------------------------------------------------------------
    // public variables
    //-------------------------------------------------------------------
    /* NOTHING */

    //-------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------
    Model();
    virtual ~Model();
    void                            Release();
	std::string                     GetName();
    std::vector< IMesh* >           GetMeshes();
    std::vector< IAnimationClip* >  GetClips();
	IMesh*                          GetMesh( uint32_t index );
	IAnimationClip*                 GetClip( uint32_t index );
	uint32_t                        GetNumMeshes();
	uint32_t                        GetNumClips();

	uint32_t						GetNumBones(); 
	std::vector< IBone* >				GetBones(); 
	IBone*							GetBone(uint32_t index); 

    void SetName( const std::string &name );
    void SetMeshes( const std::vector< Mesh > &meshes );
    void SetClips( const std::vector< AnimationClip > &clips );
    Model& operator = ( const Model &model );

protected:
    //-------------------------------------------------------------------
    // protected variables
    //-------------------------------------------------------------------
    std::string					 mName;
    std::vector< Mesh >			 mMeshes;
    std::vector< AnimationClip > mClips;
	std::vector< Bone >             mBaseBone;

    //-------------------------------------------------------------------
    // protected methods
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
};

/////////////////////////////////////////////////////////////////////////
// ModelLoaderBase class
/////////////////////////////////////////////////////////////////////////
class ModelLoaderBase : public IModelLoader
{
    //-------------------------------------------------------------------
    // list of friend classes
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
    ModelLoaderBase( uint32_t size );
    virtual ~ModelLoaderBase();
    virtual bool            Load( const char *filename );
    virtual bool            IsLoading();
    virtual void            Parse() = 0;
	virtual IModel*         GetModel();
    virtual void            Release();

protected:
    //-------------------------------------------------------------------
    // protected variables
    //-------------------------------------------------------------------
	Model							mModel;
    FILE*							mpFile;
    bool							mIsLoading;
    uint32_t						mBufSize;
    uint32_t						mCurSize;
    uint32_t						mRestSize;
    char*							mpBuf;
	Asura::Util::Tokenizer			mToken;
    std::string						mFileName;

    //-------------------------------------------------------------------
    // protected methods
    //-------------------------------------------------------------------
    std::string             GetExt();
    std::string             GetDir();
};

} // namespace Graphics
} // namespace Asura


#endif //__ASURA_MODEL_IMPL_H__