#ifndef AVANGO_GUA_PLANE_SHAPE_HPP
#define AVANGO_GUA_PLANE_SHAPE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/physics/PlaneShape.hpp>

#include <avango/gua/physics/CollisionShape.hpp>
#include <avango/gua/Fields.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::physics::PlaneShape
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL PlaneShape : public av::gua::CollisionShape
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::physics::PlaneShape is created.
       * Otherwise, the given ::gua::physics::PlaneShape is used.
       */
      PlaneShape(::gua::physics::PlaneShape* guashape =
                 new ::gua::physics::PlaneShape(0.f, 0.f, -1.f, 0.f)); // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~PlaneShape();

    public:

      SFVec3  Normal;
      SFFloat PlaneConstant;

      /**
       * Get the wrapped ::gua::physics::CollisionShape.
       */
      ::gua::physics::PlaneShape* getGuaShape() const;

    public:

      virtual void getNormalCB(const SFVec3::GetValueEvent& event);
      virtual void setNormalCB(const SFVec3::SetValueEvent& event);

      virtual void getPlaneConstantCB(const SFFloat::GetValueEvent& event);
      virtual void setPlaneConstantCB(const SFFloat::SetValueEvent& event);

    private:
      ::gua::physics::PlaneShape *m_guaShape;

      PlaneShape(const PlaneShape&);
      PlaneShape& operator=(const PlaneShape&);
    };

    using SFPlaneShape = SingleField<Link<PlaneShape> >;
    using MFPlaneShape = MultiField<Link<PlaneShape> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::PlaneShape> >;
  template class AV_GUA_DLL MultiField<Link<gua::PlaneShape> >;
#endif

}

#endif //AVANGO_GUA_PLANE_SHAPE_HPP
