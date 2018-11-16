#pragma once 
 
////////////////////////////////////////////////////////////////////// 
// INCLUDES 
////////////////////////////////////////////////////////////////////// 
 
#include <windows.h> 
#include <math.h> 
 
////////////////////////////////////////////////////////////////////// 
// CLASSES 
////////////////////////////////////////////////////////////////////// 
 
class CTimer 
{ 
private: 
    //performance timer variables 
    __int64       m_i64PerformanceTimerStart; 
    __int64       m_i64PerformanceTimerElapsed; 
 
    //multimedia timer variables 
    unsigned long m_ulMMTimerElapsed; 
    unsigned long m_ulMMTimerStart; 
 
    //general timer variables 
    __int64       m_i64Frequency; 
    float         m_fResolution; 
    bool          m_bPerformanceTimer; 
 
    //FPS variables 
    float m_fTime1; 
    float m_fTime2; 
    float m_fDiffTime; 
    float m_fFPS; 
    int m_iFramesElapsed; 
 
public: 
 
    //---------------------------------------------------------- 
    // Name:    CTimer::CTimer 
    // Desc:    Default constructor 
    // Args:    None 
    // Rets:    None 
    //---------------------------------------------------------- 
    CTimer( void );
 
    //---------------------------------------------------------- 
    // Name:    CTimer::CTimer 
    // Desc:    Default destructor 
    // Args:    None 
    // Rets:    None 
    //---------------------------------------------------------- 
    virtual ~CTimer( void );
 
    //---------------------------------------------------------- 
    // Name:    CTimer::Init - public 
    // Desc:    Initiate the timer for the program 
    // Args:    None 
    // Rets:    bool:   -true: using performance timer 
    //                  -false: using multimedia timer 
    //---------------------------------------------------------- 
    bool Init( void );
 
    //---------------------------------------------------------- 
    // Name:    CTimer::Update - public 
    // Desc:    Update the timer (perform FPS counter calculations) 
    // Args:    None 
    // Rets:    None 
    //---------------------------------------------------------- 
    void Update( void );
 
    //---------------------------------------------------------- 
    // Name:    CTimer::GetTime - public 
    // Desc:    Get the current time since the program started 
    // Args:    None 
    // Rets:    float: The time elapsed since the program started. 
    //---------------------------------------------------------- 
    float GetTime( void );
 
    //---------------------------------------------------------- 
    // Name:    CTimer::GetElapsedSeconds - public 
    // Desc:    Get the elapsed seconds since the last frame was drawn. 
    // Args:    elapsedFrames: 
    // Rets:    float: The time elapsed since the program started. 
    //---------------------------------------------------------- 
    float GetElapsedSeconds(unsigned long elapsedFrames = 1);
 
    //---------------------------------------------------------- 
    // Name:    CTimer::GetFPS - public 
    // Desc:    Get the current number of frames per second 
    // Args:    None 
    // Rets:    float: the number of frames per second 
    //---------------------------------------------------------- 
    inline float GetFPS( void );
}; 
 
