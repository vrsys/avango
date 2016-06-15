#include <avango/gua/scenegraph/TexturedScreenSpaceQuadNode.hpp>
#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::TexturedScreenSpaceQuadNode);

AV_FIELD_DEFINE(av::gua::SFTexturedScreenSpaceQuadNode);
AV_FIELD_DEFINE(av::gua::MFTexturedScreenSpaceQuadNode);

av::gua::TexturedScreenSpaceQuadNode::TexturedScreenSpaceQuadNode(std::shared_ptr< ::gua::node::TexturedScreenSpaceQuadNode> guanode)
    : Node(guanode),
      m_guaNode(guanode)
{
    AV_FC_ADD_ADAPTOR_FIELD(Texture,
                          std::bind(&TexturedScreenSpaceQuadNode::getTextureCB, this,std::placeholders::_1),
                          std::bind(&TexturedScreenSpaceQuadNode::setTextureCB, this,std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(Width,
                          std::bind(&TexturedScreenSpaceQuadNode::getWidthCB, this,std::placeholders::_1),
                          std::bind(&TexturedScreenSpaceQuadNode::setWidthCB, this,std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(Height,
                          std::bind(&TexturedScreenSpaceQuadNode::getHeightCB, this,std::placeholders::_1),
                          std::bind(&TexturedScreenSpaceQuadNode::setHeightCB, this,std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(Anchor,
                          std::bind(&TexturedScreenSpaceQuadNode::getAnchorCB, this,std::placeholders::_1),
                          std::bind(&TexturedScreenSpaceQuadNode::setAnchorCB, this,std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(Offset,
                          std::bind(&TexturedScreenSpaceQuadNode::getOffsetCB, this,std::placeholders::_1),
                          std::bind(&TexturedScreenSpaceQuadNode::setOffsetCB, this,std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(Opacity,
                          std::bind(&TexturedScreenSpaceQuadNode::getOpacityCB, this,std::placeholders::_1),
                          std::bind(&TexturedScreenSpaceQuadNode::setOpacityCB, this,std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(FlipX,
                          std::bind(&TexturedScreenSpaceQuadNode::getFlipXCB, this,std::placeholders::_1),
                          std::bind(&TexturedScreenSpaceQuadNode::setFlipXCB, this,std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(FlipY,
                          std::bind(&TexturedScreenSpaceQuadNode::getFlipYCB, this,std::placeholders::_1),
                          std::bind(&TexturedScreenSpaceQuadNode::setFlipYCB, this,std::placeholders::_1));

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

bool
av::gua::TexturedScreenSpaceQuadNode::pixel_to_texcoords(::gua::math::vec2 const& pixel,
                                                         ::gua::math::vec2ui const& screen_size,
                                                         ::gua::math::vec2& result) const {

  return m_guaNode->pixel_to_texcoords(pixel, screen_size, result);
}
