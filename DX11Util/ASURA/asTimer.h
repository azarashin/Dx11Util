//----------------------------------------------------------------------
// File : AsTimer.h
// Desc : Asura Timer Util
// Copyright(c) Project Asura All right reserved.
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// History
//----------------------------------------------------------------------
// <---> 2010/12/21 [Pocol] �V�K�쐬
// <End>

#ifndef __ASURA_TIMER_H__
#define __ASURA_TIMER_H__

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include <windows.h>
#include <cassert>
#include <fstream>

namespace Asura {

////////////////////////////////////////////////////////////////////////
// Timer
////////////////////////////////////////////////////////////////////////
class Timer 
{
    //------------------------------------------------------------------
    // list of friend classes
    //------------------------------------------------------------------
    /* NOTHING */

public:
    //------------------------------------------------------------------
    // public variables
    //------------------------------------------------------------------
    /* NOTHING */

    //------------------------------------------------------------------
    // public methods
    //------------------------------------------------------------------

    ///-----------------------------------------------------------------
    ///<summary>
    ///�������������s��
    ///</summary>
    ///-----------------------------------------------------------------
    Timer()
    {
        mIsStop = true;
        mStopTime = 0;
        mLastElapsedTime = 0;
        mBaseTime = 0;

        LARGE_INTEGER qwTicksPerSec = { 0 };
        QueryPerformanceFrequency( &qwTicksPerSec );
        mTicksPerSec = qwTicksPerSec.QuadPart;
    }

    ///-----------------------------------------------------------------
    ///<summary>
    ///�f�X�g���N�^
    ///</summary>
    ///-----------------------------------------------------------------
    ~Timer()
    {
        /* DO NOTHING */
    }

    ///-----------------------------------------------------------------
    ///<summary>
    ///�^�C�}�[�����Z�b�g����
    ///</summary>
    ///-----------------------------------------------------------------
    void Reset()
    {
        LARGE_INTEGER qwTime = GetAdjustedCurrentTime();

        mBaseTime = qwTime.QuadPart;
        mLastElapsedTime = qwTime.QuadPart;
        mStopTime = 0;
        mIsStop = false;
    }

    ///-----------------------------------------------------------------
    ///<summary>
    ///�^�C�}�[���J�n����
    ///</summary>
    ///-----------------------------------------------------------------
    void Start()
    {
        LARGE_INTEGER qwTime = { 0 };
        QueryPerformanceCounter( &qwTime );

        if ( mIsStop )
        {
            mBaseTime += qwTime.QuadPart - mStopTime;
        }
        mStopTime = 0;
        mLastElapsedTime = qwTime.QuadPart;
        mIsStop = false;
    }

    ///-----------------------------------------------------------------
    ///<summary>
    ///�^�C�}�[���~����
    ///</summary>
    ///-----------------------------------------------------------------
    void Stop()
    {
        if ( !mIsStop )
        {
            LARGE_INTEGER qwTime = { 0 };
            QueryPerformanceCounter( &qwTime );
            mStopTime = qwTime.QuadPart;
            mLastElapsedTime = qwTime.QuadPart;
            mIsStop = true;
        }
    }

    ///-----------------------------------------------------------------
    ///<summary>
    ///0.1�b�^�C�}�[��i�߂�
    ///</summary>
    ///-----------------------------------------------------------------
    void Advance()
    {
        mStopTime += mTicksPerSec / 10;
    }

    ///-----------------------------------------------------------------
    ///<summary>
    ///�V�X�e�����Ԃ��擾����
    ///</summary>
    ///<returns>�V�X�e�����Ԃ�ԋp����</returns>
    ///-----------------------------------------------------------------
    double GetAbsoluteTime()
    {
        LARGE_INTEGER qwTime = { 0 };
        QueryPerformanceCounter( &qwTime );
        double dTime = qwTime.QuadPart / (double)mTicksPerSec;
        return dTime;
    }

    ///-----------------------------------------------------------------
    ///<summary>
    ///���Ԃ��擾����
    ///</summary>
    ///<returns>�^�C�}�[���J�n����Ă���̎��Ԃ�ԋp����</returns>
    ///-----------------------------------------------------------------
    double GetTime()
    {
        LARGE_INTEGER qwTime = GetAdjustedCurrentTime();
        double dTime = (double)( qwTime.QuadPart - mBaseTime ) / (double)mTicksPerSec;
        return dTime;
    }

    ///-----------------------------------------------------------------
    ///<summary>
    ///�o�ߎ��Ԃ��擾����
    ///</summary>
    ///<returns>�ȑO�ɌĂяo������Ă���̌o�ߎ��Ԃ�ԋp����</returns>
    ///-----------------------------------------------------------------
    double GetElapsedTime()
    {
        LARGE_INTEGER qwTime = GetAdjustedCurrentTime();
        double dElapsedTime = (double)( qwTime.QuadPart - mLastElapsedTime ) / (double)mTicksPerSec;
		mLastElapsedTime = qwTime.QuadPart;

        if ( dElapsedTime < 0 )
            dElapsedTime = 0;

        return dElapsedTime;
    }

