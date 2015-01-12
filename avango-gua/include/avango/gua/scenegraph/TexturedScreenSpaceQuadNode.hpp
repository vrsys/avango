#ifndef AVANGO_GUA_TEXTURED_SCREEN_SPACE_QUAD_NODE_HPP
#define AVANGO_GUA_TEXTURED_SCREEN_SPACE_QUAD_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/TexturedScreenSpaceQuadNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/Node.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::TexturedScreenSpaceQuadNode
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL TexturedScreenSpaceQuadNode : public av::gua::Node
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::TexturedScreenSpaceQuadNode is created.
       * Otherwise, the given ::gua::TexturedScreenSpaceQuadNode is used.
       */
      TexturedScreenSpaceQuadNode(std::shared_ptr< ::gua::node::TexturedScreenSpaceQuadNode> guanode = std::shared_ptr< ::gua::node::TexturedScreenSpaceQuadNode>(new ::gua::node::TexturedScreenSpaceQuadNode("")));

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~TexturedScreenSpaceQuadNode();

    public:


      SFString Texture;

      SFInt   Width;
      SFInt   Height;

      SFVec2   Anchor;
      SFVec2   Offset;

      SFFloat  Opacity;

      SFBool   FlipX;
      SFBool   FlipY;

      /**
       * Get the wrapped ::gua::TexturedScreenSpaceQuadNode.
       */
      std::shared_ptr< ::gua::node::TexturedScreenSpaceQuadNode> getGuaNode() const;

    public:

      virtual void getTextureCB(const SFString::GetValueEvent& event);
      virtual void setTextureCB(const SFString::SetValueEvent& event);

      virtual void getWidthCB(const SFInt::GetValueEvent& event);
      virtual void setWidthCB(const SFInt::SetValueEvent& event);

      virtual void getHeightCB(const SFInt::GetValueEvent& event);
      virtual void setHeightCB(const SFInt::SetValueEvent& event);

      virtual void getAnchorCB(const SFVec2::GetValueEvent& event);
      virtual void setAnchorCB(const SFVec2::SetValueEvent& event);

      virtual void getOffsetCB(const SFVec2::GetValueEvent& event);
      virtual void setOffsetCB(const SFVec2::SetValueEvent& event);

      virtual void getOpacityCB(const SFFloat::GetValueEvent& event);
      virtual void setOpacityCB(const SFFloat::SetValueEvent& event);

      virtual void getFlipXCB(const SFBool::GetValueEvent& event);
      virtual void setFlipXCB(const SFBool::SetValueEvent& event);

      virtual void getFlipYCB(const SFBool::GetValueEvent& event);
      virtual void setFlipYCB(const SFBool::SetValueEvent& event);

      bool pixel_to_texcoords(::gua::math::vec2 const& pixel,
                              ::gua::math::vec2ui const& screen_size,
                              ::gua::math::vec2& result) const;


    private:

      std::shared_ptr< ::gua::node::TexturedScreenSpaceQuadNode> m_guaNode;

      TexturedScreenSpaceQuadNode(const TexturedScreenSpaceQuadNode&);
      TexturedScreenSpaceQuadNode& operator=(const TexturedScreenSpaceQuadNode&);
    };

    typedef SingleField<Link<TexturedScreenSpaceQuadNode> > SFTexturedScreenSpaceQuadNode;
    typedef MultiField<Link<TexturedScreenSpaceQuadNode> > MFTexturedScreenSpaceQuadNode;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::TexturedScreenSpaceQuadNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::TexturedScreenSpaceQuadNode> >;
#endif

}

#endif //AVANGO_GUA_TEXTURED_SCREEN_SPACE_QUAD_NODE_HPP
