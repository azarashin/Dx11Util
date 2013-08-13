//-----------------------------------------------------------------------
// File : AsuraModelLoaderX.h
// Desc : Asura X File Parser
// Copyright(c) Project Asura All right reserved.
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// History
//-----------------------------------------------------------------------
// <---> 2010/11/13 [Pocol] êVãKçÏê¨.
// <End>

#ifndef __ASURA_MODEL_LOADER_X_H__
#define __ASURA_MODEL_LOADER_X_H__

//-----------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------
#include "asModelImpl.h"
#include "asTokenizer.h"


namespace Asura {
namespace Graphics {

/////////////////////////////////////////////////////////////////////////
// SkinWeightCache class
/////////////////////////////////////////////////////////////////////////
class SkinWeightCache
{
    //-------------------------------------------------------------------
    // list of friend classes.
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
    SkinWeightCache( uint32_t mesh_index );
    ~SkinWeightCache();
    void Release();
    void ClearWeights();
    void ClearIndices();
    void ResizeWeights( int size );
    void ResizeIndices( int size );

    void SetName( std::string name );
    void SetWeight( int index, float value );
    void SetIndex( int index, uint32_t value );
    void SetMatrix( int index, float value );

    std::string                 GetName();
    uint32_t                    GetNumWeights();
    uint32_t                    GetNumIndices();
    float                       GetWeight( uint32_t index );
    uint32_t                    GetIndex( uint32_t index );
    Asura::Math::Matrix			GetMatrix();
    uint32_t                    GetMeshIndex();

    SkinWeightCache& operator = ( const SkinWeightCache &cache );

protected:
    //-------------------------------------------------------------------
    // protected variables
    //-------------------------------------------------------------------
    /* NOTHING */

    //-------------------------------------------------------------------
    // protected methods
    //-------------------------------------------------------------------
    /* NOTHING */

private:
    //-------------------------------------------------------------------
    // private variables
    //-------------------------------------------------------------------
    std::string                 mName;
    std::vector< float >        mWeights;
    std::vector< int >          mIndices;
    Asura::Math::Matrix			mMatrix;
    uint32_t                    mMeshIndex;

    //-------------------------------------------------------------------
    // private methods
    //-------------------------------------------------------------------
    /* NOTHING */
};

/////////////////////////////////////////////////////////////////////////
// ModelLoaderX class
/////////////////////////////////////////////////////////////////////////
class ModelLoaderX : public ModelLoaderBase
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
    ModelLoaderX( uint32_t token_size );
    virtual ~ModelLoaderX();
	void Parse();

protected:
	//-------------------------------------------------------------------
	// protected variables
	//-------------------------------------------------------------------
	/* NOTHING */

	//-------------------------------------------------------------------
	// protected methods
	//-------------------------------------------------------------------
    void    CheckNode(int parent);
	void    SkipNode();
	void    ParseMesh(int parent);
	void    ParseMeshNormals(int parent);
	void    ParseMeshTextureCoords(int parent);
	void    ParseMeshVertexColors(int parent);
	void    ParseMeshMaterialList(int parent);
	void    ParseMaterial(int parent);
	void    ParseTextureFileName(int parent);
#if 0 // azarashin modified...
	void    ParseFrame();
#else
	void    ParseFrame(int parent);
#endif
	void    ParseSkinWeights(int parent);
	void    ParseAnimationSet(int parent);
    void    ParseAnimation(int parent);
	short   FindFrame( std::string name );
    void    SetSkinDataFromCache();

private:
	//-------------------------------------------------------------------
	// private variables
	//-------------------------------------------------------------------
	std::vector< Bone >             mBoneCaches;
	std::vector< unsigned short >   mFaceElement;
    std::vector< SkinWeightCache >  mSkinWeightCaches;
    std::vector< Animation >        mAnimationCaches;
	int                             mParentIndex;
    float                           mMaxTime;

	//-------------------------------------------------------------------
	// private methods
	//-------------------------------------------------------------------
	/* NOTHING */
};

} // namespace Graphics
} // namespace Asura


#endif //__ASURA_X_PARSER_H__