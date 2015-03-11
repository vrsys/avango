#ifndef AVANGO_GUA_BLENDER_VIEWER_HPP
#define AVANGO_GUA_BLENDER_VIEWER_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/events.hpp>
#include <avango/gua/Fields.hpp>
#include <avango/gua/renderer/Renderer.hpp>
#include <avango/gua/renderer/HeadlessSurface.hpp>
#include <avango/gua/scenegraph/CameraNode.hpp>
#include <avango/gua/scenegraph/SceneGraph.hpp>
#if defined(AVANGO_PHYSICS_SUPPORT)
#include <avango/gua/physics/Physics.hpp>
#endif
#include <avango/FieldContainer.h>
#include <avango/gua/windows_specific_gua.hpp>

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
      MFHeadlessSurface Windows;
#if defined(AVANGO_PHYSICS_SUPPORT)
      SFPhysics    Physics;
#endif

      void frame();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~BlenderViewer() {}


    private:

      av::gua::Renderer m_renderer;
      void draw_image(::gua::HeadlessSurface::Image const&);

      BlenderViewer(const BlenderViewer&) = delete;
      BlenderViewer& operator=(const BlenderViewer&) = delete;
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
