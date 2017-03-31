#include <Python.h> // has to be first in order to prevent warnings

#include <avango/gua/viewer/Viewer.hpp>
#include <avango/gua/Types.hpp>

#include <avango/Base.h>
#include <avango/Application.h>
#include <avango/Logger.h>

#ifdef AVANGO_AWESOMIUM_SUPPORT
#include <gua/gui/Interface.hpp>
#endif

#include <functional>

#include <chrono>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::Viewer"));
}

AV_FC_DEFINE(av::gua::Viewer);

AV_FIELD_DEFINE(av::gua::SFViewer);
AV_FIELD_DEFINE(av::gua::MFViewer);

av::gua::Viewer::Viewer()
    : m_renderer(nullptr),
      m_loop(),
      m_ticker(m_loop, 1.f/60.f)
{
    AV_FC_ADD_FIELD(SceneGraphs, MFSceneGraph::ContainerType());
    AV_FC_ADD_FIELD(Windows,     MFWindowBase::ContainerType());
#if defined(AVANGO_PHYSICS_SUPPORT)
    AV_FC_ADD_FIELD(Physics, nullptr);
#endif

    AV_FC_ADD_ADAPTOR_FIELD(DesiredFPS,
        [this](av::SFFloat::GetValueEvent const& e) {
            *(e.getValuePtr()) = 1.f/m_ticker.get_tick_time();
        },
        [this](av::SFFloat::SetValueEvent const& e) {
            m_ticker.set_tick_time(1.f/e.getValue());
        }
    );

    AV_FC_ADD_ADAPTOR_FIELD(ApplicationFPS,
        [this](av::SFFloat::GetValueEvent const& e) {
          if (m_renderer) {
            *(e.getValuePtr()) = m_renderer->getGuaRenderer()->get_application_fps();
          }
        },
        [](av::SFFloat::SetValueEvent const&) {}
    );
}

av::gua::Viewer::~Viewer()
{
  if (m_renderer) {
    m_renderer->getGuaRenderer()->stop();
    delete m_renderer;
    m_renderer = nullptr;
  }
}

void
av::gua::Viewer::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::Viewer, true);

        SFViewer::initClass("av::gua::SFViewer", "av::Field");
        MFViewer::initClass("av::gua::MFViewer", "av::Field");
    }
}

void
av::gua::Viewer::frame() {

  if (!m_renderer) {
    m_renderer = new av::gua::Renderer(new ::gua::Renderer());
  }

#if 0
  for (auto& window: Windows.getValue()) {
    if(!window->is_open()) {
      window->open();
    }
  }
#endif

  av::ApplicationInstance::get().evaluate();

#ifdef AVANGO_AWESOMIUM_SUPPORT
  ::gua::Interface::instance()->update();
#endif

  if (SceneGraphs.getValue().size() > 0) {

    std::vector<av::gua::SceneGraph const*> graphs;

    for (auto graph : SceneGraphs.getValue()) {
      graphs.push_back(reinterpret_cast<av::gua::SceneGraph*> (graph.getBasePtr()));
    }

    m_renderer->queue_draw(graphs);
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

void
av::gua::Viewer::run() {

  ::gua::Logger::enable_debug = false;

  if (!m_renderer) {
    m_renderer = new av::gua::Renderer(new ::gua::Renderer());
  }


#if defined(AVANGO_PHYSICS_SUPPORT)
  if (Physics.getValue().isValid()) {
    Physics.getValue()->State.setValue(static_cast<int>(av::gua::Physics::RunningState::RUNNING));
  }
#endif

  //PyThreadState* save_state(PyEval_SaveThread());

  m_ticker.on_tick.connect([&,this]() {
    //PyEval_RestoreThread(save_state);

    frame();

    //save_state = PyEval_SaveThread();
  });

  m_loop.start();

  for (auto& window: Windows.getValue()) {
    window->close();
  }

}
