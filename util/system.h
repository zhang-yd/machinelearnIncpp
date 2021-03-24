
#ifndef _SYSTEM_H_ 
#define _SYSTEM_H_  


#if defined(_WIN32)
#include <windows.h>  
#endif 

#if defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#include <sys/time.h>
#include<sys/types.h>
#include <dirent.h>
#include <fnmatch.h>
#include<dirent.h>
#endif 

void PISleep(unsigned long timems); 

double PITime(); 

#endif // _SYSTEM_H_  

