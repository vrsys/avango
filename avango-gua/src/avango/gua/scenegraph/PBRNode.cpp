// #include <avango/gua/scenegraph/PBRNode.hpp>
// #include <avango/Base.h>

// AV_FC_DEFINE(av::gua::PBRNode);

// AV_FIELD_DEFINE(av::gua::SFPBRNode);
// AV_FIELD_DEFINE(av::gua::MFPBRNode);

// av::gua::PBRNode::PBRNode(std::shared_ptr< ::gua::node::PBRNode> guanode)
//     : GeometryNode(guanode)
// {
// }

// //av::gua::PBRNode::~PBRNode()
// //{}

// void
// av::gua::PBRNode::initClass()
// {
//   if (!isTypeInitialized()) {
//     av::gua::GeometryNode::initClass();

//     AV_FC_INIT(av::gua::GeometryNode, av::gua::PBRNode, true);

//     SFPBRNode::initClass("av::gua::SFPBRNode", "av::Field");
//     MFPBRNode::initClass("av::gua::MFPBRNode", "av::Field");

//     sClassTypeId.setDistributable(true);
//   }
// }
