#ifndef AVANGO_GUA_COLLISION_SHAPE_NODE_HPP
#define AVANGO_GUA_COLLISION_SHAPE_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/physics/CollisionShapeNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/Node.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::physics::CollisionShapeNode
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL CollisionShapeNode : public av::gua::Node
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::physics::CollisionShapeNode is created.
       * Otherwise, the given ::gua::physics::CollisionShapeNode is used.
       */
      CollisionShapeNode(std::shared_ptr< ::gua::physics::CollisionShapeNode> guanode = std::shared_ptr< ::gua::physics::CollisionShapeNode>(new ::gua::physics::CollisionShapeNode("")));

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~CollisionShapeNode();

    public:

      SFString ShapeName;


      /**
       * Get the wrapped ::gua::physics::CollisionShapeNode.
       */
      std::shared_ptr< ::gua::physics::CollisionShapeNode>  getGuaNode() const;

    public:

      virtual void getShapeNameCB(const SFString::GetValueEvent& event);
      virtual void setShapeNameCB(const SFString::SetValueEvent& event);

    private:

      std::shared_ptr< ::gua::physics::CollisionShapeNode> m_guaNode;

      CollisionShapeNode(const CollisionShapeNode&);
      CollisionShapeNode& operator=(const CollisionShapeNode&);
    };

    typedef SingleField<Link<CollisionShapeNode> > SFCollisionShapeNode;
    typedef MultiField<Link<CollisionShapeNode> > MFCollisionShapeNode;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::CollisionShapeNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::CollisionShapeNode> >;
#endif

}

#endif //AVANGO_GUA_COLLISION_SHAPE_NODE_HPP
