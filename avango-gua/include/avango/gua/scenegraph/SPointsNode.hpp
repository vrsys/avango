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
      // TODO libpcc extend to add fields
      SFInt GlobalGridDimensionX;
      SFInt GlobalGridDimensionY;
      SFInt GlobalGridDimensionZ;
      SFInt GlobalPointPrecisionX;
      SFInt GlobalPointPrecisionY;
      SFInt GlobalPointPrecisionZ;
      SFInt GlobalColorPrecisionX;
      SFInt GlobalColorPrecisionY;
      SFInt GlobalColorPrecisionZ;
      /*
      SFInt grid_y;
      SFInt grid_z;
      */

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

      virtual void getGlobalGridDimensionXCB(const SFInt::GetValueEvent& event);
      virtual void setGlobalGridDimensionXCB(const SFInt::SetValueEvent& event);

      virtual void getGlobalGridDimensionYCB(const SFInt::GetValueEvent& event);
      virtual void setGlobalGridDimensionYCB(const SFInt::SetValueEvent& event);

      virtual void getGlobalGridDimensionZCB(const SFInt::GetValueEvent& event);
      virtual void setGlobalGridDimensionZCB(const SFInt::SetValueEvent& event);

      virtual void getGlobalPointPrecisionXCB(const SFInt::GetValueEvent& event);
      virtual void setGlobalPointPrecisionXCB(const SFInt::SetValueEvent& event);

      virtual void getGlobalPointPrecisionYCB(const SFInt::GetValueEvent& event);
      virtual void setGlobalPointPrecisionYCB(const SFInt::SetValueEvent& event);

      virtual void getGlobalPointPrecisionZCB(const SFInt::GetValueEvent& event);
      virtual void setGlobalPointPrecisionZCB(const SFInt::SetValueEvent& event);

      virtual void getGlobalColorPrecisionXCB(const SFInt::GetValueEvent& event);
      virtual void setGlobalColorPrecisionXCB(const SFInt::SetValueEvent& event);

      virtual void getGlobalColorPrecisionYCB(const SFInt::GetValueEvent& event);
      virtual void setGlobalColorPrecisionYCB(const SFInt::SetValueEvent& event);

      virtual void getGlobalColorPrecisionZCB(const SFInt::GetValueEvent& event);
      virtual void setGlobalColorPrecisionZCB(const SFInt::SetValueEvent& event);
      
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
