#include <Python.h>  // has to be first in order to prevent warnings

#include <avango/gua/viewer/BlenderViewer.hpp>
#include <avango/gua/Types.hpp>

#include <avango/Base.h>
#include <avango/Application.h>
#include <avango/Logger.h>

#include <gua/scenegraph.hpp>
#include <gua/renderer/GBuffer.hpp>
#include <gua/renderer/Pipeline.hpp>
#include <gua/databases/WindowDatabase.hpp>
#include <gua/node/CameraNode.hpp>
#include <gua/memory.hpp>

#include <scm/gl_core/render_device/opengl/util/data_format_helper.h>

#include <GL/gl.h>

#ifdef AVANGO_AWESOMIUM_SUPPORT
#include <gua/gui/Interface.hpp>
#endif

#include <boost/make_shared.hpp>
#include <boost/bind.hpp>

#include <cstdint>
#include <chrono>
#include <algorithm>

namespace {

av::Logger& logger(av::getLogger("av::gua::BlenderViewer"));

av::gua::BlenderViewer::Image screenshot(
    scm::gl::render_context_ptr const& ctx,
    scm::gl::texture_2d_ptr const& tex ) {

  av::gua::BlenderViewer::Image img;

  if (!tex)
    return img;

  img.width = tex->descriptor()._size.x;
  img.height = tex->descriptor()._size.y;
  img.bpp = scm::gl::bit_per_pixel(tex->format());
  img.gl_type = scm::gl::util::gl_base_type(tex->format());
  img.gl_internal_format = scm::gl::util::gl_internal_format(tex->format());
  img.gl_base_format = scm::gl::util::gl_base_format(tex->format());

  int n = img.width * img.height * scm::gl::size_of_format(tex->format());
  img.data = std::vector<char>(n);

  ctx->retrieve_texture_data(tex, 0, img.data.data());

  return img;
}

void draw_image(av::gua::BlenderViewer::Image const& im, GLuint texid, bool resize) {
  glColor3f(1.0f, 1.0f, 1.0f);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, texid);
  //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, w, h, 0, GL_RGBA,
  //GL_HALF_FLOAT, data_pointer);
  if (resize) {
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 im.width,
                 im.height,
                 0,
                 im.gl_base_format,
                 im.gl_type,
                 im.data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  } else {
    glTexSubImage2D(GL_TEXTURE_2D,
      0, 0, 0,
      im.width,
      im.height,
      im.gl_base_format,
      im.gl_type,
      im.data.data());
  }

  glEnable(GL_TEXTURE_2D);

  //if(draw_params.bind_display_space_shader_cb) {
  //        draw_params.bind_display_space_shader_cb();
  //}

  auto width(im.width);
  auto height(im.height);

  glPushMatrix();

  glBegin(GL_QUADS);

  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(0.0f, 0.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(float(width), 0.0f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(float(width), float(height));
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(0.0f, float(height));

  glEnd();

  glPopMatrix();

  //if(draw_params.unbind_display_space_shader_cb) {
  //        draw_params.unbind_display_space_shader_cb();
  //}

  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);
}

}

AV_FC_DEFINE(av::gua::BlenderViewer);

AV_FIELD_DEFINE(av::gua::SFBlenderViewer);
AV_FIELD_DEFINE(av::gua::MFBlenderViewer);

av::gua::BlenderViewer::BlenderViewer()
    : m_mutex(),
      m_condition(),
      m_gua_graphs(),
      m_current_engine(""),
      m_camera_name(""),
      m_image(),
      m_ready(false),
      m_processed(false),
      m_done(false),
      m_worker(std::bind(&av::gua::BlenderViewer::render_thread, this)),
      m_engines(),
      m_unregister_queue() {
  AV_FC_ADD_FIELD(SceneGraphs, MFSceneGraph::ContainerType());
  AV_FC_ADD_FIELD(Window, SFHeadlessSurface::ValueType());
#if defined(AVANGO_PHYSICS_SUPPORT)
  AV_FC_ADD_FIELD(Physics, nullptr);
#endif
}

av::gua::BlenderViewer::~BlenderViewer() {
  // stopping thread
  m_done = true;
  // send data to the worker thread
  {
    std::lock_guard<std::mutex> lk(m_mutex);
    m_ready = true;
  }
  m_condition.notify_one();

  // wait for the worker
  {
    std::unique_lock<std::mutex> lk(m_mutex);
    m_condition.wait(lk,
                     [&] {
      return m_processed;
    });
    m_processed = false;
  }
  m_worker.join();
}

void av::gua::BlenderViewer::initClass() {
  if (!isTypeInitialized()) {
    av::FieldContainer::initClass();

    AV_FC_INIT(av::FieldContainer, av::gua::BlenderViewer, true);

    SFBlenderViewer::initClass("av::gua::SFBlenderViewer", "av::Field");
    MFBlenderViewer::initClass("av::gua::MFBlenderViewer", "av::Field");
  }
}

