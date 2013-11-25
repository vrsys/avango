#include <avango/gua/scenegraph/TexturedQuadNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::TexturedQuadNode);

AV_FIELD_DEFINE(av::gua::SFTexturedQuadNode);
AV_FIELD_DEFINE(av::gua::MFTexturedQuadNode);

av::gua::TexturedQuadNode::TexturedQuadNode(std::shared_ptr< ::gua::TexturedQuadNode> guanode)
    : Node(guanode),
      m_guaNode(std::dynamic_pointer_cast< ::gua::TexturedQuadNode>(Node::getGuaNode()))
{
    AV_FC_ADD_ADAPTOR_FIELD(Texture,
                          boost::bind(&TexturedQuadNode::getTextureCB, this, _1),
                          boost::bind(&TexturedQuadNode::setTextureCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Width,
                          boost::bind(&TexturedQuadNode::getWidthCB, this, _1),
                          boost::bind(&TexturedQuadNode::setWidthCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Height,
                          boost::bind(&TexturedQuadNode::getHeightCB, this, _1),
                          boost::bind(&TexturedQuadNode::setHeightCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(IsStereoTexture,
                          boost::bind(&TexturedQuadNode::getIsStereoTextureCB, this, _1),
                          boost::bind(&TexturedQuadNode::setIsStereoTextureCB, this, _1));
}

//av::gua::TexturedQuadNode::~TexturedQuadNode()
//{}

void
av::gua::TexturedQuadNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::Node::initClass();

        AV_FC_INIT(av::gua::Node, av::gua::TexturedQuadNode, true);

        SFTexturedQuadNode::initClass("av::gua::SFTexturedQuadNode", "av::Field");
        MFTexturedQuadNode::initClass("av::gua::MFTexturedQuadNode", "av::Field");
    }
}

std::shared_ptr< ::gua::TexturedQuadNode>
av::gua::TexturedQuadNode::getGuaNode() const
{
    return m_guaNode;
}

void
av::gua::TexturedQuadNode::getTextureCB(const SFString::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.texture();
}

void
av::gua::TexturedQuadNode::setTextureCB(const SFString::SetValueEvent& event)
{
    m_guaNode->data.texture() = event.getValue();
}

void
av::gua::TexturedQuadNode::getWidthCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.size()[0];
}

void
av::gua::TexturedQuadNode::setWidthCB(const SFFloat::SetValueEvent& event)
{
    m_guaNode->data.size()[0] = event.getValue();
}

void
av::gua::TexturedQuadNode::getHeightCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.size()[1];
}

void
av::gua::TexturedQuadNode::setHeightCB(const SFFloat::SetValueEvent& event)
{
    m_guaNode->data.size()[1] = event.getValue();
}

void
av::gua::TexturedQuadNode::getIsStereoTextureCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.is_stereo_texture();
}

void
av::gua::TexturedQuadNode::setIsStereoTextureCB(const SFBool::SetValueEvent& event)
{
    m_guaNode->data.is_stereo_texture() = event.getValue();
}
