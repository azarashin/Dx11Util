//-----------------------------------------------------------------------------------
// File : asdxModelUtil.h
// Desc : Asura Model Utility for DirectX 11
// Copyright(c) Project Asura. All right reserved.
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------
#include "asDef.h"
#include "asModel.h"
#include "asTimer.h"
#include <d3d11.h>
#include <d3dx10math.h>

namespace Asura {
namespace Util {

//////////////////////////////////////////////////////////////////////////
// ModelUtil class
//////////////////////////////////////////////////////////////////////////
class ModelUtil
{
	//--------------------------------------------------------------------
	// list of friend classes and methods
	//--------------------------------------------------------------------
	/* NOTHING */

private:
	//--------------------------------------------------------------------
	// private variables
	//--------------------------------------------------------------------
	/* NOTHING */

	//--------------------------------------------------------------------
	// private methods
	//--------------------------------------------------------------------
	/* NOTHING */

protected:
	//--------------------------------------------------------------------
	// protected variables
	//--------------------------------------------------------------------
	/* NOTHING */

	//--------------------------------------------------------------------
	// protected methods
	//--------------------------------------------------------------------
	/* NOTHING */

public:
	//--------------------------------------------------------------------
	// public variables
	//--------------------------------------------------------------------
	/* NOTHING */

	//---------------------------------------------------------------------
	// public methods
	//---------------------------------------------------------------------
    static bool Initialize( ID3D11Device* pDevice );
    static bool Release();
    static ID3D11VertexShader* GetDefaultVertexShader();
    static ID3D11PixelShader*  GetDefaultPixelShader();
    static ID3D11InputLayout*  GetDefaultInputLayout();
    static void SetMatrix( 
        ID3D11DeviceContext* pImmediateContext,
        const D3DXMATRIX &world,
        const D3DXMATRIX &view,
        const D3DXMATRIX &proj );
    static void SetVector(
        ID3D11DeviceContext* pImmediateContext,
        const D3DXVECTOR3 &lightDir,
        const D3DXVECTOR3 &viewDir );
};


/////////////////////////////////////////////////////////////////////////////////////
// SubsetData strcture
/////////////////////////////////////////////////////////////////////////////////////
typedef struct _SubsetData
{
    uint32_t			 count;			//!< ���_�J�E���g��
	uint32_t             startIndex;	//!< �J�n�ԍ�
    Asura::Math::Vector4 diffuse;		//!< �g�U���ːF
    Asura::Math::Vector3 specular;		//!< ���ʔ��ːF
    Asura::Math::Vector3 emissive;		//!< ���Ȕ����F
    float				 power;			//!< ���ʔ��˂̋���
    ID3D11ShaderResourceView* pSRV;		//!< �e�N�X�`���f�[�^
} SubsetData;


/////////////////////////////////////////////////////////////////////////////////////
// MeshData class
/////////////////////////////////////////////////////////////////////////////////////
class MeshData
{
    //-------------------------------------------------------------------------------
    // list of friend classes and methods
    //-------------------------------------------------------------------------------
    /* NOTHING */

private:
    //-------------------------------------------------------------------------------
    // private variables
    //-------------------------------------------------------------------------------
    /* NOTHING */

    //-------------------------------------------------------------------------------
    // private methods
    //-------------------------------------------------------------------------------
    /* NOTHING */

protected:
    //-------------------------------------------------------------------------------
    // protected variables
    //-------------------------------------------------------------------------------
    std::vector< SubsetData >       mSubsets;      //!< �T�u�Z�b�g
    ID3D11Buffer*                   mpVB;          //!< ���_�o�b�t�@
    ID3D11Buffer*                   mpMB;          //!< �}�e���A���o�b�t�@
	ID3D11Buffer*					mpIB;		   //!< �C���f�b�N�X�o�b�t�@
    uint32_t                        mOffset;       //!< ���_�o�b�t�@�̃I�t�Z�b�g
    uint32_t                        mStride;       //!< ���_�o�b�t�@�̃X�g���C�h
    
    //-------------------------------------------------------------------------------
    // protected methods
    //-------------------------------------------------------------------------------
    /* NOTHING */

public:
    //-------------------------------------------------------------------------------
    // public variables
    //-------------------------------------------------------------------------------
    /* NOTHING */

