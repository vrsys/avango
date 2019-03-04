#ifndef AVANGO_GUA_NURBS_INIT_HPP
#define AVANGO_GUA_NURBS_INIT_HPP

#include <avango/Typed.h>

#if defined(_MSC_VER)
#pragma warning(disable : 4251) // needs to have dll-interface to be used by clients of class
#pragma warning(disable : 4661) // no suitable definition provided for explicit template instantiation request

#if defined(AV_NURBS_LIBRARY)
#define AV_NURBS_DLL __declspec(dllexport)
#else
#define AV_NURBS_DLL __declspec(dllimport)
#endif
#else
#define AV_NURBS_DLL
#endif

namespace av
{
namespace gua
{
namespace nurbs
{
/**
 * Initializes the Avango guacamole library.
 * Usage: \code av::gua::Init::initClass(); \endcode
 *
 * \ingroup av_gua
 */
class AV_NURBS_DLL Init
{
    AV_TYPED_DECLARE_ABSTRACT();
};
} // namespace nurbs
} // namespace gua
} // namespace av

#endif // AVANGO_GUA_NURBS_INIT_HPP
