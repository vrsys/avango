#ifndef AVANGO_GUA_PLOD_NODE_HPP
#define AVANGO_GUA_PLOD_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/scenegraph/PLODNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/Node.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::PLODNode
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL PLODNode : public av::gua::Node
    {
      AV_FC_DECLARE();

    public:

      enum ShadowModeEnum {
        OFF = static_cast<unsigned>(::gua::ShadowMode::OFF),
        LOW_QUALITY = static_cast<unsigned>(::gua::ShadowMode::LOW_QUALITY),
        HIGH_QUALITY = static_cast<unsigned>(::gua::ShadowMode::HIGH_QUALITY)
      };

      /**
       * Constructor. When called without arguments, a new ::gua::PLODNode 
       * is created.
       * Otherwise, the given ::gua::PLODNode is used.
       */
      PLODNode(std::shared_ptr< ::gua::PLODNode> guanode = 
          std::shared_ptr< ::gua::PLODNode>(new ::gua::PLODNode("")));

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~PLODNode();

    public:


      SFString Geometry;
      SFString Material;
      SFUInt   ShadowMode;


      /**
       * Get the wrapped ::gua::PLODNode.
       */
      std::shared_ptr< ::gua::PLODNode> getGuaNode() const;

    public:

      virtual void getGeometryCB(const SFString::GetValueEvent& event);
      virtual void setGeometryCB(const SFString::SetValueEvent& event);

      virtual void getMaterialCB(const SFString::GetValueEvent& event);
      virtual void setMaterialCB(const SFString::SetValueEvent& event);

      virtual void getShadowModeCB(const SFUInt::GetValueEvent& event);
      virtual void setShadowModeCB(const SFUInt::SetValueEvent& event);

    private:

      std::shared_ptr< ::gua::PLODNode> m_guaNode;

      PLODNode(const PLODNode&);
      PLODNode& operator=(const PLODNode&);
    };

    typedef SingleField<Link<PLODNode> > SFPLODNode;
    typedef MultiField<Link<PLODNode> > MFPLODNode;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::PLODNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::PLODNode> >;
#endif

}

#endif //AVANGO_GUA_PLOD_NODE_HPP
