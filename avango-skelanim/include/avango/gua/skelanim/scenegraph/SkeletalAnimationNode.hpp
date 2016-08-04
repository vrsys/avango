#ifndef AVANGO_GUA_SKELETAL_ANIMATION_NODE_HPP
#define AVANGO_GUA_SKELETAL_ANIMATION_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/SkeletalAnimationNode.hpp>
 // necessary to prevent incomplete type error
#include <gua/utils/SkeletalAnimation.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/renderer/Material.hpp>

namespace av
{
  namespace gua
  {

    namespace skelanim {
      /**
       * Wrapper for ::gua::SkeletalAnimationNode
       *
       * \ingroup av_gua
       */
      class AV_GUA_ANIM_DLL SkeletalAnimationNode : public av::gua::GeometryNode
      {
        AV_FC_DECLARE();

      public:

        /**
         * Constructor. When called without arguments, a new ::gua::SkeletalAnimationNode is created.
         * Otherwise, the given ::gua::SkeletalAnimationNode is used.
         */
        SkeletalAnimationNode(std::shared_ptr< ::gua::node::SkeletalAnimationNode> guanode =
            std::shared_ptr< ::gua::node::SkeletalAnimationNode>(new ::gua::node::SkeletalAnimationNode("")));

        #if defined(AVANGO_DISTRIBUTION_SUPPORT)
        virtual void on_distribute(av::gua::NetTransform& netNode);
        virtual void on_undistribute(av::gua::NetTransform& netNode);
      #endif
      protected:

        /**
         * Destructor made protected to prevent allocation on stack.
         */
  //      virtual ~SkeletalAnimationNode();

      public:
        // trimesh
        MFString   Geometries;
        MultiField<Link<Material>> Materials;
        SFBool     RenderToGBuffer;
        SFBool     RenderToStencilBuffer;

        virtual void getGeometriesCB(const MFString::GetValueEvent& event);
        virtual void setGeometriesCB(const MFString::SetValueEvent& event);

        virtual void getMaterialsCB(const MultiField<Link<Material>>::GetValueEvent& event);
        virtual void setMaterialsCB(const MultiField<Link<Material>>::SetValueEvent& event);

        virtual void getRenderToGBufferCB(const SFBool::GetValueEvent& event);
        virtual void setRenderToGBufferCB(const SFBool::SetValueEvent& event);

        virtual void getRenderToStencilBufferCB(const SFBool::GetValueEvent& event);
        virtual void setRenderToStencilBufferCB(const SFBool::SetValueEvent& event);
       
        //skelanim 
        SFString Animation1;
        virtual void getAnimation1CB(const SFString::GetValueEvent& event);
        virtual void setAnimation1CB(const SFString::SetValueEvent& event);

        SFString Animation2;
        virtual void getAnimation2CB(const SFString::GetValueEvent& event);
        virtual void setAnimation2CB(const SFString::SetValueEvent& event);

        SFFloat BlendFactor;
        virtual void getBlendFactorCB(const SFFloat::GetValueEvent& event);
        virtual void setBlendFactorCB(const SFFloat::SetValueEvent& event);

        SFFloat Time1;
        virtual void getTime1CB(const SFFloat::GetValueEvent& event);
        virtual void setTime1CB(const SFFloat::SetValueEvent& event);

        SFFloat Time2;
        virtual void getTime2CB(const SFFloat::GetValueEvent& event);
        virtual void setTime2CB(const SFFloat::SetValueEvent& event);

        float getAnimDuration(std::string const& name) const;

        void loadAnimation(std::string const& file_name,
                                    std::string const& animation_name) const;

        /*SFInt LoopNr;

        virtual void getLoopNrCB(const SFInt::GetValueEvent& event);
        virtual void setLoopNrCB(const SFInt::SetValueEvent& event);*/

        /**
         * Get the wrapped ::gua::SkeletalAnimationNode.
         */
        std::shared_ptr< ::gua::node::SkeletalAnimationNode> getGuaSkeletalAnimationNode() const;

      private:

        std::shared_ptr< ::gua::node::SkeletalAnimationNode> m_guaSkeletalAnimationNode;
          unsigned m_materialsUserDataHandle;

        SkeletalAnimationNode(const SkeletalAnimationNode&);
        SkeletalAnimationNode& operator=(const SkeletalAnimationNode&);
      };

      typedef SingleField<Link<SkeletalAnimationNode> > SFSkeletalAnimationNode;
      typedef MultiField<Link<SkeletalAnimationNode> > MFSkeletalAnimationNode;
    }

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_ANIM_DLL SingleField<Link<gua::skelanim::SkeletalAnimationNode> >;
  template class AV_GUA_ANIM_DLL MultiField<Link<gua::skelanim::SkeletalAnimationNode> >;
#endif

}

#endif //AVANGO_GUA_SKELETAL_ANIMATION_NODE_HPP