    //-------------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------------
    MeshData();
    ~MeshData();
    void            Release();
    bool            CreateBuffer( ID3D11Device* pDevice, Asura::Graphics::IMesh* pMesh );
    void            Draw( ID3D11DeviceContext* pImmediateContext );
    void            DrawSubset( ID3D11DeviceContext* pImmeidateContext, const int index );
    uint32_t        GetNumSubsets();
    SubsetData&     GetSubset( const int index );
    ID3D11Buffer*   GetVertexBuffer();
    ID3D11Buffer*   GetMaterialBuffer();
	ID3D11Buffer*   GetIndexBuffer();
    uint32_t        GetStride();
    uint32_t        GetOffset();
};


/////////////////////////////////////////////////////////////////////////////////////
// ModelData class
/////////////////////////////////////////////////////////////////////////////////////
class ModelData
{
    //-------------------------------------------------------------------------------
    // list of friend classes and methods
    //-------------------------------------------------------------------------------
    /* NOTHING */

private:
    //-------------------------------------------------------------------------------
    // private variables
    //-------------------------------------------------------------------------------
    /* NOTHING */

    //-------------------------------------------------------------------------------
    // private methods
    //-------------------------------------------------------------------------------
    /* NOTHING */

protected:
    //-------------------------------------------------------------------------------
    // protected variables
    //-------------------------------------------------------------------------------
    std::vector< MeshData >     mMeshes;       //<! ���b�V���^�C�v
    Asura::Graphics::IModel*    mpInterface;   //!< �C���^�[�t�F�[�X

    //-------------------------------------------------------------------------------
    // protected methods
    //-------------------------------------------------------------------------------
    /* NOTHING */

public:
    //-------------------------------------------------------------------------------
    // public variables
    //-------------------------------------------------------------------------------
    /* NOTHING */

    //-------------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------------
    ModelData();
    virtual ~ModelData();
    virtual bool LoadFromX( const char* filename );
    virtual void Release();
	bool         CreateBuffer( ID3D11Device* pDevice );
	void         ReleaseBuffer();
    void         Draw( ID3D11DeviceContext* pImmediateContext );
    void         DrawMesh( ID3D11DeviceContext* pImmediateConet, const int index );
    uint32_t     GetNumMeshes();
    MeshData&    GetMesh( const int index );
	Asura::Graphics::IModel* GetInterface();
};


//////////////////////////////////////////////////////////////////////////////////////
// SkinningModelData class
//////////////////////////////////////////////////////////////////////////////////////
class SkinningModelData : public ModelData
{
    //-------------------------------------------------------------------------------
    // list of friend classes and methods
    //-------------------------------------------------------------------------------
    /* NOTHING */

private:
    //-------------------------------------------------------------------------------
    // private variables
    //-------------------------------------------------------------------------------
    /* NOTHING */

    //-------------------------------------------------------------------------------
    // private methods
    //-------------------------------------------------------------------------------
    /* NOTHING */

protected:
    //-------------------------------------------------------------------------------
    // protected variables
    //-------------------------------------------------------------------------------
	Asura::Math::Matrix 			   mRoot;
    Asura::Timer					   mTimer;
	Asura::Graphics::IAnimationPlayer* mpPlayer;

    //-------------------------------------------------------------------------------
    // protected methods
    //-------------------------------------------------------------------------------
    /* NOTHING */

public:
    //-------------------------------------------------------------------------------
    // public variables
    //-------------------------------------------------------------------------------
    /* NOTHING */

    //-------------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------------
    SkinningModelData();
    virtual ~SkinningModelData();
    virtual bool LoadFromX( const char* filename );
    virtual void Release();
    uint32_t     GetNumClips();
    void         SetClip( const uint32_t index );
	void         SetRootMatrix( const Asura::Math::Matrix &matrix );
	void         SetD3DXRootMatrix( const D3DXMATRIX &matrix );
    void         Update( const bool isStop = false, const float speed = 1.0f );

	std::vector< Asura::Math::Matrix > GetSkinTransformMatrix();
	std::vector< D3DXMATRIX >          GetD3DXSkinTransformMatrix();
	Asura::Math::Matrix                GetRootMatrix();
	D3DXMATRIX                         GetD3DXRootMatrix();
};


} // namespace Util
} // namespace Asura