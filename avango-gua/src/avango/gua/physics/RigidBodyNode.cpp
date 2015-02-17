#include <avango/gua/physics/RigidBodyNode.hpp>
#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::RigidBodyNode"));
}

AV_FC_DEFINE(av::gua::RigidBodyNode);

AV_FIELD_DEFINE(av::gua::SFRigidBodyNode);
AV_FIELD_DEFINE(av::gua::MFRigidBodyNode);

av::gua::RigidBodyNode::RigidBodyNode(std::shared_ptr< ::gua::physics::RigidBodyNode> guanode)
  : Node(guanode),
    m_guaNode(guanode)
{
    AV_FC_ADD_ADAPTOR_FIELD(IsKinematic,
                        boost::bind(&RigidBodyNode::getIsKinematicCB, this, _1),
                        boost::bind(&RigidBodyNode::setIsKinematicCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Mass,
                        boost::bind(&RigidBodyNode::getMassCB, this, _1),
                        boost::bind(&RigidBodyNode::setMassCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Friction,
                        boost::bind(&RigidBodyNode::getFrictionCB, this, _1),
                        boost::bind(&RigidBodyNode::setFrictionCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(RollingFriction,
                        boost::bind(&RigidBodyNode::getRollingFrictionCB, this, _1),
                        boost::bind(&RigidBodyNode::setRollingFrictionCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Restitution,
                        boost::bind(&RigidBodyNode::getRestitutionCB, this, _1),
                        boost::bind(&RigidBodyNode::setRestitutionCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(LinearDamping,
                        boost::bind(&RigidBodyNode::getLinearDampingCB, this, _1),
                        boost::bind(&RigidBodyNode::setLinearDampingCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(AngularDamping,
                        boost::bind(&RigidBodyNode::getAngularDampingCB, this, _1),
                        boost::bind(&RigidBodyNode::setAngularDampingCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(LinearVelocity,
                        boost::bind(&RigidBodyNode::getLinearVelocityCB, this, _1),
                        boost::bind(&RigidBodyNode::setLinearVelocityCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(AngularVelocity,
                        boost::bind(&RigidBodyNode::getAngularVelocityCB, this, _1),
                        boost::bind(&RigidBodyNode::setAngularVelocityCB, this, _1));

    AV_FC_ADD_FIELD        (Group, static_cast<int>(RigidBodyNode::CollisionFilterGroups::STATIC_FILTER));
    AV_FC_ADD_FIELD        (Mask,  static_cast<int>(RigidBodyNode::CollisionFilterGroups::ALL_FILTER) ^ static_cast<int>(RigidBodyNode::CollisionFilterGroups::STATIC_FILTER));

    Group.setValue(static_cast<int>(CollisionFilterGroups::NONE));
    Mask.setValue(static_cast<int>(CollisionFilterGroups::NONE));
}

av::gua::RigidBodyNode::~RigidBodyNode()
{}

void
av::gua::RigidBodyNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::Node::initClass();

        AV_FC_INIT(av::gua::Node, av::gua::RigidBodyNode, true);

        SFRigidBodyNode::initClass("av::gua::SFRigidBodyNode", "av::Field");
        MFRigidBodyNode::initClass("av::gua::MFRigidBodyNode", "av::Field");
    }
}

std::shared_ptr< ::gua::physics::RigidBodyNode>
av::gua::RigidBodyNode::getGuaNode() const
{
    return m_guaNode;
}

void
av::gua::RigidBodyNode::getIsKinematicCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->is_kinematic();
}

void
av::gua::RigidBodyNode::setIsKinematicCB(const SFBool::SetValueEvent& event)
{
    m_guaNode->set_kinematic(event.getValue());
}

void
av::gua::RigidBodyNode::getMassCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->mass();
}

void
av::gua::RigidBodyNode::setMassCB(const SFFloat::SetValueEvent& event)
{
    m_guaNode->set_mass(event.getValue());
}

void
av::gua::RigidBodyNode::getFrictionCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->friction();
}

void
av::gua::RigidBodyNode::setFrictionCB(const SFFloat::SetValueEvent& event)
{
    m_guaNode->set_friction(event.getValue());
}

void
av::gua::RigidBodyNode::getRollingFrictionCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->rolling_friction();
}

void
av::gua::RigidBodyNode::setRollingFrictionCB(const SFFloat::SetValueEvent& event)
{
    m_guaNode->set_rolling_friction(event.getValue());
}

void
av::gua::RigidBodyNode::getRestitutionCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->restitution();
}

void
av::gua::RigidBodyNode::setRestitutionCB(const SFFloat::SetValueEvent& event)
{
    m_guaNode->set_restitution(event.getValue());
}

void
av::gua::RigidBodyNode::getLinearDampingCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->linear_damping();
}

void
av::gua::RigidBodyNode::setLinearDampingCB(const SFFloat::SetValueEvent& event)
{
    m_guaNode->set_damping(event.getValue(), AngularDamping.getValue());
}

void
av::gua::RigidBodyNode::getAngularDampingCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->angular_damping();
}

void
av::gua::RigidBodyNode::setAngularDampingCB(const SFFloat::SetValueEvent& event)
{
    m_guaNode->set_damping(LinearDamping.getValue(), event.getValue());
}

void
av::gua::RigidBodyNode::getLinearVelocityCB(const SFVec3::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->linear_velocity();
}

void
av::gua::RigidBodyNode::setLinearVelocityCB(const SFVec3::SetValueEvent& event)
{
    m_guaNode->set_linear_velocity(event.getValue());
}

void
av::gua::RigidBodyNode::getAngularVelocityCB(const SFVec3::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->angular_velocity();
}

void
av::gua::RigidBodyNode::setAngularVelocityCB(const SFVec3::SetValueEvent& event)
{
    m_guaNode->set_angular_velocity(event.getValue());
}
