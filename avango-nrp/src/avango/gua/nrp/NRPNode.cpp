#include <avango/Base.h>
#include <avango/gua/nrp/NRPNode.hpp>
#include <boost/bind.hpp>

namespace
{
av::Logger &logger(av::getLogger("av::gua::nrp::NRPNode"));
}

AV_FC_DEFINE(av::gua::nrp::NRPNode);

AV_FIELD_DEFINE(av::gua::nrp::SFNRPNode);
AV_FIELD_DEFINE(av::gua::nrp::MFNRPNode);

av::gua::nrp::NRPNode::NRPNode(std::shared_ptr<::gua::nrp::NRPNode> guanode) : TransformNode(guanode), m_guaNode(guanode) {}

av::gua::nrp::NRPNode::~NRPNode() {}

void av::gua::nrp::NRPNode::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::TransformNode::initClass();

        AV_FC_INIT(av::gua::TransformNode, av::gua::nrp::NRPNode, true);

        av::gua::nrp::SFNRPNode::initClass("av::gua::nrp::SFNRPNode", "av::Field");
        av::gua::nrp::MFNRPNode::initClass("av::gua::nrp::MFNRPNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}
std::shared_ptr<gua::node::Node> av::gua::nrp::NRPNode::getGuaNode() const { return m_guaNode; }