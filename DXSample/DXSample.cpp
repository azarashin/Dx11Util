// DXSample.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "DXSample.h"

#define MAX_LOADSTRING 100


#define STRICT					// 型チェックを厳密に行なう
#define WIN32_LEAN_AND_MEAN		// ヘッダーからあまり使われない関数を省く
#define WINVER        0x0600	// Windows Vista以降対応アプリを指定(なくてもよい)
#define _WIN32_WINNT  0x0600	// 同上

#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }	// 解放マクロ

#include <windows.h>
#include <crtdbg.h>
#include <d3dx11.h>
#include <dxerr.h>

#include <DX11Util/Core/Dx11Context.h>

#include "SampleScene01.h"
#include "SampleScene02.h"
#include "SampleScene03.h"
#include "SampleScene04.h"
#include "SampleScene06.h"
#include "SampleScene07.h"

// サウンド系

#define _WIN32_DCOM
#include <windows.h>
#include <commdlg.h>
//#include <xact3.h>
#include <shellapi.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )


// サウンド系
//-----------------------------------------------------------------------------
// Forward declaration
//-----------------------------------------------------------------------------
//HRESULT PrepareXACT();
//void UpdateAudio();
//VOID CleanupXACT();
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
//void WINAPI XACTNotificationCallback( const XACT_NOTIFICATION* pNotification );
HRESULT FindMediaFileCch( WCHAR* strDestPath, int cchDest, LPCWSTR strFilename );
bool DoesCommandLineContainAuditionSwitch();


//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// Used by D3D11Sample01.rc
//
#define IDI_ICON1                       102

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        103
#define _APS_NEXT_COMMAND_VALUE         40002
#define _APS_NEXT_CONTROL_VALUE         1001
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif


/*-------------------------------------------
	グローバル変数(アプリケーション関連)
--------------------------------------------*/
static HINSTANCE	s_hInstance		= NULL;	// インスタンス ハンドル
static HWND		s_hWindow		= NULL;	// ウインドウ ハンドル

static WCHAR		s_szAppTitle[]	= L"Direct3D 11 Sample01";
static WCHAR		s_szWndClass[]	= L"D3D11S01";

// 起動時の描画領域サイズ
static SIZE		s_sizeWindow	= { 640, 480 };		// ウインドウのクライアント領域


static Dx11Context* s_scene; 

/*-------------------------------------------
	関数定義
--------------------------------------------*/

static LRESULT CALLBACK MainWndProc(HWND hWnd,UINT msg,UINT wParam,LONG lParam);
static HRESULT InitBackBuffer(void);

/*-------------------------------------------
	アプリケーション初期化
--------------------------------------------*/
HRESULT InitApp(HINSTANCE hInst)
{
	// アプリケーションのインスタンス ハンドルを保存
	s_hInstance = hInst;

	// ウインドウ クラスの登録
	WNDCLASS wc;
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= (WNDPROC)MainWndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInst;
	wc.hIcon			= LoadIcon(hInst, (LPCTSTR)IDI_ICON1);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= s_szWndClass;

	if (!RegisterClass(&wc))
		return DXTRACE_ERR(L"InitApp", GetLastError());

	// メイン ウインドウ作成
	RECT rect;
	rect.top	= 0;
	rect.left	= 0;
	rect.right	= s_sizeWindow.cx;
	rect.bottom	= s_sizeWindow.cy;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, TRUE);

	s_hWindow = CreateWindow(s_szWndClass, s_szAppTitle,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			rect.right - rect.left, rect.bottom - rect.top,
			NULL, NULL, hInst, NULL);
	if (s_hWindow == NULL)
		return DXTRACE_ERR(L"InitApp", GetLastError());

	// ウインドウ表示
	ShowWindow(s_hWindow, SW_SHOWNORMAL);
	UpdateWindow(s_hWindow);

//	PrepareXACT(); 

	return S_OK;
}

/*-------------------------------------------
	Direct3D初期化
--------------------------------------------*/
HRESULT InitDirect3D(void)
{
	 s_scene = new Dx11Context(); 
	 s_scene->Setup(s_hWindow, new SampleScene07()); 

	 return S_OK;
}



/*-------------------------------------------
	Direct3Dの終了処理
--------------------------------------------*/
bool CleanupDirect3D(void)
{
	s_scene->Term(); 
	return true;
}

/*-------------------------------------------
	アプリケーションの終了処理
--------------------------------------------*/
bool CleanupApp(void)
{
//	CleanupXACT(); 
	// ウインドウ クラスの登録解除
	UnregisterClass(s_szWndClass, s_hInstance);
	return true;
}

/*-------------------------------------------
	ウィンドウ処理
--------------------------------------------*/
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, UINT wParam, LONG lParam)
{
	HRESULT hr = S_OK;
	int nWidth, nHeight; 

	switch(msg)
	{
	case WM_DESTROY:
		// Direct3Dの終了処理
		CleanupDirect3D();
		// ウインドウを閉じる
		PostQuitMessage(0);
		s_hWindow = NULL;
		return 0;

	case WM_SIZE:
		nWidth  =  lParam & 0xFFFF;        // xサイズ
		nHeight = (lParam >> 16) & 0xFFFF; // yサイズ
		if(s_scene) {
			s_scene->WindowResize(nWidth, nHeight); 
		}
		break; 

	case WM_KEYDOWN:
		// キー入力の処理
		switch(wParam)
		{
		case VK_ESCAPE:	// [ESC]キーでウインドウを閉じる
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case VK_F5:		// [F5]キーで画面モードを切り替える
			s_scene->ToggleFullScreen(); 
			break; 
		}
		break;
	}

	// デフォルト処理
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*--------------------------------------------
	デバイスの消失処理
--------------------------------------------*/
HRESULT IsDeviceRemoved(void)
{
	HRESULT hr;
	return S_OK;         // 正常
}

/*--------------------------------------------
	アイドル時の処理
--------------------------------------------*/
bool AppIdle(void)
{
//	if (!s_pD3DDevice)
	//	return false;

	HRESULT hr;
	// デバイスの消失処理
	hr = IsDeviceRemoved();
	if (FAILED(hr)) {
		return false;
	}

	// 画面の更新
	hr = s_scene->Update(); 
	if (FAILED(hr)) {
		return false;
	}

	if(hr == S_FALSE) {
		return false;
	}

//	UpdateAudio(); 

	return true;
}

/*--------------------------------------------
	メイン
---------------------------------------------*/
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, int)
{
	// デバッグ ヒープ マネージャによるメモリ割り当ての追跡方法を設定
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// アプリケーションに関する初期化
	HRESULT hr = InitApp(hInst);
	if (FAILED(hr))
	{
		DXTRACE_ERR(L"WinMain InitApp", hr);
		return 0;
	}

	// Direct3Dの初期化
	hr = InitDirect3D();
	if (FAILED(hr)) {
		DXTRACE_ERR(L"WinMain InitDirect3D", hr);
		CleanupDirect3D();
		CleanupApp();
		return 0;
	}


	// メッセージ ループ
	MSG msg;
	do
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// アイドル処理
			if (!AppIdle())
				// エラーがある場合，アプリケーションを終了する
				DestroyWindow(s_hWindow);
		}
	} while (msg.message != WM_QUIT);

	// アプリケーションの終了処理
	CleanupApp();

	return (int)msg.wParam;
}
