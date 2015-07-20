#include <avango/gua/scenegraph/Video3DNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::Video3DNode);

AV_FIELD_DEFINE(av::gua::SFVideo3DNode);
AV_FIELD_DEFINE(av::gua::MFVideo3DNode);

av::gua::Video3DNode::Video3DNode(std::shared_ptr< ::gua::node::Video3DNode> guanode)
  : GeometryNode(guanode)
{
}

//av::gua::Video3DNode::~Video3DNode()
//{}

void
av::gua::Video3DNode::initClass()
{
  if (!isTypeInitialized()) {
    av::gua::GeometryNode::initClass();

    AV_FC_INIT(av::gua::GeometryNode, av::gua::Video3DNode, true);

    SFVideo3DNode::initClass("av::gua::SFVideo3DNode", "av::Field");
    MFVideo3DNode::initClass("av::gua::MFVideo3DNode", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}
