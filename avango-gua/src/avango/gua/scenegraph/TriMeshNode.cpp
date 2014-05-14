#include <avango/gua/scenegraph/TriMeshNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::TriMeshNode);

AV_FIELD_DEFINE(av::gua::SFTriMeshNode);
AV_FIELD_DEFINE(av::gua::MFTriMeshNode);

av::gua::TriMeshNode::TriMeshNode(std::shared_ptr< ::gua::TriMeshNode> guanode)
    : Node(guanode),
      m_guaNode(std::dynamic_pointer_cast< ::gua::TriMeshNode>(Node::getGuaNode()))
{
    AV_FC_ADD_ADAPTOR_FIELD(Geometry,
                          boost::bind(&TriMeshNode::getGeometryCB, this, _1),
                          boost::bind(&TriMeshNode::setGeometryCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Material,
                          boost::bind(&TriMeshNode::getMaterialCB, this, _1),
                          boost::bind(&TriMeshNode::setMaterialCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(ShadowMode,
                          boost::bind(&TriMeshNode::getShadowModeCB, this, _1),
                          boost::bind(&TriMeshNode::setShadowModeCB, this, _1));
}

//av::gua::TriMeshNode::~TriMeshNode()
//{}

void
av::gua::TriMeshNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::Node::initClass();

        AV_FC_INIT(av::gua::Node, av::gua::TriMeshNode, true);

        SFTriMeshNode::initClass("av::gua::SFTriMeshNode", "av::Field");
        MFTriMeshNode::initClass("av::gua::MFTriMeshNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr< ::gua::TriMeshNode>
av::gua::TriMeshNode::getGuaNode() const
{
    return m_guaNode;
}

void
av::gua::TriMeshNode::getGeometryCB(const SFString::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->get_filename();
}

void
av::gua::TriMeshNode::setGeometryCB(const SFString::SetValueEvent& event)
{
    m_guaNode->set_filename(event.getValue());
}

void
av::gua::TriMeshNode::getMaterialCB(const SFString::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->get_material();
}

void
av::gua::TriMeshNode::setMaterialCB(const SFString::SetValueEvent& event)
{
    m_guaNode->set_material(event.getValue());
}

void
av::gua::TriMeshNode::getShadowModeCB(const SFUInt::GetValueEvent& event)
{
    *(event.getValuePtr()) = static_cast<unsigned>(m_guaNode->get_shadow_mode());
}

void
av::gua::TriMeshNode::setShadowModeCB(const SFUInt::SetValueEvent& event)
{
    m_guaNode->set_shadow_mode(static_cast< ::gua::ShadowMode>(event.getValue()));
}
