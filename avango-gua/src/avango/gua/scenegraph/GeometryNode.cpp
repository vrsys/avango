#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::GeometryNode);

AV_FIELD_DEFINE(av::gua::SFGeometryNode);
AV_FIELD_DEFINE(av::gua::MFGeometryNode);

av::gua::GeometryNode::GeometryNode(std::shared_ptr< ::gua::GeometryNode> guanode)
    : Node(guanode),
      m_guaNode(std::dynamic_pointer_cast< ::gua::GeometryNode>(Node::getGuaNode()))
{
    AV_FC_ADD_ADAPTOR_FIELD(Geometry,
                          boost::bind(&GeometryNode::getGeometryCB, this, _1),
                          boost::bind(&GeometryNode::setGeometryCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Material,
                          boost::bind(&GeometryNode::getMaterialCB, this, _1),
                          boost::bind(&GeometryNode::setMaterialCB, this, _1));
}

//av::gua::GeometryNode::~GeometryNode()
//{}

void
av::gua::GeometryNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::Node::initClass();

        AV_FC_INIT(av::gua::Node, av::gua::GeometryNode, true);

        SFGeometryNode::initClass("av::gua::SFGeometryNode", "av::Field");
        MFGeometryNode::initClass("av::gua::MFGeometryNode", "av::Field");
    }
}

std::shared_ptr< ::gua::GeometryNode>
av::gua::GeometryNode::getGuaNode() const
{
    return m_guaNode;
}

void
av::gua::GeometryNode::getGeometryCB(const SFString::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.geometry();
}

void
av::gua::GeometryNode::setGeometryCB(const SFString::SetValueEvent& event)
{
    m_guaNode->data.geometry() = event.getValue();
}

void
av::gua::GeometryNode::getMaterialCB(const SFString::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.material();
}

void
av::gua::GeometryNode::setMaterialCB(const SFString::SetValueEvent& event)
{
    m_guaNode->data.material() = event.getValue();
}
