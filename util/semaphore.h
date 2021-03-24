
#ifndef _SEMAPHORE_H_ 
#define _SEMAPHORE_H_ 


#if defined(__linux__)  
#include <semaphore.h> 
#include <string>  
#endif 

#if defined(__APPLE__) 
#include <dispatch/dispatch.h> 
#endif 

#if defined(_WIN32)
#include <windows.h>  
#endif 


class Semaphore
{
public:
    enum 
    {
        Fail = -1, 
        Succeed = 0, 
        Timeout = 1, 
    }; 

public:
    Semaphore(const char* inName, int initCount = 0); 
    ~Semaphore(); 

    int TryDown(unsigined long timeout); 
    bool Down(); 
    void Up(); 

private:
    Semaphore(const Semaphore&); 
    Semaphore& operator=(const Semaphore&); 


#if defined(__linux__)
    sem_t m_sem; 
#endif 

#if defined(__APPLE__) 
    dispatch_semaphore_t m_sem_GCD; 
#endif 

#if defined(_WIN32)
    HANDLE m_Semaphore; 
#endif 

}; 

#endif //_SEMAPHORE_H_
