#include <avango/gua/renderer/PipelineDescription.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::PipelineDescription"));
}

AV_FC_DEFINE(av::gua::PipelineDescription);

AV_FIELD_DEFINE(av::gua::SFPipelineDescription);
AV_FIELD_DEFINE(av::gua::MFPipelineDescription);

av::gua::PipelineDescription::PipelineDescription(
  std::shared_ptr< ::gua::PipelineDescription> const& guaPipelineDescription)
    : m_guaPipelineDescription(guaPipelineDescription)
{}

void
av::gua::PipelineDescription::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::PipelineDescription, true);

        SFPipelineDescription::initClass("av::gua::SFPipelineDescription", "av::Field");
        MFPipelineDescription::initClass("av::gua::MFPipelineDescription", "av::Field");
    }
}


av::gua::TriMeshPassDescription*
av::gua::PipelineDescription::add_tri_mesh_pass() const
{
    auto pass(m_guaPipelineDescription->add_pass<::gua::TriMeshPassDescription>());

    return new av::gua::TriMeshPassDescription(std::shared_ptr<::gua::TriMeshPassDescription>(&pass));
}

av::gua::BackgroundPassDescription*
av::gua::PipelineDescription::add_background_pass() const
{
    auto pass(m_guaPipelineDescription->add_pass<::gua::BackgroundPassDescription>());

    return new av::gua::BackgroundPassDescription(std::shared_ptr<::gua::BackgroundPassDescription>(&pass));
}



std::shared_ptr< ::gua::PipelineDescription> const&
av::gua::PipelineDescription::getGuaPipelineDescription() const
{
    return m_guaPipelineDescription;
}


