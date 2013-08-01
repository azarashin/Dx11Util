//----------------------------------------------------------------------
// File : AsCamera.cpp
// Desc : Asura Camera For Demo Application
// Copyright(c) Project Asura All right reserved.
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// History
//----------------------------------------------------------------------
// <---> 2011/04/16 [Pocol] �V�K�쐬
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
///�����ݒ菈��
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
///�f�X�g���N�^
///</summary>
///---------------------------------------------------------------------
Camera::~Camera()
{
    /* DO_NOTHING */
}

///---------------------------------------------------------------------
///<summary>
///�J�����^�C�v��ݒ肷��
///</summary>
///<param name="type">�J�����^�C�v</param>
///---------------------------------------------------------------------
void Camera::SetType( int type )
{
    mType = type;
}

///---------------------------------------------------------------------
///<summary>
///�J�����ʒu��ݒ肷��
///</summary>
///<param name="position">�J�����ʒu</param>
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
///�J�����̕�����ݒ肷��
///</summary>
///<param name="direction">�J�����̕�����ݒ肷��</param>
///---------------------------------------------------------------------
void Camera::SetDirection( Vector3 direction )
{
    mDirection = direction;
}

///---------------------------------------------------------------------
///<summary>
///�J�����̃A�b�v�x�N�g����ݒ肷��
///</summary>
///<param name="upvector">�J�����̃A�b�v�x�N�g��</param>
///---------------------------------------------------------------------
void Camera::SetUpVector( Vector3 upvector )
{
    mUpVector = upvector;
}

///---------------------------------------------------------------------
///<summary>
///�r���[�s����v�Z����
///</summary>
///---------------------------------------------------------------------
void Camera::Compute()
{
    switch( mType )
    {
        case CAMERA_TYPE_THIRD_PERSON:
            {
                // 3�l�̃J����
                ComputeThirdPersonCamera();
            }
            break;

        case CAMERA_TYPE_FIRST_PERSON:
            {
                // 1�l�̃J����
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
///�J�����^�C�v���擾����
///</summary>
///<returns>�J�����^�C�v��ԋp����</returns>
///---------------------------------------------------------------------
int Camera::GetType()
{
    return mType;
}

///---------------------------------------------------------------------
///<summary>
///�J�����ʒu���擾����
///</summary>
///<returns>�v�Z��̃J�����ʒu��ԋp����</returns>
///---------------------------------------------------------------------
Vector3 Camera::GetPosition()
{
    return mPosition;
}

///---------------------------------------------------------------------
///<summary>
///�J�����̕������擾����
///</summary>
///<returns>�v�Z��̃J����������ԋp����</returns>
///---------------------------------------------------------------------
Vector3 Camera::GetDirection()
{
    return mDirection;
}

///---------------------------------------------------------------------
///<summary>
///�J�����̃A�b�v�x�N�g�����擾����
///</summary>
///<returns>�v�Z��̃J�����̃A�b�v�x�N�g����ԋp����</returns>
///---------------------------------------------------------------------
Vector3 Camera::GetUpVector()
{
    return mUpVector;
}

///---------------------------------------------------------------------
///<summary>
///�J�����̃r���[�s����擾����
///</summary>
///<returns>�v�Z��̃J�����̃r���[�s���ԋp����</returns>
///---------------------------------------------------------------------
Matrix Camera::GetView()
{
    return mView;
}

///---------------------------------------------------------------------
///<summary>
///�T�[�h�p�[�\���J�����Ƃ��ăr���[�s����v�Z����
///</summary>
///---------------------------------------------------------------------
void Camera::ComputeThirdPersonCamera()
{
    // ��]
    if ( App::GetInstance()->IsMouseDown( App::MOUSE_BUTTON_LEFT ) )
    {
        // �����ꂽ���̃J�[�\���ʒu���擾����
        int cursorX = App::GetInstance()->GetCursorX();
        int cursorY = App::GetInstance()->GetCursorY();

        // �h���b�O���łȂ��ꍇ
        if ( !mLeft.isDrag )
        {
            // �h���b�O�t���OON
            mLeft.isDrag = true;

            // �ŏ��ɉ����ꂽ�Ƃ��́C���������K�v.
            mLeft.cursorX = cursorX;
            mLeft.cursorY = cursorY;
        }

        // �������v�Z
        int deltaX = cursorX - mLeft.cursorX;
        int deltaY = cursorY - mLeft.cursorY;

        // �X�V
        mLeft.cursorX = cursorX;
        mLeft.cursorY = cursorY;

        // �p�x�v�Z
        mAngle.x += ToRadian( static_cast< float >( deltaX ) );
        mAngle.y += ToRadian( static_cast< float >( deltaY ) );

        // 0~360�Ɏ��߂�
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
        // �h���b�O�t���OOFF
        mLeft.isDrag = false;
    }
    
    // �Y�[��
    if ( App::GetInstance()->IsMouseDown( App::MOUSE_BUTTON_RIGHT ) )
    {
        // �����ꂽ���̃J�[�\���ʒu���擾
        int cursorX = App::GetInstance()->GetCursorX();
        int cursorY = App::GetInstance()->GetCursorY();

        // �h���b�O���łȂ��ꍇ
        if ( !mRight.isDrag )
        {
            // �h���b�O�t���OON
            mRight.isDrag = true;

            // �ŏ��ɉ����ꂽ�Ƃ��́C���������K�v
            mRight.cursorX = cursorX;
            mRight.cursorY = cursorY;
        }

        // �����v�Z
        int deltaX = cursorX - mRight.cursorX;
        int deltaY = cursorY - mRight.cursorY;

        // �X�V
        mRight.cursorX = cursorX;
        mRight.cursorY = cursorY;

        // �Y�[���X�s�[�h�����W��
        const float speed = 0.1f;

        // �Y�[�������v�Z
        mDistance += ( deltaY ) * speed;
        
        // 0�ȉ��ɂȂ�Ȃ��悤�ɃK�[�h����
        if ( mDistance < Math::F_Epsilon )
        { mDistance = Math::F_Epsilon; }
    }
    else if ( App::GetInstance()->IsMouseUp( App::MOUSE_BUTTON_RIGHT ) )
    {
        // �h���b�O�t���OOFF
        mRight.isDrag = false;
    }

    // �p�x����
    mPosition.x = sinf( mAngle.x ) * cosf( mAngle.y );
    mPosition.y = sinf( mAngle.y );
    mPosition.z = cosf( mAngle.x ) * cosf( mAngle.y );

    // �ʒu����
    mPosition *= mDistance;

    // �A�b�v�x�N�g���v�Z
    if ( mAngle.y >= ToRadian( 90.0f ) && mAngle.y <= ToRadian( 270.0f ) )
    { mUpVector = Vector3( 0.0f, -1.0f, 0.0f ); }
    else
    { mUpVector = Vector3( 0.0f, 1.0f, 0.0f ); }

    // �r���[�s��v�Z
    mView = CreateLookAt( mPosition, mDirection, mUpVector );
}

///---------------------------------------------------------------------
///<summary>
///�t�@�[�X�g�p�[�\���J�����Ƃ��ăr���[�s����v�Z����
///</summary>
///---------------------------------------------------------------------
void Camera::ComputeFirstPersonCamera()
{
    /* NOT_IMPLEMENTED */
}


} // namespace Graphics
} // namespace Asura
