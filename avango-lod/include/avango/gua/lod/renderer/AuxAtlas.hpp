#ifndef AVANGO_GUA_LOD_AUX_ATLAS_HPP
#define AVANGO_GUA_LOD_AUX_ATLAS_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/Aux.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {

    namespace lod {
      /**
       * Wrapper for ::gua::AuxAtlas
       *
       * \ingroup av_gua
       */
      class AV_GUA_LOD_DLL AuxAtlas : public av::FieldContainer
      {
        AV_FC_DECLARE();

      public:

        /**
         * Constructor. When called without arguments, a new ::gua::AuxAtlas is created.
         * Otherwise, the given ::gua::AuxAtlas is used.
         */
        AuxAtlas(std::shared_ptr< ::gua::Aux::atlas> guanode =
            std::shared_ptr< ::gua::Aux::atlas>(new ::gua::Aux::atlas()));
            

      protected:

        /**
         * Destructor made protected to prevent allocation on stack.
         */
  //      virtual ~AuxAtlas();

      public:
        /**
         * Get the wrapped ::gua::AuxAtlas.
         */
        std::shared_ptr< ::gua::Aux::atlas> getGuaAuxAtlas() const;


        SFUInt NumAtlasTiles;
        SFUInt AtlasWidth;
        SFUInt AtlasHeight;
        SFUInt Rotated;

        virtual void getNumAtlasTilesCB(const SFUInt::GetValueEvent& event);
        
        virtual void getAtlasWidthCB(const SFUInt::GetValueEvent& event);
        
        virtual void getAtlasHeightCB(const SFUInt::GetValueEvent& event);
        
        virtual void getRotatedCB(const SFUInt::GetValueEvent& event);

        inline uint32_t getNumAtlasTiles() {
          if (m_guaAuxAtlas)
            return m_guaAuxAtlas->num_atlas_tiles_;
          else
            return 0;
        }

        inline uint32_t getAtlasWidth() {
          if (m_guaAuxAtlas)
            return m_guaAuxAtlas->atlas_width_;
          else
            return 0;
        }

        inline uint32_t getAtlasHeight() {
          if (m_guaAuxAtlas)
            return m_guaAuxAtlas->atlas_height_;
          else
            return 0;
        }
        inline uint32_t getRotated() {
          if (m_guaAuxAtlas)
            return m_guaAuxAtlas->rotated_;
          else
            return 0;
        }


      private:
        std::shared_ptr< ::gua::Aux::atlas> m_guaAuxAtlas;

        AuxAtlas(const AuxAtlas&);
        AuxAtlas& operator=(const AuxAtlas&);
      };

      typedef SingleField<Link<AuxAtlas> > SFAuxAtlas;
      typedef MultiField<Link<AuxAtlas> > MFAuxAtlas;
    }

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_LOD_DLL SingleField<Link<gua::lod::AuxAtlas> >;
  template class AV_GUA_LOD_DLL MultiField<Link<gua::lod::AuxAtlas> >;
#endif

}

#endif //AVANGO_GUA_LOD_AUX_ATLAS_HPP
