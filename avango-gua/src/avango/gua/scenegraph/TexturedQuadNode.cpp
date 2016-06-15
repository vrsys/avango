#include <avango/gua/scenegraph/TexturedQuadNode.hpp>
#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::TexturedQuadNode);

AV_FIELD_DEFINE(av::gua::SFTexturedQuadNode);
AV_FIELD_DEFINE(av::gua::MFTexturedQuadNode);

av::gua::TexturedQuadNode::TexturedQuadNode(std::shared_ptr< ::gua::node::TexturedQuadNode> guanode)
    : Node(guanode),
      m_guaNode(guanode)
{
    AV_FC_ADD_ADAPTOR_FIELD(Texture,
                          std::bind(&TexturedQuadNode::getTextureCB, this,std::placeholders::_1),
                          std::bind(&TexturedQuadNode::setTextureCB, this,std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(Width,
                          std::bind(&TexturedQuadNode::getWidthCB, this,std::placeholders::_1),
                          std::bind(&TexturedQuadNode::setWidthCB, this,std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(Height,
                          std::bind(&TexturedQuadNode::getHeightCB, this,std::placeholders::_1),
                          std::bind(&TexturedQuadNode::setHeightCB, this,std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(FlipX,
                          std::bind(&TexturedQuadNode::getFlipXCB, this,std::placeholders::_1),
                          std::bind(&TexturedQuadNode::setFlipXCB, this,std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(FlipY,
                          std::bind(&TexturedQuadNode::getFlipYCB, this,std::placeholders::_1),
                          std::bind(&TexturedQuadNode::setFlipYCB, this,std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(ScaledTransform,
                          std::bind(&TexturedQuadNode::getScaledTransformCB, this,std::placeholders::_1),
                          std::bind(&TexturedQuadNode::setScaledTransformCB, this,std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(ScaledWorldTransform,
                          std::bind(&TexturedQuadNode::getScaledWorldTransformCB, this,std::placeholders::_1),
                          std::bind(&TexturedQuadNode::setScaledWorldTransformCB, this,std::placeholders::_1));

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

        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr< ::gua::node::TexturedQuadNode>
av::gua::TexturedQuadNode::getGuaNode() const
{
    return m_guaNode;
}

void
av::gua::TexturedQuadNode::getTextureCB(const SFString::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.get_texture();
}

void
av::gua::TexturedQuadNode::setTextureCB(const SFString::SetValueEvent& event)
{
    m_guaNode->data.set_texture(event.getValue());
}

void
av::gua::TexturedQuadNode::getWidthCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.get_size()[0];
}

void
av::gua::TexturedQuadNode::setWidthCB(const SFFloat::SetValueEvent& event)
{
  auto size = m_guaNode->data.get_size();
  size[0] = event.getValue();
  m_guaNode->data.set_size(size);
}

void
av::gua::TexturedQuadNode::getHeightCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.get_size()[1];
}

void
av::gua::TexturedQuadNode::setHeightCB(const SFFloat::SetValueEvent& event)
{
  auto size = m_guaNode->data.get_size();
  size[1] = event.getValue();
  m_guaNode->data.set_size(size);
}

void
av::gua::TexturedQuadNode::getFlipXCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.flip_x();
}

void
av::gua::TexturedQuadNode::setFlipXCB(const SFBool::SetValueEvent& event)
{
  m_guaNode->data.flip_x() = event.getValue();
}

void
av::gua::TexturedQuadNode::getFlipYCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.flip_y();
}

void
av::gua::TexturedQuadNode::setFlipYCB(const SFBool::SetValueEvent& event)
{
  m_guaNode->data.flip_y() = event.getValue();
}

void
av::gua::TexturedQuadNode::getScaledTransformCB(const SFMatrix::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->get_scaled_transform();
}

void
av::gua::TexturedQuadNode::setScaledTransformCB(const SFMatrix::SetValueEvent& event)
{}

void
av::gua::TexturedQuadNode::getScaledWorldTransformCB(const SFMatrix::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->get_scaled_world_transform();
}

void
av::gua::TexturedQuadNode::setScaledWorldTransformCB(const SFMatrix::SetValueEvent& event)
{}

