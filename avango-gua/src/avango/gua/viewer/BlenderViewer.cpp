#include <Python.h> // has to be first in order to prevent warnings

#include <avango/gua/viewer/BlenderViewer.hpp>
#include <avango/gua/Types.hpp>

#include <avango/Base.h>
#include <avango/Application.h>
#include <avango/Logger.h>

#ifdef AVANGO_AWESOMIUM_SUPPORT
#include <gua/gui/Interface.hpp>
#endif

#include <boost/bind.hpp>

#include <chrono>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::BlenderViewer"));
}

AV_FC_DEFINE(av::gua::BlenderViewer);

AV_FIELD_DEFINE(av::gua::SFBlenderViewer);
AV_FIELD_DEFINE(av::gua::MFBlenderViewer);

av::gua::BlenderViewer::BlenderViewer()
    : m_renderer(new ::gua::Renderer)
{
    AV_FC_ADD_FIELD(SceneGraphs, MFSceneGraph::ContainerType());
    AV_FC_ADD_FIELD(Windows,     MFWindowBase::ContainerType());
#if defined(AVANGO_PHYSICS_SUPPORT)
    AV_FC_ADD_FIELD(Physics, nullptr);
#endif
}

void
av::gua::BlenderViewer::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::BlenderViewer, true);

        SFBlenderViewer::initClass("av::gua::SFBlenderViewer", "av::Field");
        MFBlenderViewer::initClass("av::gua::MFBlenderViewer", "av::Field");
    }
}

void
av::gua::BlenderViewer::frame() {

  av::ApplicationInstance::get().evaluate();

#ifdef AVANGO_AWESOMIUM_SUPPORT
  ::gua::Interface::instance()->update();
#endif

  if (SceneGraphs.getValue().size() > 0) {

    std::vector<av::gua::SceneGraph const*> graphs;

    for (auto graph : SceneGraphs.getValue()) {
      graphs.push_back(reinterpret_cast<av::gua::SceneGraph*> (graph.getBasePtr()));
    }

    m_renderer.queue_draw(graphs);
  }

#if defined(AVANGO_PHYSICS_SUPPORT)
  if (this->Physics.getValue().isValid()) {
    this->Physics.getValue()->synchronize(true);
  }
#endif

  for (auto& window: Windows.getValue()) {
    window->process_events();

    if(window->should_close()) {
      window->close();
    }
  }
}
