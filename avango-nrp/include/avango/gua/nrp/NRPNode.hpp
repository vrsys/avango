#ifndef AVANGO_GUA_NRP_NODE_HPP
#define AVANGO_GUA_NRP_NODE_HPP

#include <gua/nrp/nrp_node.hpp>

#include <avango/gua/scenegraph/TransformNode.hpp>
#include <avango/gua/scenegraph/TriMeshNode.hpp>

namespace av
{
namespace gua
{
namespace nrp
{
class AV_GUA_DLL NRPNode : public av::gua::TransformNode
{
    AV_FC_DECLARE();

  public:
    NRPNode(std::shared_ptr<::gua::nrp::NRPNode> guanode = std::make_shared<::gua::nrp::NRPNode>(""));

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~NRPNode();

  public:
    /**
     * Get the wrapped ::gua::nrp::NRPNode.
     */
    std::shared_ptr<::gua::nrp::NRPNode> getGuaNode() const;
    std::mutex& getMutexNode();
    void createChildren(std::shared_ptr<::gua::node::Node> root, bool not_root = true) const;

  private:
    std::shared_ptr<::gua::nrp::NRPNode> m_guaNode;
    std::mutex m_mutex_node;

    NRPNode(const NRPNode &);
    NRPNode &operator=(const NRPNode &);
};

using SFNRPNode = SingleField<Link<NRPNode>>;
using MFNRPNode = MultiField<Link<NRPNode>>;
}
}

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL av::SingleField<av::Link<gua::nrp::NRPNode>>;
template class AV_GUA_DLL av::MultiField<av::Link<gua::nrp::NRPNode>>;
#endif
}

#endif // AVANGO_GUA_NRP_NODE_HPP
