#ifndef AVANGO_GUA_NURBS_LOADER_HPP
#define AVANGO_GUA_NURBS_LOADER_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/NURBSLoader.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/gua/scenegraph/TriMeshNode.hpp>
#include <avango/FieldContainer.h>

#include <avango/gua/windows_specific_gua.hpp>


namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::NURBSLoader
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL NURBSLoader : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      enum Flags {
        DEFAULTS = ::gua::NURBSLoader::DEFAULTS,
        LOAD_MATERIALS = ::gua::NURBSLoader::LOAD_MATERIALS,
        OPTIMIZE_GEOMETRY = ::gua::NURBSLoader::OPTIMIZE_GEOMETRY,
        MAKE_PICKABLE = ::gua::NURBSLoader::MAKE_PICKABLE,
        NORMALIZE_SCALE = ::gua::NURBSLoader::NORMALIZE_SCALE,
        NORMALIZE_POSITION = ::gua::NURBSLoader::NORMALIZE_POSITION
      };

      /**
       * Constructor. When called without arguments, a new ::gua::NURBSLoader is created.
       * Otherwise, the given ::gua::NURBSLoader is used.
       */
      NURBSLoader(::gua::NURBSLoader* guaNURBSLoader = new ::gua::NURBSLoader());

      av::Link<av::gua::Node> createGeometryFromFile(
                                           std::string const& nodeName,
                                           std::string const& fileName,
                                           std::string const& fallbackMaterial,
                                           Flags flags = DEFAULTS) const;

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~NURBSLoader();

    public:

      /**
       * Get the wrapped ::gua::NURBSLoader.
       */
      ::gua::NURBSLoader* getGuaNURBSLoader() const;


    private:

      ::gua::NURBSLoader *m_guaNURBSLoader;

      av::gua::Node* createChildren(std::shared_ptr< ::gua::Node> root) const;

      NURBSLoader(const NURBSLoader&);
      NURBSLoader& operator=(const NURBSLoader&);
    };

    typedef SingleField<Link<NURBSLoader> > SFNURBSLoader;
    typedef MultiField<Link<NURBSLoader> > MFNURBSLoader;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::NURBSLoader> >;
  template class AV_GUA_DLL MultiField<Link<gua::NURBSLoader> >;
#endif

}

#endif //AVANGO_GUA_NURBS_LOADER_HPP
