//-----------------------------------------------------------------------
// File : AsDemoCamera.h
// Desc : Asura Camera For Demo Application
// Copyright(c) Project Asura All right reserved.
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// History
//-----------------------------------------------------------------------
// <---> 2011/04/16 [Pocol] êVãKçÏê¨
// <End>

#ifndef __ASURA_DEMO_CAMERA_H__
#define __ASURA_DEMO_CAMERA_H__

//-----------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------
#include "asDef.h"
#include "asMath.h"
//#include "asdxApp.h"


namespace Asura {
namespace Graphics {

/////////////////////////////////////////////////////////////////////////
// Camera class
/////////////////////////////////////////////////////////////////////////
class Camera
{
    //-------------------------------------------------------------------
    // list of friend classes and methods
    //-------------------------------------------------------------------
    /* NOTHING */

public:
    //-------------------------------------------------------------------
    // public variables
    //-------------------------------------------------------------------
    enum 
    {
        CAMERA_TYPE_THIRD_PERSON = 0,
        CAMERA_TYPE_FIRST_PERSON,
        NUM_CAMERA_TYPE
    };

    //-------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------
    Camera();
    virtual ~Camera();
    void SetType( int type );
    void SetPosition( Asura::Math::Vector3 position );
    void SetDirection( Asura::Math::Vector3 direction );
    void SetUpVector( Asura::Math::Vector3 upvector );
    void Compute();
    int  GetType();
    Asura::Math::Vector3 GetPosition();
    Asura::Math::Vector3 GetDirection();
    Asura::Math::Vector3 GetUpVector();
    Asura::Math::Matrix  GetView();

protected:
    //-------------------------------------------------------------------
    // protected variables
    //-------------------------------------------------------------------
    typedef struct _MouseState
    {
        int cursorX;
        int cursorY;
        bool isDrag;
    } MouseState;

    int        mType;
    MouseState mLeft;
    MouseState mRight;
    MouseState mMiddle;
    Asura::Math::Vector2 mAngle;
    float      mDistance;

    //-------------------------------------------------------------------
    // protected methods
    //-------------------------------------------------------------------
    void ComputeThirdPersonCamera();
    void ComputeFirstPersonCamera();

private:
    //-------------------------------------------------------------------
    // private variables
    //-------------------------------------------------------------------
    Asura::Math::Vector3 mPosition;
    Asura::Math::Vector3 mDirection;
    Asura::Math::Vector3 mUpVector;
    Asura::Math::Matrix  mView;

    //-------------------------------------------------------------------
    // private methods
    //-------------------------------------------------------------------
    /* NOTHING */
};

} // namespace Graphics
} // namespace Asura


#endif//__ASURA_DEMO_CEMERA_H__