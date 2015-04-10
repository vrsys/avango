#ifndef AVANGO_GUA_SKELETAL_ANIMATION_LOADER_HPP
#define AVANGO_GUA_SKELETAL_ANIMATION_LOADER_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/SkeletalAnimationLoader.hpp>

#include <avango/gua/Fields.hpp>
// #include <avango/gua/skelanim/scenegraph/SkeletalAnimationNode.hpp>
#include <avango/gua/scenegraph/Node.hpp>
#include <avango/gua/renderer/Material.hpp>
#include <avango/FieldContainer.h>

#include <avango/gua/windows_specific_gua.hpp>


namespace av
{
  namespace gua
  {

    namespace skelanim {
      /**
       * Wrapper for ::gua::SkeletalAnimationLoader
       *
       * \ingroup av_gua
       */
      class AV_GUA_DLL SkeletalAnimationLoader : public av::FieldContainer
      {
        AV_FC_DECLARE();

      public:

        enum Flags {
          DEFAULTS = ::gua::SkeletalAnimationLoader::DEFAULTS,
          LOAD_MATERIALS = ::gua::SkeletalAnimationLoader::LOAD_MATERIALS,
          OPTIMIZE_GEOMETRY = ::gua::SkeletalAnimationLoader::OPTIMIZE_GEOMETRY,
          MAKE_PICKABLE = ::gua::SkeletalAnimationLoader::MAKE_PICKABLE,
          NORMALIZE_SCALE = ::gua::SkeletalAnimationLoader::NORMALIZE_SCALE,
          NORMALIZE_POSITION = ::gua::SkeletalAnimationLoader::NORMALIZE_POSITION
        };

        /**
         * Constructor. When called without arguments, a new ::gua::SkeletalAnimationLoader is created.
         * Otherwise, the given ::gua::SkeletalAnimationLoader is used.
         */
        SkeletalAnimationLoader(::gua::SkeletalAnimationLoader* guaSkeletalAnimationLoader = new ::gua::SkeletalAnimationLoader());

        av::Link<av::gua::Node> createGeometryFromFile(std::string const& nodeName,
                                                       std::string const& fileName,
                                                       av::Link<av::gua::Material> const& fallbackMaterial,
                                                       Flags flags = DEFAULTS) const;

        av::Link<av::gua::Node> createGeometryFromFile(std::string const& nodeName,
                                                       std::string const& fileName,
                                                       Flags flags = DEFAULTS) const;

      protected:

        /**
         * Destructor made protected to prevent allocation on stack.
         */
  //      virtual ~SkeletalAnimationLoader();

      public:

        /**
         * Get the wrapped ::gua::SkeletalAnimationLoader.
         */
        ::gua::SkeletalAnimationLoader* getGuaSkeletalAnimationLoader() const;


      private:

        ::gua::SkeletalAnimationLoader *m_guaSkeletalAnimationLoader;

        av::gua::Node* createChildren(std::shared_ptr< ::gua::node::Node> root) const;

        SkeletalAnimationLoader(const SkeletalAnimationLoader&);
        SkeletalAnimationLoader& operator=(const SkeletalAnimationLoader&);
      };

      typedef SingleField<Link<SkeletalAnimationLoader> > SFSkeletalAnimationLoader;
      typedef MultiField<Link<SkeletalAnimationLoader> > MFSkeletalAnimationLoader;
        
    }
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::SkeletalAnimationLoader> >;
  template class AV_GUA_DLL MultiField<Link<gua::SkeletalAnimationLoader> >;
#endif

}

#endif //AVANGO_GUA_SKELETAL_ANIMATION_LOADER_HPP
