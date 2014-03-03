#ifndef AVANGO_GUA_LOD_NODE_HPP
#define AVANGO_GUA_LOD_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/scenegraph/LODNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/TransformNode.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::LODNode
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL LODNode : public av::gua::TransformNode
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::LODNode is created.
       * Otherwise, the given ::gua::LODNode is used.
       */
      LODNode(std::shared_ptr< ::gua::LODNode> guanode = std::shared_ptr< ::gua::LODNode>(new ::gua::LODNode("")));

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~LODNode();

    public:

      MFFloat LODDistances;

      /**
       * Get the wrapped ::gua::LODNode.
       */
      std::shared_ptr< ::gua::LODNode> getGuaNode() const;

    public:

      virtual void getLODDistancesCB(const MFFloat::GetValueEvent& event);
      virtual void setLODDistancesCB(const MFFloat::SetValueEvent& event);


    private:

      std::shared_ptr< ::gua::LODNode> m_guaNode;

      LODNode(const LODNode&);
      LODNode& operator=(const LODNode&);
    };

    typedef SingleField<Link<LODNode> > SFLODNode;
    typedef MultiField<Link<LODNode> > MFLODNode;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::LODNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::LODNode> >;
#endif

}

#endif //AVANGO_GUA_LOD_NODE_HPP
