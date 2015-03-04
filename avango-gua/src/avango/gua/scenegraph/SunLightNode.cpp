#include <avango/gua/scenegraph/SunLightNode.hpp>

#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::SunLightNode);

AV_FIELD_DEFINE(av::gua::SFSunLightNode);
AV_FIELD_DEFINE(av::gua::MFSunLightNode);

av::gua::SunLightNode::SunLightNode(std::shared_ptr< ::gua::node::SunLightNode> guanode)
    : av::gua::Node(guanode),
      m_guaNode(std::dynamic_pointer_cast< ::gua::node::SunLightNode>(Node::getGuaNode()))
{
    AV_FC_ADD_ADAPTOR_FIELD(Color,
                          boost::bind(&SunLightNode::getColorCB, this, _1),
                          boost::bind(&SunLightNode::setColorCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Brightness,
                          boost::bind(&SunLightNode::getBrightnessCB, this, _1),
                          boost::bind(&SunLightNode::setBrightnessCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableShadows,
                          boost::bind(&SunLightNode::getEnableShadowsCB, this, _1),
                          boost::bind(&SunLightNode::setEnableShadowsCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableGodrays,
                          boost::bind(&SunLightNode::getEnableGodraysCB, this, _1),
                          boost::bind(&SunLightNode::setEnableGodraysCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableDiffuseShading,
                          boost::bind(&SunLightNode::getEnableDiffuseShadingCB, this, _1),
                          boost::bind(&SunLightNode::setEnableDiffuseShadingCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableSpecularShading,
                          boost::bind(&SunLightNode::getEnableSpecularShadingCB, this, _1),
                          boost::bind(&SunLightNode::setEnableSpecularShadingCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(ShadowOffset,
                          boost::bind(&SunLightNode::getShadowOffsetCB, this, _1),
                          boost::bind(&SunLightNode::setShadowOffsetCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(ShadowMapSize,
                          boost::bind(&SunLightNode::getShadowMapSizeCB, this, _1),
                          boost::bind(&SunLightNode::setShadowMapSizeCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(ShadowCascadedSplits,
                          boost::bind(&SunLightNode::getShadowCascadedSplitsCB, this, _1),
                          boost::bind(&SunLightNode::setShadowCascadedSplitsCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(ShadowNearClippingInSunDirection,
                          boost::bind(&SunLightNode::getShadowNearClippingInSunDirectionCB, this, _1),
                          boost::bind(&SunLightNode::setShadowNearClippingInSunDirectionCB, this, _1));
}

av::gua::SunLightNode::~SunLightNode()
{}

void
av::gua::SunLightNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::Node::initClass();

        AV_FC_INIT(av::gua::Node, av::gua::SunLightNode, true);

        SFSunLightNode::initClass("av::gua::SFSunLightNode", "av::Field");
        MFSunLightNode::initClass("av::gua::MFSunLightNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr< ::gua::node::SunLightNode>
av::gua::SunLightNode::getGuaNode() const
{
    return m_guaNode;
}

void
av::gua::SunLightNode::getColorCB(const SFColor::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.color();
}

void
av::gua::SunLightNode::setColorCB(const SFColor::SetValueEvent& event)
{
    m_guaNode->data.color() = event.getValue();
}

void
av::gua::SunLightNode::getBrightnessCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.brightness();
}

void
av::gua::SunLightNode::setBrightnessCB(const SFFloat::SetValueEvent& event)
{
    m_guaNode->data.brightness = event.getValue();
}

void
av::gua::SunLightNode::getEnableShadowsCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.enable_shadows();
}

void
av::gua::SunLightNode::setEnableShadowsCB(const SFBool::SetValueEvent& event)
{
    m_guaNode->data.enable_shadows() = event.getValue();
}

void
av::gua::SunLightNode::getEnableGodraysCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.enable_godrays();
}

void
av::gua::SunLightNode::setEnableGodraysCB(const SFBool::SetValueEvent& event)
{
    m_guaNode->data.enable_godrays() = event.getValue();
}

void
av::gua::SunLightNode::getEnableDiffuseShadingCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.enable_diffuse_shading();
}

void
av::gua::SunLightNode::setEnableDiffuseShadingCB(const SFBool::SetValueEvent& event)
{
    m_guaNode->data.enable_diffuse_shading() = event.getValue();
}

void
av::gua::SunLightNode::getEnableSpecularShadingCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.enable_specular_shading();
}

void
av::gua::SunLightNode::setEnableSpecularShadingCB(const SFBool::SetValueEvent& event)
{
    m_guaNode->data.enable_specular_shading() = event.getValue();
}

void
av::gua::SunLightNode::getShadowOffsetCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.shadow_offset();
}

void
av::gua::SunLightNode::setShadowOffsetCB(const SFFloat::SetValueEvent& event)
{
    m_guaNode->data.shadow_offset() = event.getValue();
}

void
av::gua::SunLightNode::getShadowMapSizeCB(const SFUInt::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.shadow_map_size();
}

void
av::gua::SunLightNode::setShadowMapSizeCB(const SFUInt::SetValueEvent& event)
{
    m_guaNode->data.shadow_map_size() = event.getValue();
}

void
av::gua::SunLightNode::getShadowCascadedSplitsCB(const MFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.shadow_cascaded_splits();
}

void
av::gua::SunLightNode::setShadowCascadedSplitsCB(const MFFloat::SetValueEvent& event)
{
    m_guaNode->data.shadow_cascaded_splits() = event.getValue();
}

void
av::gua::SunLightNode::getShadowNearClippingInSunDirectionCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.shadow_near_clipping_in_sun_direction();
}

void
av::gua::SunLightNode::setShadowNearClippingInSunDirectionCB(const SFFloat::SetValueEvent& event)
{
    m_guaNode->data.shadow_near_clipping_in_sun_direction() = event.getValue();
}
