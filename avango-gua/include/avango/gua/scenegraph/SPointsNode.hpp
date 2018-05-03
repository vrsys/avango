#ifndef AVANGO_GUA_SPOINTS_NODE_HPP
#define AVANGO_GUA_SPOINTS_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/spoints/SPointsNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/renderer/Material.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::SPointsNode
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL SPointsNode : public av::gua::GeometryNode
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::SPointsNode
       * is created.
       * Otherwise, the given ::gua::SPointsNode is used.
       */
      SPointsNode(std::shared_ptr< ::gua::node::SPointsNode> guanode =
          std::shared_ptr< ::gua::node::SPointsNode>(new ::gua::node::SPointsNode("")));

      virtual void on_distribute(av::gua::NetTransform& netNode);
      virtual void on_undistribute(av::gua::NetTransform& netNode);


    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~SPointsNode();

    public:

      SFString   Geometry;
      SFMaterial Material;
      SFBool     RenderToGBuffer;
      SFBool     RenderToStencilBuffer;
      SFFloat    ScreenSpacePointSize;

      virtual void getGeometryCB(const SFString::GetValueEvent& event);
      virtual void setGeometryCB(const SFString::SetValueEvent& event);

      virtual void getMaterialCB(const SFMaterial::GetValueEvent& event);
      virtual void setMaterialCB(const SFMaterial::SetValueEvent& event);

      virtual void getRenderToGBufferCB(const SFBool::GetValueEvent& event);
      virtual void setRenderToGBufferCB(const SFBool::SetValueEvent& event);

      virtual void getRenderToStencilBufferCB(const SFBool::GetValueEvent& event);
      virtual void setRenderToStencilBufferCB(const SFBool::SetValueEvent& event);

      virtual void getScreenSpacePointSizeCB(const SFFloat::GetValueEvent& event);
      virtual void setScreenSpacePointSizeCB(const SFFloat::SetValueEvent& event);

      /**
       * Get the wrapped ::gua::SPointsNode.
       */
      std::shared_ptr< ::gua::node::SPointsNode> getGuaSPointsNode() const;

    private:

      std::shared_ptr< ::gua::node::SPointsNode> m_guaSPointsNode;
      av::Link< av::gua::Material> m_Material;

      SPointsNode(const SPointsNode&);
      SPointsNode& operator=(const SPointsNode&);
    };

    using SFSPointsNode = SingleField<Link<SPointsNode> >;
    using MFSPointsNode = MultiField<Link<SPointsNode> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::SPointsNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::SPointsNode> >;
#endif

}

#endif //AVANGO_GUA_SPOINTS_NODE_HPP
