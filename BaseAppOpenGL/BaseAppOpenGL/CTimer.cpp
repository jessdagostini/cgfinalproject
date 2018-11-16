#include "CTimer.h"


//---------------------------------------------------------- 
    // Name:    CTimer::CTimer 
    // Desc:    Default constructor 
    // Args:    None 
    // Rets:    None 
    //---------------------------------------------------------- 
    CTimer::CTimer( void ) 
        : m_fFPS(0.0f), m_fTime1(0.0f), m_fTime2(0.0f), m_fDiffTime(0.0f), m_iFramesElapsed(0) 
    {   } 
 
    //---------------------------------------------------------- 
    // Name:    CTimer::CTimer 
    // Desc:    Default destructor 
    // Args:    None 
    // Rets:    None 
    //---------------------------------------------------------- 
    CTimer::~CTimer( void ) 
    {   } 
 
    //---------------------------------------------------------- 
    // Name:    CTimer::Init - public 
    // Desc:    Initiate the timer for the program 
    // Args:    None 
    // Rets:    bool:   -true: using performance timer 
    //                  -false: using multimedia timer 
    //---------------------------------------------------------- 
    bool CTimer::Init( void ) 
    { 
        //check to see if we are going to be using the performance counter 
        if( QueryPerformanceFrequency( ( LARGE_INTEGER* )&m_i64Frequency ) ) 
        { 
            //we are able to use the performance timer 
            m_bPerformanceTimer= true; 
 
            //get the current time and store it in m_i64PerformanceTimerStart 
            QueryPerformanceCounter( ( LARGE_INTEGER* )&m_i64PerformanceTimerStart ); 
 
            //calculate the timer resolution 
            m_fResolution= ( float )( ( ( double )1.0f )/( ( double )m_i64Frequency ) ); 
 
            //initialize the elapsed time variable 
            m_i64PerformanceTimerElapsed= m_i64PerformanceTimerStart; 
        } 
 
        //we cannot use the performence counter, so we'll use the multimedia counter 
        else 
        { 
            //we're using the multimedia counter 
            m_bPerformanceTimer= false; 
 
            m_ulMMTimerStart   = timeGetTime( );    //record the time the program started 
            m_ulMMTimerElapsed = m_ulMMTimerStart;  //initialize the elapsed time variable 
            m_fResolution      = 1.0f/1000.0f; 
            m_i64Frequency     = 1000; 
        } 
 
        return m_bPerformanceTimer; 
    } 
 
    //---------------------------------------------------------- 
    // Name:    CTimer::Update - public 
    // Desc:    Update the timer (perform FPS counter calculations) 
    // Args:    None 
    // Rets:    None 
    //---------------------------------------------------------- 
    void CTimer::Update( void ) 
    { 
        //increase the number of frames that have passed 
        m_iFramesElapsed++; 
 
        if ( m_iFramesElapsed % 5 == 1 ) 
          m_fTime1 = GetTime( )/1000; 
 
        else if ( m_iFramesElapsed % 5 == 0 )  
        { 
            m_fTime1 = m_fTime2; 
            m_fTime2   = GetTime( )/1000; 
            m_fDiffTime= ( float )fabs( m_fTime2-m_fTime1 );       
        }   
 
        m_fFPS= 5/( m_fDiffTime ); 
 
 
 
        /*m_fTime2   = GetTime( )/1000; 
        m_fDiffTime= ( float )fabs( m_fTime2-m_fTime1 ); 
        if (m_fDiffTime > 1.0f) 
        { 
            m_fTime1 = m_fTime2; 
            m_fFPS= m_iFramesElapsed / ( m_fDiffTime ); 
            m_iFramesElapsed = 0; 
        } 
        */ 
    } 
 
    //---------------------------------------------------------- 
    // Name:    CTimer::GetTime - public 
    // Desc:    Get the current time since the program started 
    // Args:    None 
    // Rets:    float: The time elapsed since the program started. 
    //---------------------------------------------------------- 
    float CTimer::GetTime( void ) 
    { 
         __int64 i64Time; 
 
        //check to see if we are using the performance counter 
        if( m_bPerformanceTimer ) 
        { 
            //get the current performance time 
            QueryPerformanceCounter( ( LARGE_INTEGER* )&i64Time ); 
 
            //return the time since the program started 
            return ( ( float )( i64Time - m_i64PerformanceTimerStart )*m_fResolution )*1000.0f; 
        } 
 
        //we are using the multimedia counter 
        else 
        { 
            //return the time since the program started 
            return ( ( float )( timeGetTime( ) - m_ulMMTimerStart )*m_fResolution )*1000.0f; 
        } 
    } 
 
    //---------------------------------------------------------- 
    // Name:    CTimer::GetElapsedSeconds - public 
    // Desc:    Get the elapsed seconds since the last frame was drawn. 
    // Args:    elapsedFrames: 
    // Rets:    float: The time elapsed since the program started. 
    //---------------------------------------------------------- 
    float CTimer::GetElapsedSeconds(unsigned long elapsedFrames) 
    {   return m_fDiffTime;     } 
 
    //---------------------------------------------------------- 
    // Name:    CTimer::GetFPS - public 
    // Desc:    Get the current number of frames per second 
    // Args:    None 
    // Rets:    float: the number of frames per second 
    //---------------------------------------------------------- 
    inline float CTimer::GetFPS( void ) 
    {   return m_fFPS;  } 
