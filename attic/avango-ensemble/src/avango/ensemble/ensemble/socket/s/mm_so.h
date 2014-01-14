#ifndef __MM_SO_H__
#define __MM_SO_H__

//#define LINKDLL 

#ifndef LINKDLL 
#ifdef _WIN32

#ifdef CE_MAKE_A_DLL
#define LINKDLL __declspec( dllexport)
#else
#define LINKDLL __declspec( dllimport)
#endif

#else
#define LINKDLL 
#endif 
#endif 

#endif  // __MM_SO_H__


