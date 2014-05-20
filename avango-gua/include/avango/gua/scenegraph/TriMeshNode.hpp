#ifndef AVANGO_GUA_TRI_MESH_NODE_HPP
#define AVANGO_GUA_TRI_MESH_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/scenegraph/TriMeshNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/Node.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::TriMeshNode
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL TriMeshNode : public av::gua::Node
    {
      AV_FC_DECLARE();

    public:

      enum ShadowModeEnum {
        OFF = static_cast<unsigned>(::gua::ShadowMode::OFF),
        LOW_QUALITY = static_cast<unsigned>(::gua::ShadowMode::LOW_QUALITY),
        HIGH_QUALITY = static_cast<unsigned>(::gua::ShadowMode::HIGH_QUALITY)
      };

      /**
       * Constructor. When called without arguments, a new ::gua::TriMeshNode is created.
       * Otherwise, the given ::gua::TriMeshNode is used.
       */
      TriMeshNode(std::shared_ptr< ::gua::TriMeshNode> guanode = std::shared_ptr< ::gua::TriMeshNode>(new ::gua::TriMeshNode("")));

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~TriMeshNode();

    public:


      SFString Geometry;
      SFString Material;
      SFUInt   ShadowMode;


      /**
       * Get the wrapped ::gua::TriMeshNode.
       */
      std::shared_ptr< ::gua::TriMeshNode> getGuaNode() const;

    public:

      virtual void getGeometryCB(const SFString::GetValueEvent& event);
      virtual void setGeometryCB(const SFString::SetValueEvent& event);

      virtual void getMaterialCB(const SFString::GetValueEvent& event);
      virtual void setMaterialCB(const SFString::SetValueEvent& event);

      virtual void getShadowModeCB(const SFUInt::GetValueEvent& event);
      virtual void setShadowModeCB(const SFUInt::SetValueEvent& event);

    private:

      std::shared_ptr< ::gua::TriMeshNode> m_guaNode;

      TriMeshNode(const TriMeshNode&);
      TriMeshNode& operator=(const TriMeshNode&);
    };

    typedef SingleField<Link<TriMeshNode> > SFTriMeshNode;
    typedef MultiField<Link<TriMeshNode> > MFTriMeshNode;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::TriMeshNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::TriMeshNode> >;
#endif

}

#endif //AVANGO_GUA_TRI_MESH_NODE_HPP
