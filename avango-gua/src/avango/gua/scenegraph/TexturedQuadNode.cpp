#include <avango/gua/scenegraph/TexturedQuadNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::TexturedQuadNode);

AV_FIELD_DEFINE(av::gua::SFTexturedQuadNode);
AV_FIELD_DEFINE(av::gua::MFTexturedQuadNode);

av::gua::TexturedQuadNode::TexturedQuadNode(std::shared_ptr< ::gua::node::TexturedQuadNode> guanode)
    : Node(guanode),
      m_guaNode(std::dynamic_pointer_cast< ::gua::node::TexturedQuadNode>(Node::getGuaNode()))
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

