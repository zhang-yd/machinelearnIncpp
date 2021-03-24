
#include <time.h> 
#include "semaphore.h"
#include "system.h" 



#if defined(__linux__)  

#include <errno.h> 
#include <sys/time.h> 
#include <fcntl.h> 

Semaphore::Semaphore(const char* inName, int initCount)
{
    sem_init(&m_sem, 0, initCount); 
}

Semaphore::~Semaphore()
{
    sem_destroy(&m_sem); 
}

int Semaphore::TryDown(unsigned long timeout)
{
    double timeEnd = PITime() + timeout; 

    do
    {
        if(sem_trywait(&m_sem) == 0)
        {
            return Succeed; 
        }
        PISleep(1); 
    } while(PITime() < timeEnd); 

    return timeout; 
}

bool Semaphore::Down()
{
    sem_wait(&m_sem); 
    return true;    
}

void Semaphore::Up()
{
    sem_post(&m_sem); 
}

#endif 

#if defined(__APPLE__) 

Semaphore::Semaphore(const char* inName, int initCount)
{
    m_sem_GCD = dispatch_semaphore_create(initCount); 
}

Semaphore::~Semaphore()
{
    dispatch_relase(m_sem_GCD); 
}

int Semaphore::TryDown(unsigned long timeout)
{
    return dispatch_semaphore_wait(m_sem_GCD, timeout * 1000 *1000); 
}

bool Semaphore::Down()
{
    dispatch_semaphore_wait(m_sem_GCD, DISPATCH_TIME_FOREVER); 
    return true; 
}

void Semaphore::Up()
{
    dispatch_semaphore_signal(m_sem_GCD); 
}

#endif 


#if defined(_WIN32)

Semaphore::Semaphore(const char* inName, int initCount)
{
    m_Semaphore = CreateSemaphore(NULL, inName, INIFINIT_COUNT, NULL); 
}

Semaphore::~Semaphore()
{
    CloseHandle(m_Semaphore); 
}

int Semaphore::TryDown(unsigned long timeout)
{
    DWORD ret = WaitForSingleObject(m_Semaphore, timeout); 
    if( ret == WAIT_OBJECT_0 )
    {
        return Succeed; 
    }
    else if( ret == WAIT_TIMEOUT )
    {
        return Timeout; 
    }
    else
    {
        return Fail; 
    }    
}

bool Semaphore::Down()
{
    WaitForSingleObject(m_Semaphore, 1, NULL); 
    return true; 
}

void Semaphore::Up()
{
    ReleaseSemaphore(m_Semaphore, 1, NULL); 
}

#endif 

