
#include <iostream>

#include "system.h" 


void PISleep(unsigned long timems)
{
#if defined(__linux__)  || defined(__APPLE__) 
    usleep((timems << 10) - (timems << 4) - (timems << 3)); 
#endif 

#if defined(_WIN32) 
    Sleep(timems); 
#endif 
}

double PITime()
{
#if defined(__linux__) || defined(__APPLE__) 
    static bool hasInit = false; 
    static timeval beginTime; 
    struct timezone tz = {0, 0}; 
    if(!hasInit)
    {
        gettimeofday(&beginTime, &tz);
        hasInit = true;     
    }
    timeval time; 
    gettimeofday(&time, &tz); 
    return ((time.tv_sec - beginTime.tv_sec)*1000.0 + time.tv_usec / 1000.0); 

#endif 

#if defined(_WIN32)
    static bool hasInit = false; 
    static bool useHighReslution = true; 
    static double freqUint; 
    static LARGE_INTEGER beginTime; 
   
    if(!hasInit)
    {
        LARGE_INTEGER val; 
        if(QueryPerformanceFrequency(&val) == 0)
        {
            useHighReslution = false; 
        }
        else
        {
            freqUint = double(1000.0 / val.QuadPart); 
            QueryPerformanceCounter(&beginTime); 
        }
        hasInit = true; 
    }
    if(!useHighReslution)
    {
        return GetTickCount(); 
    }

    LARGE_INTEGER now; 
    QueryPerformanceCounter(&now); 
    return (now.QuadPart - beginTime.QuadPart) * freqUint; 
#endif 
}


