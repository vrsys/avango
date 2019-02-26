#ifndef AVANGO_GUA_RAY_NODE_HPP
#define AVANGO_GUA_RAY_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/RayNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/Node.hpp>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::RayNode
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL RayNode : public av::gua::Node
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::RayNode is created.
     * Otherwise, the given ::gua::RayNode is used.
     */
    RayNode(std::shared_ptr<::gua::node::RayNode> guanode = std::shared_ptr<::gua::node::RayNode>(new ::gua::node::RayNode("")));

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    //      virtual ~RayNode();

  public:
    /**
     * Get the wrapped ::gua::RayNode.
     */
    std::shared_ptr<::gua::node::RayNode> getGuaNode() const;

  public:
  private:
    std::shared_ptr<::gua::node::RayNode> m_guaNode;

    RayNode(const RayNode&);
    RayNode& operator=(const RayNode&);
};

using SFRayNode = SingleField<Link<RayNode>>;
using MFRayNode = MultiField<Link<RayNode>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::RayNode>>;
template class AV_GUA_DLL MultiField<Link<gua::RayNode>>;
#endif

} // namespace av

#endif // AVANGO_GUA_RAY_NODE_HPP
