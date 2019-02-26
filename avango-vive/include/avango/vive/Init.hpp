#ifndef AVANGO_VIVE_INIT_HPP
#define AVANGO_VIVE_INIT_HPP

#include <avango/Typed.h>

#include <avango/vive/windows_specific.hpp>
#include <avango/vive/ViveWindow.hpp>

#include <avango/gua/Fields.hpp>

namespace av
{
namespace vive
{
/**
 * Initializes the Avango vive library.
 * Usage: \code av::vive::Init::initClass(); \endcode
 *
 * \ingroup av_vive
 */

class AV_VIVE_DLL Init
{
    AV_TYPED_DECLARE_ABSTRACT();
};
} // namespace vive
} // namespace av

#endif // AVANGO_VIVE_INIT_HPP
