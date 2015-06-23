#include <avango/gua/volume/Init.hpp>

#include <avango/Logger.h>

AV_TYPED_DEFINE_ABSTRACT(av::volume::Init);

/* static */ void
av::volume::Init::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::VolumeNode::initClass();
        av::gua::VolumePassDescription::initClass();
        av::gua::VolumeLoader::initClass();

      AV_TYPED_INIT_ABSTRACT(av::Type::badType(), "av::volume::Init", true);
    }
}
