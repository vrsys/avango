#include <avango/gua/scenegraph/DepthMapNode.hpp>

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
#include <avango/gua/network/NetTransform.h>
#endif

#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::DepthMapNode);

AV_FIELD_DEFINE(av::gua::SFDepthMapNode);
AV_FIELD_DEFINE(av::gua::MFDepthMapNode);

av::gua::DepthMapNode::DepthMapNode(std::shared_ptr< ::gua::node::CubemapNode> guanode)
    : Node(guanode)
    , m_guaDepthMapNode(guanode)
{
  AV_FC_ADD_ADAPTOR_FIELD(Active,
                      std::bind(&DepthMapNode::getActiveCB, this,std::placeholders::_1),
                      std::bind(&DepthMapNode::setActiveCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(MinDistance,
                      std::bind(&DepthMapNode::getMinDistanceCB, this,std::placeholders::_1),
                      std::bind(&DepthMapNode::setMinDistanceCB, this,std::placeholders::_1));
  AV_FC_ADD_ADAPTOR_FIELD(MinDistanceWorldPosition,
                      std::bind(&DepthMapNode::getMinDistanceWorldPositionCB, this,std::placeholders::_1),
                      std::bind(&DepthMapNode::setMinDistanceWorldPositionCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(WeightedMinDistance,
                      std::bind(&DepthMapNode::getWeightedMinDistanceCB, this,std::placeholders::_1),
                      std::bind(&DepthMapNode::setWeightedMinDistanceCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(TextureName,
                      std::bind(&DepthMapNode::getTextureNameCB, this,std::placeholders::_1),
                      std::bind(&DepthMapNode::setTextureNameCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(NearClip,
                      std::bind(&DepthMapNode::getNearClipCB, this,std::placeholders::_1),
                      std::bind(&DepthMapNode::setNearClipCB, this,std::placeholders::_1));
  AV_FC_ADD_ADAPTOR_FIELD(FarClip,
                      std::bind(&DepthMapNode::getFarClipCB, this,std::placeholders::_1),
                      std::bind(&DepthMapNode::setFarClipCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(RenderMode,
                      std::bind(&DepthMapNode::getRenderModeCB, this,std::placeholders::_1),
                      std::bind(&DepthMapNode::setRenderModeCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(Resolution,
                      std::bind(&DepthMapNode::getResolutionCB, this,std::placeholders::_1),
                      std::bind(&DepthMapNode::setResolutionCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(WhiteList,
                      std::bind(&DepthMapNode::getWhiteListCB, this,std::placeholders::_1),
                      std::bind(&DepthMapNode::setWhiteListCB, this,std::placeholders::_1));
  AV_FC_ADD_ADAPTOR_FIELD(BlackList,
                      std::bind(&DepthMapNode::getBlackListCB, this,std::placeholders::_1),
                      std::bind(&DepthMapNode::setBlackListCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(ViewID,
                      std::bind(&DepthMapNode::getViewIDCB, this,std::placeholders::_1),
                      std::bind(&DepthMapNode::setViewIDCB, this,std::placeholders::_1));
}

//av::gua::DepthMapNode::~DepthMapNode()
//{}

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
void av::gua::DepthMapNode::on_distribute(av::gua::NetTransform& netNode) 
{
    Node::on_distribute(netNode);
}

void av::gua::DepthMapNode::on_undistribute(av::gua::NetTransform& netNode) 
{
    Node::on_undistribute(netNode);    
}
#endif

void av::gua::DepthMapNode::create_weights(::gua::math::vec3 const& view_direction, ::gua::math::vec3 const& move_direction)
{
    m_guaDepthMapNode->create_weights(view_direction, move_direction);
}

::gua::math::vec3 av::gua::DepthMapNode::get_push_back(float radius, float softness)
{
    return m_guaDepthMapNode->get_push_back(radius, softness);
}

::gua::math::vec3 av::gua::DepthMapNode::get_pull_in(float inner_radius, float outer_radius, float softness)
{
    return m_guaDepthMapNode->get_pull_in(inner_radius, outer_radius, softness);
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

//Active
void
av::gua::DepthMapNode::getActiveCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaDepthMapNode->config.get_active();
}

void
av::gua::DepthMapNode::setActiveCB(const SFBool::SetValueEvent& event)
{
  m_guaDepthMapNode->config.set_active(event.getValue());
}

//MinDistance
void
av::gua::DepthMapNode::getMinDistanceCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaDepthMapNode->get_min_distance();
}

void
av::gua::DepthMapNode::setMinDistanceCB(const SFFloat::SetValueEvent& event)
{
  std::cout << "\"MinDistance\" value can't be set" << std::endl;
}

//MinDistanceWorldPosition
void
av::gua::DepthMapNode::getMinDistanceWorldPositionCB(const SFVec3::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaDepthMapNode->get_min_distance_position();
}

void
av::gua::DepthMapNode::setMinDistanceWorldPositionCB(const SFVec3::SetValueEvent& event)
{
  std::cout << "\"MinDistanceWorldPosition\" value can't be set" << std::endl;
}

//WeightedMinDistance
void
av::gua::DepthMapNode::getWeightedMinDistanceCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaDepthMapNode->get_weighted_min_distance();
}

void
av::gua::DepthMapNode::setWeightedMinDistanceCB(const SFFloat::SetValueEvent& event)
{
  std::cout << "\"WeightedMinDistance\" value can't be set" << std::endl;
}

//TextureName
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

//NearClip
void
av::gua::DepthMapNode::getNearClipCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaDepthMapNode->config.get_near_clip();
}

void
av::gua::DepthMapNode::setNearClipCB(const SFFloat::SetValueEvent& event)
{
  m_guaDepthMapNode->config.set_near_clip(event.getValue());
}

//FarClip
void
av::gua::DepthMapNode::getFarClipCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaDepthMapNode->config.get_far_clip();
}

void
av::gua::DepthMapNode::setFarClipCB(const SFFloat::SetValueEvent& event)
{
  m_guaDepthMapNode->config.set_far_clip(event.getValue());
}

void
av::gua::DepthMapNode::getRenderModeCB(const SFUInt::GetValueEvent& event)
{
    *(event.getValuePtr()) = static_cast<unsigned>(m_guaDepthMapNode->config.get_render_mode());
}

void
av::gua::DepthMapNode::setRenderModeCB(const SFUInt::SetValueEvent& event)
{
  m_guaDepthMapNode->config.render_mode() = static_cast< ::gua::node::CubemapNode::RenderMode>(event.getValue());
}

//Resolution
void
av::gua::DepthMapNode::getResolutionCB(const SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaDepthMapNode->config.get_resolution();
}

void
av::gua::DepthMapNode::setResolutionCB(const SFInt::SetValueEvent& event)
{
  m_guaDepthMapNode->config.set_resolution(event.getValue());
}

//WhiteList
void
av::gua::DepthMapNode::getWhiteListCB(const MFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaDepthMapNode->config.mask().whitelist.get_strings();
}

void
av::gua::DepthMapNode::setWhiteListCB(const MFString::SetValueEvent& event)
{
  m_guaDepthMapNode->config.mask().whitelist.clear_tags();
  for (auto tag : event.getValue()) {
    m_guaDepthMapNode->config.mask().whitelist.add_tag(tag);
  }
}

//BlackList
void
av::gua::DepthMapNode::getBlackListCB(const MFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaDepthMapNode->config.mask().blacklist.get_strings();
}

void
av::gua::DepthMapNode::setBlackListCB(const MFString::SetValueEvent& event)
{
  m_guaDepthMapNode->config.mask().blacklist.clear_tags();
  for (auto tag : event.getValue()) {
    m_guaDepthMapNode->config.mask().blacklist.add_tag(tag);
  }
}

//ViewID
void
av::gua::DepthMapNode::getViewIDCB(const SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaDepthMapNode->config.view_id();
}

void
av::gua::DepthMapNode::setViewIDCB(const SFInt::SetValueEvent& event)
{
  m_guaDepthMapNode->config.set_view_id(event.getValue());
}

std::shared_ptr< ::gua::node::CubemapNode>
av::gua::DepthMapNode::getGuaDepthMapNode() const {
  return m_guaDepthMapNode;
}
