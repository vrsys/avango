#include <avango/gua/scenegraph/LightNode.hpp>

#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::LightNode);

AV_FIELD_DEFINE(av::gua::SFLightNode);
AV_FIELD_DEFINE(av::gua::MFLightNode);

av::gua::LightNode::LightNode(std::shared_ptr<::gua::node::LightNode> guanode) : av::gua::Node(guanode), m_guaNode(std::dynamic_pointer_cast<::gua::node::LightNode>(Node::getGuaNode()))
{
    AV_FC_ADD_ADAPTOR_FIELD(Type, std::bind(&LightNode::getTypeCB, this, std::placeholders::_1), std::bind(&LightNode::setTypeCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(Color, std::bind(&LightNode::getColorCB, this, std::placeholders::_1), std::bind(&LightNode::setColorCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(Brightness, std::bind(&LightNode::getBrightnessCB, this, std::placeholders::_1), std::bind(&LightNode::setBrightnessCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(Falloff, std::bind(&LightNode::getFallofCB, this, std::placeholders::_1), std::bind(&LightNode::setFallofCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(Softness, std::bind(&LightNode::getSoftnessCB, this, std::placeholders::_1), std::bind(&LightNode::setSoftnessCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableShadows, std::bind(&LightNode::getEnableShadowsCB, this, std::placeholders::_1), std::bind(&LightNode::setEnableShadowsCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableGodrays, std::bind(&LightNode::getEnableGodraysCB, this, std::placeholders::_1), std::bind(&LightNode::setEnableGodraysCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        EnableDiffuseShading, std::bind(&LightNode::getEnableDiffuseShadingCB, this, std::placeholders::_1), std::bind(&LightNode::setEnableDiffuseShadingCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        EnableSpecularShading, std::bind(&LightNode::getEnableSpecularShadingCB, this, std::placeholders::_1), std::bind(&LightNode::setEnableSpecularShadingCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(ShadowOffset, std::bind(&LightNode::getShadowOffsetCB, this, std::placeholders::_1), std::bind(&LightNode::setShadowOffsetCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(ShadowMapSize, std::bind(&LightNode::getShadowMapSizeCB, this, std::placeholders::_1), std::bind(&LightNode::setShadowMapSizeCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(ShadowMaxDistance, std::bind(&LightNode::getShadowMaxDistanceCB, this, std::placeholders::_1), std::bind(&LightNode::setShadowMaxDistanceCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        ShadowCascadedSplits, std::bind(&LightNode::getShadowCascadedSplitsCB, this, std::placeholders::_1), std::bind(&LightNode::setShadowCascadedSplitsCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(ShadowNearClippingInSunDirection,
                            std::bind(&LightNode::getShadowNearClippingInSunDirectionCB, this, std::placeholders::_1),
                            std::bind(&LightNode::setShadowNearClippingInSunDirectionCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(ShadowFarClippingInSunDirection,
                            std::bind(&LightNode::getShadowFarClippingInSunDirectionCB, this, std::placeholders::_1),
                            std::bind(&LightNode::setShadowFarClippingInSunDirectionCB, this, std::placeholders::_1));
}

void av::gua::LightNode::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::Node::initClass();

        AV_FC_INIT(av::gua::Node, av::gua::LightNode, true);

        SFLightNode::initClass("av::gua::SFLightNode", "av::Field");
        MFLightNode::initClass("av::gua::MFLightNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr<::gua::node::LightNode> av::gua::LightNode::getGuaNode() const { return m_guaNode; }

void av::gua::LightNode::getTypeCB(const SFUInt::GetValueEvent& event) { *(event.getValuePtr()) = static_cast<unsigned>(m_guaNode->data.type()); }

void av::gua::LightNode::setTypeCB(const SFUInt::SetValueEvent& event) { m_guaNode->data.type() = static_cast<::gua::node::LightNode::Type>(event.getValue()); }

void av::gua::LightNode::getColorCB(const SFColor::GetValueEvent& event) { *(event.getValuePtr()) = m_guaNode->data.color(); }

void av::gua::LightNode::setColorCB(const SFColor::SetValueEvent& event) { m_guaNode->data.color() = event.getValue(); }

void av::gua::LightNode::getBrightnessCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaNode->data.brightness(); }

void av::gua::LightNode::setBrightnessCB(const SFFloat::SetValueEvent& event) { m_guaNode->data.brightness = event.getValue(); }

void av::gua::LightNode::getFallofCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaNode->data.falloff(); }

void av::gua::LightNode::setFallofCB(const SFFloat::SetValueEvent& event) { m_guaNode->data.falloff() = event.getValue(); }

void av::gua::LightNode::getSoftnessCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaNode->data.softness(); }

void av::gua::LightNode::setSoftnessCB(const SFFloat::SetValueEvent& event) { m_guaNode->data.softness() = event.getValue(); }

void av::gua::LightNode::getEnableShadowsCB(const SFBool::GetValueEvent& event) { *(event.getValuePtr()) = m_guaNode->data.enable_shadows(); }

void av::gua::LightNode::setEnableShadowsCB(const SFBool::SetValueEvent& event) { m_guaNode->data.enable_shadows() = event.getValue(); }

void av::gua::LightNode::getEnableGodraysCB(const SFBool::GetValueEvent& event) { *(event.getValuePtr()) = m_guaNode->data.enable_godrays(); }

void av::gua::LightNode::setEnableGodraysCB(const SFBool::SetValueEvent& event) { m_guaNode->data.enable_godrays() = event.getValue(); }

void av::gua::LightNode::getEnableDiffuseShadingCB(const SFBool::GetValueEvent& event) { *(event.getValuePtr()) = m_guaNode->data.enable_diffuse_shading(); }

void av::gua::LightNode::setEnableDiffuseShadingCB(const SFBool::SetValueEvent& event) { m_guaNode->data.enable_diffuse_shading() = event.getValue(); }

void av::gua::LightNode::getEnableSpecularShadingCB(const SFBool::GetValueEvent& event) { *(event.getValuePtr()) = m_guaNode->data.enable_specular_shading(); }

void av::gua::LightNode::setEnableSpecularShadingCB(const SFBool::SetValueEvent& event) { m_guaNode->data.enable_specular_shading() = event.getValue(); }

void av::gua::LightNode::getShadowOffsetCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaNode->data.shadow_offset(); }

void av::gua::LightNode::setShadowOffsetCB(const SFFloat::SetValueEvent& event) { m_guaNode->data.shadow_offset() = event.getValue(); }

void av::gua::LightNode::getShadowMapSizeCB(const SFUInt::GetValueEvent& event) { *(event.getValuePtr()) = m_guaNode->data.shadow_map_size(); }

void av::gua::LightNode::setShadowMapSizeCB(const SFUInt::SetValueEvent& event) { m_guaNode->data.shadow_map_size() = event.getValue(); }

void av::gua::LightNode::getShadowMaxDistanceCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaNode->data.max_shadow_dist(); }

void av::gua::LightNode::setShadowMaxDistanceCB(const SFFloat::SetValueEvent& event) { m_guaNode->data.max_shadow_dist() = event.getValue(); }

void av::gua::LightNode::getShadowCascadedSplitsCB(const MFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaNode->data.shadow_cascaded_splits(); }

void av::gua::LightNode::setShadowCascadedSplitsCB(const MFFloat::SetValueEvent& event) { m_guaNode->data.shadow_cascaded_splits() = event.getValue(); }

void av::gua::LightNode::getShadowNearClippingInSunDirectionCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaNode->data.shadow_near_clipping_in_sun_direction(); }

void av::gua::LightNode::setShadowNearClippingInSunDirectionCB(const SFFloat::SetValueEvent& event) { m_guaNode->data.shadow_near_clipping_in_sun_direction() = event.getValue(); }

void av::gua::LightNode::getShadowFarClippingInSunDirectionCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaNode->data.shadow_far_clipping_in_sun_direction(); }

void av::gua::LightNode::setShadowFarClippingInSunDirectionCB(const SFFloat::SetValueEvent& event) { m_guaNode->data.shadow_far_clipping_in_sun_direction() = event.getValue(); }
