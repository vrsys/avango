#include <avango/gua/scenegraph/LightNode.hpp>

#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::LightNode);

AV_FIELD_DEFINE(av::gua::SFLightNode);
AV_FIELD_DEFINE(av::gua::MFLightNode);

av::gua::LightNode::LightNode(std::shared_ptr< ::gua::node::LightNode> guanode)
    : av::gua::Node(guanode),
      m_guaNode(std::dynamic_pointer_cast< ::gua::node::LightNode>(Node::getGuaNode()))
{
    AV_FC_ADD_ADAPTOR_FIELD(Type,
                          boost::bind(&LightNode::getTypeCB, this, _1),
                          boost::bind(&LightNode::setTypeCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Color,
                          boost::bind(&LightNode::getColorCB, this, _1),
                          boost::bind(&LightNode::setColorCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Brightness,
                          boost::bind(&LightNode::getBrightnessCB, this, _1),
                          boost::bind(&LightNode::setBrightnessCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Falloff,
                          boost::bind(&LightNode::getFallofCB, this, _1),
                          boost::bind(&LightNode::setFallofCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Softness,
                          boost::bind(&LightNode::getSoftnessCB, this, _1),
                          boost::bind(&LightNode::setSoftnessCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableShadows,
                          boost::bind(&LightNode::getEnableShadowsCB, this, _1),
                          boost::bind(&LightNode::setEnableShadowsCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableGodrays,
                          boost::bind(&LightNode::getEnableGodraysCB, this, _1),
                          boost::bind(&LightNode::setEnableGodraysCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableDiffuseShading,
                          boost::bind(&LightNode::getEnableDiffuseShadingCB, this, _1),
                          boost::bind(&LightNode::setEnableDiffuseShadingCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableSpecularShading,
                          boost::bind(&LightNode::getEnableSpecularShadingCB, this, _1),
                          boost::bind(&LightNode::setEnableSpecularShadingCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(ShadowOffset,
                          boost::bind(&LightNode::getShadowOffsetCB, this, _1),
                          boost::bind(&LightNode::setShadowOffsetCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(ShadowMapSize,
                          boost::bind(&LightNode::getShadowMapSizeCB, this, _1),
                          boost::bind(&LightNode::setShadowMapSizeCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(ShadowCascadedSplits,
                          boost::bind(&LightNode::getShadowCascadedSplitsCB, this, _1),
                          boost::bind(&LightNode::setShadowCascadedSplitsCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(ShadowNearClippingInSunDirection,
                          boost::bind(&LightNode::getShadowNearClippingInSunDirectionCB, this, _1),
                          boost::bind(&LightNode::setShadowNearClippingInSunDirectionCB, this, _1));
}

void
av::gua::LightNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::Node::initClass();

        AV_FC_INIT(av::gua::Node, av::gua::LightNode, true);

        SFLightNode::initClass("av::gua::SFLightNode", "av::Field");
        MFLightNode::initClass("av::gua::MFLightNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr< ::gua::node::LightNode>
av::gua::LightNode::getGuaNode() const
{
    return m_guaNode;
}

void
av::gua::LightNode::getTypeCB(const SFUInt::GetValueEvent& event)
{
    *(event.getValuePtr()) = static_cast<unsigned>(m_guaNode->data.type());
}

void
av::gua::LightNode::setTypeCB(const SFUInt::SetValueEvent& event)
{
    m_guaNode->data.type() = static_cast< ::gua::node::LightNode::Type>(event.getValue());
}

void
av::gua::LightNode::getColorCB(const SFColor::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.color();
}

void
av::gua::LightNode::setColorCB(const SFColor::SetValueEvent& event)
{
    m_guaNode->data.color() = event.getValue();
}

void
av::gua::LightNode::getBrightnessCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.brightness();
}

void
av::gua::LightNode::setBrightnessCB(const SFFloat::SetValueEvent& event)
{
    m_guaNode->data.brightness = event.getValue();
}

void
av::gua::LightNode::getFallofCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.falloff();
}

void
av::gua::LightNode::setFallofCB(const SFFloat::SetValueEvent& event)
{
    m_guaNode->data.falloff() = event.getValue();
}

void
av::gua::LightNode::getSoftnessCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.softness();
}

void
av::gua::LightNode::setSoftnessCB(const SFFloat::SetValueEvent& event)
{
    m_guaNode->data.softness() = event.getValue();
}

void
av::gua::LightNode::getEnableShadowsCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.enable_shadows();
}

void
av::gua::LightNode::setEnableShadowsCB(const SFBool::SetValueEvent& event)
{
    m_guaNode->data.enable_shadows() = event.getValue();
}

void
av::gua::LightNode::getEnableGodraysCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.enable_godrays();
}

void
av::gua::LightNode::setEnableGodraysCB(const SFBool::SetValueEvent& event)
{
    m_guaNode->data.enable_godrays() = event.getValue();
}

void
av::gua::LightNode::getEnableDiffuseShadingCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.enable_diffuse_shading();
}

void
av::gua::LightNode::setEnableDiffuseShadingCB(const SFBool::SetValueEvent& event)
{
    m_guaNode->data.enable_diffuse_shading() = event.getValue();
}

void
av::gua::LightNode::getEnableSpecularShadingCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.enable_specular_shading();
}

void
av::gua::LightNode::setEnableSpecularShadingCB(const SFBool::SetValueEvent& event)
{
    m_guaNode->data.enable_specular_shading() = event.getValue();
}

void
av::gua::LightNode::getShadowOffsetCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.shadow_offset();
}

void
av::gua::LightNode::setShadowOffsetCB(const SFFloat::SetValueEvent& event)
{
    m_guaNode->data.shadow_offset() = event.getValue();
}

void
av::gua::LightNode::getShadowMapSizeCB(const SFUInt::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.shadow_map_size();
}

void
av::gua::LightNode::setShadowMapSizeCB(const SFUInt::SetValueEvent& event)
{
    m_guaNode->data.shadow_map_size() = event.getValue();
}

void
av::gua::LightNode::getShadowCascadedSplitsCB(const MFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.shadow_cascaded_splits();
}

void
av::gua::LightNode::setShadowCascadedSplitsCB(const MFFloat::SetValueEvent& event)
{
    m_guaNode->data.shadow_cascaded_splits() = event.getValue();
}

void
av::gua::LightNode::getShadowNearClippingInSunDirectionCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.shadow_near_clipping_in_sun_direction();
}

void
av::gua::LightNode::setShadowNearClippingInSunDirectionCB(const SFFloat::SetValueEvent& event)
{
    m_guaNode->data.shadow_near_clipping_in_sun_direction() = event.getValue();
}
