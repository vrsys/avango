#ifndef AVANGO_VOLUME_INIT_HPP
#define AVANGO_VOLUME_INIT_HPP

#include <avango/Typed.h>

#include <avango/gua/volume/VolumeLoader.hpp>
#include <avango/gua/volume/VolumeNode.hpp>
#include <avango/gua/volume/VolumePassDescription.hpp>
#include <avango/gua/volume/windows_specific.hpp>
#include <avango/gua/Fields.hpp>

namespace av
{
  namespace volume
  {
    /**
     * Initializes the Avango volume library.
     * Usage: \code av::volume::Init::initClass(); \endcode
     *
     * \ingroup av_volume
     */

    class AV_VOLUME_DLL Init
    {
      AV_TYPED_DECLARE_ABSTRACT();
    };
  }
}

#endif //AVANGO_VOLUME_INIT_HPP
