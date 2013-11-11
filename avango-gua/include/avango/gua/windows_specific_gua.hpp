
#if !defined(AV_WINDOWS_SPECIFIC_GUA_H)
#define AV_WINDOWS_SPECIFIC_GUA_H

/**
 * \file
 * \ingroup av_gua
 */

#if defined(_MSC_VER)
  #if defined(AV_GUA_LIBRARY)
    #define AV_GUA_DLL __declspec( dllexport )
  #else
    #define AV_GUA_DLL __declspec( dllimport )
  #endif
#else
  #define AV_GUA_DLL
#endif // #if defined(_MSC_VER)

#endif // #if !defined(AV_WINDOWS_SPECIFIC_GUA_H)
