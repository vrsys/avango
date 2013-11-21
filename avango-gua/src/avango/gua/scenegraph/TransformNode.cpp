#include <avango/gua/scenegraph/TransformNode.hpp>
#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::TransformNode"));
}

AV_FC_DEFINE(av::gua::TransformNode);

AV_FIELD_DEFINE(av::gua::SFTransformNode);
AV_FIELD_DEFINE(av::gua::MFTransformNode);

av::gua::TransformNode::TransformNode(std::shared_ptr< ::gua::TransformNode> guanode)
  : Node(guanode),
    m_guaNode(std::dynamic_pointer_cast< ::gua::TransformNode> (Node::getGuaNode()))
{}

av::gua::TransformNode::~TransformNode()
{}

void
av::gua::TransformNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::Node::initClass();

        AV_FC_INIT(av::gua::Node, av::gua::TransformNode, true);

        SFTransformNode::initClass("av::gua::SFTransformNode", "av::Field");
        MFTransformNode::initClass("av::gua::MFTransformNode", "av::Field");
    }
}

std::shared_ptr< ::gua::TransformNode>
av::gua::TransformNode::getGuaNode() const
{
    return m_guaNode;
}
