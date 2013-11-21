#ifndef AVANGO_GUA_CYLINDER_SHAPE_HPP
#define AVANGO_GUA_CYLINDER_SHAPE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/physics/CylinderShape.hpp>

#include <avango/gua/physics/CollisionShape.hpp>
#include <avango/gua/Fields.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::physics::CylinderShape
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL CylinderShape : public av::gua::CollisionShape
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::physics::CylinderShape is created.
       * Otherwise, the given ::gua::physics::CylinderShape is used.
       */
      CylinderShape(::gua::physics::CylinderShape* guashape =
               new ::gua::physics::CylinderShape(::gua::math::vec3(0.5f, 0.5f, 0.5f))); // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~CylinderShape();

    public:

      SFVec3 HalfExtents;

      /**
       * Get the wrapped ::gua::physics::CollisionShape.
       */
      ::gua::physics::CylinderShape* getGuaShape() const;

    public:

      virtual void getHalfExtentsCB(const SFVec3::GetValueEvent& event);
      virtual void setHalfExtentsCB(const SFVec3::SetValueEvent& event);

    private:
      ::gua::physics::CylinderShape *m_guaShape;

      CylinderShape(const CylinderShape&);
      CylinderShape& operator=(const CylinderShape&);
    };

    typedef SingleField<Link<CylinderShape> > SFCylinderShape;
    typedef MultiField<Link<CylinderShape> > MFCylinderShape;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::CylinderShape> >;
  template class AV_GUA_DLL MultiField<Link<gua::CylinderShape> >;
#endif

}

#endif //AVANGO_GUA_CYLINDER_SHAPE_HPP
