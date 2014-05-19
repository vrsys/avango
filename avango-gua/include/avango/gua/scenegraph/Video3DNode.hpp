#ifndef AVANGO_GUA_VIDEO3D_NODE_HPP
#define AVANGO_GUA_VIDEO3D_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/scenegraph/Video3DNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/Node.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::Video3DNode
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL Video3DNode : public av::gua::Node
    {
      AV_FC_DECLARE();

    public:

      enum ShadowModeEnum {
        OFF = static_cast<unsigned>(::gua::ShadowMode::OFF),
        LOW_QUALITY = static_cast<unsigned>(::gua::ShadowMode::LOW_QUALITY),
        HIGH_QUALITY = static_cast<unsigned>(::gua::ShadowMode::HIGH_QUALITY)
      };

      /**
       * Constructor. When called without arguments, a new ::gua::Video3DNode 
       * is created.
       * Otherwise, the given ::gua::Video3DNode is used.
       */
      Video3DNode(std::shared_ptr< ::gua::Video3DNode> guanode = 
          std::shared_ptr< ::gua::Video3DNode>(new ::gua::Video3DNode("")));

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~Video3DNode();

    public:


      SFString Geometry;
      SFString Material;
      SFUInt   ShadowMode;


      /**
       * Get the wrapped ::gua::Video3DNode.
       */
      std::shared_ptr< ::gua::Video3DNode> getGuaNode() const;

    public:

      virtual void getGeometryCB(const SFString::GetValueEvent& event);
      virtual void setGeometryCB(const SFString::SetValueEvent& event);

      virtual void getMaterialCB(const SFString::GetValueEvent& event);
      virtual void setMaterialCB(const SFString::SetValueEvent& event);

      virtual void getShadowModeCB(const SFUInt::GetValueEvent& event);
      virtual void setShadowModeCB(const SFUInt::SetValueEvent& event);

    private:

      std::shared_ptr< ::gua::Video3DNode> m_guaNode;

      Video3DNode(const Video3DNode&);
      Video3DNode& operator=(const Video3DNode&);
    };

    typedef SingleField<Link<Video3DNode> > SFVideo3DNode;
    typedef MultiField<Link<Video3DNode> > MFVideo3DNode;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::Video3DNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::Video3DNode> >;
#endif

}

#endif //AVANGO_GUA_VIDEO3D_NODE_HPP
