#ifndef AVANGO_GUA_SPHERE_SHAPE_HPP
#define AVANGO_GUA_SPHERE_SHAPE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/physics/SphereShape.hpp>

#include <avango/gua/physics/CollisionShape.hpp>
#include <avango/gua/Fields.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::physics::SphereShape
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL SphereShape : public av::gua::CollisionShape
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::physics::SphereShape is created.
       * Otherwise, the given ::gua::physics::SphereShape is used.
       */
      SphereShape(::gua::physics::SphereShape* guashape =
                  new ::gua::physics::SphereShape(1.f)); // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~SphereShape();

    public:

      SFFloat Radius;

      /**
       * Get the wrapped ::gua::physics::CollisionShape.
       */
      ::gua::physics::SphereShape* getGuaShape() const;

    public:

      virtual void getRadiusCB(const SFFloat::GetValueEvent& event);
      virtual void setRadiusCB(const SFFloat::SetValueEvent& event);

    private:
      ::gua::physics::SphereShape *m_guaShape;

      SphereShape(const SphereShape&);
      SphereShape& operator=(const SphereShape&);
    };

    using SFSphereShape = SingleField<Link<SphereShape> >;
    using MFSphereShape = MultiField<Link<SphereShape> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::SphereShape> >;
  template class AV_GUA_DLL MultiField<Link<gua::SphereShape> >;
#endif

}

#endif //AVANGO_GUA_SPHERE_SHAPE_HPP
