#include <avango/gua/scenegraph/ScreenNode.hpp>
#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::ScreenNode);

AV_FIELD_DEFINE(av::gua::SFScreenNode);
AV_FIELD_DEFINE(av::gua::MFScreenNode);

av::gua::ScreenNode::ScreenNode(std::shared_ptr<::gua::node::ScreenNode> guanode) : Node(guanode), m_guaNode(guanode)
{
    AV_FC_ADD_ADAPTOR_FIELD(Width, std::bind(&ScreenNode::getWidthCB, this, std::placeholders::_1), std::bind(&ScreenNode::setWidthCB, this, std::placeholders::_1));
    AV_FC_ADD_ADAPTOR_FIELD(Height, std::bind(&ScreenNode::getHeightCB, this, std::placeholders::_1), std::bind(&ScreenNode::setHeightCB, this, std::placeholders::_1));
    AV_FC_ADD_ADAPTOR_FIELD(
        ScaledWorldTransform, std::bind(&ScreenNode::getScaledWorldTransformCB, this, std::placeholders::_1), std::bind(&ScreenNode::setScaledWorldTransformCB, this, std::placeholders::_1));
}

av::gua::ScreenNode::~ScreenNode() {}

void av::gua::ScreenNode::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::Node::initClass();

        AV_FC_INIT(av::gua::Node, av::gua::ScreenNode, true);

        SFScreenNode::initClass("av::gua::SFScreenNode", "av::Field");
        MFScreenNode::initClass("av::gua::MFScreenNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr<::gua::node::ScreenNode> av::gua::ScreenNode::getGuaNode() const { return m_guaNode; }

void av::gua::ScreenNode::getWidthCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaNode->data.size()[0]; }

void av::gua::ScreenNode::setWidthCB(const SFFloat::SetValueEvent& event) { m_guaNode->data.size()[0] = event.getValue(); }

void av::gua::ScreenNode::getHeightCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaNode->data.size()[1]; }

void av::gua::ScreenNode::setHeightCB(const SFFloat::SetValueEvent& event) { m_guaNode->data.size()[1] = event.getValue(); }

void av::gua::ScreenNode::getScaledWorldTransformCB(const SFMatrix::GetValueEvent& event) { *(event.getValuePtr()) = m_guaNode->get_scaled_world_transform(); }

void av::gua::ScreenNode::setScaledWorldTransformCB(const SFMatrix::SetValueEvent& event) {}
