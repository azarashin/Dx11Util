//-------------------------------------------------------------------------------------
// File : asdxDemoApp.h
// Desc : Asura Demo Application For DirectX 11
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// History
//-------------------------------------------------------------------------------------
// <---> 2011/08/04 [Pocol] 新規作成
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

// メッセージプロシージャ
LRESULT CALLBACK AsdxDemoApp_MsgProc( 
    HWND hWnd,
    UINT uMsg, 
    WPARAM wp,
    LPARAM lp,
    bool* pbNoFurtherProcessing, 
    void* pUserContext );

// キーボードの処理
void CALLBACK AsdxDemoApp_OnKeyboard(
    UINT nChar,
    bool bKeyDown,
    bool bAltDown,
    void* pUserContext );

// マウスの処理
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

// GUIイベントの処理
void CALLBACK AsdxDemoApp_OnGUIEvent(
    UINT nEvent,
    int nControlID,
    CDXUTControl* pControl,
    void* pUserContext );

// 更新処理
void CALLBACK AsdxDemoApp_OnFrameMove(
    double dTime,
    float fElapsedTime,
    void* pUserContext );

// デバイス設定の修正時の処理
bool CALLBACK AsdxDemoApp_ModifyDeviceSettings(
    DXUTDeviceSettings* pDeviceSettings,
    void* pUserContext );

// D3D11デバイスが許容されているかどうか
bool CALLBACK AsdxDemoApp_IsD3D11DeviceAcceptable(
    const CD3D11EnumAdapterInfo* pAdapterInfo,
    UINT nOutput,
    const CD3D11EnumDeviceInfo* pDeviceInfo,
    DXGI_FORMAT BackBufferFormat,
    bool isWindowed,
    void* pUserContext );

// D3D11デバイスが生成された時の処理
HRESULT CALLBACK AsdxDemoApp_OnD3D11CreateDevice(
    ID3D11Device* pDevice,
    const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
    void* pUserContext );

// スワップチェインがリサイズされた時の処理
HRESULT CALLBACK AsdxDemoApp_OnD3D11ResizedSwapChain(
    ID3D11Device* pDevice,
    IDXGISwapChain* pSwapChain,
    const DXGI_SURFACE_DESC *pBackBufferSurfaceDesc,
    void* pUserContext );

// スワップチェインが解放された時の処理
void CALLBACK AsdxDemoApp_OnD3D11ReleasingSwapChain( void* pUserContext );

// デバイスが破棄された時の処理
void CALLBACK AsdxDemoApp_OnD3D11DestroyDevice( void* pUserContext );

// デバイスが削除された時の処理
void CALLBACK AsdxDemoApp_OnDeviceRemoved( void *pUserContext );

// 描画処理
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
//! @brief 初期値設定
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
//! @brief 破棄処理
//------------------------------------------------------------------------------------
DemoAppImpl::~DemoAppImpl()
{
    mRenderers.clear();
}

//------------------------------------------------------------------------------------
//! @breif タイトルを設定する
//! @param [in] title 設定するタイトル名
//------------------------------------------------------------------------------------
void DemoAppImpl::SetTitle( const wchar_t* title )
{ mTitle = title; }

//------------------------------------------------------------------------------------
//! @breif ウィンドウのサイズを設定する
//! @param [in] w ウィンドウの幅
//! @param [in] h ウィンドウの高さ
//------------------------------------------------------------------------------------
void DemoAppImpl::SetSize( const int w, const int h )
{
    mWidth = w;
    mHeight = h;
}

//------------------------------------------------------------------------------------
//! @brief デフォルトのクリアカラーを取得する
//------------------------------------------------------------------------------------
float* DemoAppImpl::GetDefaultClearColor()
{ return mClearColor; }

//------------------------------------------------------------------------------------
//! @brief デフォルトのレンダーターゲットビューを取得する
//------------------------------------------------------------------------------------
ID3D11RenderTargetView* DemoAppImpl::GetDefaultRenderTargetView()
{ return DXUTGetD3D11RenderTargetView(); }

//------------------------------------------------------------------------------------
//! @brief デフォルトの深度・ステンシルビューを取得する
//------------------------------------------------------------------------------------
ID3D11DepthStencilView* DemoAppImpl::GetDefaultDepthStencilView()
{ return DXUTGetD3D11DepthStencilView(); }

//------------------------------------------------------------------------
//! @brief アスペクト比を取得する
//------------------------------------------------------------------------
float DemoAppImpl::GetAspectRatio()
{ return static_cast< float >( mWidth ) / static_cast< float >( mHeight ); }

//------------------------------------------------------------------------
//! @brief ウィンドウの幅を取得する
//------------------------------------------------------------------------
int DemoAppImpl::GetWidth()
{ return mWidth; }

//------------------------------------------------------------------------
//! @brief ウィンドウの高さを取得する
//------------------------------------------------------------------------
int DemoAppImpl::GetHeight()
{ return mHeight; }

