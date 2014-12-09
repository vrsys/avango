#include <avango/gua/renderer/TriMeshPassDescription.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::TriMeshPassDescription"));
}

AV_FC_DEFINE(av::gua::TriMeshPassDescription);

AV_FIELD_DEFINE(av::gua::SFTriMeshPassDescription);
AV_FIELD_DEFINE(av::gua::MFTriMeshPassDescription);

av::gua::TriMeshPassDescription::TriMeshPassDescription(
  std::shared_ptr< ::gua::TriMeshPassDescription> const& guaTriMeshPassDescription)
    : PipelinePassDescription(guaTriMeshPassDescription)
    , m_guaTriMeshPassDescription(guaTriMeshPassDescription)
{

}

void
av::gua::TriMeshPassDescription::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::PipelinePassDescription, av::gua::TriMeshPassDescription, true);

        SFTriMeshPassDescription::initClass("av::gua::SFTriMeshPassDescription", "av::Field");
        MFTriMeshPassDescription::initClass("av::gua::MFTriMeshPassDescription", "av::Field");
    }
}


std::shared_ptr< ::gua::TriMeshPassDescription> const&
av::gua::TriMeshPassDescription::getGuaTriMeshPassDescription() const
{
    return m_guaTriMeshPassDescription;
}


