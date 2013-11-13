#include <avango/gua/viewer/Viewer.hpp>
#include <avango/gua/Types.hpp>

#include <avango/Base.h>
#include <avango/Application.h>
#include <avango/Logger.h>
#include <boost/bind.hpp>

#include <gua/events.hpp>
#include <chrono>
#include <Python.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::Viewer"));
}

AV_FC_DEFINE(av::gua::Viewer);

AV_FIELD_DEFINE(av::gua::SFViewer);
AV_FIELD_DEFINE(av::gua::MFViewer);

av::gua::Viewer::Viewer()
    : m_renderer(nullptr)
{
    AV_FC_ADD_FIELD(Pipelines, MFPipeline::ContainerType());
    AV_FC_ADD_FIELD(SceneGraphs, MFSceneGraph::ContainerType());
    AV_FC_ADD_FIELD(Physics, nullptr);

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
av::gua::Viewer::run() const {
  if (!m_renderer) {
    std::vector< ::gua::Pipeline*> pipes;

    for (auto pipe : Pipelines.getValue()) {
      pipes.push_back(pipe->getGuaPipeline());
    }
    m_renderer = new av::gua::Renderer(new ::gua::Renderer(pipes));
  }


  if (Physics.getValue().isValid()) {
    Physics.getValue()->State.setValue(static_cast<int>(av::gua::Physics::RunningState::RUNNING));
  }

  PyThreadState* save_state(PyEval_SaveThread());
  ::gua::events::MainLoop loop;

  ::gua::events::Ticker ticker(loop, 1.f/60.f);
  ticker.on_tick.connect([&]() {
    PyEval_RestoreThread(save_state);

    av::ApplicationInstance::get().evaluate();

    if (SceneGraphs.getValue().size() > 0) {
      std::vector<av::gua::SceneGraph const*> graphs;

      for (auto graph : SceneGraphs.getValue()) {
        graphs.push_back(reinterpret_cast<gua::SceneGraph*> (graph.getBasePtr()));
      }

      m_renderer->queue_draw(graphs);
    }

    if (Physics.getValue().isValid()) {
      Physics.getValue()->synchronize(true);
    }

    save_state = PyEval_SaveThread();

  });

  loop.start();

}
