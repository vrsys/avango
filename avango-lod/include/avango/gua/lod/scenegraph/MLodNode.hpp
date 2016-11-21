#ifndef AVANGO_GUA_LOD_MLOD_NODE_HPP
#define AVANGO_GUA_LOD_MLOD_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/MLodNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/renderer/Material.hpp>

namespace av
{
  namespace gua
  {

    namespace lod {
      /**
       * Wrapper for ::gua::MLodNode
       *
       * \ingroup av_gua
       */
      class AV_GUA_LOD_DLL MLodNode : public av::gua::GeometryNode
      {
        AV_FC_DECLARE();

      public:

        /**
         * Constructor. When called without arguments, a new ::gua::MLodNode is created.
         * Otherwise, the given ::gua::MLodNode is used.
         */
        MLodNode(std::shared_ptr< ::gua::node::MLodNode> guanode =
            std::shared_ptr< ::gua::node::MLodNode>(new ::gua::node::MLodNode("")));
            
        virtual void on_distribute(av::gua::NetTransform& netNode);
        virtual void on_undistribute(av::gua::NetTransform& netNode);

      protected:

        /**
         * Destructor made protected to prevent allocation on stack.
         */
  //      virtual ~MLodNode();

      public:
        /**
         * Get the wrapped ::gua::MLodNode.
         */
        std::shared_ptr< ::gua::node::MLodNode> getGuaMLodNode() const;


        SFString   Geometry;
        SFMaterial Material;
        SFFloat    ErrorThreshold;
        
        virtual void getGeometryCB(const SFString::GetValueEvent& event);
        virtual void setGeometryCB(const SFString::SetValueEvent& event);

        virtual void getMaterialCB(const SFMaterial::GetValueEvent& event);
        virtual void setMaterialCB(const SFMaterial::SetValueEvent& event);

        virtual void getErrorThresholdCB(const SFFloat::GetValueEvent& event);
        virtual void setErrorThresholdCB(const SFFloat::SetValueEvent& event);


      private:

        std::shared_ptr< ::gua::node::MLodNode> m_guaMLodNode;
        av::Link< av::gua::Material> m_Material;

        MLodNode(const MLodNode&);
        MLodNode& operator=(const MLodNode&);
      };

      typedef SingleField<Link<MLodNode> > SFMLodNode;
      typedef MultiField<Link<MLodNode> > MFMLodNode;
    }

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_LOD_DLL SingleField<Link<gua::lod::MLodNode> >;
  template class AV_GUA_LOD_DLL MultiField<Link<gua::lod::MLodNode> >;
#endif

}

#endif //AVANGO_GUA_LOD_MLOD_NODE_HPP
