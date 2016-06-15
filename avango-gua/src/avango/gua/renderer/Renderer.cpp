#include <avango/gua/renderer/Renderer.hpp>
#include <avango/gua/scenegraph/CameraNode.hpp>
#include <avango/gua/scenegraph/SceneGraph.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::Renderer"));
}

AV_FC_DEFINE(av::gua::Renderer);

AV_FIELD_DEFINE(av::gua::SFRenderer);
AV_FIELD_DEFINE(av::gua::MFRenderer);

av::gua::Renderer::Renderer(::gua::Renderer* guaRenderer)
    : m_guaRenderer(guaRenderer)
{}

//av::gua::Renderer::~Renderer()
//{}

void
av::gua::Renderer::queue_draw(std::vector<av::gua::SceneGraph const*> const& graphs) const
{
  std::vector< ::gua::SceneGraph const*> gua_graphs;
  for (auto graph : graphs) {
    gua_graphs.push_back(graph->getGuaSceneGraph());
  }

  m_guaRenderer->queue_draw(gua_graphs);
}

void
av::gua::Renderer::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::Renderer, true);

        SFRenderer::initClass("av::gua::SFRenderer", "av::Field");
        MFRenderer::initClass("av::gua::MFRenderer", "av::Field");
    }
}

::gua::Renderer*
av::gua::Renderer::getGuaRenderer() const
{
    return m_guaRenderer;
}


