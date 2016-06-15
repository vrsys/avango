#include <avango/gua/scenegraph/ClippingPlaneNode.hpp>

#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::ClippingPlaneNode);

AV_FIELD_DEFINE(av::gua::SFClippingPlaneNode);
AV_FIELD_DEFINE(av::gua::MFClippingPlaneNode);

av::gua::ClippingPlaneNode::ClippingPlaneNode(std::shared_ptr< ::gua::node::ClippingPlaneNode> guanode)
    : av::gua::Node(guanode),
      m_guaNode(std::dynamic_pointer_cast< ::gua::node::ClippingPlaneNode>(Node::getGuaNode()))
{
    AV_FC_ADD_ADAPTOR_FIELD(ViewIDs,
                          std::bind(&ClippingPlaneNode::getViewIDsCB, this,std::placeholders::_1),
                          std::bind(&ClippingPlaneNode::setViewIDsCB, this,std::placeholders::_1));
}

av::gua::ClippingPlaneNode::~ClippingPlaneNode()
{}

void
av::gua::ClippingPlaneNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::Node::initClass();

        AV_FC_INIT(av::gua::Node, av::gua::ClippingPlaneNode, true);

        SFClippingPlaneNode::initClass("av::gua::SFClippingPlaneNode", "av::Field");
        MFClippingPlaneNode::initClass("av::gua::MFClippingPlaneNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr< ::gua::node::ClippingPlaneNode>
av::gua::ClippingPlaneNode::getGuaNode() const
{
    return m_guaNode;
}

void
av::gua::ClippingPlaneNode::getViewIDsCB(const MFInt::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->config.view_ids();
}

void
av::gua::ClippingPlaneNode::setViewIDsCB(const MFInt::SetValueEvent& event)
{
    m_guaNode->config.view_ids() = event.getValue();
}
