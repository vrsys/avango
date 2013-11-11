#include <avango/gua/physics/HingeConstraint.hpp>

#include <avango/gua/Types.hpp>

#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::HingeConstraint"));
}

AV_FC_DEFINE(av::gua::HingeConstraint);

AV_FIELD_DEFINE(av::gua::SFHingeConstraint);
AV_FIELD_DEFINE(av::gua::MFHingeConstraint);

av::gua::HingeConstraint::HingeConstraint(::gua::physics::HingeConstraint* guaconstraint)
  : Constraint(guaconstraint),
    m_guaConstraint(reinterpret_cast< ::gua::physics::HingeConstraint*>(Constraint::getGuaConstraint()))
{
    AV_FC_ADD_ADAPTOR_FIELD(PivotA,
                        boost::bind(&HingeConstraint::getPivotACB, this, _1),
                        boost::bind(&HingeConstraint::setPivotACB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(PivotB,
                        boost::bind(&HingeConstraint::getPivotBCB, this, _1),
                        boost::bind(&HingeConstraint::setPivotBCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(AxisA,
                        boost::bind(&HingeConstraint::getAxisACB, this, _1),
                        boost::bind(&HingeConstraint::setAxisACB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(AxisB,
                        boost::bind(&HingeConstraint::getAxisBCB, this, _1),
                        boost::bind(&HingeConstraint::setAxisBCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(UpperLimit,
                        boost::bind(&HingeConstraint::getUpperLimitCB, this, _1),
                        boost::bind(&HingeConstraint::setUpperLimitCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(LowerLimit,
                        boost::bind(&HingeConstraint::getLowerLimitCB, this, _1),
                        boost::bind(&HingeConstraint::setLowerLimitCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(MotorEnabled,
                        boost::bind(&HingeConstraint::getMotorEnabledCB, this, _1),
                        boost::bind(&HingeConstraint::setMotorEnabledCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(MotorTargetVelocity,
                        boost::bind(&HingeConstraint::getMotorTargetVelocityCB, this, _1),
                        boost::bind(&HingeConstraint::setMotorTargetVelocityCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(MotorMaxImpulse,
                        boost::bind(&HingeConstraint::getMotorMaxImpulseCB, this, _1),
                        boost::bind(&HingeConstraint::setMotorMaxImpulseCB, this, _1));
}

av::gua::HingeConstraint::~HingeConstraint()
{}

void
av::gua::HingeConstraint::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::Constraint::initClass();

        AV_FC_INIT_ABSTRACT(av::gua::Constraint, av::gua::HingeConstraint, true);

        SFHingeConstraint::initClass("av::gua::SFHingeConstraint", "av::Field");
        MFHingeConstraint::initClass("av::gua::MFHingeConstraint", "av::Field");
    }
}

::gua::physics::HingeConstraint*
av::gua::HingeConstraint::getGuaConstraint() const
{
    return m_guaConstraint;
}

void
av::gua::HingeConstraint::getPivotACB(const SFVec3::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaConstraint->pivot_a();
}

void
av::gua::HingeConstraint::setPivotACB(const SFVec3::SetValueEvent& event)
{
    m_guaConstraint->set_pivot_a(event.getValue());
}

void
av::gua::HingeConstraint::getPivotBCB(const SFVec3::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaConstraint->pivot_b();
}

void
av::gua::HingeConstraint::setPivotBCB(const SFVec3::SetValueEvent& event)
{
    m_guaConstraint->set_pivot_b(event.getValue());
}

void
av::gua::HingeConstraint::getAxisACB(const SFVec3::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaConstraint->axis_a();
}

void
av::gua::HingeConstraint::setAxisACB(const SFVec3::SetValueEvent& event)
{
    m_guaConstraint->set_axis_a(event.getValue());
}

void
av::gua::HingeConstraint::getAxisBCB(const SFVec3::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaConstraint->axis_b();
}

void
av::gua::HingeConstraint::setAxisBCB(const SFVec3::SetValueEvent& event)
{
    m_guaConstraint->set_axis_b(event.getValue());
}

void
av::gua::HingeConstraint::getUpperLimitCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaConstraint->upper_limit();
}

void
av::gua::HingeConstraint::setUpperLimitCB(const SFFloat::SetValueEvent& event)
{
    m_guaConstraint->set_upper_limit(event.getValue());
}

void
av::gua::HingeConstraint::getLowerLimitCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaConstraint->lower_limit();
}

void
av::gua::HingeConstraint::setLowerLimitCB(const SFFloat::SetValueEvent& event)
{
    m_guaConstraint->set_lower_limit(event.getValue());
}

void
av::gua::HingeConstraint::getMotorEnabledCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaConstraint->motor_enabled();
}

void
av::gua::HingeConstraint::setMotorEnabledCB(const SFBool::SetValueEvent& event)
{
    m_guaConstraint->enable_motor(event.getValue());
}

void
av::gua::HingeConstraint::getMotorTargetVelocityCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaConstraint->motor_target_velocity();
}

void
av::gua::HingeConstraint::setMotorTargetVelocityCB(const SFFloat::SetValueEvent& event)
{
    m_guaConstraint->enable_angular_motor(m_guaConstraint->motor_enabled(),
                                          event.getValue(),
                                          m_guaConstraint->motor_target_impulse());
}

void
av::gua::HingeConstraint::getMotorMaxImpulseCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaConstraint->motor_target_impulse();
}

void
av::gua::HingeConstraint::setMotorMaxImpulseCB(const SFFloat::SetValueEvent& event)
{
    m_guaConstraint->enable_angular_motor(m_guaConstraint->motor_enabled(),
                                          m_guaConstraint->motor_target_velocity(),
                                          event.getValue());
}
