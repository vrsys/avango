#include <avango/gua/scenegraph/PLODNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::PLODNode);

AV_FIELD_DEFINE(av::gua::SFPLODNode);
AV_FIELD_DEFINE(av::gua::MFPLODNode);

av::gua::PLODNode::PLODNode(std::shared_ptr< ::gua::node::PLODNode> guanode)
    : GeometryNode(guanode)
{
}

//av::gua::PLODNode::~PLODNode()
//{}

void
av::gua::PLODNode::initClass()
{
  if (!isTypeInitialized()) {
    av::gua::GeometryNode::initClass();

    AV_FC_INIT(av::gua::GeometryNode, av::gua::PLODNode, true);

    SFPLODNode::initClass("av::gua::SFPLODNode", "av::Field");
    MFPLODNode::initClass("av::gua::MFPLODNode", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}
