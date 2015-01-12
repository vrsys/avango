#include <avango/gua/scenegraph/TexturedScreenSpaceQuadNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::TexturedScreenSpaceQuadNode);

AV_FIELD_DEFINE(av::gua::SFTexturedScreenSpaceQuadNode);
AV_FIELD_DEFINE(av::gua::MFTexturedScreenSpaceQuadNode);

av::gua::TexturedScreenSpaceQuadNode::TexturedScreenSpaceQuadNode(std::shared_ptr< ::gua::node::TexturedScreenSpaceQuadNode> guanode)
    : Node(guanode),
      m_guaNode(guanode)
{
    AV_FC_ADD_ADAPTOR_FIELD(Texture,
                          boost::bind(&TexturedScreenSpaceQuadNode::getTextureCB, this, _1),
                          boost::bind(&TexturedScreenSpaceQuadNode::setTextureCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Width,
                          boost::bind(&TexturedScreenSpaceQuadNode::getWidthCB, this, _1),
                          boost::bind(&TexturedScreenSpaceQuadNode::setWidthCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Height,
                          boost::bind(&TexturedScreenSpaceQuadNode::getHeightCB, this, _1),
                          boost::bind(&TexturedScreenSpaceQuadNode::setHeightCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Anchor,
                          boost::bind(&TexturedScreenSpaceQuadNode::getAnchorCB, this, _1),
                          boost::bind(&TexturedScreenSpaceQuadNode::setAnchorCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Offset,
                          boost::bind(&TexturedScreenSpaceQuadNode::getOffsetCB, this, _1),
                          boost::bind(&TexturedScreenSpaceQuadNode::setOffsetCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Opacity,
                          boost::bind(&TexturedScreenSpaceQuadNode::getOpacityCB, this, _1),
                          boost::bind(&TexturedScreenSpaceQuadNode::setOpacityCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(FlipX,
                          boost::bind(&TexturedScreenSpaceQuadNode::getFlipXCB, this, _1),
                          boost::bind(&TexturedScreenSpaceQuadNode::setFlipXCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(FlipY,
                          boost::bind(&TexturedScreenSpaceQuadNode::getFlipYCB, this, _1),
                          boost::bind(&TexturedScreenSpaceQuadNode::setFlipYCB, this, _1));

}

//av::gua::TexturedScreenSpaceQuadNode::~TexturedScreenSpaceQuadNode()
//{}

void
av::gua::TexturedScreenSpaceQuadNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::Node::initClass();

        AV_FC_INIT(av::gua::Node, av::gua::TexturedScreenSpaceQuadNode, true);

        SFTexturedScreenSpaceQuadNode::initClass("av::gua::SFTexturedScreenSpaceQuadNode", "av::Field");
        MFTexturedScreenSpaceQuadNode::initClass("av::gua::MFTexturedScreenSpaceQuadNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr< ::gua::node::TexturedScreenSpaceQuadNode>
av::gua::TexturedScreenSpaceQuadNode::getGuaNode() const
{
    return m_guaNode;
}

void
av::gua::TexturedScreenSpaceQuadNode::getTextureCB(const SFString::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.get_texture();
}

void
av::gua::TexturedScreenSpaceQuadNode::setTextureCB(const SFString::SetValueEvent& event)
{
    m_guaNode->data.set_texture(event.getValue());
}

void
av::gua::TexturedScreenSpaceQuadNode::getWidthCB(const SFInt::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.get_size()[0];
}

void
av::gua::TexturedScreenSpaceQuadNode::setWidthCB(const SFInt::SetValueEvent& event)
{
  auto size = m_guaNode->data.get_size();
  size[0] = event.getValue();
  m_guaNode->data.set_size(size);
}

void
av::gua::TexturedScreenSpaceQuadNode::getHeightCB(const SFInt::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.get_size()[1];
}

void
av::gua::TexturedScreenSpaceQuadNode::setHeightCB(const SFInt::SetValueEvent& event)
{
  auto size = m_guaNode->data.get_size();
  size[1] = event.getValue();
  m_guaNode->data.set_size(size);
}

void
av::gua::TexturedScreenSpaceQuadNode::getAnchorCB(const SFVec2::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.anchor();
}

void
av::gua::TexturedScreenSpaceQuadNode::setAnchorCB(const SFVec2::SetValueEvent& event)
{
  m_guaNode->data.anchor() = event.getValue();
}

void
av::gua::TexturedScreenSpaceQuadNode::getOffsetCB(const SFVec2::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.offset();
}

void
av::gua::TexturedScreenSpaceQuadNode::setOffsetCB(const SFVec2::SetValueEvent& event)
{
  m_guaNode->data.offset() = event.getValue();
}

void
av::gua::TexturedScreenSpaceQuadNode::getOpacityCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.opacity();
}

void
av::gua::TexturedScreenSpaceQuadNode::setOpacityCB(const SFFloat::SetValueEvent& event)
{
  m_guaNode->data.opacity() = event.getValue();
}

void
av::gua::TexturedScreenSpaceQuadNode::getFlipXCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.flip_x();
}

void
av::gua::TexturedScreenSpaceQuadNode::setFlipXCB(const SFBool::SetValueEvent& event)
{
  m_guaNode->data.flip_x() = event.getValue();
}

void
av::gua::TexturedScreenSpaceQuadNode::getFlipYCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.flip_y();
}

void
av::gua::TexturedScreenSpaceQuadNode::setFlipYCB(const SFBool::SetValueEvent& event)
{
  m_guaNode->data.flip_y() = event.getValue();
}

