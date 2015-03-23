#ifndef AVANGO_GUA_BLENDER_VIEWER_HPP
#define AVANGO_GUA_BLENDER_VIEWER_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/events.hpp>
#include <avango/gua/Fields.hpp>
#include <avango/gua/renderer/HeadlessSurface.hpp>
#include <avango/gua/scenegraph/CameraNode.hpp>
#include <avango/gua/scenegraph/SceneGraph.hpp>
#if defined(AVANGO_PHYSICS_SUPPORT)
#include <avango/gua/physics/Physics.hpp>
#endif
#include <avango/FieldContainer.h>
#include <avango/gua/windows_specific_gua.hpp>

#include <string>
#include <thread>
#include <mutex>
#include <memory>
#include <condition_variable>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::BlenderViewer
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL BlenderViewer : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      BlenderViewer();

      MFSceneGraph SceneGraphs;
      SFHeadlessSurface Window;
#if defined(AVANGO_PHYSICS_SUPPORT)
      SFPhysics    Physics;
#endif

      void frame(std::string const& uuid);

      struct Image {
        int32_t width = 0;
        int32_t height = 0;
        uint32_t bpp = 0;
        uint32_t gl_internal_format = 0;
        uint32_t gl_base_format = 0;
        uint32_t gl_type = 0;
        std::vector<char> data = {};

        size_t size_header() const {
        return sizeof(width) + sizeof(height) + sizeof(bpp) +
               sizeof(gl_internal_format) + sizeof(gl_base_format) +
               sizeof(gl_type);
        }
        size_t size() const { return size_header() + data.size(); }
      };

      void unregister_engine(std::string const& uuid);

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~BlenderViewer();

    private:

      BlenderViewer(const BlenderViewer&) = delete;
      BlenderViewer& operator=(const BlenderViewer&) = delete;

      void render_thread();
      Image screenshot(::gua::Pipeline const& pipe);

      std::mutex m_mutex;
      std::condition_variable m_condition;
      std::vector<std::unique_ptr<const ::gua::SceneGraph> > m_gua_graphs;
      std::string m_current_engine_uuid = "";
      Image m_image;
      bool m_ready = false;
      bool m_processed = false;
      bool m_done = false;
      std::thread m_worker;

      struct EngineData
      {
        scm::gl::frame_buffer_ptr fbo = nullptr;
        scm::gl::texture_2d_ptr rgba8_texture = nullptr;
      };
      std::map<std::string, EngineData> m_engines;

      //scm::gl::frame_buffer_ptr tmp_fbo = nullptr;
      //scm::gl::texture_2d_ptr tmp_rgba8_texture = nullptr;
    };

    typedef SingleField<Link<BlenderViewer> > SFBlenderViewer;
    typedef MultiField<Link<BlenderViewer> > MFBlenderViewer;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::BlenderViewer> >;
  template class AV_GUA_DLL MultiField<Link<gua::BlenderViewer> >;
#endif

}

#endif //AVANGO_GUA_BLENDER_VIEWER_HPP
