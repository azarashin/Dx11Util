//----------------------------------------------------------------------
// File : AsTimer.h
// Desc : Asura Timer Util
// Copyright(c) Project Asura All right reserved.
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// History
//----------------------------------------------------------------------
// <---> 2010/12/21 [Pocol] 新規作成
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
    ///初期化処理を行う
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
    ///デストラクタ
    ///</summary>
    ///-----------------------------------------------------------------
    ~Timer()
    {
        /* DO NOTHING */
    }

    ///-----------------------------------------------------------------
    ///<summary>
    ///タイマーをリセットする
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
    ///タイマーを開始する
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
    ///タイマーを停止する
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
    ///0.1秒タイマーを進める
    ///</summary>
    ///-----------------------------------------------------------------
    void Advance()
    {
        mStopTime += mTicksPerSec / 10;
    }

    ///-----------------------------------------------------------------
    ///<summary>
    ///システム時間を取得する
    ///</summary>
    ///<returns>システム時間を返却する</returns>
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
    ///時間を取得する
    ///</summary>
    ///<returns>タイマーが開始されてからの時間を返却する</returns>
    ///-----------------------------------------------------------------
    double GetTime()
    {
        LARGE_INTEGER qwTime = GetAdjustedCurrentTime();
        double dTime = (double)( qwTime.QuadPart - mBaseTime ) / (double)mTicksPerSec;
        return dTime;
    }

    ///-----------------------------------------------------------------
    ///<summary>
    ///経過時間を取得する
    ///</summary>
    ///<returns>以前に呼び出しされてからの経過時間を返却する</returns>
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
    ///時間の値を取得する
    ///</summary>
    ///<param name="pTime">時間を格納する変数へのポインタ</param>
    ///<param name="pAbsoluteTime">システム時間を格納する変数へのポインタ</param>
    ///<param name="pElapsedTime">経過時間を格納する変数へのポインタ</param>
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
    ///タイマーが停止しているか確認する
    ///</summary>
    ///<returns>タイマが―停止していたらtrueを返却</returns>
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
    ///コンストラクタ
    ///</summary>
    ///-----------------------------------------------------------------
    MeasureTimer()
    {
        QueryPerformanceFrequency( &mTicksPerSec );
    }

    ///-----------------------------------------------------------------
    ///<summary>
    ///デストラクタ
    ///</summary>
    ///-----------------------------------------------------------------
    ~MeasureTimer()
    {
        /* DO NOTHING */
    }

    ///-----------------------------------------------------------------
    ///<summary>
    ///時間計測を開始する
    ///</summary>
    ///-----------------------------------------------------------------
    void Start()
    {
        QueryPerformanceCounter( &mStartTime ); 
    }

    ///-----------------------------------------------------------------
    ///<summary>
    ///時間計測を終了する
    ///</summary>
    ///-----------------------------------------------------------------
    void End()
    {
        QueryPerformanceCounter( &mEndTime );
    }

    ///-----------------------------------------------------------------
    ///<summary>
    ///経過時間を取得する
    ///</summary>
    ///<returns>経過時間を秒単位で返却する</returns>
    ///-----------------------------------------------------------------
    double GetElapsedTime()
    {
        return (double)( mEndTime.QuadPart - mStartTime.QuadPart ) / (double)mTicksPerSec.QuadPart;
    }

    ///-----------------------------------------------------------------
    ///<summary>
    ///経過時間を取得する
    ///</summary>
    ///<returns>経過時間をミリ秒単位で返却する</returns>
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