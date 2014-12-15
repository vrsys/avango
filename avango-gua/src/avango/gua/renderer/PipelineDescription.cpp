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


av::Link<av::gua::TriMeshPassDescription>
av::gua::PipelineDescription::add_tri_mesh_pass()
{
    auto& pass(m_guaPipelineDescription->add_pass<::gua::TriMeshPassDescription>());

    auto desc(new av::Link<av::gua::TriMeshPassDescription>(
                        new av::gua::TriMeshPassDescription(
                            std::shared_ptr<::gua::TriMeshPassDescription>(&pass))));

    pass.set_user_data(desc);

    return *desc;
}

av::Link<av::gua::TriMeshPassDescription>
av::gua::PipelineDescription::get_tri_mesh_pass()
{
    auto& pass(m_guaPipelineDescription->get_pass<::gua::TriMeshPassDescription>());

    if (pass.get_user_data()) {
      auto desc = *static_cast<av::Link<av::gua::TriMeshPassDescription>*>(pass.get_user_data());
      return desc;
    } else {
      auto desc(new av::Link<av::gua::TriMeshPassDescription>(
                        new av::gua::TriMeshPassDescription(
                            std::shared_ptr<::gua::TriMeshPassDescription>(&pass))));

      pass.set_user_data(desc);

      return *desc;
    }
}

std::vector<av::Link<av::gua::TriMeshPassDescription>>
av::gua::PipelineDescription::get_tri_mesh_passes()
{
    auto passes(m_guaPipelineDescription->get_passes<::gua::TriMeshPassDescription>());
    std::vector<av::Link<av::gua::TriMeshPassDescription>> result;
    for (auto pass : passes) {
      if (pass->get_user_data()) {
        result.push_back(*static_cast<av::Link<av::gua::TriMeshPassDescription>*>(pass->get_user_data()));
      } else {
        auto desc(new av::Link<av::gua::TriMeshPassDescription>(
                        new av::gua::TriMeshPassDescription(
                            std::shared_ptr<::gua::TriMeshPassDescription>(pass))));

        pass->set_user_data(desc);
        result.push_back(*desc);
      }
    }

    return result;
}

av::Link<av::gua::BackgroundPassDescription>
av::gua::PipelineDescription::add_background_pass()
{
    auto& pass(m_guaPipelineDescription->add_pass<::gua::BackgroundPassDescription>());

    auto desc(new av::Link<av::gua::BackgroundPassDescription>(
                        new av::gua::BackgroundPassDescription(
                            std::shared_ptr<::gua::BackgroundPassDescription>(&pass))));

    pass.set_user_data(desc);

    return *desc;
}

av::Link<av::gua::BackgroundPassDescription>
av::gua::PipelineDescription::get_background_pass()
{
    auto& pass(m_guaPipelineDescription->get_pass<::gua::BackgroundPassDescription>());

    if (pass.get_user_data()) {
      auto desc = *static_cast<av::Link<av::gua::BackgroundPassDescription>*>(pass.get_user_data());
      return desc;
    } else {
      auto desc(new av::Link<av::gua::BackgroundPassDescription>(
                        new av::gua::BackgroundPassDescription(
                            std::shared_ptr<::gua::BackgroundPassDescription>(&pass))));

      pass.set_user_data(desc);

      return *desc;
    }
}

std::vector<av::Link<av::gua::BackgroundPassDescription>>
av::gua::PipelineDescription::get_background_passes()
{
    auto passes(m_guaPipelineDescription->get_passes<::gua::BackgroundPassDescription>());
    std::vector<av::Link<av::gua::BackgroundPassDescription>> result;
    for (auto pass : passes) {
      if (pass->get_user_data()) {
        result.push_back(*static_cast<av::Link<av::gua::BackgroundPassDescription>*>(pass->get_user_data()));
      } else {
        auto desc(new av::Link<av::gua::BackgroundPassDescription>(
                        new av::gua::BackgroundPassDescription(
                            std::shared_ptr<::gua::BackgroundPassDescription>(pass))));

        pass->set_user_data(desc);
        result.push_back(*desc);
      }
    }

    return result;
}


std::shared_ptr< ::gua::PipelineDescription> const&
av::gua::PipelineDescription::getGuaPipelineDescription() const
{
    return m_guaPipelineDescription;
}


