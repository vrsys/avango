#include <avango/gua/scenegraph/ScreenNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::ScreenNode);

AV_FIELD_DEFINE(av::gua::SFScreenNode);
AV_FIELD_DEFINE(av::gua::MFScreenNode);

av::gua::ScreenNode::ScreenNode(std::shared_ptr< ::gua::node::ScreenNode> guanode)
    : Node(guanode),
      m_guaNode(std::dynamic_pointer_cast< ::gua::node::ScreenNode>(Node::getGuaNode()))
{
    AV_FC_ADD_ADAPTOR_FIELD(Width,
                          boost::bind(&ScreenNode::getWidthCB, this, _1),
                          boost::bind(&ScreenNode::setWidthCB, this, _1));
    AV_FC_ADD_ADAPTOR_FIELD(Height,
                          boost::bind(&ScreenNode::getHeightCB, this, _1),
                          boost::bind(&ScreenNode::setHeightCB, this, _1));
}

av::gua::ScreenNode::~ScreenNode()
{}

void
av::gua::ScreenNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::Node::initClass();

        AV_FC_INIT(av::gua::Node, av::gua::ScreenNode, true);

        SFScreenNode::initClass("av::gua::SFScreenNode", "av::Field");
        MFScreenNode::initClass("av::gua::MFScreenNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr< ::gua::node::ScreenNode>
av::gua::ScreenNode::getGuaNode() const
{
    return m_guaNode;
}

void
av::gua::ScreenNode::getWidthCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.size()[0];
}

void
av::gua::ScreenNode::setWidthCB(const SFFloat::SetValueEvent& event)
{
    m_guaNode->data.size()[0] = event.getValue();
}

void
av::gua::ScreenNode::getHeightCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.size()[1];
}

void
av::gua::ScreenNode::setHeightCB(const SFFloat::SetValueEvent& event)
{
    m_guaNode->data.size()[1] = event.getValue();
}
