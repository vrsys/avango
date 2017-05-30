#ifndef AVANGO_GUA_TV_3_NODE_HPP
#define AVANGO_GUA_TV_3_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/TV_3Node.hpp>

#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/renderer/Material.hpp>

namespace av
{
  namespace gua
  {
    namespace tv_3 
    {
      /**
       * Wrapper for ::gua::TV_3Node
       *
       * \ingroup av_gua
       */
      class AV_GUA_DLL TV_3Node : public av::gua::GeometryNode
      {
        AV_FC_DECLARE();

      public:

        enum RenderMode {
          VOL_ISOSURFACE    = ::gua::node::TV_3Node::VOL_ISOSURFACE,
          VOL_MAX_INTENSITY = ::gua::node::TV_3Node::VOL_MAX_INTENSITY,
          VOL_COMPOSITING   = ::gua::node::TV_3Node::VOL_COMPOSITING,
          VOL_AVG_INTENSITY = ::gua::node::TV_3Node::VOL_AVG_INTENSITY,
          SUR_PBR           = ::gua::node::TV_3Node::SUR_PBR
        };

        enum SpatialFilterMode {
          S_NEAREST = ::gua::node::TV_3Node::S_NEAREST, 
          S_LINEAR  = ::gua::node::TV_3Node::S_LINEAR
        };

        enum TemporalFilterMode {
          T_NEAREST = ::gua::node::TV_3Node::T_NEAREST, 
          T_LINEAR  = ::gua::node::TV_3Node::T_LINEAR
        };

        using SFRenderMode         = SingleField<Link<::gua::node::TV_3Node::RenderMode>>;
        using SFSpatialFilterMode  = SingleField<Link<::gua::node::TV_3Node::SpatialFilterMode>>;
        using SFTemporalFilterMode = SingleField<Link<::gua::node::TV_3Node::TemporalFilterMode>>;
        using MFRenderMode         = MultiField<Link<::gua::node::TV_3Node::RenderMode>>;
        using MFSpatialFilterMode  = MultiField<Link<::gua::node::TV_3Node::SpatialFilterMode>>;
        using MFTemporalFilterMode = MultiField<Link<::gua::node::TV_3Node::TemporalFilterMode>>;
        /**
         * Constructor. When called without arguments, a new ::gua::TV_3Node
         * is created.
         * Otherwise, the given ::gua::TV_3Node is used.
         */
        TV_3Node(std::shared_ptr< ::gua::node::TV_3Node> guanode =
            std::shared_ptr< ::gua::node::TV_3Node>(new ::gua::node::TV_3Node("")));

        virtual void on_distribute(av::gua::NetTransform& netNode);
        virtual void on_undistribute(av::gua::NetTransform& netNode);

      protected:

        /**
         * Destructor made protected to prevent allocation on stack.
         */
  //      virtual ~TV_3Node();

      public:

        SFString   Geometry;
        SFMaterial Material;
        SFInt      RenderMode;
        SFInt      SpatialFilterMode;
        SFInt      TemporalFilterMode;
        SFFloat    IsoValue;

        virtual void getGeometryCB(const SFString::GetValueEvent& event);
        virtual void setGeometryCB(const SFString::SetValueEvent& event);

        virtual void getMaterialCB(const SFMaterial::GetValueEvent& event);
        virtual void setMaterialCB(const SFMaterial::SetValueEvent& event);

        virtual void getRenderModeCB(const SFInt::GetValueEvent& event);
        virtual void setRenderModeCB(const SFInt::SetValueEvent& event);

        virtual void getSpatialFilterModeCB(const SFInt::GetValueEvent& event);
        virtual void enableSpatialLinearFilterCB(const SFInt::SetValueEvent& event);

        virtual void getTemporalFilterModeCB(const SFInt::GetValueEvent& event);
        virtual void enableTemporalLinearFilterCB(const SFInt::SetValueEvent& event);

        virtual void getIsoValueCB(const SFFloat::GetValueEvent& event);
        virtual void setIsoValueCB(const SFFloat::SetValueEvent& event);

        /**
         * Get the wrapped ::gua::TV_3Node.
         */
        std::shared_ptr< ::gua::node::TV_3Node> getGuaTV_3Node() const;

      private:

        std::shared_ptr< ::gua::node::TV_3Node> m_guaTV_3Node;
        av::Link< av::gua::Material> m_Material;

        TV_3Node(const TV_3Node&);
        TV_3Node& operator=(const TV_3Node&);
      };

      using SFTV_3Node = SingleField<Link<TV_3Node> >;
      using MFTV_3Node = MultiField<Link<TV_3Node> >;
    }
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::TV_3Node> >;
  template class AV_GUA_DLL MultiField<Link<gua::TV_3Node> >;
#endif

}

#endif //AVANGO_GUA_TV_3_NODE_HPP
