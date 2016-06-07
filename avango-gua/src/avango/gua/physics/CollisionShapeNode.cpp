#include <avango/gua/physics/CollisionShapeNode.hpp>
#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::CollisionShapeNode"));
}

AV_FC_DEFINE(av::gua::CollisionShapeNode);

AV_FIELD_DEFINE(av::gua::SFCollisionShapeNode);
AV_FIELD_DEFINE(av::gua::MFCollisionShapeNode);

av::gua::CollisionShapeNode::CollisionShapeNode(std::shared_ptr< ::gua::physics::CollisionShapeNode> guanode)
  : TransformNode(std::dynamic_pointer_cast<::gua::node::TransformNode>(guanode)),
    m_guaNode(guanode)
{
    AV_FC_ADD_ADAPTOR_FIELD(ShapeName,
                        boost::bind(&CollisionShapeNode::getShapeNameCB, this, _1),
                        boost::bind(&CollisionShapeNode::setShapeNameCB, this, _1));
}

av::gua::CollisionShapeNode::~CollisionShapeNode()
{}

void
av::gua::CollisionShapeNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::TransformNode::initClass();

        AV_FC_INIT(av::gua::TransformNode, av::gua::CollisionShapeNode, true);

        SFCollisionShapeNode::initClass("av::gua::SFCollisionShapeNode", "av::Field");
        MFCollisionShapeNode::initClass("av::gua::MFCollisionShapeNode", "av::Field");
    }
}

std::shared_ptr< ::gua::physics::CollisionShapeNode>
av::gua::CollisionShapeNode::getGuaNode() const
{
    return m_guaNode;
}

void
av::gua::CollisionShapeNode::getShapeNameCB(const SFString::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.shape();
}

void
av::gua::CollisionShapeNode::setShapeNameCB(const SFString::SetValueEvent& event)
{
    m_guaNode->data.shape() = event.getValue();
}
