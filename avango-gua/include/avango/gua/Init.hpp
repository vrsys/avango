#ifndef AVANGO_GUA_INIT_HPP
#define AVANGO_GUA_INIT_HPP

/**
 * \file
 * \ingroup av_osg
 */

#include <avango/Typed.h>
#include <avango/gua/windows_specific_gua.hpp>

//#include <avango/osg/Types.h>

namespace av
{
namespace gua
{
/**
 * Initializes the Avango guacamole library.
 * Usage: \code av::gua::Init::initClass(); \endcode
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL Init
{
    AV_TYPED_DECLARE_ABSTRACT();
};
} // namespace gua
} // namespace av

#endif // AVANGO_GUA_INIT_HPP
