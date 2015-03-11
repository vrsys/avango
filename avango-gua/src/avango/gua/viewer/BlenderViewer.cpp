#include <Python.h> // has to be first in order to prevent warnings

#include <avango/gua/viewer/BlenderViewer.hpp>
#include <avango/gua/Types.hpp>

#include <avango/Base.h>
#include <avango/Application.h>
#include <avango/Logger.h>
#include <GL/gl.h>

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
    AV_FC_ADD_FIELD(Windows,     MFHeadlessSurface::ContainerType());
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
    auto im = window->getGuaWindow()->get_image();
    draw_image(im);
  }

  for (auto& window: Windows.getValue()) {
    window->process_events();

    if(window->should_close()) {
      window->close();
    }
  }
}

void
av::gua::BlenderViewer::draw_image(::gua::HeadlessSurface::Image const& im)
{
  glColor3f(1.0f, 1.0f, 1.0f);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  GLuint texid;
  glGenTextures(1, &texid);
  glBindTexture(GL_TEXTURE_2D, texid);
  //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, w, h, 0, GL_RGBA, GL_HALF_FLOAT, data_pointer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, im.width, im.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, im.data.data());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glEnable(GL_TEXTURE_2D);

  //if(draw_params.bind_display_space_shader_cb) {
  //        draw_params.bind_display_space_shader_cb();
  //}

  glPushMatrix();
  //glTranslatef(0.0f, float(dy), 0.0f);

  glBegin(GL_QUADS);

  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(0.0f, 0.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(float(im.width), 0.0f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(float(im.width), float(im.height));
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(0.0f, float(im.height));

  glEnd();

  glPopMatrix();

  //if(draw_params.unbind_display_space_shader_cb) {
  //        draw_params.unbind_display_space_shader_cb();
  //}

  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);
  glDeleteTextures(1, &texid);
}
