
#include "mutex.h" 

#if defined(__linux__) || defined(__APPLE__) 

Mutex::Mutex()
{
    pthread_mutexattr_init(&m_attr); 
    pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_RECURSIVE); 
    pthread_mutex_init(&m_lock, &m_attr); 
}

Mutex::~Mutex()
{
    pthread_mutex_destroy(&m_lock); 
    pthread_mutexattr_destroy(&m_attr); 
}

bool Mutex::Lock()
{
    return ( pthread_mutex_lock(&m_lock) == 0 ); 
}

bool Mutex::TryLock()
{
    return ( pthread_mutex_trylock(&m_lock) == 0 ); 
}

bool Mutex::UnLock()
{
    return ( pthread_mutex_unlock(&m_lock) == 0); 
}

// 

ReadWriteLock::ReadWriteLock()
{
    pthread_rwlock_init(&m_lock, NULL); 
}

ReadWriteLock::~ReadWriteLock()
{
    pthread_rwlock_destroy(&m_lock); 
}

void ReadWriteLock::LockRead()
{
    pthread_rwlock_rdlock(&m_lock); 
}

void ReadWriteLock::UnLockRead()
{
    pthread_rwlock_unlock(&m_lock); 
}

void ReadWriteLock::LockWrite()
{
    pthread_rwlock_wrlock(&m_lock); 
}

void ReadWriteLock::UnLockWrite()
{
    pthread_rwlock_unlock(&m_lock); 
}

#endif 

#if defined(_WIN32)


Mutex::Mutex()
{
    InitializeCriticalSection(&m_CriticalSection); 
}

Mutex::~Mutex()
{
    DeleteCriticalSection(&m_CriticalSection); 
}

bool Mutex::Lock()
{
    EnterCriticalSection(&m_CriticalSection); 
    return true; 
}

bool Mutex::TryLock()
{
    return ( TryEnterCriticalSection(&m_CriticalSection) ); 
}

bool Mutex::UnLock()
{
    LeaveCriticalSection(&m_CriticalSection); 
    return true; 
}

// 
ReadWriteLock::ReadWriteLock()
{   
}

ReadWriteLock::~ReadWriteLock()
{
}

void ReadWriteLock::LockRead()
{
    m_lock.lock_shared(); 
}

void ReadWriteLock::UnLockRead()
{
    m_lock.unlock_shared(); 
}

void ReadWriteLock::LockWrite()
{
    m_lock.lock(); 
}

void ReadWriteLock::UnLockWrite()
{
    m_lock.unlock(); 
}

#endif 



