#ifndef AVANGO_GUA_LOD_AUXSPARSEPOINT_HPP
#define AVANGO_GUA_LOD_AUXSPARSEPOINT_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/Auxi.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>
#include "avango/gua/lod/renderer/AuxSparsePoint.hpp"

#include <avango/gua/lod/renderer/AuxFeature.hpp>
namespace gua {
namespace renderer {
  class auxi;
  // namespace aux{
  struct feature;
  // }
}
}

namespace av
{
  namespace gua
  {

    namespace lod {
      /**
       * Wrapper for ::gua::AuxSparsePoint
       *
       * \ingroup av_gua
       */
      class AV_GUA_LOD_DLL AuxSparsePoint : public av::FieldContainer
      {
        AV_FC_DECLARE();

      public:

        /**
         * Constructor. When called without arguments, a new ::gua::AuxSparsePoint is created.
         * Otherwise, the given ::gua::AuxSparsePoint is used.
         */
        AuxSparsePoint(std::shared_ptr< ::gua::Auxi::sparse_point> guanode =
            std::shared_ptr< ::gua::Auxi::sparse_point>(new ::gua::Auxi::sparse_point()));
            

      protected:

        /**
         * Destructor made protected to prevent allocation on stack.
         */
  //      virtual ~AuxSparsePoint();

      public:
        /**
         * Get the wrapped ::gua::AuxSparsePoint.
         */
        std::shared_ptr< ::gua::Auxi::sparse_point> getGuaAuxSparsePoint() const;


        
        SFVec3 Position;
        SFUInt Red;
        SFUInt Green;
        SFUInt Blue;
        SFUInt Alpha;
        //MFMultiField

        virtual void getPositionCB(const SFVec3::GetValueEvent& event);
        virtual void getRedCB(const SFUInt::GetValueEvent& event);
        virtual void getGreenCB(const SFUInt::GetValueEvent& event);
        virtual void getBlueCB(const SFUInt::GetValueEvent& event);
        virtual void getAlphaCB(const SFUInt::GetValueEvent& event);


        inline ::gua::math::vec3 getPosition() {
          if (m_guaAuxSparsePoint)
            return ::gua::math::vec3(m_guaAuxSparsePoint->pos_);
          else
            return ::gua::math::vec3();
        }

        inline uint32_t getRed() {
          if (m_guaAuxSparsePoint)
            return m_guaAuxSparsePoint->r_;
          else
            return 0;
        }
        inline uint32_t getGreen() {
          if (m_guaAuxSparsePoint)
            return m_guaAuxSparsePoint->g_;
          else
            return 0;
        }
        inline uint32_t getBlue() {
          if (m_guaAuxSparsePoint)
            return m_guaAuxSparsePoint->b_;
          else
            return 0;
        }
        inline uint32_t getAlpha() {
          if (m_guaAuxSparsePoint)
            return m_guaAuxSparsePoint->a_;
          else
            return 0;
        }

        inline std::string getFeatures() {
          if (m_guaAuxSparsePoint)
            return "Features not implemented yet";
          else
            return "Features not implemented yet";
        }

        inline uint32_t getNumFeatures() {
          if (m_guaAuxSparsePoint)
            return m_guaAuxSparsePoint->features_.size();
          else
            return 0;
        }

        av::Link<av::gua::lod::AuxFeature> getFeatureById(uint32_t id) const;


      private:
        std::shared_ptr< ::gua::Auxi::sparse_point> m_guaAuxSparsePoint;

        AuxSparsePoint(const AuxSparsePoint&);
        AuxSparsePoint& operator=(const AuxSparsePoint&);
      };

      typedef SingleField<Link<AuxSparsePoint> > SFAuxSparsePoint;
      typedef MultiField<Link<AuxSparsePoint> > MFAuxSparsePoint;
    }

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_LOD_DLL SingleField<Link<gua::lod::AuxSparsePoint> >;
  template class AV_GUA_LOD_DLL MultiField<Link<gua::lod::AuxSparsePoint> >;
#endif

}

#endif //AVANGO_GUA_LOD_AUXSPARSEPOINT_HPP
