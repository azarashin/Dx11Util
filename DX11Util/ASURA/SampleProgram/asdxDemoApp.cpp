//-------------------------------------------------------------------------------------
// File : asdxDemoApp.h
// Desc : Asura Demo Application For DirectX 11
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// History
//-------------------------------------------------------------------------------------
// <---> 2011/08/04 [Pocol] �V�K�쐬
// <End>


//------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------
#include <vector>
#include "asdxDemoApp.h"
#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTmisc.h"
#include "DXUTSettingsDlg.h"
#include "SDKmisc.h"


namespace Asura {
namespace SampleProgram {

//------------------------------------------------------------------------------------
// Forward Declarations
//------------------------------------------------------------------------------------

// ���b�Z�[�W�v���V�[�W��
LRESULT CALLBACK AsdxDemoApp_MsgProc( 
    HWND hWnd,
    UINT uMsg, 
    WPARAM wp,
    LPARAM lp,
    bool* pbNoFurtherProcessing, 
    void* pUserContext );

// �L�[�{�[�h�̏���
void CALLBACK AsdxDemoApp_OnKeyboard(
    UINT nChar,
    bool bKeyDown,
    bool bAltDown,
    void* pUserContext );

// �}�E�X�̏���
void CALLBACK AsdxDemoApp_OnMouse(
    bool bLeftButtonDown,
    bool bRightButtonDown,
    bool bMiddleButtonDown,
    bool bSideButton1Down,
    bool bSideButton2Down,
    int  nMouseWheelDelta,
    int  xPos,
    int  yPos,
    void* pUserContext );

// GUI�C�x���g�̏���
void CALLBACK AsdxDemoApp_OnGUIEvent(
    UINT nEvent,
    int nControlID,
    CDXUTControl* pControl,
    void* pUserContext );

// �X�V����
void CALLBACK AsdxDemoApp_OnFrameMove(
    double dTime,
    float fElapsedTime,
    void* pUserContext );

// �f�o�C�X�ݒ�̏C�����̏���
bool CALLBACK AsdxDemoApp_ModifyDeviceSettings(
    DXUTDeviceSettings* pDeviceSettings,
    void* pUserContext );

// D3D11�f�o�C�X�����e����Ă��邩�ǂ���
bool CALLBACK AsdxDemoApp_IsD3D11DeviceAcceptable(
    const CD3D11EnumAdapterInfo* pAdapterInfo,
    UINT nOutput,
    const CD3D11EnumDeviceInfo* pDeviceInfo,
    DXGI_FORMAT BackBufferFormat,
    bool isWindowed,
    void* pUserContext );

// D3D11�f�o�C�X���������ꂽ���̏���
HRESULT CALLBACK AsdxDemoApp_OnD3D11CreateDevice(
    ID3D11Device* pDevice,
    const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
    void* pUserContext );

// �X���b�v�`�F�C�������T�C�Y���ꂽ���̏���
HRESULT CALLBACK AsdxDemoApp_OnD3D11ResizedSwapChain(
    ID3D11Device* pDevice,
    IDXGISwapChain* pSwapChain,
    const DXGI_SURFACE_DESC *pBackBufferSurfaceDesc,
    void* pUserContext );

// �X���b�v�`�F�C����������ꂽ���̏���
void CALLBACK AsdxDemoApp_OnD3D11ReleasingSwapChain( void* pUserContext );

// �f�o�C�X���j�����ꂽ���̏���
void CALLBACK AsdxDemoApp_OnD3D11DestroyDevice( void* pUserContext );

// �f�o�C�X���폜���ꂽ���̏���
void CALLBACK AsdxDemoApp_OnDeviceRemoved( void *pUserContext );

// �`�揈��
void CALLBACK AsdxDemoApp_OnD3D11FrameRender( 
    ID3D11Device* pDevice,
    ID3D11DeviceContext* pImmediateContext,
    double dTime,
    float fElapsedTime,
    void* pUserContext );


///////////////////////////////////////////////////////////////////////////////////////
// DemoAppImpl class
///////////////////////////////////////////////////////////////////////////////////////
class DemoAppImpl : public DemoApp
{
    //--------------------------------------------------------------------------------
    // list of friend classes and methods
    //--------------------------------------------------------------------------------
    /* NOTHING */

private:
    //--------------------------------------------------------------------------------
    // private variables
    //--------------------------------------------------------------------------------
    CDXUTDialogResourceManager      mDlgResMgr;
    CD3DSettingsDlg                 mSettingsDlg;
    CDXUTDialog                     mHUD;
    CDXUTTextHelper*                mpTextHelper;
    std::vector< IRenderer* >       mRenderers;
    std::wstring                    mTitle;
    int                             mWidth;
    int                             mHeight;
    float                           mClearColor[ 4 ];

