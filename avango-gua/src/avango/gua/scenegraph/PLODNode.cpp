#include <avango/gua/scenegraph/PLODNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::PLODNode);

AV_FIELD_DEFINE(av::gua::SFPLODNode);
AV_FIELD_DEFINE(av::gua::MFPLODNode);

av::gua::PLODNode::PLODNode(std::shared_ptr< ::gua::PLODNode> guanode)
    : Node(guanode),
      m_guaNode(std::dynamic_pointer_cast< ::gua::PLODNode>(Node::getGuaNode()))
{
  AV_FC_ADD_ADAPTOR_FIELD(Geometry,
                        boost::bind(&PLODNode::getGeometryCB, this, _1),
                        boost::bind(&PLODNode::setGeometryCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Material,
                        boost::bind(&PLODNode::getMaterialCB, this, _1),
                        boost::bind(&PLODNode::setMaterialCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(ShadowMode,
                        boost::bind(&PLODNode::getShadowModeCB, this, _1),
                        boost::bind(&PLODNode::setShadowModeCB, this, _1));
}

//av::gua::PLODNode::~PLODNode()
//{}

void
av::gua::PLODNode::initClass()
{
  if (!isTypeInitialized()) {
    av::gua::Node::initClass();

    AV_FC_INIT(av::gua::Node, av::gua::PLODNode, true);

    SFPLODNode::initClass("av::gua::SFPLODNode", "av::Field");
    MFPLODNode::initClass("av::gua::MFPLODNode", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

std::shared_ptr< ::gua::PLODNode>
av::gua::PLODNode::getGuaNode() const
{
  return m_guaNode;
}

void
av::gua::PLODNode::getGeometryCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->get_filename();
}

void
av::gua::PLODNode::setGeometryCB(const SFString::SetValueEvent& event)
{
  m_guaNode->set_filename(event.getValue());
}

void
av::gua::PLODNode::getMaterialCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->get_material();
}

void
av::gua::PLODNode::setMaterialCB(const SFString::SetValueEvent& event)
{
  m_guaNode->set_material(event.getValue());
}

void
av::gua::PLODNode::getShadowModeCB(const SFUInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = static_cast<unsigned>(m_guaNode->get_shadow_mode());
}

void
av::gua::PLODNode::setShadowModeCB(const SFUInt::SetValueEvent& event)
{
  m_guaNode->set_shadow_mode(static_cast< ::gua::ShadowMode>(event.getValue()));
}
