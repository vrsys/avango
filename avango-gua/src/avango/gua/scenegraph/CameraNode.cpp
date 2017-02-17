#include <avango/gua/scenegraph/CameraNode.hpp>
#include <avango/gua/scenegraph/SceneGraph.hpp>

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
#include <avango/gua/network/NetTransform.h>
#endif

#include <avango/Base.h>
#include <functional>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::CameraNode"));
}

AV_FC_DEFINE(av::gua::CameraNode);

AV_FIELD_DEFINE(av::gua::SFCameraNode);
AV_FIELD_DEFINE(av::gua::MFCameraNode);

av::gua::CameraNode::CameraNode(std::shared_ptr< ::gua::node::CameraNode> guaCameraNode)
    : Node(guaCameraNode)
    , m_guaNode(guaCameraNode)
{
  m_PipelineDescription = av::Link<av::gua::PipelineDescription>(new av::gua::PipelineDescription(m_guaNode->get_pipeline_description()));

  AV_FC_ADD_ADAPTOR_FIELD(Enabled,
                      std::bind(&CameraNode::getEnabledCB, this,std::placeholders::_1),
                      std::bind(&CameraNode::setEnabledCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(PipelineDescription,
                      std::bind(&CameraNode::getPipelineDescriptionCB, this,std::placeholders::_1),
                      std::bind(&CameraNode::setPipelineDescriptionCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(SceneGraph,
                      std::bind(&CameraNode::getSceneGraphCB, this,std::placeholders::_1),
                      std::bind(&CameraNode::setSceneGraphCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(ViewID,
                      std::bind(&CameraNode::getViewIDCB, this,std::placeholders::_1),
                      std::bind(&CameraNode::setViewIDCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(Mode,
                      std::bind(&CameraNode::getModeCB, this,std::placeholders::_1),
                      std::bind(&CameraNode::setModeCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(EnableStereo,
                      std::bind(&CameraNode::getEnableStereoCB, this,std::placeholders::_1),
                      std::bind(&CameraNode::setEnableStereoCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(EyeDistance,
                      std::bind(&CameraNode::getEyeDistanceCB, this,std::placeholders::_1),
                      std::bind(&CameraNode::setEyeDistanceCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(EyeOffset,
                      std::bind(&CameraNode::getEyeOffsetCB, this,std::placeholders::_1),
                      std::bind(&CameraNode::setEyeOffsetCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(LeftScreenPath,
                      std::bind(&CameraNode::getLeftScreenPathCB, this,std::placeholders::_1),
                      std::bind(&CameraNode::setLeftScreenPathCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(RightScreenPath,
                      std::bind(&CameraNode::getRightScreenPathCB, this,std::placeholders::_1),
                      std::bind(&CameraNode::setRightScreenPathCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(AlternativeFrustumCullingScreenPath,
                      std::bind(&CameraNode::getAlternativeFrustumCullingScreenPathCB, this,std::placeholders::_1),
                      std::bind(&CameraNode::setAlternativeFrustumCullingScreenPathCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(MonoMode,
                      std::bind(&CameraNode::getMonoModeCB, this,std::placeholders::_1),
                      std::bind(&CameraNode::setMonoModeCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(FarClip,
                      std::bind(&CameraNode::getFarClipCB, this,std::placeholders::_1),
                      std::bind(&CameraNode::setFarClipCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(NearClip,
                      std::bind(&CameraNode::getNearClipCB, this,std::placeholders::_1),
                      std::bind(&CameraNode::setNearClipCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(Resolution,
                      std::bind(&CameraNode::getResolutionCB, this,std::placeholders::_1),
                      std::bind(&CameraNode::setResolutionCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(OutputTextureName,
                      std::bind(&CameraNode::getOutputTextureNameCB, this,std::placeholders::_1),
                      std::bind(&CameraNode::setOutputTextureNameCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(OutputWindowName,
                      std::bind(&CameraNode::getOutputWindowNameCB, this,std::placeholders::_1),
                      std::bind(&CameraNode::setOutputWindowNameCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(LeftOutputWindow,
                      std::bind(&CameraNode::getLeftOutputWindowCB, this,std::placeholders::_1),
                      std::bind(&CameraNode::setLeftOutputWindowCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(RightOutputWindow,
                      std::bind(&CameraNode::getRightOutputWindowCB, this,std::placeholders::_1),
                      std::bind(&CameraNode::setRightOutputWindowCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(EnableFrustumCulling,
                      std::bind(&CameraNode::getEnableFrustumCullingCB, this,std::placeholders::_1),
                      std::bind(&CameraNode::setEnableFrustumCullingCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(WhiteList,
                      std::bind(&CameraNode::getWhiteListCB, this,std::placeholders::_1),
                      std::bind(&CameraNode::setWhiteListCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(BlackList,
                      std::bind(&CameraNode::getBlackListCB, this,std::placeholders::_1),
                      std::bind(&CameraNode::setBlackListCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(PreRenderCameras,
                      std::bind(&CameraNode::getPreRenderCamerasCB, this,std::placeholders::_1),
                      std::bind(&CameraNode::setPreRenderCamerasCB, this,std::placeholders::_1));
}

av::Link<av::gua::Frustum> av::gua::CameraNode::get_frustum(av::gua::SceneGraph const& graph, av::gua::CameraNode::CameraMode mode) {
  return av::Link<av::gua::Frustum>(new av::gua::Frustum(new ::gua::Frustum(m_guaNode->get_rendering_frustum(*graph.getGuaSceneGraph(), static_cast< ::gua::CameraMode >(mode)))));
}

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
void av::gua::CameraNode::on_distribute(av::gua::NetTransform& netNode)
{
    Node::on_distribute(netNode);
    // netNode.distributeFieldContainer(m_PipelineDescription);
}

void av::gua::CameraNode::on_undistribute(av::gua::NetTransform& netNode)
{
    Node::on_undistribute(netNode);
    // netNode.undistributeFieldContainer(m_PipelineDescription);
}
#endif

void
av::gua::CameraNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::Node::initClass();

        AV_FC_INIT(av::gua::Node, av::gua::CameraNode, true);

        SFCameraNode::initClass("av::gua::SFCameraNode", "av::Field");
        MFCameraNode::initClass("av::gua::MFCameraNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr< ::gua::node::CameraNode>
av::gua::CameraNode::getGuaNode() const
{
    return m_guaNode;
}

void
av::gua::CameraNode::getEnabledCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->config.enabled();
}

void
av::gua::CameraNode::setEnabledCB(const SFBool::SetValueEvent& event)
{
  m_guaNode->config.enabled() = event.getValue();
}

void
av::gua::CameraNode::getPipelineDescriptionCB(const SFPipelineDescription::GetValueEvent& event)
{
  if (m_PipelineDescription.isValid()) {
    *(event.getValuePtr()) = m_PipelineDescription;
  }
}

void
av::gua::CameraNode::setPipelineDescriptionCB(const SFPipelineDescription::SetValueEvent& event)
{
  if (event.getValue().isValid()) {
    m_PipelineDescription = event.getValue();
    m_guaNode->set_pipeline_description(m_PipelineDescription->getGuaPipelineDescription());
  }
}

void
av::gua::CameraNode::getSceneGraphCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->config.scene_graph_name();
}

void
av::gua::CameraNode::setSceneGraphCB(const SFString::SetValueEvent& event)
{
  m_guaNode->config.scene_graph_name() = event.getValue();
}

void
av::gua::CameraNode::getViewIDCB(const SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->config.view_id();
}

void
av::gua::CameraNode::setViewIDCB(const SFInt::SetValueEvent& event)
{
  m_guaNode->config.view_id() = event.getValue();
}

void
av::gua::CameraNode::getModeCB(const SFUInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = static_cast<unsigned>(m_guaNode->config.mode());
}

void
av::gua::CameraNode::setModeCB(const SFUInt::SetValueEvent& event)
{
  m_guaNode->config.mode() = static_cast< ::gua::node::CameraNode::ProjectionMode>(event.getValue());
}

void
av::gua::CameraNode::getEnableStereoCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->config.enable_stereo();
}

void
av::gua::CameraNode::setEnableStereoCB(const SFBool::SetValueEvent& event)
{
  m_guaNode->config.enable_stereo() = event.getValue();
}

void
av::gua::CameraNode::getEyeDistanceCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->config.eye_dist();
}

void
av::gua::CameraNode::setEyeDistanceCB(const SFFloat::SetValueEvent& event)
{
  m_guaNode->config.eye_dist() = event.getValue();
}

void
av::gua::CameraNode::getEyeOffsetCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->config.eye_offset();
}

void
av::gua::CameraNode::setEyeOffsetCB(const SFFloat::SetValueEvent& event)
{
  m_guaNode->config.eye_offset() = event.getValue();
}

void
av::gua::CameraNode::getLeftScreenPathCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->config.left_screen_path();
}

void
av::gua::CameraNode::setLeftScreenPathCB(const SFString::SetValueEvent& event)
{
  m_guaNode->config.left_screen_path() = event.getValue();
}

void
av::gua::CameraNode::getRightScreenPathCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->config.right_screen_path();
}

void
av::gua::CameraNode::setRightScreenPathCB(const SFString::SetValueEvent& event)
{
  m_guaNode->config.right_screen_path() = event.getValue();
}

void
av::gua::CameraNode::getAlternativeFrustumCullingScreenPathCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->config.alternative_frustum_culling_screen_path();
}

void
av::gua::CameraNode::setAlternativeFrustumCullingScreenPathCB(const SFString::SetValueEvent& event)
{
  m_guaNode->config.alternative_frustum_culling_screen_path() = event.getValue();
}

void
av::gua::CameraNode::getMonoModeCB(const SFUInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = static_cast<unsigned>(m_guaNode->config.mono_mode());
}

void
av::gua::CameraNode::setMonoModeCB(const SFUInt::SetValueEvent& event)
{
  m_guaNode->config.mono_mode() = static_cast< ::gua::CameraMode>(event.getValue());
}

void
av::gua::CameraNode::getFarClipCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->config.far_clip();
}

void
av::gua::CameraNode::setFarClipCB(const SFFloat::SetValueEvent& event)
{
  m_guaNode->config.far_clip() = event.getValue();
}

void
av::gua::CameraNode::getNearClipCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->config.near_clip();
}

void
av::gua::CameraNode::setNearClipCB(const SFFloat::SetValueEvent& event)
{
  m_guaNode->config.near_clip() = event.getValue();
}

void
av::gua::CameraNode::getResolutionCB(const SFVec2ui::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->config.resolution();
}

void
av::gua::CameraNode::setResolutionCB(const SFVec2ui::SetValueEvent& event)
{
  m_guaNode->config.resolution() = event.getValue();
}

void
av::gua::CameraNode::getOutputTextureNameCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->config.output_texture_name();
}

void
av::gua::CameraNode::setOutputTextureNameCB(const SFString::SetValueEvent& event)
{
  m_guaNode->config.output_texture_name() = event.getValue();
}

void
av::gua::CameraNode::getOutputWindowNameCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->config.output_window_name();
}

void
av::gua::CameraNode::setOutputWindowNameCB(const SFString::SetValueEvent& event)
{
  m_guaNode->config.output_window_name() = event.getValue();
}

void
av::gua::CameraNode::getLeftOutputWindowCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->config.left_output_window();
}

void
av::gua::CameraNode::setLeftOutputWindowCB(const SFString::SetValueEvent& event)
{
  m_guaNode->config.left_output_window() = event.getValue();
}

void
av::gua::CameraNode::getRightOutputWindowCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->config.right_output_window();
}

void
av::gua::CameraNode::setRightOutputWindowCB(const SFString::SetValueEvent& event)
{
  m_guaNode->config.right_output_window() = event.getValue();
}

void
av::gua::CameraNode::getEnableFrustumCullingCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->config.enable_frustum_culling();
}

void
av::gua::CameraNode::setEnableFrustumCullingCB(const SFBool::SetValueEvent& event)
{
  m_guaNode->config.enable_frustum_culling() = event.getValue();
}

void
av::gua::CameraNode::getWhiteListCB(const MFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->config.mask().whitelist.get_strings();
}

void
av::gua::CameraNode::setWhiteListCB(const MFString::SetValueEvent& event)
{
  m_guaNode->config.mask().whitelist.clear_tags();
  for (auto tag : event.getValue()) {
    m_guaNode->config.mask().whitelist.add_tag(tag);
  }
}

void
av::gua::CameraNode::getBlackListCB(const MFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->config.mask().blacklist.get_strings();
}

void
av::gua::CameraNode::setBlackListCB(const MFString::SetValueEvent& event)
{
  m_guaNode->config.mask().blacklist.clear_tags();
  for (auto tag : event.getValue()) {
    m_guaNode->config.mask().blacklist.add_tag(tag);
  }
}

void
av::gua::CameraNode::getPreRenderCamerasCB(const MultiField<Link<CameraNode>>::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_preRenderCameraNodes;
}

void
av::gua::CameraNode::setPreRenderCamerasCB(const MultiField<Link<CameraNode>>::SetValueEvent& event)
{
  std::vector< std::shared_ptr< ::gua::node::CameraNode> > gua_cams;
  m_preRenderCameraNodes.clear();
  for (auto cam: event.getValue()) {
    m_preRenderCameraNodes.push_back(cam);
    gua_cams.push_back(cam->getGuaNode());
  }

  m_guaNode->set_pre_render_cameras(gua_cams);
}
