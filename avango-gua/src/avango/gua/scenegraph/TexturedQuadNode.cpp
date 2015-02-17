#include <avango/gua/scenegraph/TexturedQuadNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::TexturedQuadNode);

AV_FIELD_DEFINE(av::gua::SFTexturedQuadNode);
AV_FIELD_DEFINE(av::gua::MFTexturedQuadNode);

av::gua::TexturedQuadNode::TexturedQuadNode(std::shared_ptr< ::gua::node::TexturedQuadNode> guanode)
    : Node(guanode),
      m_guaNode(guanode)
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

    AV_FC_ADD_ADAPTOR_FIELD(FlipX,
                          boost::bind(&TexturedQuadNode::getFlipXCB, this, _1),
                          boost::bind(&TexturedQuadNode::setFlipXCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(FlipY,
                          boost::bind(&TexturedQuadNode::getFlipYCB, this, _1),
                          boost::bind(&TexturedQuadNode::setFlipYCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(ScaledTransform,
                          boost::bind(&TexturedQuadNode::getScaledTransformCB, this, _1),
                          boost::bind(&TexturedQuadNode::setScaledTransformCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(ScaledWorldTransform,
                          boost::bind(&TexturedQuadNode::getScaledWorldTransformCB, this, _1),
                          boost::bind(&TexturedQuadNode::setScaledWorldTransformCB, this, _1));

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

