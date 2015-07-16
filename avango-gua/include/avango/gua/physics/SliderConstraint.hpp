#ifndef AVANGO_GUA_SLIDER_CONSTRAINT_HPP
#define AVANGO_GUA_SLIDER_CONSTRAINT_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/physics/SliderConstraint.hpp>

#include <avango/gua/physics/Constraint.hpp>
#include <avango/gua/Fields.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::physics::SliderConstraint
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL SliderConstraint : public av::gua::Constraint
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::physics::SliderConstraint is created.
       * Otherwise, the given ::gua::physics::SliderConstraint is used.
       */
      SliderConstraint(::gua::physics::SliderConstraint* guaconstraint =
                        new ::gua::physics::SliderConstraint(nullptr, nullptr,
                                                            ::gua::math::mat4::identity(),
                                                            ::gua::math::mat4::identity())); // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~SliderConstraint();

    public:

      SFMatrix4  FrameA;
      SFMatrix4  FrameB;

      SFFloat LowerLinearLimit;
      SFFloat UpperLinearLimit;

      SFFloat LowerAngularLimit;
      SFFloat UpperAngularLimit;


      /**
       * Get the wrapped ::gua::physics::SliderConstraint.
       */
      ::gua::physics::SliderConstraint* getGuaConstraint() const;

    public:

      virtual void getFrameACB(const SFMatrix4::GetValueEvent& event);
      virtual void setFrameACB(const SFMatrix4::SetValueEvent& event);

      virtual void getFrameBCB(const SFMatrix4::GetValueEvent& event);
      virtual void setFrameBCB(const SFMatrix4::SetValueEvent& event);

      virtual void getLowerLinearLimitCB(const SFFloat::GetValueEvent& event);
      virtual void setLowerLinearLimitCB(const SFFloat::SetValueEvent& event);

      virtual void getUpperLinearLimitCB(const SFFloat::GetValueEvent& event);
      virtual void setUpperLinearLimitCB(const SFFloat::SetValueEvent& event);

      virtual void getLowerAngularLimitCB(const SFFloat::GetValueEvent& event);
      virtual void setLowerAngularLimitCB(const SFFloat::SetValueEvent& event);

      virtual void getUpperAngularLimitCB(const SFFloat::GetValueEvent& event);
      virtual void setUpperAngularLimitCB(const SFFloat::SetValueEvent& event);

    private:

      ::gua::physics::SliderConstraint *m_guaConstraint;

      SliderConstraint(const SliderConstraint&);
      SliderConstraint& operator=(const SliderConstraint&);
    };

    using SFSliderConstraint = SingleField<Link<SliderConstraint> >;
    using MFSliderConstraint = MultiField<Link<SliderConstraint> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::SliderConstraint> >;
  template class AV_GUA_DLL MultiField<Link<gua::SliderConstraint> >;
#endif

}

#endif //AVANGO_GUA_SLIDER_CONSTRAINT_HPP
