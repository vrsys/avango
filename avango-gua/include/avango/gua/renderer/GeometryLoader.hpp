#ifndef AVANGO_GUA_GEOMETRY_LOADER_HPP
#define AVANGO_GUA_GEOMETRY_LOADER_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/GeometryLoader.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/FieldContainer.h>

#include <avango/gua/windows_specific_gua.hpp>


namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::GeometryLoader
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL GeometryLoader : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      enum Flags {
        DEFAULTS = ::gua::GeometryLoader::DEFAULTS,
        LOAD_MATERIALS = ::gua::GeometryLoader::LOAD_MATERIALS,
        OPTIMIZE_GEOMETRY = ::gua::GeometryLoader::OPTIMIZE_GEOMETRY,
        MAKE_PICKABLE = ::gua::GeometryLoader::MAKE_PICKABLE,
        NORMALIZE_POSITION = ::gua::GeometryLoader::NORMALIZE_POSITION,
        NORMALIZE_SCALE = ::gua::GeometryLoader::NORMALIZE_SCALE

      };

      /**
       * Constructor. When called without arguments, a new ::gua::GeometryLoader is created.
       * Otherwise, the given ::gua::GeometryLoader is used.
       */
      GeometryLoader(::gua::GeometryLoader* guaGeometryLoader = new ::gua::GeometryLoader());

      av::Link<av::gua::Node> createGeometryFromFile(std::string const& nodeName,
                                                             std::string const& fileName,
                                                             std::string const& fallbackMaterial,
                                                             Flags flags = DEFAULTS) const;
      av::Link<av::gua::Node> createVVolumeFromFile(std::string const& nodeName,
                                                             std::string const& vfileName,
                                                             Flags flags = DEFAULTS,
                                                             scm::size_t vol_hdd_cache_size = 2147483648,
                                                             scm::size_t vol_gpu_cache_size = 536870912
                                                             ) const;

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~GeometryLoader();

    public:

      /**
       * Get the wrapped ::gua::GeometryLoader.
       */
      ::gua::GeometryLoader* getGuaGeometryLoader() const;


    private:

      ::gua::GeometryLoader *m_guaGeometryLoader;

      av::gua::Node* createChildren(std::shared_ptr< ::gua::Node> root) const;

      GeometryLoader(const GeometryLoader&);
      GeometryLoader& operator=(const GeometryLoader&);
    };

    typedef SingleField<Link<GeometryLoader> > SFGeometryLoader;
    typedef MultiField<Link<GeometryLoader> > MFGeometryLoader;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::GeometryLoader> >;
  template class AV_GUA_DLL MultiField<Link<gua::GeometryLoader> >;
#endif

}

#endif //AVANGO_GUA_GEOMETRY_LOADER_HPP
