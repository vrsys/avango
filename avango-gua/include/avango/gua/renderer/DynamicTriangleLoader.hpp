#ifndef AVANGO_GUA_DYNAMIC_TRIANGLE_LOADER_HPP
#define AVANGO_GUA_DYNAMIC_TRIANGLE_LOADER_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/DynamicTriangleLoader.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/gua/scenegraph/DynamicTriangleNode.hpp>
#include <avango/gua/renderer/Material.hpp>
#include <avango/FieldContainer.h>

#include <avango/gua/windows_specific_gua.hpp>


namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::DynamicTriangleLoader
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL DynamicTriangleLoader : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      enum Flags {
        DEFAULTS = ::gua::DynamicTriangleLoader::DEFAULTS,
        MAKE_PICKABLE = ::gua::DynamicTriangleLoader::MAKE_PICKABLE,
        NORMALIZE_SCALE = ::gua::DynamicTriangleLoader::NORMALIZE_SCALE,
        NORMALIZE_POSITION = ::gua::DynamicTriangleLoader::NORMALIZE_POSITION,
        NO_SHARED_MATERIALS = ::gua::DynamicTriangleLoader::NO_SHARED_MATERIALS,
      };

      /**
       * Constructor. When called without arguments, a new ::gua::DynamicTriangleLoader is created.
       * Otherwise, the given ::gua::DynamicTriangleLoader is used.
       */
      DynamicTriangleLoader(::gua::DynamicTriangleLoader* guaDynamicTriangleLoader = new ::gua::DynamicTriangleLoader());

      /**
      *
      */
      av::Link<av::gua::Node> createEmptyGeometry(std::string const& nodeName,
                                                  std::string const& emptyName,
                                                  av::gua::Material const& fallbackMaterial,
                                                  Flags flags = DEFAULTS) const;

      /**
      *
      */
      av::Link<av::gua::Node> createEmptyGeometry(std::string const& nodeName,
                                                  std::string const& emptyName,
                                                  Flags flags = DEFAULTS) const;

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~DynamicTriangleLoader();

    public:

      /**
       * Get the wrapped ::gua::DynamicTriangleLoader.
       */
      ::gua::DynamicTriangleLoader* getGuaDynamicTriangleLoader() const;


    private:

      ::gua::DynamicTriangleLoader *m_guaDynamicTriangleLoader;

      av::gua::Node* createChildren(std::shared_ptr< ::gua::node::Node> root) const;

      DynamicTriangleLoader(const DynamicTriangleLoader&);
      DynamicTriangleLoader& operator=(const DynamicTriangleLoader&);
    };

    using SFDynamicTriangleLoader = SingleField<Link<DynamicTriangleLoader> >;
    using MFDynamicTriangleLoader = MultiField<Link<DynamicTriangleLoader> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::DynamicTriangleLoader> >;
  template class AV_GUA_DLL MultiField<Link<gua::DynamicTriangleLoader> >;
#endif

}

#endif //AVANGO_GUA_DYNAMIC_TRIANGLE_LOADER_HPP
