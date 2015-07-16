#ifndef AVANGO_GUA_BOUNDING_BOX_HPP
#define AVANGO_GUA_BOUNDING_BOX_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/math/BoundingBox.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::BoundingBox
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL BoundingBox : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::BoundingBox is created.
       * Otherwise, the given ::gua::BoundingBox is used.
       */
      BoundingBox(::gua::math::BoundingBox< ::gua::math::vec3>* guaBoundingBox =
                  new ::gua::math::BoundingBox< ::gua::math::vec3>());

      virtual ~BoundingBox();

    public:

      SFVec3  Min;
      SFVec3  Max;

    public:

      virtual void getMinCB(const SFVec3::GetValueEvent& event);
      virtual void setMinCB(const SFVec3::SetValueEvent& event);

      virtual void getMaxCB(const SFVec3::GetValueEvent& event);
      virtual void setMaxCB(const SFVec3::SetValueEvent& event);

      ::gua::math::BoundingBox< ::gua::math::vec3>* getGuaBoundingBox() const;

      bool operator==(av::gua::BoundingBox const& rhs) const {
        return *m_guaBoundingBox == *rhs.getGuaBoundingBox();
      }

      bool operator!=(av::gua::BoundingBox const& rhs) const {
        return *m_guaBoundingBox != *rhs.getGuaBoundingBox();
      }

    private:

      ::gua::math::BoundingBox< ::gua::math::vec3>* m_guaBoundingBox;

      // BoundingBox(const BoundingBox&);
      BoundingBox& operator=(const BoundingBox&);
    };

    using SFBoundingBox = SingleField<Link<BoundingBox> >;
    using MFBoundingBox = MultiField<Link<BoundingBox> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::BoundingBox> >;
  template class AV_GUA_DLL MultiField<Link<gua::BoundingBox> >;
#endif

}

#endif //AVANGO_GUA_BOUNDING_BOX_HPP
