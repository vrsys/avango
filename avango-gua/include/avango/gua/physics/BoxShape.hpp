#ifndef AVANGO_GUA_BOX_SHAPE_HPP
#define AVANGO_GUA_BOX_SHAPE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/physics/BoxShape.hpp>

#include <avango/gua/physics/CollisionShape.hpp>
#include <avango/gua/Fields.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::physics::BoxShape
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL BoxShape : public av::gua::CollisionShape
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::physics::BoxShape is created.
       * Otherwise, the given ::gua::physics::BoxShape is used.
       */
      BoxShape(::gua::physics::BoxShape* guashape =
               new ::gua::physics::BoxShape(0.5f)); // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~BoxShape();

    public:

      SFVec3 HalfExtents;

      /**
       * Get the wrapped ::gua::physics::CollisionShape.
       */
      ::gua::physics::BoxShape* getGuaShape() const;

    public:

      virtual void getHalfExtentsCB(const SFVec3::GetValueEvent& event);
      virtual void setHalfExtentsCB(const SFVec3::SetValueEvent& event);

    private:
      ::gua::physics::BoxShape *m_guaShape;

      BoxShape(const BoxShape&);
      BoxShape& operator=(const BoxShape&);
    };

    typedef SingleField<Link<BoxShape> > SFBoxShape;
    typedef MultiField<Link<BoxShape> > MFBoxShape;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::BoxShape> >;
  template class AV_GUA_DLL MultiField<Link<gua::BoxShape> >;
#endif

}

#endif //AVANGO_GUA_BOX_SHAPE_HPP
