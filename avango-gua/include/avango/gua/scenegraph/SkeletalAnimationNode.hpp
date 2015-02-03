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

      SFInt AnimationMode;

      virtual void getAnimationModeCB(const SFInt::GetValueEvent& event);
      virtual void setAnimationModeCB(const SFInt::SetValueEvent& event);

      SFInt BlendingMode;

      virtual void getBlendingModeCB(const SFInt::GetValueEvent& event);
      virtual void setBlendingModeCB(const SFInt::SetValueEvent& event);

      SFString Animation;

      virtual void getAnimationCB(const SFString::GetValueEvent& event);
      virtual void setAnimationCB(const SFString::SetValueEvent& event);

      SFFloat BlendingFactor;

      virtual void getBlendingFactorCB(const SFFloat::GetValueEvent& event);
      virtual void setBlendingFactorCB(const SFFloat::SetValueEvent& event);

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

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::SkeletalAnimationNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::SkeletalAnimationNode> >;
#endif

}

#endif //AVANGO_GUA_SKELETAL_ANIMATION_NODE_HPP
