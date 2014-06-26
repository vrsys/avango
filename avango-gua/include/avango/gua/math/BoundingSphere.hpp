#ifndef AVANGO_GUA_BOUNDING_SPHERE_HPP
#define AVANGO_GUA_BOUNDING_SPHERE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/math/BoundingSphere.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::BoundingSphere
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL BoundingSphere : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::BoundingSphere is created.
       * Otherwise, the given ::gua::BoundingSphere is used.
       */
      BoundingSphere(::gua::math::BoundingSphere< ::gua::math::vec3>* guaBoundingSphere =
                  new ::gua::math::BoundingSphere< ::gua::math::vec3>());

      virtual ~BoundingSphere();

    public:

      SFVec3  Center;
      SFFloat Radius;

    public:

      virtual void getCenterCB(const SFVec3::GetValueEvent& event);
      virtual void setCenterCB(const SFVec3::SetValueEvent& event);

      virtual void getRadiusCB(const SFFloat::GetValueEvent& event);
      virtual void setRadiusCB(const SFFloat::SetValueEvent& event);

      ::gua::math::BoundingSphere< ::gua::math::vec3>* getGuaBoundingSphere() const;

      bool operator==(av::gua::BoundingSphere const& rhs) const {
        return *m_guaBoundingSphere == *rhs.getGuaBoundingSphere();
      }

      bool operator!=(av::gua::BoundingSphere const& rhs) const {
        return *m_guaBoundingSphere != *rhs.getGuaBoundingSphere();
      }

    private:

      ::gua::math::BoundingSphere< ::gua::math::vec3>* m_guaBoundingSphere;

      // BoundingSphere(const BoundingSphere&);
      BoundingSphere& operator=(const BoundingSphere&);
    };

    typedef SingleField<Link<BoundingSphere> > SFBoundingSphere;
    typedef MultiField<Link<BoundingSphere> > MFBoundingSphere;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::BoundingSphere> >;
  template class AV_GUA_DLL MultiField<Link<gua::BoundingSphere> >;
#endif

}

#endif //AVANGO_GUA_BOUNDING_SPHERE_HPP
