// #include <avango/gua/scenegraph/NURBSNode.hpp>
// #include <avango/Base.h>
// #include <boost/bind.hpp>

// AV_FC_DEFINE(av::gua::NURBSNode);

// AV_FIELD_DEFINE(av::gua::SFNURBSNode);
// AV_FIELD_DEFINE(av::gua::MFNURBSNode);

// av::gua::NURBSNode::NURBSNode(std::shared_ptr< ::gua::node::NURBSNode> guanode)
//     : GeometryNode(guanode)
// {
// }

// //av::gua::NURBSNode::~NURBSNode()
// //{}

// void
// av::gua::NURBSNode::initClass()
// {
//   if (!isTypeInitialized()) {
//     av::gua::GeometryNode::initClass();

//     AV_FC_INIT(av::gua::GeometryNode, av::gua::NURBSNode, true);

//     SFNURBSNode::initClass("av::gua::SFNURBSNode", "av::Field");
//     MFNURBSNode::initClass("av::gua::MFNURBSNode", "av::Field");

//     sClassTypeId.setDistributable(true);
//   }
// }
