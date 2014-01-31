// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* Avango is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* Avango is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with Avango. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
* Avango is a trademark owned by FhG.                                    *
*                                                                        *
\************************************************************************/

#if !defined(AVANGO_OSG_GEOMETRY_H)
#define AVANGO_OSG_GEOMETRY_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/Geometry>
#include <avango/osg/Fields.h>
#include <avango/osg/Drawable.h>

namespace av
{
  namespace osg
  {
    typedef ::osg::Geometry OsgGeometry;

    /**
     * Wrapper for ::osg::Geometry
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL Geometry : public Drawable
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::osg::Geometry is created.
       * Otherwise, the given ::osg::Geometry is used.
       */
      Geometry(OsgGeometry* osggeometry = new OsgGeometry());
      // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~Geometry();

    public:

      MFVec3 VertexArray;
      MFVec3 NormalArray;
      MFVec4 ColorArray;
      MFVec2 TexCoordArray;

      enum
      {
        BIND_OFF               = ::osg::Geometry::BIND_OFF,               // 0
        BIND_OVERALL           = ::osg::Geometry::BIND_OVERALL,           // 1
        BIND_PER_PRIMITIVE_SET = ::osg::Geometry::BIND_PER_PRIMITIVE_SET, // 2
        BIND_PER_PRIMITIVE     = ::osg::Geometry::BIND_PER_PRIMITIVE,     // 3
        BIND_PER_VERTEX        = ::osg::Geometry::BIND_PER_VERTEX         // 4
      };

      SFInt  NormalBinding;
      SFInt  ColorBinding;

      enum
      {
        POINTS         = ::osg::PrimitiveSet::POINTS,         // 0
        LINES          = ::osg::PrimitiveSet::LINES,          // 1
        LINE_LOOP      = ::osg::PrimitiveSet::LINE_LOOP,      // 2
        LINE_STRIP     = ::osg::PrimitiveSet::LINE_STRIP,     // 3
        TRIANGLES      = ::osg::PrimitiveSet::TRIANGLES,      // 4
        TRIANGLE_STRIP = ::osg::PrimitiveSet::TRIANGLE_STRIP, // 5
        TRIANGLE_FAN   = ::osg::PrimitiveSet::TRIANGLE_FAN,   // 6
        QUADS          = ::osg::PrimitiveSet::QUADS,          // 7
        QUAD_STRIP     = ::osg::PrimitiveSet::QUAD_STRIP,     // 8
        POLYGON        = ::osg::PrimitiveSet::POLYGON,        // 9
        DISABLED       = ::osg::PrimitiveSet::POLYGON + 1     // 10
      };

      SFUInt Mode;

      MFUInt PrimitiveIndices;
      MFInt PrimitiveLengths;

      /**
       * Get the wrapped ::osg::Geometry.
       */
      ::osg::Geometry* getOsgGeometry() const;

      /* virtual */ void evaluateLocalSideEffect();

    protected:

      virtual void getVertexArrayCB(const MFVec3::GetValueEvent& event);
      virtual void setVertexArrayCB(const MFVec3::SetValueEvent& event);

      virtual void getNormalArrayCB(const MFVec3::GetValueEvent& event);
      virtual void setNormalArrayCB(const MFVec3::SetValueEvent& event);

      virtual void getColorArrayCB(const MFVec4::GetValueEvent& event);
      virtual void setColorArrayCB(const MFVec4::SetValueEvent& event);

      virtual void getTexCoordArrayCB(const MFVec2::GetValueEvent& event);
      virtual void setTexCoordArrayCB(const MFVec2::SetValueEvent& event);

      virtual void getNormalBindingCB(const SFInt::GetValueEvent& event);
      virtual void setNormalBindingCB(const SFInt::SetValueEvent& event);

      virtual void getColorBindingCB(const SFInt::GetValueEvent& event);
      virtual void setColorBindingCB(const SFInt::SetValueEvent& event);

    private:

      ::osg::Geometry *mOsgGeometry;
    };

    typedef SingleField<Link<Geometry> > SFGeometry;
    typedef MultiField<Link<Geometry> > MFGeometry;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::Geometry> >;
  template class AV_OSG_DLL MultiField<Link<osg::Geometry> >;
#endif

}

#endif
