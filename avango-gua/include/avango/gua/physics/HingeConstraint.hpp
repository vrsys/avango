#ifndef AVANGO_GUA_HINGE_CONSTRAINT_HPP
#define AVANGO_GUA_HINGE_CONSTRAINT_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/physics/HingeConstraint.hpp>

#include <avango/gua/physics/Constraint.hpp>
#include <avango/gua/Fields.hpp>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::physics::HingeConstraint
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL HingeConstraint : public av::gua::Constraint
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::physics::HingeConstraint is created.
     * Otherwise, the given ::gua::physics::HingeConstraint is used.
     */
    HingeConstraint(::gua::physics::HingeConstraint* guaconstraint = new ::gua::physics::HingeConstraint(nullptr,
                                                                                                         nullptr,
                                                                                                         ::gua::math::vec3(),
                                                                                                         ::gua::math::vec3(),
                                                                                                         ::gua::math::vec3(),
                                                                                                         ::gua::math::vec3())); // use defined type to circumvent compiler bug in VS8

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~HingeConstraint();

  public:
    SFVec3 PivotA;
    SFVec3 PivotB;
    SFVec3 AxisA;
    SFVec3 AxisB;

    SFFloat UpperLimit;
    SFFloat LowerLimit;

    SFBool MotorEnabled;
    SFFloat MotorTargetVelocity;
    SFFloat MotorMaxImpulse;

    /**
     * Get the wrapped ::gua::physics::HingeConstraint.
     */
    ::gua::physics::HingeConstraint* getGuaConstraint() const;

  public:
    virtual void getPivotACB(const SFVec3::GetValueEvent& event);
    virtual void setPivotACB(const SFVec3::SetValueEvent& event);

    virtual void getPivotBCB(const SFVec3::GetValueEvent& event);
    virtual void setPivotBCB(const SFVec3::SetValueEvent& event);

    virtual void getAxisACB(const SFVec3::GetValueEvent& event);
    virtual void setAxisACB(const SFVec3::SetValueEvent& event);

    virtual void getAxisBCB(const SFVec3::GetValueEvent& event);
    virtual void setAxisBCB(const SFVec3::SetValueEvent& event);

    virtual void getUpperLimitCB(const SFFloat::GetValueEvent& event);
    virtual void setUpperLimitCB(const SFFloat::SetValueEvent& event);

    virtual void getLowerLimitCB(const SFFloat::GetValueEvent& event);
    virtual void setLowerLimitCB(const SFFloat::SetValueEvent& event);

    virtual void getMotorEnabledCB(const SFBool::GetValueEvent& event);
    virtual void setMotorEnabledCB(const SFBool::SetValueEvent& event);

    virtual void getMotorTargetVelocityCB(const SFFloat::GetValueEvent& event);
    virtual void setMotorTargetVelocityCB(const SFFloat::SetValueEvent& event);

    virtual void getMotorMaxImpulseCB(const SFFloat::GetValueEvent& event);
    virtual void setMotorMaxImpulseCB(const SFFloat::SetValueEvent& event);

  private:
    ::gua::physics::HingeConstraint* m_guaConstraint;

    HingeConstraint(const HingeConstraint&);
    HingeConstraint& operator=(const HingeConstraint&);
};

using SFHingeConstraint = SingleField<Link<HingeConstraint>>;
using MFHingeConstraint = MultiField<Link<HingeConstraint>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::HingeConstraint>>;
template class AV_GUA_DLL MultiField<Link<gua::HingeConstraint>>;
#endif

} // namespace av

#endif // AVANGO_GUA_HINGE_CONSTRAINT_HPP
