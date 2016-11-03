#ifndef AVANGO_GUA_LOD_PLOD_NODE_HPP
#define AVANGO_GUA_LOD_PLOD_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/PLodNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/renderer/Material.hpp>

namespace av
{
  namespace gua
  {

    namespace lod {
      /**
       * Wrapper for ::gua::PLodNode
       *
       * \ingroup av_gua
       */
      class AV_GUA_LOD_DLL PLodNode : public av::gua::GeometryNode
      {
        AV_FC_DECLARE();

      public:

        /**
         * Constructor. When called without arguments, a new ::gua::PLodNode is created.
         * Otherwise, the given ::gua::PLodNode is used.
         */
        PLodNode(std::shared_ptr< ::gua::node::PLodNode> guanode =
            std::shared_ptr< ::gua::node::PLodNode>(new ::gua::node::PLodNode("")));

        virtual void on_distribute(av::gua::NetTransform& netNode);
        virtual void on_undistribute(av::gua::NetTransform& netNode);

      protected:

        /**
         * Destructor made protected to prevent allocation on stack.
         */
  //      virtual ~PLodNode();

      public:
        /**
         * Get the wrapped ::gua::PLodNode.
         */
        std::shared_ptr< ::gua::node::PLodNode> getGuaPLodNode() const;

        SFString Geometry;
        SFMaterial Material;
        SFFloat RadiusScale;
        SFFloat MaxSurfelRadius;
        SFFloat ErrorThreshold;
        SFBool EnableBackfaceCulling;

        virtual void setGeometryCB(const SFString::SetValueEvent& event);
        virtual void getGeometryCB(const SFString::GetValueEvent& event);

        virtual void setMaterialCB(const SFMaterial::SetValueEvent& event);
        virtual void getMaterialCB(const SFMaterial::GetValueEvent& event);

        virtual void setRadiusScaleCB(const SFFloat::SetValueEvent& event);
        virtual void getRadiusScaleCB(const SFFloat::GetValueEvent& event);

        virtual void setMaxSurfelRadiusCB(const SFFloat::SetValueEvent& event);
        virtual void getMaxSurfelRadiusCB(const SFFloat::GetValueEvent& event);

        virtual void setErrorThresholdCB(const SFFloat::SetValueEvent& event);
        virtual void getErrorThresholdCB(const SFFloat::GetValueEvent& event);

        virtual void setEnableBackfaceCullingCB(const SFBool::SetValueEvent& event);
        virtual void getEnableBackfaceCullingCB(const SFBool::GetValueEvent& event);

      private:

        std::shared_ptr< ::gua::node::PLodNode> m_guaPLodNode;
        av::Link< av::gua::Material> m_Material;

        PLodNode(const PLodNode&);
        PLodNode& operator=(const PLodNode&);
      };

      typedef SingleField<Link<PLodNode> > SFPLodNode;
      typedef MultiField<Link<PLodNode> > MFPLodNode;
    }

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_LOD_DLL SingleField<Link<gua::lod::PLodNode> >;
  template class AV_GUA_LOD_DLL MultiField<Link<gua::skelanim::PLodNode> >;
#endif

}

#endif //AVANGO_GUA_LOD_PLOD_NODE_HPP
