#ifndef AVANGO_GUA_VIDEO3D_NODE_HPP
#define AVANGO_GUA_VIDEO3D_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/video3d/Video3DNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/renderer/Material.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::Video3DNode
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL Video3DNode : public av::gua::GeometryNode
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::Video3DNode
       * is created.
       * Otherwise, the given ::gua::Video3DNode is used.
       */
      Video3DNode(std::shared_ptr< ::gua::node::Video3DNode> guanode =
          std::shared_ptr< ::gua::node::Video3DNode>(new ::gua::node::Video3DNode("")));

      virtual void on_distribute(av::gua::NetTransform& netNode);
      virtual void on_undistribute(av::gua::NetTransform& netNode);


    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~Video3DNode();

    public:

      SFString   Geometry;
      SFMaterial Material;
      SFBool     RenderToGBuffer;
      SFBool     RenderToStencilBuffer;

      virtual void getGeometryCB(const SFString::GetValueEvent& event);
      virtual void setGeometryCB(const SFString::SetValueEvent& event);

      virtual void getMaterialCB(const SFMaterial::GetValueEvent& event);
      virtual void setMaterialCB(const SFMaterial::SetValueEvent& event);

      virtual void getRenderToGBufferCB(const SFBool::GetValueEvent& event);
      virtual void setRenderToGBufferCB(const SFBool::SetValueEvent& event);

      virtual void getRenderToStencilBufferCB(const SFBool::GetValueEvent& event);
      virtual void setRenderToStencilBufferCB(const SFBool::SetValueEvent& event);

      /**
       * Get the wrapped ::gua::Video3DNode.
       */
      std::shared_ptr< ::gua::node::Video3DNode> getGuaVideo3DNode() const;

    private:

      std::shared_ptr< ::gua::node::Video3DNode> m_guaVideo3DNode;
      av::Link< av::gua::Material> m_Material;

      Video3DNode(const Video3DNode&);
      Video3DNode& operator=(const Video3DNode&);
    };

    using SFVideo3DNode = SingleField<Link<Video3DNode> >;
    using MFVideo3DNode = MultiField<Link<Video3DNode> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::Video3DNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::Video3DNode> >;
#endif

}

#endif //AVANGO_GUA_VIDEO3D_NODE_HPP
