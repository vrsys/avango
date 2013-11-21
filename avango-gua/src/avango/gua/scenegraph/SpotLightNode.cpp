#include <avango/gua/scenegraph/SpotLightNode.hpp>

#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::SpotLightNode);

AV_FIELD_DEFINE(av::gua::SFSpotLightNode);
AV_FIELD_DEFINE(av::gua::MFSpotLightNode);

av::gua::SpotLightNode::SpotLightNode(std::shared_ptr< ::gua::SpotLightNode> guanode)
    : av::gua::Node(guanode),
      m_guaNode(std::dynamic_pointer_cast< ::gua::SpotLightNode>(Node::getGuaNode()))
{
    AV_FC_ADD_ADAPTOR_FIELD(Color,
                          boost::bind(&SpotLightNode::getColorCB, this, _1),
                          boost::bind(&SpotLightNode::setColorCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Falloff,
                          boost::bind(&SpotLightNode::getFallofCB, this, _1),
                          boost::bind(&SpotLightNode::setFallofCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Softness,
                          boost::bind(&SpotLightNode::getSoftnessCB, this, _1),
                          boost::bind(&SpotLightNode::setSoftnessCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableShadows,
                          boost::bind(&SpotLightNode::getEnableShadowsCB, this, _1),
                          boost::bind(&SpotLightNode::setEnableShadowsCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableGodrays,
                          boost::bind(&SpotLightNode::getEnableGodraysCB, this, _1),
                          boost::bind(&SpotLightNode::setEnableGodraysCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableDiffuseShading,
                          boost::bind(&SpotLightNode::getEnableDiffuseShadingCB, this, _1),
                          boost::bind(&SpotLightNode::setEnableDiffuseShadingCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableSpecularShading,
                          boost::bind(&SpotLightNode::getEnableSpecularShadingCB, this, _1),
                          boost::bind(&SpotLightNode::setEnableSpecularShadingCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(ShadowOffset,
                          boost::bind(&SpotLightNode::getShadowOffsetCB, this, _1),
                          boost::bind(&SpotLightNode::setShadowOffsetCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(ShadowMapSize,
                          boost::bind(&SpotLightNode::getShadowMapSizeCB, this, _1),
                          boost::bind(&SpotLightNode::setShadowMapSizeCB, this, _1));
}

av::gua::SpotLightNode::~SpotLightNode()
{}

void
av::gua::SpotLightNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::Node::initClass();

        AV_FC_INIT(av::gua::Node, av::gua::SpotLightNode, true);

        SFSpotLightNode::initClass("av::gua::SFSpotLightNode", "av::Field");
        MFSpotLightNode::initClass("av::gua::MFSpotLightNode", "av::Field");
    }
}

std::shared_ptr< ::gua::SpotLightNode>
av::gua::SpotLightNode::getGuaNode() const
{
    return m_guaNode;
}

void
av::gua::SpotLightNode::getColorCB(const SFColor::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.color();
}

void
av::gua::SpotLightNode::setColorCB(const SFColor::SetValueEvent& event)
{
    m_guaNode->data.color() = event.getValue();
}

void
av::gua::SpotLightNode::getFallofCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.falloff();
}

void
av::gua::SpotLightNode::setFallofCB(const SFFloat::SetValueEvent& event)
{
    m_guaNode->data.falloff() = event.getValue();
}

void
av::gua::SpotLightNode::getSoftnessCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.falloff();
}

void
av::gua::SpotLightNode::setSoftnessCB(const SFFloat::SetValueEvent& event)
{
    m_guaNode->data.falloff() = event.getValue();
}

void
av::gua::SpotLightNode::getEnableShadowsCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.enable_shadows();
}

void
av::gua::SpotLightNode::setEnableShadowsCB(const SFBool::SetValueEvent& event)
{
    m_guaNode->data.enable_shadows() = event.getValue();
}

void
av::gua::SpotLightNode::getEnableGodraysCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.enable_godrays();
}

void
av::gua::SpotLightNode::setEnableGodraysCB(const SFBool::SetValueEvent& event)
{
    m_guaNode->data.enable_godrays() = event.getValue();
}

void
av::gua::SpotLightNode::getEnableDiffuseShadingCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.enable_diffuse_shading();
}

void
av::gua::SpotLightNode::setEnableDiffuseShadingCB(const SFBool::SetValueEvent& event)
{
    m_guaNode->data.enable_diffuse_shading() = event.getValue();
}

void
av::gua::SpotLightNode::getEnableSpecularShadingCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.enable_specular_shading();
}

void
av::gua::SpotLightNode::setEnableSpecularShadingCB(const SFBool::SetValueEvent& event)
{
    m_guaNode->data.enable_specular_shading() = event.getValue();
}

void
av::gua::SpotLightNode::getShadowOffsetCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.shadow_offset();
}

void
av::gua::SpotLightNode::setShadowOffsetCB(const SFFloat::SetValueEvent& event)
{
    m_guaNode->data.shadow_offset() = event.getValue();
}

void
av::gua::SpotLightNode::getShadowMapSizeCB(const SFUInt::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.shadow_map_size();
}

void
av::gua::SpotLightNode::setShadowMapSizeCB(const SFUInt::SetValueEvent& event)
{
    m_guaNode->data.shadow_map_size() = event.getValue();
}

