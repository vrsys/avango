#include <avango/gua/scenegraph/VolumeNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::VolumeNode);

AV_FIELD_DEFINE(av::gua::SFVolumeNode);
AV_FIELD_DEFINE(av::gua::MFVolumeNode);

av::gua::VolumeNode::VolumeNode(std::shared_ptr< ::gua::VolumeNode> guanode)
    : Node(guanode),
      m_guaNode(std::dynamic_pointer_cast< ::gua::VolumeNode>(Node::getGuaNode()))
{
//    AV_FC_ADD_ADAPTOR_FIELD(Geometry,
//                          boost::bind(&VolumeNode::getGeometryCB, this, _1),
//                          boost::bind(&VolumeNode::setGeometryCB, this, _1));
}

//av::gua::VolumeNode::~VolumeNode()
//{}

void
av::gua::VolumeNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::Node::initClass();

        AV_FC_INIT(av::gua::Node, av::gua::VolumeNode, true);

        SFVolumeNode::initClass("av::gua::SFVolumeNode", "av::Field");
        MFVolumeNode::initClass("av::gua::MFVolumeNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr< ::gua::VolumeNode>
av::gua::VolumeNode::getGuaNode() const
{
    return m_guaNode;
}

#if 0
void
av::gua::VolumeNode::getGeometryCB(const SFString::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.geometry();
}

void
av::gua::VolumeNode::setGeometryCB(const SFString::SetValueEvent& event)
{
    m_guaNode->data.geometry() = event.getValue();
}
#endif
