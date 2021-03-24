
#ifndef _THREAD_H_ 
#define _THREAD_H_ 

#if defined(__linux__) || defined(__APPLE__)
#include <pthread.h> 

typedef void* DWORD; 
typedef pthread_t TID; 
#endif 

#if defined(_WIN32)
#include <windows.h>  

typedef unsigned long DWORD; 
typedef  DWORD TID; 
#endif 


class Thread
{
public:
    enum Priority
    {
        LOWEST, 
        BELOW_NORMAL, 
        NORMAL, 
    }; 
    typedef DWORD (*Func)(void *); 

private:
    struct ThreadParam
    {
        Func func; 
        void *param;
    }; 

public:
    Thread(); 
    ~Thread(); 

    bool Run(Func func, void *param, unsigned int stackSize = 0); 
    bool SetThreadPriority(const Priority& p); 
    bool Join(); 
    bool Terminate(); 
    void Detach(); 

    TID GetThreadID() const; 
    int GetThreadIDNumber() const; 

    inline static TID GetCurrentThreadID()
    {

    #if defined(__linux__) || defined(__APPLE__)
        return pthread_self(); 
    #endif 

    #if defined(_WIN32)
        return ::GetCurrentThreadId();     
    #endif 
    }

    inline static int GetCurrentThreadIDNumber() 
    {
    #if defined(_WIN32)
        return (int)(::GetCurrentThreadId());     
    #endif
        return 0; 
    }

    inline static bool IsSameThread(const TID tid1, const TID tid2)
    {
    #if defined(_WIN32)
        return tid1 == tid2;    
    #else 
        return (bool)(pthread_equal(tid1, tid2)); 
    #endif 
    }

private:
#if defined(__linux__) || defined(__APPLE__)
    static DWORD ThreadProc(void *param); 
    pthread_t m_PID;  
#endif 

#if defined(_WIN32)
    static DWORD CALLBACK ThreadProc(void *param); 
    HANDLE m_Thread; 
    DWORD m_ID; 
#endif 

    int m_nThreadId; 
}; 


#endif // _THREAD_H_
