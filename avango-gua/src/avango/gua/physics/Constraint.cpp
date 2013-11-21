#include <avango/gua/physics/Constraint.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::Constraint"));
}

AV_FC_DEFINE_ABSTRACT(av::gua::Constraint);

AV_FIELD_DEFINE(av::gua::SFConstraint);
AV_FIELD_DEFINE(av::gua::MFConstraint);

av::gua::Constraint::Constraint(::gua::physics::Constraint* guaconstraint)
  : m_guaConstraint(guaconstraint)
{
    AV_FC_ADD_ADAPTOR_FIELD(BodyA,
                        boost::bind(&Constraint::getBodyACB, this, _1),
                        boost::bind(&Constraint::setBodyACB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(BodyB,
                        boost::bind(&Constraint::getBodyBCB, this, _1),
                        boost::bind(&Constraint::setBodyBCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Enabled,
                        boost::bind(&Constraint::getEnabledCB, this, _1),
                        boost::bind(&Constraint::setEnabledCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(BreakingImpulseThreshold,
                        boost::bind(&Constraint::getBreakingImpulseThresholdCB, this, _1),
                        boost::bind(&Constraint::setBreakingImpulseThresholdCB, this, _1));

    AV_FC_ADD_FIELD        (DisableCollisionBetweenLinkedBodies, false);
}

av::gua::Constraint::~Constraint()
{}

void
av::gua::Constraint::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT_ABSTRACT(av::FieldContainer, av::gua::Constraint, true);

        SFConstraint::initClass("av::gua::SFConstraint", "av::Field");
        MFConstraint::initClass("av::gua::MFConstraint", "av::Field");
    }
}

::gua::physics::Constraint*
av::gua::Constraint::getGuaConstraint() const
{
    return m_guaConstraint;
}

void
av::gua::Constraint::getBodyACB(const SFRigidBodyNode::GetValueEvent& event)
{
    if (m_guaConstraint->body_a()) {
        auto handle(BodyA.getValue()->getUserDataHandle());
        auto user_data(m_guaConstraint->body_a()->get_user_data(handle));
        if (user_data) {
            *(event.getValuePtr()) = static_cast<av::gua::RigidBodyNode*>(user_data);
        }
    }
}

void
av::gua::Constraint::setBodyACB(const SFRigidBodyNode::SetValueEvent& event)
{
    m_guaConstraint->set_body_a(event.getValue()->getGuaNode().get());
}

void
av::gua::Constraint::getBodyBCB(const SFRigidBodyNode::GetValueEvent& event)
{
    if (m_guaConstraint->body_b()) {
        auto handle(BodyB.getValue()->getUserDataHandle());
        auto user_data(m_guaConstraint->body_b()->get_user_data(handle));
        if (user_data) {
            *(event.getValuePtr()) = static_cast<av::gua::RigidBodyNode*>(user_data);
        }
    }
}

void
av::gua::Constraint::setBodyBCB(const SFRigidBodyNode::SetValueEvent& event)
{
    m_guaConstraint->set_body_b(event.getValue()->getGuaNode().get());
}

void
av::gua::Constraint::getEnabledCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaConstraint->enabled();
}

void
av::gua::Constraint::setEnabledCB(const SFBool::SetValueEvent& event)
{
    m_guaConstraint->set_enabled(event.getValue());
}

void
av::gua::Constraint::getBreakingImpulseThresholdCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaConstraint->breaking_impulse_threshold();
}

void
av::gua::Constraint::setBreakingImpulseThresholdCB(const SFFloat::SetValueEvent& event)
{
    m_guaConstraint->set_breaking_impulse_threshold(event.getValue());
}