    ///-----------------------------------------------------------------
    ///<summary>
    ///���Ԃ̒l���擾����
    ///</summary>
    ///<param name="pTime">���Ԃ��i�[����ϐ��ւ̃|�C���^</param>
    ///<param name="pAbsoluteTime">�V�X�e�����Ԃ��i�[����ϐ��ւ̃|�C���^</param>
    ///<param name="pElapsedTime">�o�ߎ��Ԃ��i�[����ϐ��ւ̃|�C���^</param>
    ///-----------------------------------------------------------------
    void GetTimeValues( double *pTime, double *pAbsoluteTime, double *pElapsedTime )
    {
        assert( pTime && pAbsoluteTime && pElapsedTime );

        LARGE_INTEGER qwTime = GetAdjustedCurrentTime();
        double dElapsedTime = (double)( qwTime.QuadPart - mLastElapsedTime ) / (double)mTicksPerSec;
        mLastElapsedTime = qwTime.QuadPart;

        if( dElapsedTime < 0 )
            dElapsedTime = 0;

        (*pAbsoluteTime) = qwTime.QuadPart / (double)mTicksPerSec;
        (*pTime) = (qwTime.QuadPart - mBaseTime ) / (double)mTicksPerSec;
        (*pElapsedTime) = dElapsedTime;
    }

    ///-----------------------------------------------------------------
    ///<summary>
    ///�^�C�}�[����~���Ă��邩�m�F����
    ///</summary>
    ///<returns>�^�C�}���\��~���Ă�����true��ԋp</returns>
    ///-----------------------------------------------------------------
    bool IsStop()
    {
        return mIsStop;
    }

protected:
    //------------------------------------------------------------------
    // protected variables
    //------------------------------------------------------------------
    /* NOTHING */

    //------------------------------------------------------------------
    // protected methods
    //------------------------------------------------------------------
    /* NOTHING */

private:
    //------------------------------------------------------------------
    // private variables
    //------------------------------------------------------------------
    bool mIsQPF;
    bool mIsStop;
    LONGLONG mTicksPerSec;
    LONGLONG mStopTime;
    LONGLONG mLastElapsedTime;
    LONGLONG mBaseTime;

    //------------------------------------------------------------------
    // private methods
    //------------------------------------------------------------------
    LARGE_INTEGER GetAdjustedCurrentTime()
    {
        LARGE_INTEGER qwTime;

        if ( mStopTime != 0 )
        {
            qwTime.QuadPart = mStopTime;
        }
        else
        {
            QueryPerformanceCounter( &qwTime );
        }
        return qwTime;
    }
};

////////////////////////////////////////////////////////////////////////
// MeasureTimer class
////////////////////////////////////////////////////////////////////////
class MeasureTimer
{
    //------------------------------------------------------------------
    // list of friend classes
    //------------------------------------------------------------------
    /* NOTHING */

public:
    //------------------------------------------------------------------
    // public variables
    //------------------------------------------------------------------
    /* NOTHING */

    //------------------------------------------------------------------
    // public methods
    //------------------------------------------------------------------

    ///-----------------------------------------------------------------
    ///<summary>
    ///�R���X�g���N�^
    ///</summary>
    ///-----------------------------------------------------------------
    MeasureTimer()
    {
        QueryPerformanceFrequency( &mTicksPerSec );
    }

    ///-----------------------------------------------------------------
    ///<summary>
    ///�f�X�g���N�^
    ///</summary>
    ///-----------------------------------------------------------------
    ~MeasureTimer()
    {
        /* DO NOTHING */
    }

    ///-----------------------------------------------------------------
    ///<summary>
    ///���Ԍv�����J�n����
    ///</summary>
    ///-----------------------------------------------------------------
    void Start()
    {
        QueryPerformanceCounter( &mStartTime ); 
    }

    ///-----------------------------------------------------------------
    ///<summary>
    ///���Ԍv�����I������
    ///</summary>
    ///-----------------------------------------------------------------
    void End()
    {
        QueryPerformanceCounter( &mEndTime );
    }

    ///-----------------------------------------------------------------
    ///<summary>
    ///�o�ߎ��Ԃ��擾����
    ///</summary>
    ///<returns>�o�ߎ��Ԃ�b�P�ʂŕԋp����</returns>
    ///-----------------------------------------------------------------
    double GetElapsedTime()
    {
        return (double)( mEndTime.QuadPart - mStartTime.QuadPart ) / (double)mTicksPerSec.QuadPart;
    }

    ///-----------------------------------------------------------------
    ///<summary>
    ///�o�ߎ��Ԃ��擾����
    ///</summary>
    ///<returns>�o�ߎ��Ԃ��~���b�P�ʂŕԋp����</returns>
    ///-----------------------------------------------------------------
    double GetElapsedTimeMsc()
    {
        return GetElapsedTime() * 1000.0; 
    }

protected:
    //------------------------------------------------------------------
    // protected variables
    //------------------------------------------------------------------
    /* NOTHING */

    //------------------------------------------------------------------
    // protected methods
    //------------------------------------------------------------------
    /* NOTHING */

private:
    //------------------------------------------------------------------
    // private variables
    //------------------------------------------------------------------
    LARGE_INTEGER mStartTime;
    LARGE_INTEGER mEndTime;
    LARGE_INTEGER mTicksPerSec;

    //------------------------------------------------------------------
    // private methods
    //------------------------------------------------------------------
    /* NOTHING */
};


} // namespace Asura


#endif // __ASURA_TIMER_H__