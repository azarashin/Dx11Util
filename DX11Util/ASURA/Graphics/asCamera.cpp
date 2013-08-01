//----------------------------------------------------------------------
// File : AsCamera.cpp
// Desc : Asura Camera For Demo Application
// Copyright(c) Project Asura All right reserved.
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// History
//----------------------------------------------------------------------
// <---> 2011/04/16 [Pocol] 新規作成
// <End>


//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "asCamera.h"

//----------------------------------------------------------------------
// Using Statements
//----------------------------------------------------------------------
using namespace Asura;
using namespace Asura::Math;


namespace Asura {
namespace Graphics {

///---------------------------------------------------------------------
///<summary>
///初期設定処理
///</summary>
///---------------------------------------------------------------------
Camera::Camera()
{
    mType = CAMERA_TYPE_THIRD_PERSON;

    mPosition = Vector3( 0.0f, 0.0f, 1.0f );
    mDirection = Vector3( 0.0f, 0.0f, 0.0f );
    mUpVector = Vector3( 0.0f, 1.0f, 0.0f );

    mAngle = Vector2( 0.0f, 0.0f );
    mDistance = 1.0f;

    std::memset( &mLeft, 0, sizeof( Camera::MouseState ) );
    std::memset( &mRight, 0, sizeof( Camera::MouseState ) );
    std::memset( &mMiddle, 0, sizeof( Camera::MouseState ) );
   
    mView.Identity();
}

///---------------------------------------------------------------------
///<summary>
///デストラクタ
///</summary>
///---------------------------------------------------------------------
Camera::~Camera()
{
    /* DO_NOTHING */
}

///---------------------------------------------------------------------
///<summary>
///カメラタイプを設定する
///</summary>
///<param name="type">カメラタイプ</param>
///---------------------------------------------------------------------
void Camera::SetType( int type )
{
    mType = type;
}

///---------------------------------------------------------------------
///<summary>
///カメラ位置を設定する
///</summary>
///<param name="position">カメラ位置</param>
///---------------------------------------------------------------------
void Camera::SetPosition( Vector3 position )
{
    if ( mType == CAMERA_TYPE_THIRD_PERSON )
    {
        float dist = mPosition.Length();
        mDistance = dist;

        Vector3 xAxis( 1.0f, 0.0f, 0.0f );
        Vector3 yAxis( 0.0f, 1.0f, 0.0f );

        Vector3 normPos = position;
        normPos.Normalize();
        float angleX = Dot( xAxis, normPos );
        float angleY = Dot( yAxis, normPos );

        mAngle.x = angleX;
        mAngle.y = angleY;

        mPosition = position;
    }
    else
    {
        mPosition = position;
    }
}

///---------------------------------------------------------------------
///<summary>
///カメラの方向を設定する
///</summary>
///<param name="direction">カメラの方向を設定する</param>
///---------------------------------------------------------------------
void Camera::SetDirection( Vector3 direction )
{
    mDirection = direction;
}

///---------------------------------------------------------------------
///<summary>
///カメラのアップベクトルを設定する
///</summary>
///<param name="upvector">カメラのアップベクトル</param>
///---------------------------------------------------------------------
void Camera::SetUpVector( Vector3 upvector )
{
    mUpVector = upvector;
}

///---------------------------------------------------------------------
///<summary>
///ビュー行列を計算する
///</summary>
///---------------------------------------------------------------------
void Camera::Compute()
{
    switch( mType )
    {
        case CAMERA_TYPE_THIRD_PERSON:
            {
                // 3人称カメラ
                ComputeThirdPersonCamera();
            }
            break;

        case CAMERA_TYPE_FIRST_PERSON:
            {
                // 1人称カメラ
                ComputeFirstPersonCamera();
            }
            break;

        default:
            {
                /* DO_NOTHING */
            }
            break;
    }
}

///---------------------------------------------------------------------
///<summary>
///カメラタイプを取得する
///</summary>
///<returns>カメラタイプを返却する</returns>
///---------------------------------------------------------------------
int Camera::GetType()
{
    return mType;
}

///---------------------------------------------------------------------
///<summary>
///カメラ位置を取得する
///</summary>
///<returns>計算後のカメラ位置を返却する</returns>
///---------------------------------------------------------------------
Vector3 Camera::GetPosition()
{
    return mPosition;
}

///---------------------------------------------------------------------
///<summary>
///カメラの方向を取得する
///</summary>
///<returns>計算後のカメラ方向を返却する</returns>
///---------------------------------------------------------------------
Vector3 Camera::GetDirection()
{
    return mDirection;
}

///---------------------------------------------------------------------
///<summary>
///カメラのアップベクトルを取得する
///</summary>
///<returns>計算後のカメラのアップベクトルを返却する</returns>
///---------------------------------------------------------------------
Vector3 Camera::GetUpVector()
{
    return mUpVector;
}

///---------------------------------------------------------------------
///<summary>
///カメラのビュー行列を取得する
///</summary>
///<returns>計算後のカメラのビュー行列を返却する</returns>
///---------------------------------------------------------------------
Matrix Camera::GetView()
{
    return mView;
}

///---------------------------------------------------------------------
///<summary>
///サードパーソンカメラとしてビュー行列を計算する
///</summary>
///---------------------------------------------------------------------
void Camera::ComputeThirdPersonCamera()
{
    // 回転
    if ( App::GetInstance()->IsMouseDown( App::MOUSE_BUTTON_LEFT ) )
    {
        // 押された時のカーソル位置を取得する
        int cursorX = App::GetInstance()->GetCursorX();
        int cursorY = App::GetInstance()->GetCursorY();

        // ドラッグ中でない場合
        if ( !mLeft.isDrag )
        {
            // ドラッグフラグON
            mLeft.isDrag = true;

            // 最初に押されたときは，初期化が必要.
            mLeft.cursorX = cursorX;
            mLeft.cursorY = cursorY;
        }

        // 差分を計算
        int deltaX = cursorX - mLeft.cursorX;
        int deltaY = cursorY - mLeft.cursorY;

        // 更新
        mLeft.cursorX = cursorX;
        mLeft.cursorY = cursorY;

        // 角度計算
        mAngle.x += ToRadian( static_cast< float >( deltaX ) );
        mAngle.y += ToRadian( static_cast< float >( deltaY ) );

        // 0~360に収める
        if ( mAngle.x > 2.0f * Math::Pi )
        { mAngle.x -= ( 2.0f * Math::Pi ); }
        else if ( mAngle.x < 0.0f )
        { mAngle.x += ( 2.0f * Math::Pi ); }

        if ( mAngle.y > 2.0f * Math::Pi )
        { mAngle.y -= ( 2.0f * Math::Pi ); }
        else if ( mAngle.y < 0.0f )
        { mAngle.y += ( 2.0f * Math::Pi ); }
    }
    else if ( App::GetInstance()->IsMouseUp( App::MOUSE_BUTTON_LEFT ) )
    {
        // ドラッグフラグOFF
        mLeft.isDrag = false;
    }
    
    // ズーム
    if ( App::GetInstance()->IsMouseDown( App::MOUSE_BUTTON_RIGHT ) )
    {
        // 押された時のカーソル位置を取得
        int cursorX = App::GetInstance()->GetCursorX();
        int cursorY = App::GetInstance()->GetCursorY();

        // ドラッグ中でない場合
        if ( !mRight.isDrag )
        {
            // ドラッグフラグON
            mRight.isDrag = true;

            // 最初に押されたときは，初期化が必要
            mRight.cursorX = cursorX;
            mRight.cursorY = cursorY;
        }

        // 差分計算
        int deltaX = cursorX - mRight.cursorX;
        int deltaY = cursorY - mRight.cursorY;

        // 更新
        mRight.cursorX = cursorX;
        mRight.cursorY = cursorY;

        // ズームスピード調整係数
        const float speed = 0.1f;

        // ズーム距離計算
        mDistance += ( deltaY ) * speed;
        
        // 0以下にならないようにガードする
        if ( mDistance < Math::F_Epsilon )
        { mDistance = Math::F_Epsilon; }
    }
    else if ( App::GetInstance()->IsMouseUp( App::MOUSE_BUTTON_RIGHT ) )
    {
        // ドラッグフラグOFF
        mRight.isDrag = false;
    }

    // 角度決め
    mPosition.x = sinf( mAngle.x ) * cosf( mAngle.y );
    mPosition.y = sinf( mAngle.y );
    mPosition.z = cosf( mAngle.x ) * cosf( mAngle.y );

    // 位置決定
    mPosition *= mDistance;

    // アップベクトル計算
    if ( mAngle.y >= ToRadian( 90.0f ) && mAngle.y <= ToRadian( 270.0f ) )
    { mUpVector = Vector3( 0.0f, -1.0f, 0.0f ); }
    else
    { mUpVector = Vector3( 0.0f, 1.0f, 0.0f ); }

    // ビュー行列計算
    mView = CreateLookAt( mPosition, mDirection, mUpVector );
}

///---------------------------------------------------------------------
///<summary>
///ファーストパーソンカメラとしてビュー行列を計算する
///</summary>
///---------------------------------------------------------------------
void Camera::ComputeFirstPersonCamera()
{
    /* NOT_IMPLEMENTED */
}


} // namespace Graphics
} // namespace Asura
