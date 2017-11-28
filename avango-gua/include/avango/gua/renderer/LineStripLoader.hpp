#ifndef AVANGO_GUA_LINE_STRIP_LOADER_HPP
#define AVANGO_GUA_LINE_STRIP_LOADER_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/LineStripLoader.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/gua/scenegraph/LineStripNode.hpp>
#include <avango/gua/renderer/Material.hpp>
#include <avango/FieldContainer.h>

#include <avango/gua/windows_specific_gua.hpp>


namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::LineStripLoader
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL LineStripLoader : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      enum Flags {
        DEFAULTS = ::gua::LineStripLoader::DEFAULTS,
        MAKE_PICKABLE = ::gua::LineStripLoader::MAKE_PICKABLE,
        NORMALIZE_SCALE = ::gua::LineStripLoader::NORMALIZE_SCALE,
        NORMALIZE_POSITION = ::gua::LineStripLoader::NORMALIZE_POSITION,
        NO_SHARED_MATERIALS = ::gua::LineStripLoader::NO_SHARED_MATERIALS,
      };

      /**
       * Constructor. When called without arguments, a new ::gua::LineStripLoader is created.
       * Otherwise, the given ::gua::LineStripLoader is used.
       */
      LineStripLoader(::gua::LineStripLoader* guaLineStripLoader = new ::gua::LineStripLoader());

      av::Link<av::gua::Node> createGeometryFromFile(std::string const& nodeName,
                                                     std::string const& fileName,
                                                     av::gua::Material const& fallbackMaterial,
                                                     Flags flags = DEFAULTS) const;

      av::Link<av::gua::Node> createGeometryFromFile(std::string const& nodeName,
                                                     std::string const& fileName,
                                                     Flags flags = DEFAULTS) const;

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~LineStripLoader();

    public:

      /**
       * Get the wrapped ::gua::LineStripLoader.
       */
      ::gua::LineStripLoader* getGuaLineStripLoader() const;


    private:

      ::gua::LineStripLoader *m_guaLineStripLoader;

      av::gua::Node* createChildren(std::shared_ptr< ::gua::node::Node> root) const;

      LineStripLoader(const LineStripLoader&);
      LineStripLoader& operator=(const LineStripLoader&);
    };

    using SFLineStripLoader = SingleField<Link<LineStripLoader> >;
    using MFLineStripLoader = MultiField<Link<LineStripLoader> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::LineStripLoader> >;
  template class AV_GUA_DLL MultiField<Link<gua::LineStripLoader> >;
#endif

}

#endif //AVANGO_GUA_LINE_STRIP_LOADER_HPP
