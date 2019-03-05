#include <avango/Base.h>
#include <avango/Logger.h>
#include <avango/gua/virtual_texturing/VTBackend.hpp>

#ifdef AVANGO_VIRTUAL_TEXTURING_SUPPORT

namespace
{
av::Logger& logger(av::getLogger("av::gua::VTBackend"));
}

AV_FC_DEFINE(av::gua::VTBackend);

AV_FIELD_DEFINE(av::gua::SFVTBackend);
AV_FIELD_DEFINE(av::gua::MFVTBackend);

av::gua::VTBackend::VTBackend() {}
av::gua::VTBackend::~VTBackend() { ::gua::VTBackend::get_instance().stop_backend(); }

void av::gua::VTBackend::initClass()
{
    if(!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::VTBackend, true);

        SFVTBackend::initClass("av::gua::SFVTBackend", "av::Field");
        MFVTBackend::initClass("av::gua::MFVTBackend", "av::Field");
    }
}
const void av::gua::VTBackend::start_backend() const { ::gua::VTBackend::get_instance().start_backend(); }
const void av::gua::VTBackend::stop_backend() const { ::gua::VTBackend::get_instance().stop_backend(); }
const void av::gua::VTBackend::add_camera(av::gua::CameraNode const& camera) const { ::gua::VTBackend::get_instance().add_camera(camera.getGuaNode()); }

#endif