#ifndef AVANGO_GUA_PLOD_NODE_HPP
#define AVANGO_GUA_PLOD_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/PLODNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/renderer/Material.hpp>

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

      virtual void on_distribute(av::gua::NetTransform& netNode);
      virtual void on_undistribute(av::gua::NetTransform& netNode);

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~PLODNode();

    public:

      SFString   Geometry;
      SFMaterial Material;
      SFFloat    Importance;
      SFBool     EnableBackfaceCulling;

      virtual void getGeometryCB(const SFString::GetValueEvent& event);
      virtual void setGeometryCB(const SFString::SetValueEvent& event);

      virtual void getMaterialCB(const SFMaterial::GetValueEvent& event);
      virtual void setMaterialCB(const SFMaterial::SetValueEvent& event);

      virtual void getImportanceCB(const SFFloat::GetValueEvent& event);
      virtual void setImportanceCB(const SFFloat::SetValueEvent& event);

      virtual void getEnableBackfaceCullingCB(const SFBool::GetValueEvent& event);
      virtual void setEnableBackfaceCullingCB(const SFBool::SetValueEvent& event);

      /**
       * Get the wrapped ::gua::PLODNode.
       */
      std::shared_ptr< ::gua::node::PLODNode> getGuaPLODNode() const;

    private:

      std::shared_ptr< ::gua::node::PLODNode> m_guaPLODNode;
      av::Link< av::gua::Material> m_Material;

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
