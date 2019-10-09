#ifndef AVANGO_GUA_LOD_AUXATLASTILE_HPP
#define AVANGO_GUA_LOD_AUXATLASTILE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/Auxi.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {

    namespace lod {
      /**
       * Wrapper for ::gua::AuxAtlasTile
       *
       * \ingroup av_gua
       */
      class AV_GUA_LOD_DLL AuxAtlasTile : public av::FieldContainer
      {
        AV_FC_DECLARE();

      public:

        /**
         * Constructor. When called without arguments, a new ::gua::AuxAtlasTile is created.
         * Otherwise, the given ::gua::AuxAtlasTile is used.
         */
        AuxAtlasTile(std::shared_ptr< ::gua::Auxi::atlas_tile> guanode =
            std::shared_ptr< ::gua::Auxi::atlas_tile>(new ::gua::Auxi::atlas_tile()));
            

      protected:

        /**
         * Destructor made protected to prevent allocation on stack.
         */
  //      virtual ~AuxAtlasTile();

      public:
        /**
         * Get the wrapped ::gua::AuxAtlasTile.
         */
        std::shared_ptr< ::gua::Auxi::atlas_tile> getGuaAuxAtlasTile() const;


        SFUInt AtlasTileId;
        SFUInt X;
        SFUInt Y;
        SFUInt Width;
        SFUInt Height;

        virtual void getAtlasTileIdCB(const SFUInt::GetValueEvent& event);
        
        virtual void getXCB(const SFUInt::GetValueEvent& event);
        
        virtual void getYCB(const SFUInt::GetValueEvent& event);
        
        virtual void getWidthCB(const SFUInt::GetValueEvent& event);

        virtual void getHeightCB(const SFUInt::GetValueEvent& event);

        inline uint32_t getAtlasTileId() {
          if (m_guaAuxAtlasTile)
            return m_guaAuxAtlasTile->atlas_tile_id_;
          else
            return 0;
        }

        inline uint32_t getX() {
          if (m_guaAuxAtlasTile)
            return m_guaAuxAtlasTile->x_;
          else
            return 0;
        }

        inline uint32_t getY() {
          if (m_guaAuxAtlasTile)
            return m_guaAuxAtlasTile->y_;
          else
            return 0;
        }
        inline uint32_t getWidth() {
          if (m_guaAuxAtlasTile)
            return m_guaAuxAtlasTile->width_;
          else
            return 0;
        }
        inline uint32_t getHeight() {
          if (m_guaAuxAtlasTile)
            return m_guaAuxAtlasTile->height_;
          else
            return 0;
        }


      private:
        std::shared_ptr< ::gua::Auxi::atlas_tile> m_guaAuxAtlasTile;

        AuxAtlasTile(const AuxAtlasTile&);
        AuxAtlasTile& operator=(const AuxAtlasTile&);
      };

      typedef SingleField<Link<AuxAtlasTile> > SFAuxAtlasTile;
      typedef MultiField<Link<AuxAtlasTile> > MFAuxAtlasTile;
    }

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_LOD_DLL SingleField<Link<gua::lod::AuxAtlasTile> >;
  template class AV_GUA_LOD_DLL MultiField<Link<gua::lod::AuxAtlasTile> >;
#endif

}

#endif //AVANGO_GUA_LOD_AUXATLASTILE_HPP
