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

av::Link<av::gua::TexturedQuadPassDescription>
av::gua::PipelineDescription::add_textured_quad_pass()
{
    auto& pass(m_guaPipelineDescription->add_pass<::gua::TexturedQuadPassDescription>());

    auto desc(new av::Link<av::gua::TexturedQuadPassDescription>(
                        new av::gua::TexturedQuadPassDescription(
                            std::shared_ptr<::gua::TexturedQuadPassDescription>(&pass))));

    pass.set_user_data(desc);

    return *desc;
}

av::Link<av::gua::TexturedQuadPassDescription>
av::gua::PipelineDescription::get_textured_quad_pass()
{
    auto& pass(m_guaPipelineDescription->get_pass<::gua::TexturedQuadPassDescription>());

    if (pass.get_user_data()) {
      auto desc = *static_cast<av::Link<av::gua::TexturedQuadPassDescription>*>(pass.get_user_data());
      return desc;
    } else {
      auto desc(new av::Link<av::gua::TexturedQuadPassDescription>(
                        new av::gua::TexturedQuadPassDescription(
                            std::shared_ptr<::gua::TexturedQuadPassDescription>(&pass))));

      pass.set_user_data(desc);

      return *desc;
    }
}

std::vector<av::Link<av::gua::TexturedQuadPassDescription>>
av::gua::PipelineDescription::get_textured_quad_passes()
{
    auto passes(m_guaPipelineDescription->get_passes<::gua::TexturedQuadPassDescription>());
    std::vector<av::Link<av::gua::TexturedQuadPassDescription>> result;
    for (auto pass : passes) {
      if (pass->get_user_data()) {
        result.push_back(*static_cast<av::Link<av::gua::TexturedQuadPassDescription>*>(pass->get_user_data()));
      } else {
        auto desc(new av::Link<av::gua::TexturedQuadPassDescription>(
                        new av::gua::TexturedQuadPassDescription(
                            std::shared_ptr<::gua::TexturedQuadPassDescription>(pass))));

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

av::Link<av::gua::BBoxPassDescription>
av::gua::PipelineDescription::add_bbox_pass()
{
    auto& pass(m_guaPipelineDescription->add_pass<::gua::BBoxPassDescription>());

    auto desc(new av::Link<av::gua::BBoxPassDescription>(
                        new av::gua::BBoxPassDescription(
                            std::shared_ptr<::gua::BBoxPassDescription>(&pass))));

    pass.set_user_data(desc);

    return *desc;
}

av::Link<av::gua::BBoxPassDescription>
av::gua::PipelineDescription::get_bbox_pass()
{
    auto& pass(m_guaPipelineDescription->get_pass<::gua::BBoxPassDescription>());

    if (pass.get_user_data()) {
      auto desc = *static_cast<av::Link<av::gua::BBoxPassDescription>*>(pass.get_user_data());
      return desc;
    } else {
      auto desc(new av::Link<av::gua::BBoxPassDescription>(
                        new av::gua::BBoxPassDescription(
                            std::shared_ptr<::gua::BBoxPassDescription>(&pass))));

      pass.set_user_data(desc);

      return *desc;
    }
}

std::vector<av::Link<av::gua::BBoxPassDescription>>
av::gua::PipelineDescription::get_bbox_passes()
{
    auto passes(m_guaPipelineDescription->get_passes<::gua::BBoxPassDescription>());
    std::vector<av::Link<av::gua::BBoxPassDescription>> result;
    for (auto pass : passes) {
      if (pass->get_user_data()) {
        result.push_back(*static_cast<av::Link<av::gua::BBoxPassDescription>*>(pass->get_user_data()));
      } else {
        auto desc(new av::Link<av::gua::BBoxPassDescription>(
                        new av::gua::BBoxPassDescription(
                            std::shared_ptr<::gua::BBoxPassDescription>(pass))));

        pass->set_user_data(desc);
        result.push_back(*desc);
      }
    }

    return result;
}

av::Link<av::gua::FullscreenPassDescription>
av::gua::PipelineDescription::add_fullscreen_pass()
{
    auto& pass(m_guaPipelineDescription->add_pass<::gua::FullscreenPassDescription>());

    auto desc(new av::Link<av::gua::FullscreenPassDescription>(
                        new av::gua::FullscreenPassDescription(
                            std::shared_ptr<::gua::FullscreenPassDescription>(&pass))));

    pass.set_user_data(desc);

    return *desc;
}

av::Link<av::gua::FullscreenPassDescription>
av::gua::PipelineDescription::get_fullscreen_pass()
{
    auto& pass(m_guaPipelineDescription->get_pass<::gua::FullscreenPassDescription>());

    if (pass.get_user_data()) {
      auto desc = *static_cast<av::Link<av::gua::FullscreenPassDescription>*>(pass.get_user_data());
      return desc;
    } else {
      auto desc(new av::Link<av::gua::FullscreenPassDescription>(
                        new av::gua::FullscreenPassDescription(
                            std::shared_ptr<::gua::FullscreenPassDescription>(&pass))));

      pass.set_user_data(desc);

      return *desc;
    }
}

std::vector<av::Link<av::gua::FullscreenPassDescription>>
av::gua::PipelineDescription::get_fullscreen_passes()
{
    auto passes(m_guaPipelineDescription->get_passes<::gua::FullscreenPassDescription>());
    std::vector<av::Link<av::gua::FullscreenPassDescription>> result;
    for (auto pass : passes) {
      if (pass->get_user_data()) {
        result.push_back(*static_cast<av::Link<av::gua::FullscreenPassDescription>*>(pass->get_user_data()));
      } else {
        auto desc(new av::Link<av::gua::FullscreenPassDescription>(
                        new av::gua::FullscreenPassDescription(
                            std::shared_ptr<::gua::FullscreenPassDescription>(pass))));

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


