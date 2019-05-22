#if !defined(AV_VIVE_WINDOWS_SPECIFIC_HPP)
#define AV_VIVE_WINDOWS_SPECIFIC_HPP

/**
 * \file
 * \ingroup av_gua
 */

#if defined(_MSC_VER)
#pragma warning(disable : 4251 4661)
#if defined(AV_VIVE_LIBRARY)
#define AV_VIVE_DLL __declspec(dllexport)
#else
#define AV_VIVE_DLL __declspec(dllimport)
#endif
#else
#define AV_VIVE_DLL
#endif

#endif // #if !defined(AV_VIVE_WINDOWS_SPECIFIC_HPP)
