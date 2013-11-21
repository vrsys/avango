
#if !defined(AV_OCULUS_WINDOWS_SPECIFIC_HPP)
#define AV_OCULUS_WINDOWS_SPECIFIC_HPP

/**
 * \file
 * \ingroup av_gua
 */

#if defined(_MSC_VER)
#pragma warning(disable:4251 4661)
  #if defined(AV_OCULUS_LIBRARY)
    #define AV_OCULUS_DLL __declspec( dllexport )
  #else
    #define AV_OCULUS_DLL __declspec( dllimport )
  #endif
#else
  #define AV_OCULUS_DLL
#endif

#endif // #if !defined(AV_OCULUS_WINDOWS_SPECIFIC_HPP)
