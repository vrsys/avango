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

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
    virtual void on_distribute(av::gua::NetTransform &netNode);
    virtual void on_undistribute(av::gua::NetTransform &netNode);
#endif

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    // virtual ~NRPNode();

  public:
    /**
     * Get the wrapped ::gua::nrp::NRPNode.
     */
    std::shared_ptr<::gua::nrp::NRPNode> getGuaNode() const;
    std::mutex &getMutexNode();
    void createChildren(std::shared_ptr<::gua::node::Node> root, bool is_root = true) const;
    /*void distributeChildren(av::gua::Node *node);*/

  private:
    std::shared_ptr<av::gua::NetTransform> m_net_transform;
    std::shared_ptr<::gua::nrp::NRPNode> m_guaNode;
    std::mutex m_mutex_node;

    NRPNode(const NRPNode &);
    NRPNode &operator=(const NRPNode &);
};

using SFNRPNode = SingleField<Link<NRPNode>>;
using MFNRPNode = MultiField<Link<NRPNode>>;
} // namespace nrp
} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL av::SingleField<av::Link<gua::nrp::NRPNode>>;
template class AV_GUA_DLL av::MultiField<av::Link<gua::nrp::NRPNode>>;
#endif
} // namespace av

#endif // AVANGO_GUA_NRP_NODE_HPP
