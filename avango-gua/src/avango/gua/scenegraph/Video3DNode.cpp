#include <avango/gua/scenegraph/Video3DNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::Video3DNode);

AV_FIELD_DEFINE(av::gua::SFVideo3DNode);
AV_FIELD_DEFINE(av::gua::MFVideo3DNode);

av::gua::Video3DNode::Video3DNode(std::shared_ptr< ::gua::Video3DNode> guanode)
    : Node(guanode),
      m_guaNode(std::dynamic_pointer_cast< ::gua::Video3DNode>(Node::getGuaNode()))
{
  AV_FC_ADD_ADAPTOR_FIELD(Geometry,
                        boost::bind(&Video3DNode::getGeometryCB, this, _1),
                        boost::bind(&Video3DNode::setGeometryCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Material,
                        boost::bind(&Video3DNode::getMaterialCB, this, _1),
                        boost::bind(&Video3DNode::setMaterialCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(ShadowMode,
                        boost::bind(&Video3DNode::getShadowModeCB, this, _1),
                        boost::bind(&Video3DNode::setShadowModeCB, this, _1));
}

//av::gua::Video3DNode::~Video3DNode()
//{}

void
av::gua::Video3DNode::initClass()
{
  if (!isTypeInitialized()) {
    av::gua::Node::initClass();

    AV_FC_INIT(av::gua::Node, av::gua::Video3DNode, true);

    SFVideo3DNode::initClass("av::gua::SFVideo3DNode", "av::Field");
    MFVideo3DNode::initClass("av::gua::MFVideo3DNode", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

std::shared_ptr< ::gua::Video3DNode>
av::gua::Video3DNode::getGuaNode() const
{
  return m_guaNode;
}

void
av::gua::Video3DNode::getGeometryCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->get_filename();
}

void
av::gua::Video3DNode::setGeometryCB(const SFString::SetValueEvent& event)
{
  m_guaNode->set_filename(event.getValue());
}

void
av::gua::Video3DNode::getMaterialCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->get_material();
}

void
av::gua::Video3DNode::setMaterialCB(const SFString::SetValueEvent& event)
{
  m_guaNode->set_material(event.getValue());
}

void
av::gua::Video3DNode::getShadowModeCB(const SFUInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = static_cast<unsigned>(m_guaNode->get_shadow_mode());
}

void
av::gua::Video3DNode::setShadowModeCB(const SFUInt::SetValueEvent& event)
{
  m_guaNode->set_shadow_mode(static_cast< ::gua::ShadowMode>(event.getValue()));
}
