#ifndef AVANGO_GUA_NURBS_NODE_HPP
#define AVANGO_GUA_NURBS_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/scenegraph/NURBSNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/Node.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::NURBSNode
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL NURBSNode : public av::gua::Node
    {
      AV_FC_DECLARE();

    public:

      enum ShadowModeEnum {
        OFF = static_cast<unsigned>(::gua::ShadowMode::OFF),
        LOW_QUALITY = static_cast<unsigned>(::gua::ShadowMode::LOW_QUALITY),
        HIGH_QUALITY = static_cast<unsigned>(::gua::ShadowMode::HIGH_QUALITY)
      };

      /**
       * Constructor. When called without arguments, a new ::gua::NURBSNode
       * is created.
       * Otherwise, the given ::gua::NURBSNode is used.
       */
      NURBSNode(std::shared_ptr< ::gua::NURBSNode> guanode =
          std::shared_ptr< ::gua::NURBSNode>(new ::gua::NURBSNode("")));

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~NURBSNode();

    public:


      SFString Geometry;
      SFString Material;
      SFUInt   ShadowMode;


      /**
       * Get the wrapped ::gua::NURBSNode.
       */
      std::shared_ptr< ::gua::NURBSNode> getGuaNode() const;

    public:

      virtual void getGeometryCB(const SFString::GetValueEvent& event);
      virtual void setGeometryCB(const SFString::SetValueEvent& event);

      virtual void getMaterialCB(const SFString::GetValueEvent& event);
      virtual void setMaterialCB(const SFString::SetValueEvent& event);

      virtual void getShadowModeCB(const SFUInt::GetValueEvent& event);
      virtual void setShadowModeCB(const SFUInt::SetValueEvent& event);

    private:

      std::shared_ptr< ::gua::NURBSNode> m_guaNode;

      NURBSNode(const NURBSNode&);
      NURBSNode& operator=(const NURBSNode&);
    };

    typedef SingleField<Link<NURBSNode> > SFNURBSNode;
    typedef MultiField<Link<NURBSNode> > MFNURBSNode;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::NURBSNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::NURBSNode> >;
#endif

}

#endif //AVANGO_GUA_NURBS_NODE_HPP
