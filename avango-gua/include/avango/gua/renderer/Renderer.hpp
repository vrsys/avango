#ifndef AVANGO_GUA_RENDERER_HPP
#define AVANGO_GUA_RENDERER_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/Renderer.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/gua/renderer/Pipeline.hpp>
#include <avango/gua/scenegraph/SceneGraph.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
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
      Renderer(::gua::Renderer* guaRenderer = new ::gua::Renderer({}));

      Renderer(std::vector<av::gua::Pipeline const*> const& pipes);

      void queue_draw(std::vector<av::gua::SceneGraph const*> const& graphs) const;

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
