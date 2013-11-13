#ifndef AVANGO_GUA_GEOMETRY_NODE_HPP
#define AVANGO_GUA_GEOMETRY_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/scenegraph/GeometryNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/Node.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::GeometryNode
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL GeometryNode : public av::gua::Node
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::GeometryNode is created.
       * Otherwise, the given ::gua::GeometryNode is used.
       */
      GeometryNode(std::shared_ptr<::gua::GeometryNode> guanode = std::shared_ptr<::gua::GeometryNode>(new ::gua::GeometryNode("")));

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~GeometryNode();

    public:


      SFString Geometry;
      SFString Material;


      /**
       * Get the wrapped ::gua::GeometryNode.
       */
      std::shared_ptr< ::gua::GeometryNode> getGuaNode() const;

    public:

      virtual void getGeometryCB(const SFString::GetValueEvent& event);
      virtual void setGeometryCB(const SFString::SetValueEvent& event);

      virtual void getMaterialCB(const SFString::GetValueEvent& event);
      virtual void setMaterialCB(const SFString::SetValueEvent& event);

    private:

      std::shared_ptr< ::gua::GeometryNode> m_guaNode;

      GeometryNode(const GeometryNode&);
      GeometryNode& operator=(const GeometryNode&);
    };

    typedef SingleField<Link<GeometryNode> > SFGeometryNode;
    typedef MultiField<Link<GeometryNode> > MFGeometryNode;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::GeometryNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::GeometryNode> >;
#endif

}

#endif //AVANGO_GUA_GEOMETRY_NODE_HPP
