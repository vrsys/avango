#include <avango/gua/scenegraph/PointLightNode.hpp>

#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::PointLightNode);

AV_FIELD_DEFINE(av::gua::SFPointLightNode);
AV_FIELD_DEFINE(av::gua::MFPointLightNode);

av::gua::PointLightNode::PointLightNode(std::shared_ptr< ::gua::PointLightNode> guanode)
    : av::gua::Node(guanode),
      m_guaNode(std::dynamic_pointer_cast< ::gua::PointLightNode>(Node::getGuaNode()))
{
    AV_FC_ADD_ADAPTOR_FIELD(Color,
                          boost::bind(&PointLightNode::getColorCB, this, _1),
                          boost::bind(&PointLightNode::setColorCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Falloff,
                          boost::bind(&PointLightNode::getFallofCB, this, _1),
                          boost::bind(&PointLightNode::setFallofCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableShadows,
                          boost::bind(&PointLightNode::getEnableShadowsCB, this, _1),
                          boost::bind(&PointLightNode::setEnableShadowsCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableGodrays,
                          boost::bind(&PointLightNode::getEnableGodraysCB, this, _1),
                          boost::bind(&PointLightNode::setEnableGodraysCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableDiffuseShading,
                          boost::bind(&PointLightNode::getEnableDiffuseShadingCB, this, _1),
                          boost::bind(&PointLightNode::setEnableDiffuseShadingCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableSpecularShading,
                          boost::bind(&PointLightNode::getEnableSpecularShadingCB, this, _1),
                          boost::bind(&PointLightNode::setEnableSpecularShadingCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(ShadowMapSize,
                          boost::bind(&PointLightNode::getShadowMapSizeCB, this, _1),
                          boost::bind(&PointLightNode::setShadowMapSizeCB, this, _1));
}

av::gua::PointLightNode::~PointLightNode()
{}

void
av::gua::PointLightNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::Node::initClass();

        AV_FC_INIT(av::gua::Node, av::gua::PointLightNode, true);

        SFPointLightNode::initClass("av::gua::SFPointLightNode", "av::Field");
        MFPointLightNode::initClass("av::gua::MFPointLightNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr< ::gua::PointLightNode>
av::gua::PointLightNode::getGuaNode() const
{
    return m_guaNode;
}

void
av::gua::PointLightNode::getColorCB(const SFColor::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.color();
}

void
av::gua::PointLightNode::setColorCB(const SFColor::SetValueEvent& event)
{
    m_guaNode->data.color() = event.getValue();
}

void
av::gua::PointLightNode::getFallofCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.falloff();
}

void
av::gua::PointLightNode::setFallofCB(const SFFloat::SetValueEvent& event)
{
    m_guaNode->data.falloff() = event.getValue();
}

void
av::gua::PointLightNode::getEnableShadowsCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.enable_shadows();
}

void
av::gua::PointLightNode::setEnableShadowsCB(const SFBool::SetValueEvent& event)
{
    m_guaNode->data.enable_shadows() = event.getValue();
}

void
av::gua::PointLightNode::getEnableGodraysCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.enable_godrays();
}

void
av::gua::PointLightNode::setEnableGodraysCB(const SFBool::SetValueEvent& event)
{
    m_guaNode->data.enable_godrays() = event.getValue();
}

void
av::gua::PointLightNode::getEnableDiffuseShadingCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.enable_diffuse_shading();
}

void
av::gua::PointLightNode::setEnableDiffuseShadingCB(const SFBool::SetValueEvent& event)
{
    m_guaNode->data.enable_diffuse_shading() = event.getValue();
}

void
av::gua::PointLightNode::getEnableSpecularShadingCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.enable_specular_shading();
}

void
av::gua::PointLightNode::setEnableSpecularShadingCB(const SFBool::SetValueEvent& event)
{
    m_guaNode->data.enable_specular_shading() = event.getValue();
}

void
av::gua::PointLightNode::getShadowMapSizeCB(const SFUInt::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.shadow_map_size();
}

void
av::gua::PointLightNode::setShadowMapSizeCB(const SFUInt::SetValueEvent& event)
{
    m_guaNode->data.shadow_map_size() = event.getValue();
}

