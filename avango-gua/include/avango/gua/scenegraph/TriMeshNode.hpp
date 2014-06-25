#ifndef AVANGO_GUA_TRI_MESH_NODE_HPP
#define AVANGO_GUA_TRI_MESH_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/TriMeshNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/GeometryNode.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::TriMeshNode
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL TriMeshNode : public av::gua::GeometryNode
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::TriMeshNode is created.
       * Otherwise, the given ::gua::TriMeshNode is used.
       */
      TriMeshNode(std::shared_ptr< ::gua::node::TriMeshNode> guanode =
          std::shared_ptr< ::gua::node::TriMeshNode>(new ::gua::node::TriMeshNode("")));

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~TriMeshNode();

    public:

      /**
       * Get the wrapped ::gua::TriMeshNode.
       */
      //std::shared_ptr< ::gua::TriMeshNode> getGuaNode() const;

    private:

      TriMeshNode(const TriMeshNode&);
      TriMeshNode& operator=(const TriMeshNode&);
    };

    typedef SingleField<Link<TriMeshNode> > SFTriMeshNode;
    typedef MultiField<Link<TriMeshNode> > MFTriMeshNode;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::TriMeshNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::TriMeshNode> >;
#endif

}

#endif //AVANGO_GUA_TRI_MESH_NODE_HPP
