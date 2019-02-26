#ifndef AVANGO_GUA_GROUP_NODE_HPP
#define AVANGO_GUA_GROUP_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/TransformNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/Node.hpp>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::TransformNode
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL TransformNode : public av::gua::Node
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::TransformNode is created.
     * Otherwise, the given ::gua::TransformNode is used.
     */
    TransformNode(std::shared_ptr<::gua::node::TransformNode> guanode = std::shared_ptr<::gua::node::TransformNode>(new ::gua::node::TransformNode("")));

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~TransformNode();

  public:
    /**
     * Get the wrapped ::gua::TransformNode.
     */
    std::shared_ptr<::gua::node::TransformNode> getGuaNode() const;

  public:
  private:
    std::shared_ptr<::gua::node::TransformNode> m_guaNode;

    TransformNode(const TransformNode&);
    TransformNode& operator=(const TransformNode&);
};

using SFTransformNode = SingleField<Link<TransformNode>>;
using MFTransformNode = MultiField<Link<TransformNode>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::TransformNode>>;
template class AV_GUA_DLL MultiField<Link<gua::TransformNode>>;
#endif

} // namespace av

#endif // AVANGO_GUA_GROUP_NODE_HPP
