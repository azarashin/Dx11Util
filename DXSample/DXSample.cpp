// DXSample.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "DXSample.h"

#define MAX_LOADSTRING 100


#define STRICT					// �^�`�F�b�N�������ɍs�Ȃ�
#define WIN32_LEAN_AND_MEAN		// �w�b�_�[���炠�܂�g���Ȃ��֐����Ȃ�
#define WINVER        0x0600	// Windows Vista�ȍ~�Ή��A�v�����w��(�Ȃ��Ă��悢)
#define _WIN32_WINNT  0x0600	// ����

#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }	// ����}�N��

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

// �T�E���h�n

#define _WIN32_DCOM
#include <windows.h>
#include <commdlg.h>
//#include <xact3.h>
#include <shellapi.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )


// �T�E���h�n
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
	�O���[�o���ϐ�(�A�v���P�[�V�����֘A)
--------------------------------------------*/
static HINSTANCE	s_hInstance		= NULL;	// �C���X�^���X �n���h��
static HWND		s_hWindow		= NULL;	// �E�C���h�E �n���h��

static WCHAR		s_szAppTitle[]	= L"Direct3D 11 Sample01";
static WCHAR		s_szWndClass[]	= L"D3D11S01";

// �N�����̕`��̈�T�C�Y
static SIZE		s_sizeWindow	= { 640, 480 };		// �E�C���h�E�̃N���C�A���g�̈�


static Dx11Context* s_scene; 

/*-------------------------------------------
	�֐���`
--------------------------------------------*/

static LRESULT CALLBACK MainWndProc(HWND hWnd,UINT msg,UINT wParam,LONG lParam);
static HRESULT InitBackBuffer(void);

/*-------------------------------------------
	�A�v���P�[�V����������
--------------------------------------------*/
HRESULT InitApp(HINSTANCE hInst)
{
	// �A�v���P�[�V�����̃C���X�^���X �n���h����ۑ�
	s_hInstance = hInst;

	// �E�C���h�E �N���X�̓o�^
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

	// ���C�� �E�C���h�E�쐬
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

	// �E�C���h�E�\��
	ShowWindow(s_hWindow, SW_SHOWNORMAL);
	UpdateWindow(s_hWindow);

//	PrepareXACT(); 

	return S_OK;
}

/*-------------------------------------------
	Direct3D������
--------------------------------------------*/
HRESULT InitDirect3D(void)
{
	 s_scene = new Dx11Context(); 
	 s_scene->Setup(s_hWindow, new SampleScene07()); 

	 return S_OK;
}



/*-------------------------------------------
	Direct3D�̏I������
--------------------------------------------*/
bool CleanupDirect3D(void)
{
	s_scene->Term(); 
	return true;
}

/*-------------------------------------------
	�A�v���P�[�V�����̏I������
--------------------------------------------*/
bool CleanupApp(void)
{
//	CleanupXACT(); 
	// �E�C���h�E �N���X�̓o�^����
	UnregisterClass(s_szWndClass, s_hInstance);
	return true;
}

/*-------------------------------------------
	�E�B���h�E����
--------------------------------------------*/
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, UINT wParam, LONG lParam)
{
	HRESULT hr = S_OK;
	int nWidth, nHeight; 

	switch(msg)
	{
	case WM_DESTROY:
		// Direct3D�̏I������
		CleanupDirect3D();
		// �E�C���h�E�����
		PostQuitMessage(0);
		s_hWindow = NULL;
		return 0;

	case WM_SIZE:
		nWidth  =  lParam & 0xFFFF;        // x�T�C�Y
		nHeight = (lParam >> 16) & 0xFFFF; // y�T�C�Y
		if(s_scene) {
			s_scene->WindowResize(nWidth, nHeight); 
		}
		break; 

	case WM_KEYDOWN:
		// �L�[���͂̏���
		switch(wParam)
		{
		case VK_ESCAPE:	// [ESC]�L�[�ŃE�C���h�E�����
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case VK_F5:		// [F5]�L�[�ŉ�ʃ��[�h��؂�ւ���
			s_scene->ToggleFullScreen(); 
			break; 
		}
		break;
	}

	// �f�t�H���g����
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*--------------------------------------------
	�f�o�C�X�̏�������
--------------------------------------------*/
HRESULT IsDeviceRemoved(void)
{
	HRESULT hr;
	return S_OK;         // ����
}

/*--------------------------------------------
	�A�C�h�����̏���
--------------------------------------------*/
bool AppIdle(void)
{
//	if (!s_pD3DDevice)
	//	return false;

	HRESULT hr;
	// �f�o�C�X�̏�������
	hr = IsDeviceRemoved();
	if (FAILED(hr)) {
		return false;
	}

	// ��ʂ̍X�V
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
	���C��
---------------------------------------------*/
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, int)
{
	// �f�o�b�O �q�[�v �}�l�[�W���ɂ�郁�������蓖�Ă̒ǐՕ��@��ݒ�
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// �A�v���P�[�V�����Ɋւ��鏉����
	HRESULT hr = InitApp(hInst);
	if (FAILED(hr))
	{
		DXTRACE_ERR(L"WinMain InitApp", hr);
		return 0;
	}

	// Direct3D�̏�����
	hr = InitDirect3D();
	if (FAILED(hr)) {
		DXTRACE_ERR(L"WinMain InitDirect3D", hr);
		CleanupDirect3D();
		CleanupApp();
		return 0;
	}


	// ���b�Z�[�W ���[�v
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
			// �A�C�h������
			if (!AppIdle())
				// �G���[������ꍇ�C�A�v���P�[�V�������I������
				DestroyWindow(s_hWindow);
		}
	} while (msg.message != WM_QUIT);

	// �A�v���P�[�V�����̏I������
	CleanupApp();

	return (int)msg.wParam;
}
