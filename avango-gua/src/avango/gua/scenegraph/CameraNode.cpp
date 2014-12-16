#include <avango/gua/scenegraph/CameraNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
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
  auto& desc(m_guaNode->config.pipeline_description());
  m_guaNode->config.pipeline_description().set_user_data(
                        new av::Link<av::gua::PipelineDescription>(
                          new av::gua::PipelineDescription(
                            std::shared_ptr<::gua::PipelineDescription>(&desc)
                          )
                        ));

  m_guaNode->config.mask().set_user_data(new av::Link<av::gua::Mask>(
    new av::gua::Mask(
      std::shared_ptr< ::gua::Mask>(&m_guaNode->config.mask())
    )
  ));

  AV_FC_ADD_ADAPTOR_FIELD(Enabled,
                      boost::bind(&CameraNode::getEnabledCB, this, _1),
                      boost::bind(&CameraNode::setEnabledCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(PipelineDescription,
                      boost::bind(&CameraNode::getPipelineDescriptionCB, this, _1),
                      boost::bind(&CameraNode::setPipelineDescriptionCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(SceneGraph,
                      boost::bind(&CameraNode::getSceneGraphCB, this, _1),
                      boost::bind(&CameraNode::setSceneGraphCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(ViewID,
                      boost::bind(&CameraNode::getViewIDCB, this, _1),
                      boost::bind(&CameraNode::setViewIDCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Mode,
                      boost::bind(&CameraNode::getModeCB, this, _1),
                      boost::bind(&CameraNode::setModeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(EnableStereo,
                      boost::bind(&CameraNode::getEnableStereoCB, this, _1),
                      boost::bind(&CameraNode::setEnableStereoCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(EyeDistance,
                      boost::bind(&CameraNode::getEyeDistanceCB, this, _1),
                      boost::bind(&CameraNode::setEyeDistanceCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(EyeOffset,
                      boost::bind(&CameraNode::getEyeOffsetCB, this, _1),
                      boost::bind(&CameraNode::setEyeOffsetCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(LeftScreenPath,
                      boost::bind(&CameraNode::getLeftScreenPathCB, this, _1),
                      boost::bind(&CameraNode::setLeftScreenPathCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(RightScreenPath,
                      boost::bind(&CameraNode::getRightScreenPathCB, this, _1),
                      boost::bind(&CameraNode::setRightScreenPathCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(MonoMode,
                      boost::bind(&CameraNode::getMonoModeCB, this, _1),
                      boost::bind(&CameraNode::setMonoModeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Resolution,
                      boost::bind(&CameraNode::getResolutionCB, this, _1),
                      boost::bind(&CameraNode::setResolutionCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(OutputTextureName,
                      boost::bind(&CameraNode::getOutputTextureNameCB, this, _1),
                      boost::bind(&CameraNode::setOutputTextureNameCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(OutputWindowName,
                      boost::bind(&CameraNode::getOutputWindowNameCB, this, _1),
                      boost::bind(&CameraNode::setOutputWindowNameCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(EnableFrustumCulling,
                      boost::bind(&CameraNode::getEnableFrustumCullingCB, this, _1),
                      boost::bind(&CameraNode::setEnableFrustumCullingCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Mask,
                      boost::bind(&CameraNode::getMaskCB, this, _1),
                      boost::bind(&CameraNode::setMaskCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(PreRenderCameras,
                      boost::bind(&CameraNode::getPreRenderCamerasCB, this, _1),
                      boost::bind(&CameraNode::setPreRenderCamerasCB, this, _1));
}

av::gua::CameraNode::~CameraNode()
{}

void
av::gua::CameraNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::Node::initClass();

        AV_FC_INIT(av::gua::Node, av::gua::CameraNode, true);

        SFCameraNode::initClass("av::gua::SFCameraNode", "av::Field");
        MFCameraNode::initClass("av::gua::MFCameraNode", "av::Field");
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
  *(event.getValuePtr()) = *static_cast<av::Link<av::gua::PipelineDescription>*>
                                          (m_guaNode->config.pipeline_description().get_user_data());
}

void
av::gua::CameraNode::setPipelineDescriptionCB(const SFPipelineDescription::SetValueEvent& event)
{
  m_guaNode->config.pipeline_description() = *event.getValue()->getGuaPipelineDescription().get();

  auto& desc(m_guaNode->config.pipeline_description());
  m_guaNode->config.pipeline_description().set_user_data(
                        new av::Link<av::gua::PipelineDescription>(
                          new av::gua::PipelineDescription(
                            std::shared_ptr<::gua::PipelineDescription>(&desc)
                          )
                        ));
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
av::gua::CameraNode::getMaskCB(const SFMask::GetValueEvent& event)
{
  if (m_guaNode->config.mask().get_user_data()) {
    *(event.getValuePtr()) = *static_cast<av::Link<av::gua::Mask>*>(m_guaNode->config.mask().get_user_data());
  }
}

void
av::gua::CameraNode::setMaskCB(const SFMask::SetValueEvent& event)
{
  m_guaNode->config.mask() = *event.getValue()->getGuaMask();
  m_guaNode->config.mask().set_user_data(new av::Link<av::gua::Mask>(
    new av::gua::Mask(
      std::shared_ptr< ::gua::Mask>(&m_guaNode->config.mask())
    )
  ));
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

