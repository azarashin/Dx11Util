//-------------------------------------------------------------------------------------
// File : asdxDemoApp.h
// Desc : Asura Demo Application For DirectX 11
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// History
//-------------------------------------------------------------------------------------
// <---> 2011/08/04 [Pocol] êVãKçÏê¨
// <End>

#ifndef __ASURA_DIRECTX_DEMO_APP_H__
#define __ASURA_DIRECTX_DEMO_APP_H__

//-------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------
#include "asDef.h"



namespace Asura {
namespace SampleProgram {

///////////////////////////////////////////////////////////////////////////////////////
// IRenderer class
///////////////////////////////////////////////////////////////////////////////////////
class IRenderer
{
    //---------------------------------------------------------------------------------
    // list of friend classes and methods
    //---------------------------------------------------------------------------------
    /* NOTHING */

private:
    //---------------------------------------------------------------------------------
    // private variables
    //---------------------------------------------------------------------------------
    /* NOTHING */

    //---------------------------------------------------------------------------------
    // private methods
    //---------------------------------------------------------------------------------
    /* NOTHING */

protected:
    //---------------------------------------------------------------------------------
    // protected variables
    //---------------------------------------------------------------------------------
    /* NOTHING */

    //---------------------------------------------------------------------------------
    // protected methods
    //---------------------------------------------------------------------------------
    /* NOTHING */

public:
    //---------------------------------------------------------------------------------
    // public variables
    //---------------------------------------------------------------------------------
    /* NOTHING */

    //---------------------------------------------------------------------------------
    // public methods
    //---------------------------------------------------------------------------------
	virtual bool    OnInit() { return true; }
	virtual void    OnRelease() {}
	virtual bool    OnCreateDevice( ID3D11Device* pDevice, const DXGI_SURFACE_DESC* pDesc ) { return true; }
	virtual void    OnDestroyDevice() {}
	virtual void    OnRemoveDevice() {}
	virtual bool    OnResizeSwapChain( ID3D11Device* pDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC *pDesc ) { return true; }
	virtual void    OnReleaseSwapChain() {}
	virtual void    OnKeyboard( UINT nChar, bool isKeyDown, bool isAltDown ) {}
	virtual void    OnMouse( bool isLeftDown, bool isRightDown, bool isMiddleDown, bool isSide1Down, bool isSide2Down, int wheelDelta, int x, int y ) {}
	virtual void    OnUpdate( double time, float elapsedTime ) {}
	virtual void    OnRender( ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext, double time, float elapsedTime ) {}
	virtual void    MsgProc( HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp ) {}
    virtual char*   GetTag() = 0;
};


///////////////////////////////////////////////////////////////////////////////////////
// DemoApp class
///////////////////////////////////////////////////////////////////////////////////////
class DemoApp
{
    //---------------------------------------------------------------------------------
    // list of friend classes and methods
    //---------------------------------------------------------------------------------
    /* NOTHING */

private:
    //---------------------------------------------------------------------------------
    // private variables
    //---------------------------------------------------------------------------------
    /* NOTHING */

    //---------------------------------------------------------------------------------
    // private methods
    //---------------------------------------------------------------------------------
    /* NOTHING */

protected:
    //---------------------------------------------------------------------------------
    // protected variables
    //---------------------------------------------------------------------------------
    /* NOTHING */

    //---------------------------------------------------------------------------------
    // protected methods
    //---------------------------------------------------------------------------------
    DemoApp() {}
    virtual ~DemoApp() {}
    DemoApp( const DemoApp &app ) {}
    DemoApp& operator = ( const DemoApp &app ) { return (*this); }

public:
    //---------------------------------------------------------------------------------
    // public variables
    //---------------------------------------------------------------------------------
    /* NOTHING */

    //---------------------------------------------------------------------------------
    // public methods
    //---------------------------------------------------------------------------------
    static DemoApp* GetInstance();
    virtual int     Register( IRenderer* pRenderer ) = 0;
    virtual int     Run() = 0;
    virtual void    SetSize( const int width, const int height ) = 0;
    virtual void    SetTitle( const wchar_t* title ) = 0;
    virtual float*  GetDefaultClearColor() = 0;
    virtual ID3D11RenderTargetView* GetDefaultRenderTargetView() = 0;
    virtual ID3D11DepthStencilView* GetDefaultDepthStencilView() = 0;
	virtual D3D11_VIEWPORT          GetDefaultViewport() = 0;
	virtual float   GetAspectRatio() = 0;
	virtual int     GetWidth() = 0;
	virtual int     GetHeight() = 0;

};

} // namespace SampleProgram
} // namespace Asura

#endif//__ASURA_DIRECTX_DEMO_APP_H__