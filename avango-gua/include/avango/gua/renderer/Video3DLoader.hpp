#ifndef AVANGO_GUA_VIDEO3D_LOADER_HPP
#define AVANGO_GUA_VIDEO3D_LOADER_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/video3d/Video3DLoader.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/gua/scenegraph/Video3DNode.hpp>
#include <avango/FieldContainer.h>

#include <avango/gua/windows_specific_gua.hpp>


namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::Video3DLoader
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL Video3DLoader : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::Video3DLoader is created.
       * Otherwise, the given ::gua::Video3DLoader is used.
       */
      Video3DLoader(::gua::Video3DLoader* guaVideo3DLoader = new ::gua::Video3DLoader());

      av::Link<av::gua::Node> load( std::string const& nodename,
                                    std::string const& fileName) const;

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~Video3DLoader();

    public:

      /**
       * Get the wrapped ::gua::Video3DLoader.
       */
      ::gua::Video3DLoader* getGuaVideo3DLoader() const;


    private:

      ::gua::Video3DLoader *m_guaVideo3DLoader;

      av::gua::Node* createChildren(std::shared_ptr< ::gua::node::Node> root) const;

      Video3DLoader(const Video3DLoader&);
      Video3DLoader& operator=(const Video3DLoader&);
    };

    using SFVideo3DLoader = SingleField<Link<Video3DLoader> >;
    using MFVideo3DLoader = MultiField<Link<Video3DLoader> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::Video3DLoader> >;
  template class AV_GUA_DLL MultiField<Link<gua::Video3DLoader> >;
#endif

}

#endif //AVANGO_GUA_VIDEO3D_LOADER_HPP
