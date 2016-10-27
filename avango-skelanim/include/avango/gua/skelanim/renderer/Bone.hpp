#ifndef AVANGO_GUA_BONE_HPP
#define AVANGO_GUA_BONE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/utils/KDTreeUtils.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    namespace skelanim {
      /**
       * Wrapper for ::gua::Ray
       *
       * \ingroup av_gua
       */
      class AV_GUA_DLL Bone : public av::FieldContainer
      {
        AV_FC_DECLARE();

      public:

        /**
         * Constructor. When called without arguments, a new ::gua::Ray is created.
         * Otherwise, the given ::gua::Ray is used.
         */
        Bone(std::shared_ptr< ::gua::Ray> guaRay =
            std::shared_ptr< ::gua::Ray>(new ::gua::Ray(
              ::gua::math::vec3(0.f,0.f,0.f), ::gua::math::vec3(0.f,0.f,0.f), 1.f
            )));

      public:
        SFVec3  Origin;
        SFVec3  Direction;
        SFFloat TMax;

        /**
         * Get the wrapped ::gua::Ray.
         */
        std::shared_ptr< ::gua::Ray> getGuaBone() const;

      public:

        virtual void getOriginCB(const SFVec3::GetValueEvent& event);
        virtual void setOriginCB(const SFVec3::SetValueEvent& event);

        virtual void getDirectionCB(const SFVec3::GetValueEvent& event);
        virtual void setDirectionCB(const SFVec3::SetValueEvent& event);

        virtual void getTMaxCB(const SFFloat::GetValueEvent& event);
        virtual void setTMaxCB(const SFFloat::SetValueEvent& event);

      private:

        std::shared_ptr< ::gua::Ray> m_guaBone;

        Bone(const Bone&);
        Bone& operator=(const Bone&);
      };

      using SFBone = SingleField<Link<Bone> >;
      using MFBone = MultiField<Link<Bone> >;
    }
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::Bone> >;
  template class AV_GUA_DLL MultiField<Link<gua::Bone> >;
#endif

}

#endif //AVANGO_GUA_BONE_HPP