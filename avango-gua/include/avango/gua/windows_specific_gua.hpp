
#if !defined(AV_WINDOWS_SPECIFIC_GUA_H)
#define AV_WINDOWS_SPECIFIC_GUA_H

/**
 * \file
 * \ingroup av_gua
 */

#if defined(_MSC_VER)
#pragma warning(disable : 4251) // needs to have dll-interface to be used by clients of class
#pragma warning(disable : 4661) // no suitable definition provided for explicit template instantiation request

#if defined(AV_GUA_LIBRARY)
#define AV_GUA_DLL __declspec(dllexport)
#else
#define AV_GUA_DLL __declspec(dllimport)
#endif

#if defined(AV_SKELANIM_LIBRARY)
#define AV_GUA_ANIM_DLL __declspec(dllexport)
#else
#define AV_GUA_ANIM_DLL __declspec(dllimport)
#endif

#if defined(AV_LOD_LIBRARY)
#define AV_GUA_LOD_DLL __declspec(dllexport)
#else
#define AV_GUA_LOD_DLL __declspec(dllimport)
#endif

#if defined(AV_VOLUME_LIBRARY)
#define AV_GUA_VOLUME_DLL __declspec(dllexport)
#else
#define AV_GUA_VOLUME_DLL __declspec(dllimport)
#endif

#else

// unix
#define AV_GUA_DLL
#define AV_GUA_ANIM_DLL
#define AV_GUA_LOD_DLL
#define AV_GUA_VOLUME_DLL

#endif // #if defined(_MSC_VER)

#endif // #if !defined(AV_WINDOWS_SPECIFIC_GUA_H)
