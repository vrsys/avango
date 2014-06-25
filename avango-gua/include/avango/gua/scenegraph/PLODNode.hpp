#ifndef AVANGO_GUA_PLOD_NODE_HPP
#define AVANGO_GUA_PLOD_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/PLODNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/GeometryNode.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::PLODNode
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL PLODNode : public av::gua::GeometryNode
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::PLODNode
       * is created.
       * Otherwise, the given ::gua::PLODNode is used.
       */
      PLODNode(std::shared_ptr< ::gua::node::PLODNode> guanode =
          std::shared_ptr< ::gua::node::PLODNode>(new ::gua::node::PLODNode("")));

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~PLODNode();

    public:

      /**
       * Get the wrapped ::gua::PLODNode.
       */
      //std::shared_ptr< ::gua::PLODNode> getGuaNode() const;

    private:

      PLODNode(const PLODNode&);
      PLODNode& operator=(const PLODNode&);
    };

    typedef SingleField<Link<PLODNode> > SFPLODNode;
    typedef MultiField<Link<PLODNode> > MFPLODNode;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::PLODNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::PLODNode> >;
#endif

}

#endif //AVANGO_GUA_PLOD_NODE_HPP
