#ifndef AVANGO_GUA_COLLISION_SHAPE_HPP
#define AVANGO_GUA_COLLISION_SHAPE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/physics/CollisionShape.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::physics::CollisionShape
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL CollisionShape : public av::FieldContainer
    {
      AV_FC_DECLARE_ABSTRACT();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::physics::CollisionShape is created.
       * Otherwise, the given ::gua::physics::CollisionShape is used.
       */
      CollisionShape(::gua::physics::CollisionShape* guashape); // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~CollisionShape();

    public:

      SFBool IsDynamicShape;
      SFBool IsStaticShape;
      SFBool HasIdenticalShapeConstructor;


      /**
       * Get the wrapped ::gua::physics::CollisionShape.
       */
      ::gua::physics::CollisionShape* getGuaShape() const;

    public:

      virtual void getIsDynamicShapeCB(const SFBool::GetValueEvent& event);
      virtual void setIsDynamicShapeCB(const SFBool::SetValueEvent& event);

      virtual void getIsStaticShapeCB(const SFBool::GetValueEvent& event);
      virtual void setIsStaticShapeCB(const SFBool::SetValueEvent& event);

      virtual void getHasIdenticalShapeConstructorCB(const SFBool::GetValueEvent& event);
      virtual void setHasIdenticalShapeConstructorCB(const SFBool::SetValueEvent& event);

    private:

      ::gua::physics::CollisionShape *m_guaShape;

      CollisionShape(const CollisionShape&);
      CollisionShape& operator=(const CollisionShape&);
    };

    using SFCollisionShape = SingleField<Link<CollisionShape> >;
    using MFCollisionShape = MultiField<Link<CollisionShape> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::CollisionShape> >;
  template class AV_GUA_DLL MultiField<Link<gua::CollisionShape> >;
#endif

}

#endif //AVANGO_GUA_COLLISION_SHAPE_HPP
