#ifndef AVANGO_GUA_TEXTURED_QUAD_NODE_HPP
#define AVANGO_GUA_TEXTURED_QUAD_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/TexturedQuadNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/Node.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::TexturedQuadNode
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL TexturedQuadNode : public av::gua::Node
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::TexturedQuadNode is created.
       * Otherwise, the given ::gua::TexturedQuadNode is used.
       */
      TexturedQuadNode(std::shared_ptr< ::gua::node::TexturedQuadNode> guanode = std::shared_ptr< ::gua::node::TexturedQuadNode>(new ::gua::node::TexturedQuadNode("")));

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~TexturedQuadNode();

    public:


      SFString Texture;
      SFFloat  Width;
      SFFloat  Height;

      SFBool   FlipX;
      SFBool   FlipY;

      SFMatrix ScaledTransform;
      SFMatrix ScaledWorldTransform;

      /**
       * Get the wrapped ::gua::TexturedQuadNode.
       */
      std::shared_ptr< ::gua::node::TexturedQuadNode> getGuaNode() const;

    public:

      virtual void getTextureCB(const SFString::GetValueEvent& event);
      virtual void setTextureCB(const SFString::SetValueEvent& event);

      virtual void getWidthCB(const SFFloat::GetValueEvent& event);
      virtual void setWidthCB(const SFFloat::SetValueEvent& event);

      virtual void getHeightCB(const SFFloat::GetValueEvent& event);
      virtual void setHeightCB(const SFFloat::SetValueEvent& event);

      virtual void getFlipXCB(const SFBool::GetValueEvent& event);
      virtual void setFlipXCB(const SFBool::SetValueEvent& event);

      virtual void getFlipYCB(const SFBool::GetValueEvent& event);
      virtual void setFlipYCB(const SFBool::SetValueEvent& event);

      virtual void getScaledTransformCB(const SFMatrix::GetValueEvent& event);
      virtual void setScaledTransformCB(const SFMatrix::SetValueEvent& event);

      virtual void getScaledWorldTransformCB(const SFMatrix::GetValueEvent& event);
      virtual void setScaledWorldTransformCB(const SFMatrix::SetValueEvent& event);


    private:

      std::shared_ptr< ::gua::node::TexturedQuadNode> m_guaNode;

      TexturedQuadNode(const TexturedQuadNode&);
      TexturedQuadNode& operator=(const TexturedQuadNode&);
    };

    typedef SingleField<Link<TexturedQuadNode> > SFTexturedQuadNode;
    typedef MultiField<Link<TexturedQuadNode> > MFTexturedQuadNode;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::TexturedQuadNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::TexturedQuadNode> >;
#endif

}

#endif //AVANGO_GUA_TEXTURED_QUAD_NODE_HPP
