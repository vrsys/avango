#include <avango/gua/scenegraph/NURBSNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::NURBSNode);

AV_FIELD_DEFINE(av::gua::SFNURBSNode);
AV_FIELD_DEFINE(av::gua::MFNURBSNode);

av::gua::NURBSNode::NURBSNode(std::shared_ptr< ::gua::NURBSNode> guanode)
    : Node(guanode),
      m_guaNode(std::dynamic_pointer_cast< ::gua::NURBSNode>(Node::getGuaNode()))
{
  AV_FC_ADD_ADAPTOR_FIELD(Geometry,
                        boost::bind(&NURBSNode::getGeometryCB, this, _1),
                        boost::bind(&NURBSNode::setGeometryCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Material,
                        boost::bind(&NURBSNode::getMaterialCB, this, _1),
                        boost::bind(&NURBSNode::setMaterialCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(ShadowMode,
                        boost::bind(&NURBSNode::getShadowModeCB, this, _1),
                        boost::bind(&NURBSNode::setShadowModeCB, this, _1));
}

//av::gua::NURBSNode::~NURBSNode()
//{}

void
av::gua::NURBSNode::initClass()
{
  if (!isTypeInitialized()) {
    av::gua::Node::initClass();

    AV_FC_INIT(av::gua::Node, av::gua::NURBSNode, true);

    SFNURBSNode::initClass("av::gua::SFNURBSNode", "av::Field");
    MFNURBSNode::initClass("av::gua::MFNURBSNode", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

std::shared_ptr< ::gua::NURBSNode>
av::gua::NURBSNode::getGuaNode() const
{
  return m_guaNode;
}

void
av::gua::NURBSNode::getGeometryCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->get_filename();
}

void
av::gua::NURBSNode::setGeometryCB(const SFString::SetValueEvent& event)
{
  m_guaNode->set_filename(event.getValue());
}

void
av::gua::NURBSNode::getMaterialCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->get_material();
}

void
av::gua::NURBSNode::setMaterialCB(const SFString::SetValueEvent& event)
{
  m_guaNode->set_material(event.getValue());
}

void
av::gua::NURBSNode::getShadowModeCB(const SFUInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = static_cast<unsigned>(m_guaNode->get_shadow_mode());
}

void
av::gua::NURBSNode::setShadowModeCB(const SFUInt::SetValueEvent& event)
{
  m_guaNode->set_shadow_mode(static_cast< ::gua::ShadowMode>(event.getValue()));
}
