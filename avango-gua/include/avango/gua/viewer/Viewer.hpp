#ifndef AVANGO_GUA_VIEWER_HPP
#define AVANGO_GUA_VIEWER_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <avango/gua/Fields.hpp>
#include <avango/gua/renderer/Renderer.hpp>
#include <avango/gua/renderer/Pipeline.hpp>
#include <avango/gua/scenegraph/SceneGraph.hpp>
#include <avango/gua/physics/Physics.hpp>
#include <avango/FieldContainer.h>
#include <avango/gua/windows_specific_gua.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::Viewer
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL Viewer : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      Viewer();

      MFPipeline   Pipelines;
      MFSceneGraph SceneGraphs;
      SFPhysics    Physics;

      void run() const;

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~Viewer();


    private:

      mutable av::gua::Renderer *m_renderer;

      Viewer(const Viewer&);
      Viewer& operator=(const Viewer&);
    };

    typedef SingleField<Link<Viewer> > SFViewer;
    typedef MultiField<Link<Viewer> > MFViewer;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::Viewer> >;
  template class AV_GUA_DLL MultiField<Link<gua::Viewer> >;
#endif

}

#endif //AVANGO_GUA_VIEWER_HPP
