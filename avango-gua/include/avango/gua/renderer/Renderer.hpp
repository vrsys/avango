#ifndef AVANGO_GUA_RENDERER_HPP
#define AVANGO_GUA_RENDERER_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <avango/FieldContainer.h>
#include <gua/renderer/Renderer.hpp>

#include <avango/gua/windows_specific_gua.hpp>

namespace gua {
  namespace node {
    class CameraNode;
  }
}

namespace av
{
  namespace gua
  {
    class CameraNode;
    class SceneGraph;

    /**
     * Wrapper for ::gua::Renderer
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL Renderer : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::Renderer is created.
       * Otherwise, the given ::gua::Renderer is used.
       */
      Renderer(::gua::Renderer* guaRenderer = new ::gua::Renderer());


      void queue_draw(std::vector<av::gua::SceneGraph const*> const& graphs,
                      std::vector<av::gua::CameraNode const*> const& cams) const;

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~Renderer();

    public:

      /**
       * Get the wrapped ::gua::Renderer.
       */
      ::gua::Renderer* getGuaRenderer() const;


    private:

      ::gua::Renderer *m_guaRenderer;

      Renderer(const Renderer&);
      Renderer& operator=(const Renderer&);
    };

    typedef SingleField<Link<Renderer> > SFRenderer;
    typedef MultiField<Link<Renderer> > MFRenderer;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::Renderer> >;
  template class AV_GUA_DLL MultiField<Link<gua::Renderer> >;
#endif

}


#endif //AVANGO_GUA_RENDERER_HPP