    //--------------------------------------------------------------------------------
    // private methods
    //--------------------------------------------------------------------------------
    /* NOTHING */

protected:
    //--------------------------------------------------------------------------------
    // protected variables
    //--------------------------------------------------------------------------------
    /* NOTHING */

    //--------------------------------------------------------------------------------
    // protected methods
    //--------------------------------------------------------------------------------
    /* NOTHING */

public:
    //--------------------------------------------------------------------------------
    // public variables
    //--------------------------------------------------------------------------------
    enum UI_CONTROL_ID
    {
        IDC_TOGGLE_FULLSCREEN = 0,
        IDC_TOGGLE_REF,
        IDC_CHANGE_DEVICE,

        NUM_UI_CONTROL,
    };

    //--------------------------------------------------------------------------------
    // public methods
    //--------------------------------------------------------------------------------
    DemoAppImpl();
    ~DemoAppImpl();

    int     Register( IRenderer* pRenderer );
    int     Run();
    bool    OnInit();
    void    OnRelease();
    HRESULT OnCreateDevice( ID3D11Device* pDevice, const DXGI_SURFACE_DESC* pDesc );
    void    OnDestroyDevice();
    void    OnRemoveDevice();
    HRESULT OnResizeSwapChain( ID3D11Device* pDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pDesc );
    void    OnReleaseSwapChain();
    void    OnKeyboard( UINT nChar, bool isKeyDown, bool isAltDown );
    void    OnMouse( bool isLeftDown, bool isRightDown, bool isMiddleDown, bool isSide1Down, bool isSide2Down, int wheelDelta, int x, int y );
    void    OnRender( ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext, double time, float elapsedTime );
    void    OnUpdate( double time, float elapsedTime );
    void    OnGUIEvent( UINT nEvent, int nCtrlID, CDXUTControl* pCtrl );
    LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp, bool* pbNoFurtherProcessing );
    void    SetTitle( const wchar_t* title );
    void    SetSize( const int w, const int h );
    float*  GetDefaultClearColor();
    ID3D11RenderTargetView* GetDefaultRenderTargetView();
    ID3D11DepthStencilView* GetDefaultDepthStencilView();
	D3D11_VIEWPORT          GetDefaultViewport();
    CDXUTTextHelper*        GetTextHelper();
	float   GetAspectRatio();
	int     GetWidth();
	int     GetHeight();
};
static DemoAppImpl gInstance;


//------------------------------------------------------------------------------------
//! @brief �����l�ݒ�
//------------------------------------------------------------------------------------
DemoAppImpl::DemoAppImpl()
{
    mpTextHelper = NULL;
    mRenderers.clear();
    mTitle = L"asdxDemoApp";
    mClearColor[ 0 ] = 0.392f;
    mClearColor[ 1 ] = 0.584f;
    mClearColor[ 2 ] = 0.929f;
    mClearColor[ 3 ] = 1.0f;
    mWidth = 800;
    mHeight = 600;
}

//------------------------------------------------------------------------------------
//! @brief �j������
//------------------------------------------------------------------------------------
DemoAppImpl::~DemoAppImpl()
{
    mRenderers.clear();
}

//------------------------------------------------------------------------------------
//! @breif �^�C�g����ݒ肷��
//! @param [in] title �ݒ肷��^�C�g����
//------------------------------------------------------------------------------------
void DemoAppImpl::SetTitle( const wchar_t* title )
{ mTitle = title; }

//------------------------------------------------------------------------------------
//! @breif �E�B���h�E�̃T�C�Y��ݒ肷��
//! @param [in] w �E�B���h�E�̕�
//! @param [in] h �E�B���h�E�̍���
//------------------------------------------------------------------------------------
void DemoAppImpl::SetSize( const int w, const int h )
{
    mWidth = w;
    mHeight = h;
}

//------------------------------------------------------------------------------------
//! @brief �f�t�H���g�̃N���A�J���[���擾����
//------------------------------------------------------------------------------------
float* DemoAppImpl::GetDefaultClearColor()
{ return mClearColor; }

//------------------------------------------------------------------------------------
//! @brief �f�t�H���g�̃����_�[�^�[�Q�b�g�r���[���擾����
//------------------------------------------------------------------------------------
ID3D11RenderTargetView* DemoAppImpl::GetDefaultRenderTargetView()
{ return DXUTGetD3D11RenderTargetView(); }

//------------------------------------------------------------------------------------
//! @brief �f�t�H���g�̐[�x�E�X�e���V���r���[���擾����
//------------------------------------------------------------------------------------
ID3D11DepthStencilView* DemoAppImpl::GetDefaultDepthStencilView()
{ return DXUTGetD3D11DepthStencilView(); }

//------------------------------------------------------------------------
//! @brief �A�X�y�N�g����擾����
//------------------------------------------------------------------------
float DemoAppImpl::GetAspectRatio()
{ return static_cast< float >( mWidth ) / static_cast< float >( mHeight ); }

//------------------------------------------------------------------------
//! @brief �E�B���h�E�̕����擾����
//------------------------------------------------------------------------
int DemoAppImpl::GetWidth()
{ return mWidth; }

//------------------------------------------------------------------------
//! @brief �E�B���h�E�̍������擾����
//------------------------------------------------------------------------
int DemoAppImpl::GetHeight()
{ return mHeight; }

//------------------------------------------------------------------------
//! @brief �f�t�H���g�̃r���[�|�[�g���擾����
//------------------------------------------------------------------------
D3D11_VIEWPORT DemoAppImpl::GetDefaultViewport()
{
	D3D11_VIEWPORT vp;
	vp.Width    = (FLOAT)DXUTGetDXGIBackBufferSurfaceDesc()->Width;
	vp.Height   = (FLOAT)DXUTGetDXGIBackBufferSurfaceDesc()->Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;

	return vp;
}

//------------------------------------------------------------------------------------
//! @brief �����_���[��o�^����
//------------------------------------------------------------------------------------
int DemoAppImpl::Register( IRenderer* pRenderer )
{
    mRenderers.push_back( pRenderer );
    return static_cast< int >( mRenderers.size() );
}

//------------------------------------------------------------------------------------
//! @brief �A�v���P�[�V�����̎��s
//------------------------------------------------------------------------------------
int DemoAppImpl::Run()
{
#if defined(DEBUG) || defined(_DEBUG)
	// ���������[�N�`�F�b�N
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// DXUT�̃R�[���o�b�N�֐��̐ݒ�
    DXUTSetCallbackMsgProc( AsdxDemoApp_MsgProc );
    DXUTSetCallbackKeyboard( AsdxDemoApp_OnKeyboard );
    DXUTSetCallbackMouse( AsdxDemoApp_OnMouse );
    DXUTSetCallbackFrameMove( AsdxDemoApp_OnFrameMove );
    DXUTSetCallbackDeviceChanging( AsdxDemoApp_ModifyDeviceSettings );

    DXUTSetCallbackD3D11DeviceAcceptable( AsdxDemoApp_IsD3D11DeviceAcceptable );
    DXUTSetCallbackD3D11DeviceCreated( AsdxDemoApp_OnD3D11CreateDevice );
    DXUTSetCallbackD3D11SwapChainResized( AsdxDemoApp_OnD3D11ResizedSwapChain );
    DXUTSetCallbackD3D11DeviceDestroyed( AsdxDemoApp_OnD3D11DestroyDevice );
    DXUTSetCallbackD3D11FrameRender( AsdxDemoApp_OnD3D11FrameRender );

	// �������������s
    if ( !OnInit() )
    {
        ELOG( "Error : Application Initialize Failed." );
        return -1;
    }

	// DXUT�̏�����
    DXUTInit( true, true, NULL );
    DXUTSetCursorSettings( true, true );

	// �E�B���h�E�ƃf�o�C�X�̐���
    DXUTCreateWindow( mTitle.c_str() );
    DXUTCreateDevice( D3D_FEATURE_LEVEL_11_0, true, mWidth, mHeight );

	// ���C�����[�v���s
    DXUTMainLoop();

	// �������������
    OnRelease();

	// �I���R�[�h���擾���ԋp����
    return DXUTGetExitCode();
}

//------------------------------------------------------------------------------------
//! @brief ����������
//------------------------------------------------------------------------------------
bool DemoAppImpl::OnInit()
{
	// GUI�Ɛݒ�_�C�A���O�{�b�N�X�̏�����
    mSettingsDlg.Init( &mDlgResMgr );
    mHUD.Init( &mDlgResMgr );

    int y = 30;
    int h = 26;

	// GUI�C�x���g�̃R�[���o�b�N�֐��̐ݒ�
    mHUD.SetCallback( AsdxDemoApp_OnGUIEvent );

	// GUI�̃{�^����ǉ�����
    mHUD.AddButton( IDC_TOGGLE_FULLSCREEN, L"Toggle full screen", 0, y,      170, 22 );
    mHUD.AddButton( IDC_TOGGLE_REF,        L"Toggle REF (F3)",    0, y += h, 170, 22, VK_F3 );
    mHUD.AddButton( IDC_CHANGE_DEVICE,     L"Change device (F2)", 0, y += h, 170, 22, VK_F2 );

	// �����_���[�̓o�^
    for( uint32_t i=0; i<mRenderers.size(); ++i )
    {
        if ( mRenderers[ i ] != NULL )
        {
            if ( !mRenderers[ i ]->OnInit() )
            {
                ELOG( "Error : %s OnInit() Failed.", mRenderers[ i ]->GetTag() );
                return false;
            }
        }
    }

    return true;
}

//------------------------------------------------------------------------------------
//! @brief �������������
//------------------------------------------------------------------------------------
void DemoAppImpl::OnRelease()
{
	// �e�L�X�g�w���p�[�j��
	SAFE_DELETE( mpTextHelper );

	// �����_���[���������������
    for( uint32_t i=0; i<mRenderers.size(); ++i )
    {
        if ( mRenderers[ i ] != NULL )
        {
            mRenderers[ i ]->OnRelease();
            mRenderers[ i ] = NULL;
        }
    }

	// �����_���[���N���A
	mRenderers.clear();
}

//------------------------------------------------------------------------------------
//! @brief �f�o�C�X�쐬���̏���
//------------------------------------------------------------------------------------
HRESULT DemoAppImpl::OnCreateDevice( ID3D11Device *pDevice, const DXGI_SURFACE_DESC *pDesc )
{
    HRESULT hr = S_OK;
    ID3D11DeviceContext* pDeviceContext = DXUTGetD3D11DeviceContext();

    V_RETURN( mDlgResMgr.OnD3D11CreateDevice( pDevice, pDeviceContext ) );
    V_RETURN( mSettingsDlg.OnD3D11CreateDevice( pDevice ) );

	// �e�L�X�g�w���p�[�̐���
    try 
    {
        mpTextHelper = new CDXUTTextHelper( pDevice, pDeviceContext, &mDlgResMgr, 15 );
    }
    catch ( std::bad_alloc ba )
    {
        ELOG( "Error : Memory Allocated Failed. reason = %s", ba.what() );
        OutputDebugString( L"Error : Memory Allocate Failed." );
        return E_FAIL;
    }

	// �����_���[�̏������Ăяo��
    for( uint32_t i=0; i<mRenderers.size(); ++i )
    {
        if ( mRenderers[ i ] != NULL )
        {
            if ( !mRenderers[ i ]->OnCreateDevice( pDevice, pDesc ) )
            {
				ELOG( "Error : %s OnCreateDevice() Failed.", mRenderers[ i ]->GetTag() );
                return E_FAIL;
            }
        }
    }

    return S_OK;
}

//------------------------------------------------------------------------------------
//! @brief �f�o�C�X�j�����̏���
//------------------------------------------------------------------------------------
void DemoAppImpl::OnDestroyDevice()
{
    mDlgResMgr.OnD3D11DestroyDevice();
    mSettingsDlg.OnD3D11DestroyDevice();
    DXUTGetGlobalResourceCache().OnDestroyDevice();

	// �e�L�X�g�w���p�[�j��
    SAFE_DELETE( mpTextHelper );

	// �����_���[�̏������Ăяo��
    for( uint32_t i=0; i<mRenderers.size(); ++i )
    {
        if ( mRenderers[ i ] != NULL )
        {
            mRenderers[ i ]->OnDestroyDevice();
        }
    }
}

//------------------------------------------------------------------------------------
//! @brief �f�o�C�X�폜���̏���
//------------------------------------------------------------------------------------
void DemoAppImpl::OnRemoveDevice()
{
	// �����_���[�̏���
    for( uint32_t i=0; i<mRenderers.size(); ++i )
    {
        if ( mRenderers[ i ] != NULL )
        {
            mRenderers[ i ]->OnRemoveDevice();
        }
    }
}

//------------------------------------------------------------------------------------
//! @brief �X���b�v�`�F�C�������T�C�Y���ꂽ���̏���
//------------------------------------------------------------------------------------
HRESULT DemoAppImpl::OnResizeSwapChain
( 
    ID3D11Device			*pDevice,
    IDXGISwapChain			*pSwapChain,
    const DXGI_SURFACE_DESC *pDesc
)
{
    HRESULT hr = S_OK;

    V_RETURN( mDlgResMgr.OnD3D11ResizedSwapChain( pDevice, pDesc ) );
    V_RETURN( mSettingsDlg.OnD3D11ResizedSwapChain( pDevice, pDesc ) );
    mWidth  = pDesc->Width;
    mHeight = pDesc->Height;

	// GUI�̈ʒu���Đݒ�
    mHUD.SetLocation( pDesc->Width - 170, 0 );
    mHUD.SetSize( 170, 170 );

	// �����_���[�̏���
    for( uint32_t i=0; i<mRenderers.size(); ++i )
    {
        if ( mRenderers[ i ] != NULL )
        {
            if ( !mRenderers[ i ]->OnResizeSwapChain( pDevice, pSwapChain, pDesc ) )
            {
                ELOG( "Error : %s OnResizeSwapChain() Failed.", mRenderers[ i ]->GetTag() );
                hr = E_FAIL;
            }
        }
    }

    return hr;
}

//------------------------------------------------------------------------------------
//! @brief �X���b�v�`�F�C����������ꂽ���̏���
//------------------------------------------------------------------------------------
void DemoAppImpl::OnReleaseSwapChain()
{
    mDlgResMgr.OnD3D11ReleasingSwapChain();

	// �����_���[�Ăяo��
    for( uint32_t i=0; i<mRenderers.size(); ++i )
    {
        if ( mRenderers[ i ] != NULL )
        {
            mRenderers[ i ]->OnReleaseSwapChain();
        }
    }
}

//------------------------------------------------------------------------------------
//! @brief �L�[�{�[�h�̏���
//------------------------------------------------------------------------------------
void DemoAppImpl::OnKeyboard
(
	UINT nChar,
	bool isKeyDown,
	bool isAltDown 
)
{
	// �����_���[�Ăяo��
    for( uint32_t i=0; i<mRenderers.size(); ++i )
    {
        if ( mRenderers[ i ] != NULL )
        {
            mRenderers[ i ]->OnKeyboard( nChar, isKeyDown, isAltDown );
        }
    }
}

//------------------------------------------------------------------------------------
//! @brief �}�E�X�̏���
//------------------------------------------------------------------------------------
void DemoAppImpl::OnMouse
(
	bool isLeftDown,
	bool isRightDown,
	bool isMiddleDown,
	bool isSide1Down,
	bool isSide2Down,
	int  wheelDelta,
	int  x,
	int  y 
)
{
	// �����_���[�Ăяo��
    for( uint32_t i=0; i<mRenderers.size(); ++i )
    {
        if ( mRenderers[ i ] != NULL )
        {
            mRenderers[ i ]->OnMouse(
				isLeftDown,
				isRightDown,
				isMiddleDown,
				isSide1Down,
				isSide2Down,
				wheelDelta,
				x,
				y );
        }
    }
}

//------------------------------------------------------------------------------------
//! @brief �X�V����
//------------------------------------------------------------------------------------
void DemoAppImpl::OnUpdate( double time, float elapsedTime )
{
	// �����_���[�Ăяo��
    for( uint32_t i=0; i<mRenderers.size(); ++i )
    {
        if ( mRenderers[ i ] != NULL )
        {
            mRenderers[ i ]->OnUpdate( time, elapsedTime );
        }
    }
}

//------------------------------------------------------------------------------------
//! @brief �`�揈��
//------------------------------------------------------------------------------------
void DemoAppImpl::OnRender
(
	ID3D11Device		 *pDevice,
	ID3D11DeviceContext  *pImmediateContext,
	double				 time,
	float				 elapsedTime
)
{
	// �ݒ�_�C�A���O���A�N�e�B�u�Ȃ�`�悵�ďI��
    if( mSettingsDlg.IsActive() )
    {
        mSettingsDlg.OnRender( elapsedTime );
        return;
    }       

	// �����_�[�^�[�Q�b�g�Ɛ[�x�X�e���V���r���[���擾
    ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
    ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();

	// �r���[�|�[�g�C�����_�[�^�[�Q�b�g�r���[�C�[�x�X�e���V���r���[��ݒ�
	DXUTSetupD3D11Views( pImmediateContext );
 
	// �^�[�Q�b�g�r���[���N���A
    pImmediateContext->ClearRenderTargetView( pRTV, mClearColor );
    pImmediateContext->ClearDepthStencilView( pDSV, D3D11_CLEAR_DEPTH, 1.0, 0 );

	// �����_���[�Ăяo��
    for( uint32_t i=0; i<mRenderers.size(); ++i )
    {
        if ( mRenderers[ i ] != NULL )
        {
            mRenderers[ i ]->OnRender( pDevice, pImmediateContext, time, elapsedTime );
        }
    }
     
    DXUT_BeginPerfEvent( DXUT_PERFEVENTCOLOR, L"HUD / Stats" );

	// GUI�`��
    mHUD.OnRender( elapsedTime );

	// �e�L�X�g�`��
    mpTextHelper->Begin();
    mpTextHelper->SetInsertionPos( 5, 5 );
    mpTextHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
    mpTextHelper->DrawTextLine( DXUTGetFrameStats( DXUTIsVsyncEnabled() ) );
    mpTextHelper->DrawTextLine( DXUTGetDeviceStats() );
    mpTextHelper->End();
    DXUT_EndPerfEvent();

    static DWORD dwTimefirst = GetTickCount();
    if ( GetTickCount() - dwTimefirst > 5000 )
    {    
        OutputDebugString( DXUTGetFrameStats( DXUTIsVsyncEnabled() ) );
        OutputDebugString( L"\n" );
        dwTimefirst = GetTickCount();
    }

}

//------------------------------------------------------------------------------------
//! @brief GUI�̃C�x���g����
//------------------------------------------------------------------------------------
void DemoAppImpl::OnGUIEvent( UINT nEvent, int nCtrlID, CDXUTControl* pCtrl )
{
    switch( nCtrlID )
    {
		// �t���X�N���[�����^�E�B���h�E���̐؂�ւ�
        case IDC_TOGGLE_FULLSCREEN:
            { DXUTToggleFullScreen(); }
            break;

		// REF�̐؂�ւ�
        case IDC_TOGGLE_REF:
            { DXUTToggleREF();}
            break;

		// �f�o�C�X�̕ϊ�
        case IDC_CHANGE_DEVICE:
            { mSettingsDlg.SetActive( !mSettingsDlg.IsActive() ); }
            break;

        default:
            /* DO_NOTHING */
            break;
    }
}

//------------------------------------------------------------------------------------
//! @brief ���b�Z�[�W�v���V�[�W��
//------------------------------------------------------------------------------------
LRESULT DemoAppImpl::MsgProc( HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp, bool* pbNoFurtherProcessing )
{
    (*pbNoFurtherProcessing) = mDlgResMgr.MsgProc( hWnd, uMsg, wp, lp );
    if( (*pbNoFurtherProcessing) )
	{ return 0; }

    if( mSettingsDlg.IsActive() )
	{
		mSettingsDlg.MsgProc( hWnd, uMsg, wp, lp );
		return 0;
    }

    (*pbNoFurtherProcessing) = mHUD.MsgProc( hWnd, uMsg, wp, lp );
    if( (*pbNoFurtherProcessing) )
    { return 0; }

	// �����_���[�̌Ăяo��
    for( uint32_t i=0; i<mRenderers.size(); ++i )
    {
        if ( mRenderers[ i ] != NULL )
        {
            mRenderers[ i ]->MsgProc( hWnd, uMsg, wp, lp );
        }
    }

    return 0;
}


//------------------------------------------------------------------------------------
//! @brief �B��̃C���X�^���X���擾
//------------------------------------------------------------------------------------
DemoApp* DemoApp::GetInstance()
{ return &gInstance; }


///////////////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------
//! @brief D3D11�f�o�C�X�����e�ł��邩�ǂ���
//-------------------------------------------------------------------------------------
bool CALLBACK AsdxDemoApp_IsD3D11DeviceAcceptable
( 
    const CD3D11EnumAdapterInfo *AdapterInfo,
    UINT nOutput,
    const CD3D11EnumDeviceInfo *pDeviceInfo,
    DXGI_FORMAT BackBufferFormat,
    bool isWindowed,
    void* pUserContext
)
{
    /* DO_NOTHING */
    return true;
}

//-------------------------------------------------------------------------------------
//! @brief D3D11�f�o�C�X�쐬���̏���
//-------------------------------------------------------------------------------------
HRESULT CALLBACK AsdxDemoApp_OnD3D11CreateDevice
(
    ID3D11Device* pDevice,
    const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
    void* pUserContext
)
{
    return gInstance.OnCreateDevice( pDevice, pBackBufferSurfaceDesc );
}

//-------------------------------------------------------------------------------------
//! @brief ���T�C�Y���̏���
//-------------------------------------------------------------------------------------
HRESULT CALLBACK AsdxDemoApp_OnD3D11ResizedSwapChain
( 
    ID3D11Device			*pDevice,
    IDXGISwapChain			*pSwapChain,
    const DXGI_SURFACE_DESC *pBackBufferSurfaceDesc,
    void					*pUserContext 
)
{
    return gInstance.OnResizeSwapChain(
        pDevice,
        pSwapChain,
        pBackBufferSurfaceDesc );
}

//-------------------------------------------------------------------------------------
//! @brief �t���[���X�V���̏���
//-------------------------------------------------------------------------------------
void CALLBACK AsdxDemoApp_OnFrameMove
(
    double dTime,
    float  fElapsedTime,
    void   *pUserContext 
)
{
    gInstance.OnUpdate( dTime, fElapsedTime );
}

//-------------------------------------------------------------------------------------
//! @brief �t���[���`�揈��
//-------------------------------------------------------------------------------------
void CALLBACK AsdxDemoApp_OnD3D11FrameRender
(
    ID3D11Device		 *pDevice,
    ID3D11DeviceContext  *pDeviceContext,
    double				 dTime,
    float				 fElapsedTime,
    void				 *pUserContext
)
{
    gInstance.OnRender( 
        pDevice, 
        pDeviceContext,
        dTime,
        fElapsedTime );    
}

//-------------------------------------------------------------------------------------
//! @brief �X���b�v�`�F�C��������̏���
//-------------------------------------------------------------------------------------
void CALLBACK AsdxDemoApp_OnD3D11ReleasingSwapChain( void* pUserContext )
{
    gInstance.OnReleaseSwapChain();
}

//-------------------------------------------------------------------------------------
//! @brief D3D11�f�o�C�X�j�����̏���
//-------------------------------------------------------------------------------------
void CALLBACK AsdxDemoApp_OnD3D11DestroyDevice( void* pUserContext )
{
    gInstance.OnDestroyDevice();
}

//-------------------------------------------------------------------------------------
//! @brief �f�o�C�X�ݒ�C�����̏���
//-------------------------------------------------------------------------------------
bool CALLBACK AsdxDemoApp_ModifyDeviceSettings
(
    DXUTDeviceSettings  *pDeviceSettings,
    void				*pUserContext
)
{
    // For the first device created if its a REF device, optionally display a warning dialog box
    static bool isFirstTime = true;
    if( isFirstTime )
    {
        isFirstTime = false;
        if( ( DXUT_D3D11_DEVICE == pDeviceSettings->ver )
         && ( pDeviceSettings->d3d11.DriverType == D3D_DRIVER_TYPE_REFERENCE ) )
        {
            DXUTDisplaySwitchingToREFWarning( pDeviceSettings->ver );
        }
    }

    return true;
}

//-------------------------------------------------------------------------------------
//! @brief ���b�Z�[�W�v���V�[�W��
//-------------------------------------------------------------------------------------
LRESULT CALLBACK AsdxDemoApp_MsgProc
(
    HWND    hWnd,
    UINT    uMsg,
    WPARAM  wp,
    LPARAM  lp,
    bool	*pbNoFurtherProcessing,
    void	*pUserContext
)
{
     return gInstance.MsgProc( hWnd, uMsg, wp, lp, pbNoFurtherProcessing );
}

//-------------------------------------------------------------------------------------
//! @brief �L�[�{�[�h�̏���
//-------------------------------------------------------------------------------------
void CALLBACK AsdxDemoApp_OnKeyboard
(
    UINT nChar,
    bool bKeyDown,
    bool bAltDown,
    void *pUserContext
)
{
    gInstance.OnKeyboard( nChar, bKeyDown, bAltDown );
}

//------------------------------------------------------------------------------------
//! @brief �}�E�X�̏���
//------------------------------------------------------------------------------------
void CALLBACK AsdxDemoApp_OnMouse
(
    bool bLeftButtonDown,
    bool bRightButtonDown,
    bool bMiddleButtonDown,
    bool bSideButton1Down,
    bool bSideButton2Down,
    int  nMouseWheelDelta,
    int  xPos,
    int  yPos,
    void *pUserContext
)
{
    gInstance.OnMouse( 
        bLeftButtonDown,
        bRightButtonDown,
        bMiddleButtonDown,
        bSideButton1Down, 
        bSideButton2Down, 
        nMouseWheelDelta,
        xPos, 
        yPos );
}

//-------------------------------------------------------------------------------------
//! @brief GUI�C�x���g�̏���
//-------------------------------------------------------------------------------------
void CALLBACK AsdxDemoApp_OnGUIEvent
(
    UINT nEvent,
    int nControlID,
    CDXUTControl* pControl,
    void* pUserContext
)
{
    gInstance.OnGUIEvent( nEvent, nControlID, pControl );
}



} // namespace SampleProgram
} // namespace Asura