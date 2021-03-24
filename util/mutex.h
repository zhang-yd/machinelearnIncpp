
#ifndef _MUTEX_H_ 
#define _MUTEX_H_ 

#if defined(__linux__) || defined(__APPLE__)
#include <pthread.h> 
#endif 

#if defined(_WIN32)
#include <windows.h> 
#include <boost/thread.hpp> // waiting to be replicated 
#endif 


class Mutex
{
public:
    Mutex(); 
    ~Mutex(); 

    bool Lock(); 
    bool TryLock(); 
    bool UnLock(); 

private:
    Mutex(const Mutex&); 
    Mutex& operator=(const Mutex&); 

#if defined(__linux__) || defined(__APPLE__)
    pthread_mutex_t m_lock; 
    pthread_mutexattr_t m_attr; 
#endif 

#if defined(_WIN32)
    CRITICAL_SECTION m_CriticalSection;
#endif
}; 



class MutexGuard
{
public:
    MutexGuard(Mutex* mutex): m_mutex(mutex)
    {
        m_mutex->Lock(); 
    }

    ~MutexGuard()
    {
        if(m_mutex != NULL)
        {
            m_mutex->UnLock(); 
        }
    }

    void Unlock()
    {
        m_mutex->UnLock(); 
        m_mutex = NULL; 
    }

private:
    Mutex* m_mutex; 
}; 


class ReadWriteLock
{
public:
    ReadWriteLock(); 
    ~ReadWriteLock(); 

    void LockRead(); 
    void UnLockRead(); 
    void LockWrite(); 
    void UnLockWrite(); 

protected:
#if defined(_WIN32)
    boost::shared_mutex m_lock; 
#else
    pthread_rwlock_t m_lock; 
#endif 
}; 



#endif //_MUTEX_H_ 
