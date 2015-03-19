#ifndef AVANGO_GUA_SKELETAL_ANIMATION_NODE_HPP
#define AVANGO_GUA_SKELETAL_ANIMATION_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/SkeletalAnimationNode.hpp>
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
      class AV_GUA_DLL SkeletalAnimationNode : public av::gua::GeometryNode
      {
        AV_FC_DECLARE();

      public:

        /**
         * Constructor. When called without arguments, a new ::gua::SkeletalAnimationNode is created.
         * Otherwise, the given ::gua::SkeletalAnimationNode is used.
         */
        SkeletalAnimationNode(std::shared_ptr< ::gua::node::SkeletalAnimationNode> guanode =
            std::shared_ptr< ::gua::node::SkeletalAnimationNode>(new ::gua::node::SkeletalAnimationNode("")));

      protected:

        /**
         * Destructor made protected to prevent allocation on stack.
         */
  //      virtual ~SkeletalAnimationNode();

      public:

        SFMaterial Material;
        virtual void getMaterialCB(const SFMaterial::GetValueEvent& event);
        virtual void setMaterialCB(const SFMaterial::SetValueEvent& event);

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
        /**
         * Get the wrapped ::gua::SkeletalAnimationNode.
         */
        std::shared_ptr< ::gua::node::SkeletalAnimationNode> getGuaSkeletalAnimationNode() const;

      private:

        std::shared_ptr< ::gua::node::SkeletalAnimationNode> m_guaSkeletalAnimationNode;
        av::Link< av::gua::Material> m_Material;

        SkeletalAnimationNode(const SkeletalAnimationNode&);
        SkeletalAnimationNode& operator=(const SkeletalAnimationNode&);
      };

      typedef SingleField<Link<SkeletalAnimationNode> > SFSkeletalAnimationNode;
      typedef MultiField<Link<SkeletalAnimationNode> > MFSkeletalAnimationNode;
    }

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::SkeletalAnimationNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::SkeletalAnimationNode> >;
#endif

}

#endif //AVANGO_GUA_SKELETAL_ANIMATION_NODE_HPP
