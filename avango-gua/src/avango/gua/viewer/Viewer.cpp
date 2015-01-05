#include <Python.h> // has to be first in order to prevent warnings

#include <avango/gua/viewer/Viewer.hpp>
#include <avango/gua/Types.hpp>

#include <avango/Base.h>
#include <avango/Application.h>
#include <avango/Logger.h>

#include <gua/gui/Interface.hpp>

#include <boost/bind.hpp>

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
    AV_FC_ADD_FIELD(CameraNodes, MFCameraNode::ContainerType());
    AV_FC_ADD_FIELD(SceneGraphs, MFSceneGraph::ContainerType());
    AV_FC_ADD_FIELD(Window, nullptr);
#if defined(AVANGO_PHYSICS_SUPPORT)
    AV_FC_ADD_FIELD(Physics, nullptr);
#endif

    AV_FC_ADD_ADAPTOR_FIELD(DesiredFPS,
                    boost::bind(&Viewer::getDesiredFPSCB, this, _1),
                    boost::bind(&Viewer::setDesiredFPSCB, this, _1));
}


//av::gua::Viewer::~Viewer()
//{}

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
av::gua::Viewer::getDesiredFPSCB(const av::SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = 1.f/m_ticker.get_tick_time();
}

void
av::gua::Viewer::setDesiredFPSCB(const av::SFFloat::SetValueEvent& event)
{
  m_ticker.set_tick_time(1.f/event.getValue());
}

void
av::gua::Viewer::run() const {

  // ::gua::Logger::enable_debug = false;

  if (!m_renderer) {
    m_renderer = new av::gua::Renderer(new ::gua::Renderer());
  }


#if defined(AVANGO_PHYSICS_SUPPORT)
  if (Physics.getValue().isValid()) {
    Physics.getValue()->State.setValue(static_cast<int>(av::gua::Physics::RunningState::RUNNING));
  }
#endif

  PyThreadState* save_state(PyEval_SaveThread());

  m_ticker.on_tick.connect([&,this]() {
    PyEval_RestoreThread(save_state);

    if (Window.getValue().isValid() && !Window.getValue()->is_open()) {
      Window.getValue()->open();
    }

    av::ApplicationInstance::get().evaluate();

    ::gua::Interface::instance()->update();

    if (SceneGraphs.getValue().size() > 0 && CameraNodes.getValue().size() > 0) {

      std::vector<av::gua::CameraNode const*> cams;

      for (auto cam : CameraNodes.getValue()) {
        cams.push_back(reinterpret_cast<av::gua::CameraNode*> (cam.getBasePtr()));
      }

      std::vector<av::gua::SceneGraph const*> graphs;

      for (auto graph : SceneGraphs.getValue()) {
        graphs.push_back(reinterpret_cast<av::gua::SceneGraph*> (graph.getBasePtr()));
      }

      m_renderer->queue_draw(graphs, cams);
    }

#if defined(AVANGO_PHYSICS_SUPPORT)
    if (this->Physics.getValue().isValid()) {
      this->Physics.getValue()->synchronize(true);
    }
#endif

    if (Window.getValue().isValid()) {
      Window.getValue()->process_events();

      if (Window.getValue()->should_close()) {
        Window.getValue()->close();
      }
    }

    save_state = PyEval_SaveThread();

  });

  m_loop.start();

 if (Window.getValue().isValid() && Window.getValue()->is_open()) {
    Window.getValue()->close();
 }

}
