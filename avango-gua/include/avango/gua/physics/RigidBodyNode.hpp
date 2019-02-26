#ifndef AVANGO_GUA_RIGID_BODY_NODE_HPP
#define AVANGO_GUA_RIGID_BODY_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/physics/RigidBodyNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/TransformNode.hpp>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::physics::RigidBodyNode
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL RigidBodyNode : public av::gua::TransformNode
{
    AV_FC_DECLARE();

  public:
    enum class CollisionFilterGroups
    {
        NONE = -1,
        DEFAULT_FILTER = ::gua::physics::RigidBodyNode::CollisionFilterGroups::DEFAULT_FILTER,
        STATIC_FILTER = ::gua::physics::RigidBodyNode::CollisionFilterGroups::STATIC_FILTER,
        KINEMATIC_FILTER = ::gua::physics::RigidBodyNode::CollisionFilterGroups::KINEMATIC_FILTER,
        DEBRIS_FILTER = ::gua::physics::RigidBodyNode::CollisionFilterGroups::DEBRIS_FILTER,
        SENSOR_TRIGGER = ::gua::physics::RigidBodyNode::CollisionFilterGroups::SENSOR_TRIGGER,
        CHARACTER_FILTER = ::gua::physics::RigidBodyNode::CollisionFilterGroups::CHARACTER_FILTER,
        ALL_FILTER = ::gua::physics::RigidBodyNode::CollisionFilterGroups::ALL_FILTER
    };

    /**
     * Constructor. When called without arguments, a new ::gua::physics::RigidBodyNode is created.
     * Otherwise, the given ::gua::physics::RigidBodyNode is used.
     */
    RigidBodyNode(std::shared_ptr<::gua::physics::RigidBodyNode> guanode = std::shared_ptr<::gua::physics::RigidBodyNode>(new ::gua::physics::RigidBodyNode("")));

    virtual ~RigidBodyNode();

  public:
    SFBool IsKinematic;
    SFFloat Mass;
    SFFloat Friction;
    SFFloat RollingFriction;
    SFFloat Restitution;
    SFFloat LinearDamping;
    SFFloat AngularDamping;
    SFVec3 LinearVelocity;
    SFVec3 AngularVelocity;
    SFInt Group;
    SFInt Mask;

    /**
     * Get the wrapped ::gua::physics::RigidBodyNode.
     */
    std::shared_ptr<::gua::physics::RigidBodyNode> getGuaNode() const;

  public:
    virtual void getIsKinematicCB(const SFBool::GetValueEvent& event);
    virtual void setIsKinematicCB(const SFBool::SetValueEvent& event);

    virtual void getMassCB(const SFFloat::GetValueEvent& event);
    virtual void setMassCB(const SFFloat::SetValueEvent& event);

    virtual void getFrictionCB(const SFFloat::GetValueEvent& event);
    virtual void setFrictionCB(const SFFloat::SetValueEvent& event);

    virtual void getRollingFrictionCB(const SFFloat::GetValueEvent& event);
    virtual void setRollingFrictionCB(const SFFloat::SetValueEvent& event);

    virtual void getRestitutionCB(const SFFloat::GetValueEvent& event);
    virtual void setRestitutionCB(const SFFloat::SetValueEvent& event);

    virtual void getLinearDampingCB(const SFFloat::GetValueEvent& event);
    virtual void setLinearDampingCB(const SFFloat::SetValueEvent& event);

    virtual void getAngularDampingCB(const SFFloat::GetValueEvent& event);
    virtual void setAngularDampingCB(const SFFloat::SetValueEvent& event);

    virtual void getLinearVelocityCB(const SFVec3::GetValueEvent& event);
    virtual void setLinearVelocityCB(const SFVec3::SetValueEvent& event);

    virtual void getAngularVelocityCB(const SFVec3::GetValueEvent& event);
    virtual void setAngularVelocityCB(const SFVec3::SetValueEvent& event);

  private:
    std::shared_ptr<::gua::physics::RigidBodyNode> m_guaNode;

    RigidBodyNode(const RigidBodyNode&);
    RigidBodyNode& operator=(const RigidBodyNode&);
};

using SFRigidBodyNode = SingleField<Link<RigidBodyNode>>;
using MFRigidBodyNode = MultiField<Link<RigidBodyNode>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::RigidBodyNode>>;
template class AV_GUA_DLL MultiField<Link<gua::RigidBodyNode>>;
#endif

} // namespace av

#endif // AVANGO_GUA_RIGID_BODY_NODE_HPP
