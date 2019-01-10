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

std::shared_ptr<gua::nrp::NRPNode> av::gua::nrp::NRPNode::getGuaNode() const { return m_guaNode; }
std::mutex &av::gua::nrp::NRPNode::getMutexNode() { return m_mutex_node; }

void av::gua::nrp::NRPNode::createChildren(std::shared_ptr<::gua::node::Node> root, bool is_root) const
{
    av::gua::Node *av_node(nullptr);

    if(!is_root)
    {
        auto group_cast(std::dynamic_pointer_cast<::gua::node::TransformNode>(root));

        if(group_cast)
        {
            av_node = new av::gua::TransformNode(group_cast);
        }

        if(!av_node)
        {
            auto geom_cast(std::dynamic_pointer_cast<::gua::node::TriMeshNode>(root));
            if(geom_cast)
            {
                av_node = new av::gua::TriMeshNode(geom_cast);
            }
        }

        if(av_node)
        {
            av_node->addToParentChildren();
        }
    }

    for(auto c : root->get_children())
    {
        createChildren(c, false);
    }
}

av::gua::nrp::NRPNode::NRPNode(std::shared_ptr<::gua::nrp::NRPNode> guanode) : TransformNode(guanode), m_guaNode(guanode)
{
    // maintain a hook to pre-update state
    guanode->set_pre_pass([&] {});
    guanode->set_post_pass([&] {
        if(this->getGuaNode()->get_should_update_avango())
        {
            std::unique_lock<std::mutex> lock_scene(this->getMutexNode());
            std::cout << "post pass" << std::endl;
            this->createChildren(this->getGuaNode(), true);

            this->getGuaNode()->set_should_update_avango(false);
        }
    });
}

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