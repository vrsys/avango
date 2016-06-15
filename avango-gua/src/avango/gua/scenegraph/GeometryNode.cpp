#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE_ABSTRACT(av::gua::GeometryNode);

AV_FIELD_DEFINE(av::gua::SFGeometryNode);
AV_FIELD_DEFINE(av::gua::MFGeometryNode);

av::gua::GeometryNode::GeometryNode(std::shared_ptr< ::gua::node::GeometryNode> guanode)
  : Node(guanode),
    m_guaNode(std::dynamic_pointer_cast< ::gua::node::GeometryNode>(Node::getGuaNode()))
{

  AV_FC_ADD_ADAPTOR_FIELD(ShadowMode,
                        std::bind(&GeometryNode::getShadowModeCB, this,std::placeholders::_1),
                        std::bind(&GeometryNode::setShadowModeCB, this,std::placeholders::_1));
}

void
av::gua::GeometryNode::initClass()
{
  if (!isTypeInitialized()) {
    av::gua::Node::initClass();

    AV_FC_INIT_ABSTRACT(av::gua::Node, av::gua::GeometryNode, true);

    SFGeometryNode::initClass("av::gua::SFGeometryNode", "av::Field");
    MFGeometryNode::initClass("av::gua::MFGeometryNode", "av::Field");

    //sClassTypeId.setDistributable(true);
  }
}

std::shared_ptr< ::gua::node::GeometryNode>
av::gua::GeometryNode::getGuaNode() const
{
  return m_guaNode;
}

void
av::gua::GeometryNode::getShadowModeCB(const SFUInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = static_cast<unsigned>(m_guaNode->get_shadow_mode());
}

void
av::gua::GeometryNode::setShadowModeCB(const SFUInt::SetValueEvent& event)
{
  m_guaNode->set_shadow_mode(static_cast< ::gua::ShadowMode>(event.getValue()));
}
