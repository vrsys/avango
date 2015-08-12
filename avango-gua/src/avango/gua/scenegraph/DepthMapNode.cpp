#include <avango/gua/scenegraph/DepthMapNode.hpp>
#include <avango/gua/network/NetTransform.h>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::DepthMapNode);

AV_FIELD_DEFINE(av::gua::SFDepthMapNode);
AV_FIELD_DEFINE(av::gua::MFDepthMapNode);

av::gua::DepthMapNode::DepthMapNode(std::shared_ptr< ::gua::node::CubemapNode> guanode)
    : Node(guanode)
    , m_guaDepthMapNode(guanode)
{
  AV_FC_ADD_ADAPTOR_FIELD(ClosestDistance,
                      boost::bind(&DepthMapNode::getClosestDistanceCB, this, _1),
                      boost::bind(&DepthMapNode::setClosestDistanceCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(TextureName,
                      boost::bind(&DepthMapNode::getTextureNameCB, this, _1),
                      boost::bind(&DepthMapNode::setTextureNameCB, this, _1));
}

//av::gua::DepthMapNode::~DepthMapNode()
//{}

void av::gua::DepthMapNode::on_distribute(av::gua::NetTransform& netNode) 
{
    Node::on_distribute(netNode);
}

void av::gua::DepthMapNode::on_undistribute(av::gua::NetTransform& netNode) 
{
    Node::on_undistribute(netNode);    
}


void
av::gua::DepthMapNode::initClass()
{
  if (!isTypeInitialized()) {
    av::gua::Node::initClass();

    AV_FC_INIT(av::gua::Node, av::gua::DepthMapNode, true);

    SFDepthMapNode::initClass("av::gua::SFDepthMapNode", "av::Field");
    MFDepthMapNode::initClass("av::gua::MFDepthMapNode", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

void
av::gua::DepthMapNode::getClosestDistanceCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaDepthMapNode->get_closest_distance();
}

void
av::gua::DepthMapNode::setClosestDistanceCB(const SFFloat::SetValueEvent& event)
{
  std::cout << "this value can't be set" << std::endl;
}

void
av::gua::DepthMapNode::getTextureNameCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaDepthMapNode->config.get_texture_name();
}

void
av::gua::DepthMapNode::setTextureNameCB(const SFString::SetValueEvent& event)
{
  m_guaDepthMapNode->config.set_texture_name(event.getValue());
}

std::shared_ptr< ::gua::node::CubemapNode>
av::gua::DepthMapNode::getGuaDepthMapNode() const {
  return m_guaDepthMapNode;
}
