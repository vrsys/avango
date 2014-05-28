#include <avango/gua/scenegraph/PBRNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::PBRNode);

AV_FIELD_DEFINE(av::gua::SFPBRNode);
AV_FIELD_DEFINE(av::gua::MFPBRNode);

av::gua::PBRNode::PBRNode(std::shared_ptr< ::gua::PBRNode> guanode)
    : Node(guanode),
      m_guaNode(std::dynamic_pointer_cast< ::gua::PBRNode>(Node::getGuaNode()))
{
  AV_FC_ADD_ADAPTOR_FIELD(Geometry,
                        boost::bind(&PBRNode::getGeometryCB, this, _1),
                        boost::bind(&PBRNode::setGeometryCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Material,
                        boost::bind(&PBRNode::getMaterialCB, this, _1),
                        boost::bind(&PBRNode::setMaterialCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(ShadowMode,
                        boost::bind(&PBRNode::getShadowModeCB, this, _1),
                        boost::bind(&PBRNode::setShadowModeCB, this, _1));
}

//av::gua::PBRNode::~PBRNode()
//{}

void
av::gua::PBRNode::initClass()
{
  if (!isTypeInitialized()) {
    av::gua::Node::initClass();

    AV_FC_INIT(av::gua::Node, av::gua::PBRNode, true);

    SFPBRNode::initClass("av::gua::SFPBRNode", "av::Field");
    MFPBRNode::initClass("av::gua::MFPBRNode", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

std::shared_ptr< ::gua::PBRNode>
av::gua::PBRNode::getGuaNode() const
{
  return m_guaNode;
}

void
av::gua::PBRNode::getGeometryCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->get_filename();
}

void
av::gua::PBRNode::setGeometryCB(const SFString::SetValueEvent& event)
{
  m_guaNode->set_filename(event.getValue());
}

void
av::gua::PBRNode::getMaterialCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->get_material();
}

void
av::gua::PBRNode::setMaterialCB(const SFString::SetValueEvent& event)
{
  m_guaNode->set_material(event.getValue());
}

void
av::gua::PBRNode::getShadowModeCB(const SFUInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = static_cast<unsigned>(m_guaNode->get_shadow_mode());
}

void
av::gua::PBRNode::setShadowModeCB(const SFUInt::SetValueEvent& event)
{
  m_guaNode->set_shadow_mode(static_cast< ::gua::ShadowMode>(event.getValue()));
}
