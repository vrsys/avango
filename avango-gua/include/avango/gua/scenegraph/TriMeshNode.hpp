#ifndef AVANGO_GUA_TRI_MESH_NODE_HPP
#define AVANGO_GUA_TRI_MESH_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/TriMeshNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/renderer/Material.hpp>

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

      virtual void on_distribute(av::gua::NetTransform& netNode);
      virtual void on_undistribute(av::gua::NetTransform& netNode);

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~TriMeshNode();

    public:

      SFString   Geometry;
      SFMaterial Material;

      virtual void getGeometryCB(const SFString::GetValueEvent& event);
      virtual void setGeometryCB(const SFString::SetValueEvent& event);

      virtual void getMaterialCB(const SFMaterial::GetValueEvent& event);
      virtual void setMaterialCB(const SFMaterial::SetValueEvent& event);

      /**
       * Get the wrapped ::gua::TriMeshNode.
       */
      std::shared_ptr< ::gua::node::TriMeshNode> getGuaTriMeshNode() const;

    private:

      std::shared_ptr< ::gua::node::TriMeshNode> m_guaTriMeshNode;
      av::Link< av::gua::Material> m_Material;

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
