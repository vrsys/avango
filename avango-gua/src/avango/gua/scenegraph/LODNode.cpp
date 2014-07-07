#include <avango/gua/scenegraph/LODNode.hpp>
#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::LODNode"));
}

AV_FC_DEFINE(av::gua::LODNode);

AV_FIELD_DEFINE(av::gua::SFLODNode);
AV_FIELD_DEFINE(av::gua::MFLODNode);

av::gua::LODNode::LODNode(std::shared_ptr< ::gua::node::LODNode> guanode)
  : TransformNode(guanode),
    m_guaNode(std::dynamic_pointer_cast< ::gua::node::LODNode> (Node::getGuaNode()))
{
    AV_FC_ADD_ADAPTOR_FIELD(LODDistances,
                          boost::bind(&LODNode::getLODDistancesCB, this, _1),
                          boost::bind(&LODNode::setLODDistancesCB, this, _1));

}

av::gua::LODNode::~LODNode()
{}

void
av::gua::LODNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::TransformNode::initClass();

        AV_FC_INIT(av::gua::TransformNode, av::gua::LODNode, true);

        SFLODNode::initClass("av::gua::SFLODNode", "av::Field");
        MFLODNode::initClass("av::gua::MFLODNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr< ::gua::node::LODNode>
av::gua::LODNode::getGuaNode() const
{
    return m_guaNode;
}

void
av::gua::LODNode::getLODDistancesCB(const MFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->data.lod_distances();
}

void
av::gua::LODNode::setLODDistancesCB(const MFFloat::SetValueEvent& event)
{
    m_guaNode->data.lod_distances() = event.getValue();
}