void av::gua::BlenderViewer::frame(std::string const& engine, std::string const& camera_name) {

  av::ApplicationInstance::get().evaluate();

#ifdef AVANGO_AWESOMIUM_SUPPORT
  ::gua::Interface::instance()->update();
#endif

  m_current_engine = engine;
  m_camera_name = camera_name;
  if (SceneGraphs.getValue().size() > 0) {
    m_gua_graphs.clear();
    for (auto graph : SceneGraphs.getValue()) {
      //graphs.push_back(reinterpret_cast<av::gua::SceneGraph*>
      //(graph.getBasePtr()));
      auto g = reinterpret_cast<av::gua::SceneGraph*>(graph.getBasePtr());
      g->getGuaSceneGraph()->update_cache();
      m_gua_graphs.push_back(
          ::gua::make_unique< ::gua::SceneGraph>(*g->getGuaSceneGraph()));
    }

    // send data to the worker thread
    {
      std::lock_guard<std::mutex> lk(m_mutex);
      m_ready = true;
    }
    m_condition.notify_one();

    // wait for the worker
    {
      std::unique_lock<std::mutex> lk(m_mutex);
      m_condition.wait(lk,
                       [&] {
        return m_processed;
      });
      m_processed = false;
    }
  }

#if defined(AVANGO_PHYSICS_SUPPORT)
  if (this->Physics.getValue().isValid()) {
    this->Physics.getValue()->synchronize(true);
  }
#endif

  auto& engine_data = m_engines[m_current_engine];
  if (!engine_data.blendertex_id) {
    glGenTextures(1, &engine_data.blendertex_id);
  }

  draw_image(m_image, engine_data.blendertex_id, engine_data.resize);
  engine_data.resize = false;
}

void av::gua::BlenderViewer::render_thread() {
  bool keep_running = true;

  while (keep_running) {
    // Wait until main() sends data
    std::unique_lock<std::mutex> lk(m_mutex);
    m_condition.wait(lk,
                     [&] {
      return m_ready;
    });
    m_ready = false;

    auto av_win = Window.getValue();
    auto window = av_win->getGuaWindow();
    if (window && !window->get_is_open()) {
      window->open();
      window->set_active(true);
    }

    if (window && window->get_is_open()) {

      if (!m_gua_graphs.empty()) {
        auto& graph = m_gua_graphs.front();

        // "blender_window"
        auto& cams = graph->get_camera_nodes();
        auto it = std::find_if(cams.begin(),
                               cams.end(),
                               [this](::gua::node::CameraNode * const & c)->bool {
          return "blender_window" == c->config.get_output_window_name()
              && m_camera_name == c->get_name();
        });
        if (it != cams.end()) {
          auto& cam = *it;
          auto serialized_cam = cam->serialize();

          // make sure pipeline was created
          std::shared_ptr< ::gua::Pipeline> pipe = nullptr;
          auto pipe_iter =
              window->get_context()->render_pipelines.find(serialized_cam.uuid);

          if (pipe_iter == window->get_context()->render_pipelines.end()) {
            pipe = std::make_shared< ::gua::Pipeline>(
                *window->get_context(), serialized_cam.config.get_resolution());
            window->get_context()->render_pipelines
                .insert(std::make_pair(serialized_cam.uuid, pipe));
          } else {
            pipe = pipe_iter->second;
          }

          if (serialized_cam.config.get_enable_stereo()) {
            pipe->render_scene(::gua::CameraMode::LEFT,  serialized_cam, m_gua_graphs);
            pipe->render_scene(::gua::CameraMode::RIGHT, serialized_cam, m_gua_graphs);
          } else {
            pipe->render_scene(serialized_cam.config.get_mono_mode(),
                serialized_cam, m_gua_graphs);
          }
          m_image = screenshot(*pipe);

          // swap buffers
          window->finish_frame();
          ++(window->get_context()->framecount);
        }
      }
    }

    // garbage collection of old engine data

    {
      std::string engine;
      while (m_unregister_queue.pop(engine)) {
        m_engines.erase(engine);
      }
    }

    if (m_done)
      keep_running = false;

    // after the wait, we own the lock.

    // Send data back to main()
    m_processed = true;

    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lk.unlock();
    m_condition.notify_one();

  }
}

av::gua::BlenderViewer::Image av::gua::BlenderViewer::screenshot(::gua::Pipeline const& pipe) {
  auto const& ctx(pipe.get_context());

  auto target(&pipe.get_current_target());
  auto gbuffer(dynamic_cast<::gua::GBuffer*>(target));

  if (!gbuffer)
    return av::gua::BlenderViewer::Image{};

  auto color = gbuffer->get_color_buffer();

  if (!color)
    return av::gua::BlenderViewer::Image{};

  auto texture_ptr = color->get_buffer(ctx);
  auto tex = boost::dynamic_pointer_cast<scm::gl::texture_2d>(texture_ptr);

  auto& tmp = m_engines[m_current_engine];

  if (!tmp.fbo)
    tmp.fbo = ctx.render_device->create_frame_buffer();

  if (!tmp.rgba8_texture || tmp.rgba8_texture->descriptor()._size != tex->descriptor()._size) {
    tmp.fbo->clear_attachments();
    tmp.rgba8_texture = ctx.render_device->create_texture_2d(
        tex->descriptor()._size, scm::gl::FORMAT_RGBA_8);
    tmp.fbo->attach_color_buffer(0, tmp.rgba8_texture, 0, 0);
    tmp.resize = true;
  }

  ctx.render_context->copy_color_buffer(((::gua::GBuffer&)pipe.get_current_target()).get_fbo_read(), tmp.fbo, 0);
  return ::screenshot(ctx.render_context, tmp.rgba8_texture);
}

void av::gua::BlenderViewer::unregister_engine(std::string const& engine) {
  auto& engine_data = m_engines[engine];
  glDeleteTextures(1, &engine_data.blendertex_id);
  engine_data.blendertex_id = 0;
  m_unregister_queue.push(engine);
}
