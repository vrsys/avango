#ifndef AVANGO_GUA_LOD_AUXFEATURE_HPP
#define AVANGO_GUA_LOD_AUXFEATURE_HPP

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
       * Wrapper for ::gua::AuxFeature
       *
       * \ingroup av_gua
       */
      class AV_GUA_LOD_DLL AuxFeature : public av::FieldContainer
      {
        AV_FC_DECLARE();

      public:

        /**
         * Constructor. When called without arguments, a new ::gua::AuxFeature is created.
         * Otherwise, the given ::gua::AuxFeature is used.
         */
        AuxFeature(std::shared_ptr< ::gua::Auxi::feature> guanode =
            std::shared_ptr< ::gua::Auxi::feature>(new ::gua::Auxi::feature()));
            

      protected:

        /**
         * Destructor made protected to prevent allocation on stack.
         */
  //      virtual ~AuxFeature();

      public:
        /**
         * Get the wrapped ::gua::AuxFeature.
         */
        std::shared_ptr< ::gua::Auxi::feature> getGuaAuxFeature() const;


        SFUInt CameraId;
        SFUInt UsingCount;
        SFVec2 Coords;
        SFVec2 Error;
        

        virtual void getCameraIdCB(const SFUInt::GetValueEvent& event);
        virtual void getUsingCountCB(const SFUInt::GetValueEvent& event);
        virtual void getCoordsCB(const SFVec2::GetValueEvent& event);
        virtual void getErrorCB(const SFVec2::GetValueEvent& event);
        

        inline uint32_t getCameraId() {
          if (m_guaAuxFeature)
            return m_guaAuxFeature->camera_id_;
          else
            return 0;
        }

        inline uint32_t getUsingCount() {
          if (m_guaAuxFeature)
            return m_guaAuxFeature->using_count_;
          else
            return 0;
        }

        inline ::gua::math::vec2 getCoords() {
          if (m_guaAuxFeature)
            return ::gua::math::vec2(m_guaAuxFeature->coords_);
          else
            return ::gua::math::vec2();
        }

        inline ::gua::math::vec2 getError() {
          if (m_guaAuxFeature)
            return ::gua::math::vec2(m_guaAuxFeature->error_);
          else
            return ::gua::math::vec2();
        }


      private:
        std::shared_ptr< ::gua::Auxi::feature> m_guaAuxFeature;

        AuxFeature(const AuxFeature&);
        AuxFeature& operator=(const AuxFeature&);
      };

      typedef SingleField<Link<AuxFeature> > SFAuxFeature;
      typedef MultiField<Link<AuxFeature> > MFAuxFeature;
    }

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_LOD_DLL SingleField<Link<gua::lod::AuxFeature> >;
  template class AV_GUA_LOD_DLL MultiField<Link<gua::lod::AuxFeature> >;
#endif

}

#endif //AVANGO_GUA_LOD_AUXFEATURE_HPP
