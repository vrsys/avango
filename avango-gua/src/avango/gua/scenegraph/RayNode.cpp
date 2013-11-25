#include <avango/gua/scenegraph/RayNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::RayNode);

AV_FIELD_DEFINE(av::gua::SFRayNode);
AV_FIELD_DEFINE(av::gua::MFRayNode);

av::gua::RayNode::RayNode(std::shared_ptr< ::gua::RayNode> guanode)
    : av::gua::Node(guanode),
      m_guaNode(std::dynamic_pointer_cast< ::gua::RayNode>(Node::getGuaNode()))
{}

//av::gua::RayNode::~RayNode()
//{}

void
av::gua::RayNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::Node::initClass();

        AV_FC_INIT(av::gua::Node, av::gua::RayNode, true);

        SFRayNode::initClass("av::gua::SFRayNode", "av::Field");
        MFRayNode::initClass("av::gua::MFRayNode", "av::Field");
    }
}

std::shared_ptr< ::gua::RayNode>
av::gua::RayNode::getGuaNode() const
{
    return m_guaNode;
}
