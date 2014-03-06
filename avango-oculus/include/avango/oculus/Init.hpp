#ifndef AVANGO_OCULUS_INIT_HPP
#define AVANGO_OCULUS_INIT_HPP

#include <avango/Typed.h>

#include <avango/oculus/windows_specific.hpp>
#include <avango/oculus/OculusWindow.hpp>

#include <avango/gua/Fields.hpp>

namespace av
{
  namespace oculus
  {
    /**
     * Initializes the Avango oculus library.
     * Usage: \code av::oculus::Init::initClass(); \endcode
     *
     * \ingroup av_oculus
     */

    class AV_OCULUS_DLL Init
    {
      AV_TYPED_DECLARE_ABSTRACT();
    };
  }
}

#endif //AVANGO_OCULUS_INIT_HPP
