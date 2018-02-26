#ifndef AVANGO_GUA_NURBS_NODE_HPP
#define AVANGO_GUA_NURBS_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <avango/gua/nurbs/Init.hpp>
#include <gua/node/NURBSNode.hpp>

#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/renderer/Material.hpp>

namespace av
{
  namespace gua
  {
    namespace nurbs
    {
      /**
       * Wrapper for ::gua::NURBSNode
       *
       * \ingroup av_gua
       */
      class AV_NURBS_DLL NURBSNode : public av::gua::GeometryNode
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

        virtual void on_distribute(av::gua::NetTransform& netNode);
        virtual void on_undistribute(av::gua::NetTransform& netNode);

      protected:

        /**
         * Destructor made protected to prevent allocation on stack.
         */
  //      virtual ~NURBSNode();

      public:

        SFString   Geometry;
        SFMaterial Material;
        SFFloat    PixelErrorTolerance;

        virtual void getGeometryCB(const SFString::GetValueEvent& event);
        virtual void setGeometryCB(const SFString::SetValueEvent& event);

        virtual void getMaterialCB(const SFMaterial::GetValueEvent& event);
        virtual void setMaterialCB(const SFMaterial::SetValueEvent& event);

        virtual void getPixelErrorToleranceCB(const SFFloat::GetValueEvent& event);
        virtual void setPixelErrorToleranceCB(const SFFloat::SetValueEvent& event);

        /**
         * Get the wrapped ::gua::NURBSNode.
         */
        std::shared_ptr< ::gua::node::NURBSNode> getGuaNURBSNode() const;

      private:

        std::shared_ptr< ::gua::node::NURBSNode> m_guaNURBSNode;
        av::Link< av::gua::Material> m_Material;

        NURBSNode(const NURBSNode&);
        NURBSNode& operator=(const NURBSNode&);
      };

      using SFNURBSNode = SingleField<Link<NURBSNode> >;
      using MFNURBSNode = MultiField<Link<NURBSNode> >;
    }
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_NURBS_DLL SingleField<Link<gua::nurbs::NURBSNode> >;
  template class AV_NURBS_DLL MultiField<Link<gua::nurbs::NURBSNode> >;
#endif

}

#endif //AVANGO_GUA_NURBS_NODE_HPP