//------------------------------------------------------------------------
//! @brief デフォルトのビューポートを取得する
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
//! @brief レンダラーを登録する
//------------------------------------------------------------------------------------
int DemoAppImpl::Register( IRenderer* pRenderer )
{
    mRenderers.push_back( pRenderer );
    return static_cast< int >( mRenderers.size() );
}

//------------------------------------------------------------------------------------
//! @brief アプリケーションの実行
//------------------------------------------------------------------------------------
int DemoAppImpl::Run()
{
#if defined(DEBUG) || defined(_DEBUG)
	// メモリリークチェック
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// DXUTのコールバック関数の設定
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

	// 初期化処理実行
    if ( !OnInit() )
    {
        ELOG( "Error : Application Initialize Failed." );
        return -1;
    }

	// DXUTの初期化
    DXUTInit( true, true, NULL );
    DXUTSetCursorSettings( true, true );

	// ウィンドウとデバイスの生成
    DXUTCreateWindow( mTitle.c_str() );
    DXUTCreateDevice( D3D_FEATURE_LEVEL_11_0, true, mWidth, mHeight );

	// メインループ実行
    DXUTMainLoop();

	// メモリ解放処理
    OnRelease();

	// 終了コードを取得し返却する
    return DXUTGetExitCode();
}

//------------------------------------------------------------------------------------
//! @brief 初期化処理
//------------------------------------------------------------------------------------
bool DemoAppImpl::OnInit()
{
	// GUIと設定ダイアログボックスの初期化
    mSettingsDlg.Init( &mDlgResMgr );
    mHUD.Init( &mDlgResMgr );

    int y = 30;
    int h = 26;

	// GUIイベントのコールバック関数の設定
    mHUD.SetCallback( AsdxDemoApp_OnGUIEvent );

	// GUIのボタンを追加する
    mHUD.AddButton( IDC_TOGGLE_FULLSCREEN, L"Toggle full screen", 0, y,      170, 22 );
    mHUD.AddButton( IDC_TOGGLE_REF,        L"Toggle REF (F3)",    0, y += h, 170, 22, VK_F3 );
    mHUD.AddButton( IDC_CHANGE_DEVICE,     L"Change device (F2)", 0, y += h, 170, 22, VK_F2 );

	// レンダラーの登録
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
//! @brief メモリ解放処理
//------------------------------------------------------------------------------------
void DemoAppImpl::OnRelease()
{
	// テキストヘルパー破棄
	SAFE_DELETE( mpTextHelper );

	// レンダラーが持つメモリを解放
    for( uint32_t i=0; i<mRenderers.size(); ++i )
    {
        if ( mRenderers[ i ] != NULL )
        {
            mRenderers[ i ]->OnRelease();
            mRenderers[ i ] = NULL;
        }
    }

	// レンダラーをクリア
	mRenderers.clear();
}

//------------------------------------------------------------------------------------
//! @brief デバイス作成時の処理
//------------------------------------------------------------------------------------
HRESULT DemoAppImpl::OnCreateDevice( ID3D11Device *pDevice, const DXGI_SURFACE_DESC *pDesc )
{
    HRESULT hr = S_OK;
    ID3D11DeviceContext* pDeviceContext = DXUTGetD3D11DeviceContext();

    V_RETURN( mDlgResMgr.OnD3D11CreateDevice( pDevice, pDeviceContext ) );
    V_RETURN( mSettingsDlg.OnD3D11CreateDevice( pDevice ) );

	// テキストヘルパーの生成
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

	// レンダラーの処理を呼び出し
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
//! @brief デバイス破棄時の処理
//------------------------------------------------------------------------------------
void DemoAppImpl::OnDestroyDevice()
{
    mDlgResMgr.OnD3D11DestroyDevice();
    mSettingsDlg.OnD3D11DestroyDevice();
    DXUTGetGlobalResourceCache().OnDestroyDevice();

	// テキストヘルパー破棄
    SAFE_DELETE( mpTextHelper );

	// レンダラーの処理を呼び出し
    for( uint32_t i=0; i<mRenderers.size(); ++i )
    {
        if ( mRenderers[ i ] != NULL )
        {
            mRenderers[ i ]->OnDestroyDevice();
        }
    }
}

//------------------------------------------------------------------------------------
//! @brief デバイス削除時の処理
//------------------------------------------------------------------------------------
void DemoAppImpl::OnRemoveDevice()
{
	// レンダラーの処理
    for( uint32_t i=0; i<mRenderers.size(); ++i )
    {
        if ( mRenderers[ i ] != NULL )
        {
            mRenderers[ i ]->OnRemoveDevice();
        }
    }
}

//------------------------------------------------------------------------------------
//! @brief スワップチェインがリサイズされた時の処理
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

	// GUIの位置を再設定
    mHUD.SetLocation( pDesc->Width - 170, 0 );
    mHUD.SetSize( 170, 170 );

	// レンダラーの処理
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
//! @brief スワップチェインが解放された時の処理
//------------------------------------------------------------------------------------
void DemoAppImpl::OnReleaseSwapChain()
{
    mDlgResMgr.OnD3D11ReleasingSwapChain();

	// レンダラー呼び出し
    for( uint32_t i=0; i<mRenderers.size(); ++i )
    {
        if ( mRenderers[ i ] != NULL )
        {
            mRenderers[ i ]->OnReleaseSwapChain();
        }
    }
}

//------------------------------------------------------------------------------------
//! @brief キーボードの処理
//------------------------------------------------------------------------------------
void DemoAppImpl::OnKeyboard
(
	UINT nChar,
	bool isKeyDown,
	bool isAltDown 
)
{
	// レンダラー呼び出し
    for( uint32_t i=0; i<mRenderers.size(); ++i )
    {
        if ( mRenderers[ i ] != NULL )
        {
            mRenderers[ i ]->OnKeyboard( nChar, isKeyDown, isAltDown );
        }
    }
}

//------------------------------------------------------------------------------------
//! @brief マウスの処理
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
	// レンダラー呼び出し
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
//! @brief 更新処理
//------------------------------------------------------------------------------------
void DemoAppImpl::OnUpdate( double time, float elapsedTime )
{
	// レンダラー呼び出し
    for( uint32_t i=0; i<mRenderers.size(); ++i )
    {
        if ( mRenderers[ i ] != NULL )
        {
            mRenderers[ i ]->OnUpdate( time, elapsedTime );
        }
    }
}

//------------------------------------------------------------------------------------
//! @brief 描画処理
//------------------------------------------------------------------------------------
void DemoAppImpl::OnRender
(
	ID3D11Device		 *pDevice,
	ID3D11DeviceContext  *pImmediateContext,
	double				 time,
	float				 elapsedTime
)
{
	// 設定ダイアログがアクティブなら描画して終了
    if( mSettingsDlg.IsActive() )
    {
        mSettingsDlg.OnRender( elapsedTime );
        return;
    }       

	// レンダーターゲットと深度ステンシルビューを取得
    ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
    ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();

	// ビューポート，レンダーターゲットビュー，深度ステンシルビューを設定
	DXUTSetupD3D11Views( pImmediateContext );
 
	// ターゲットビューをクリア
    pImmediateContext->ClearRenderTargetView( pRTV, mClearColor );
    pImmediateContext->ClearDepthStencilView( pDSV, D3D11_CLEAR_DEPTH, 1.0, 0 );

	// レンダラー呼び出し
    for( uint32_t i=0; i<mRenderers.size(); ++i )
    {
        if ( mRenderers[ i ] != NULL )
        {
            mRenderers[ i ]->OnRender( pDevice, pImmediateContext, time, elapsedTime );
        }
    }
     
    DXUT_BeginPerfEvent( DXUT_PERFEVENTCOLOR, L"HUD / Stats" );

	// GUI描画
    mHUD.OnRender( elapsedTime );

	// テキスト描画
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
//! @brief GUIのイベント処理
//------------------------------------------------------------------------------------
void DemoAppImpl::OnGUIEvent( UINT nEvent, int nCtrlID, CDXUTControl* pCtrl )
{
    switch( nCtrlID )
    {
		// フルスクリーン化／ウィンドウ化の切り替え
        case IDC_TOGGLE_FULLSCREEN:
            { DXUTToggleFullScreen(); }
            break;

		// REFの切り替え
        case IDC_TOGGLE_REF:
            { DXUTToggleREF();}
            break;

		// デバイスの変換
        case IDC_CHANGE_DEVICE:
            { mSettingsDlg.SetActive( !mSettingsDlg.IsActive() ); }
            break;

        default:
            /* DO_NOTHING */
            break;
    }
}

//------------------------------------------------------------------------------------
//! @brief メッセージプロシージャ
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

	// レンダラーの呼び出し
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
//! @brief 唯一のインスタンスを取得
//------------------------------------------------------------------------------------
DemoApp* DemoApp::GetInstance()
{ return &gInstance; }


///////////////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------
//! @brief D3D11デバイスが許容できるかどうか
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
//! @brief D3D11デバイス作成時の処理
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
//! @brief リサイズ時の処理
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
//! @brief フレーム更新時の処理
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
//! @brief フレーム描画処理
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
//! @brief スワップチェイン解放時の処理
//-------------------------------------------------------------------------------------
void CALLBACK AsdxDemoApp_OnD3D11ReleasingSwapChain( void* pUserContext )
{
    gInstance.OnReleaseSwapChain();
}

//-------------------------------------------------------------------------------------
//! @brief D3D11デバイス破棄時の処理
//-------------------------------------------------------------------------------------
void CALLBACK AsdxDemoApp_OnD3D11DestroyDevice( void* pUserContext )
{
    gInstance.OnDestroyDevice();
}

//-------------------------------------------------------------------------------------
//! @brief デバイス設定修正時の処理
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
//! @brief メッセージプロシージャ
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
//! @brief キーボードの処理
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
//! @brief マウスの処理
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
//! @brief GUIイベントの処理
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