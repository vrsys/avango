#ifndef AVANGO_GUA_NURBS_NODE_HPP
#define AVANGO_GUA_NURBS_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/NURBSNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/GeometryNode.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::NURBSNode
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL NURBSNode : public av::gua::GeometryNode
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::NURBSNode
       * is created.
       * Otherwise, the given ::gua::NURBSNode is used.
       */
      NURBSNode(std::shared_ptr< ::gua::node::NURBSNode> guanode =
          std::shared_ptr< ::gua::node::NURBSNode>(new ::gua::node::NURBSNode("")));

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~NURBSNode();

    public:


      /**
       * Get the wrapped ::gua::NURBSNode.
       */
      //std::shared_ptr< ::gua::NURBSNode> getGuaNode() const;

    private:

      NURBSNode(const NURBSNode&);
      NURBSNode& operator=(const NURBSNode&);
    };

    typedef SingleField<Link<NURBSNode> > SFNURBSNode;
    typedef MultiField<Link<NURBSNode> > MFNURBSNode;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::NURBSNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::NURBSNode> >;
#endif

}

#endif //AVANGO_GUA_NURBS_NODE_HPP
