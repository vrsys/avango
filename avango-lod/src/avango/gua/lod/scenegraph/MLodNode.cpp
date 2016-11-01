#include <avango/gua/lod/scenegraph/MLodNode.hpp>
#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::lod::MLodNode);

AV_FIELD_DEFINE(av::gua::lod::SFMLodNode);
AV_FIELD_DEFINE(av::gua::lod::MFMLodNode);

av::gua::lod::MLodNode::MLodNode(std::shared_ptr< ::gua::node::MLodNode> guanode)
    : GeometryNode(guanode)
    , m_guaMLodNode(std::dynamic_pointer_cast< ::gua::node::MLodNode>(GeometryNode::getGuaNode()))
{
}

void
av::gua::lod::MLodNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::GeometryNode::initClass();

        AV_FC_INIT(av::gua::GeometryNode, av::gua::lod::MLodNode, true);

        SFMLodNode::initClass("av::gua::lod::SFMLodNode", "av::Field");
        MFMLodNode::initClass("av::gua::lod::MFMLodNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}
