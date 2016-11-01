#include <avango/gua/lod/scenegraph/PLodNode.hpp>
#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::lod::PLodNode);

AV_FIELD_DEFINE(av::gua::lod::SFPLodNode);
AV_FIELD_DEFINE(av::gua::lod::MFPLodNode);

av::gua::lod::PLodNode::PLodNode(std::shared_ptr< ::gua::node::PLodNode> guanode)
    : GeometryNode(guanode)
    , m_guaPLodNode(std::dynamic_pointer_cast< ::gua::node::PLodNode>(GeometryNode::getGuaNode()))
{
}

void
av::gua::lod::PLodNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::GeometryNode::initClass();

        AV_FC_INIT(av::gua::GeometryNode, av::gua::lod::PLodNode, true);

        SFPLodNode::initClass("av::gua::lod::SFPLodNode", "av::Field");
        MFPLodNode::initClass("av::gua::lod::MFPLodNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}
