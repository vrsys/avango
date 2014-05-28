#ifndef AVANGO_GUA_PBR_NODE_HPP
#define AVANGO_GUA_PBR_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/scenegraph/PBRNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/Node.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::PBRNode
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL PBRNode : public av::gua::Node
    {
      AV_FC_DECLARE();

    public:

      enum ShadowModeEnum {
        OFF = static_cast<unsigned>(::gua::ShadowMode::OFF),
        LOW_QUALITY = static_cast<unsigned>(::gua::ShadowMode::LOW_QUALITY),
        HIGH_QUALITY = static_cast<unsigned>(::gua::ShadowMode::HIGH_QUALITY)
      };

      /**
       * Constructor. When called without arguments, a new ::gua::PBRNode 
       * is created.
       * Otherwise, the given ::gua::PBRNode is used.
       */
      PBRNode(std::shared_ptr< ::gua::PBRNode> guanode = 
          std::shared_ptr< ::gua::PBRNode>(new ::gua::PBRNode("")));

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~PBRNode();

    public:


      SFString Geometry;
      SFString Material;
      SFUInt   ShadowMode;


      /**
       * Get the wrapped ::gua::PBRNode.
       */
      std::shared_ptr< ::gua::PBRNode> getGuaNode() const;

    public:

      virtual void getGeometryCB(const SFString::GetValueEvent& event);
      virtual void setGeometryCB(const SFString::SetValueEvent& event);

      virtual void getMaterialCB(const SFString::GetValueEvent& event);
      virtual void setMaterialCB(const SFString::SetValueEvent& event);

      virtual void getShadowModeCB(const SFUInt::GetValueEvent& event);
      virtual void setShadowModeCB(const SFUInt::SetValueEvent& event);

    private:

      std::shared_ptr< ::gua::PBRNode> m_guaNode;

      PBRNode(const PBRNode&);
      PBRNode& operator=(const PBRNode&);
    };

    typedef SingleField<Link<PBRNode> > SFPBRNode;
    typedef MultiField<Link<PBRNode> > MFPBRNode;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::PBRNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::PBRNode> >;
#endif

}

#endif //AVANGO_GUA_PBR_NODE_HPP
