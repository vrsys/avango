#ifndef AVANGO_GUA_TEXTURED_QUAD_NODE_HPP
#define AVANGO_GUA_TEXTURED_QUAD_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/scenegraph/TexturedQuadNode.hpp>
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
      TexturedQuadNode(std::shared_ptr< ::gua::TexturedQuadNode> guanode =
                       std::make_shared< ::gua::TexturedQuadNode> (""));

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~TexturedQuadNode();

    public:


      SFString Texture;
      SFFloat  Width;
      SFFloat  Height;
      SFBool   IsStereoTexture;

      /**
       * Get the wrapped ::gua::TexturedQuadNode.
       */
      std::shared_ptr< ::gua::TexturedQuadNode> getGuaNode() const;

    public:

      virtual void getTextureCB(const SFString::GetValueEvent& event);
      virtual void setTextureCB(const SFString::SetValueEvent& event);

      virtual void getWidthCB(const SFFloat::GetValueEvent& event);
      virtual void setWidthCB(const SFFloat::SetValueEvent& event);

      virtual void getHeightCB(const SFFloat::GetValueEvent& event);
      virtual void setHeightCB(const SFFloat::SetValueEvent& event);

      virtual void getIsStereoTextureCB(const SFBool::GetValueEvent& event);
      virtual void setIsStereoTextureCB(const SFBool::SetValueEvent& event);

    private:

      std::shared_ptr< ::gua::TexturedQuadNode> m_guaNode;

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
