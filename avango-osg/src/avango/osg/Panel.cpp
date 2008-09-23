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

#include <avango/osg/Panel.h>
#include <avango/Logger.h>
#include <boost/bind.hpp>

#if defined(_WIN32)
#define _USE_MATH_DEFINES
#endif

#include <cmath>

#if defined(_WIN32)
#include <math.h>
#endif

AV_FC_DEFINE(av::osg::Panel);

AV_FIELD_DEFINE(av::osg::SFPanel);
AV_FIELD_DEFINE(av::osg::MFPanel);

av::osg::Panel::Panel() :
  Geometry(),
  mGeometryChanged(false),
  mColorChanged(false)
{
  AV_FC_ADD_FIELD(Width, 1.0f);
  AV_FC_ADD_FIELD(Height, 1.0f);
  AV_FC_ADD_FIELD(EdgeRadius, 0.08f);
  AV_FC_ADD_FIELD(EdgeSmooth, 10);
  AV_FC_ADD_FIELD(PanelColor, ::osg::Vec4(0.8f,0.8f,0.8f,0.5f));
  AV_FC_ADD_FIELD(BorderColor, ::osg::Vec4(0.1f,0.1f,0.1f,0.9f));
  AV_FC_ADD_FIELD(BorderWidth, 0.015f);
  AV_FC_ADD_FIELD(Position, ::osg::Vec3(0,0,0));

  updateGeometry();
}

/* virtual */
av::osg::Panel::~Panel()
{}

/* static */ void
av::osg::Panel::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Geometry::initClass();

    AV_FC_INIT(av::osg::Geometry, av::osg::Panel, true);

    SFPanel::initClass("av::osg::SFPanel", "av::Field");
    MFPanel::initClass("av::osg::MFPanel", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

/* virtual */ void
av::osg::Panel::fieldHasChangedLocalSideEffect(const av::Field& field)
{
  Geometry::fieldHasChangedLocalSideEffect(field);

  if (&field == &Width ||
      &field == &Height ||
      &field == &Position ||
      &field == &EdgeRadius ||
      &field == &EdgeSmooth ||
      &field == &BorderWidth)
  {
    mGeometryChanged = true;
  }

  if (&field == &PanelColor ||
      &field == &BorderColor)
  {
    mColorChanged = true;
  }
}

/* virtual */ void
av::osg::Panel::evaluateLocalSideEffect()
{
  Geometry::evaluateLocalSideEffect();

  if (mGeometryChanged)
  {
    updateGeometry();
    mGeometryChanged = false;
  }

  if (mColorChanged)
  {
    updateColor();
    mColorChanged = false;
  }
}

void
av::osg::Panel::updateGeometry()
{
  // create vertices in XY plane
  int e = EdgeSmooth.getValue();
  float w  = Width.getValue() * 0.5f;
  float h  = Height.getValue() * 0.5f;
  float b  = BorderWidth.getValue();
  float x = Position.getValue().x();
  float y = Position.getValue().y();
  float z = Position.getValue().z();

  ::osg::ref_ptr< ::osg::Vec3Array> vertices = new ::osg::Vec3Array(e * 12 + 2);

  if(e > 1) // rounded Rectangle
  {
      float br = EdgeRadius.getValue() + BorderWidth.getValue();
      float r  = EdgeRadius.getValue();
      float step = (M_PI / 2.0f) / (e - 1);
      float a = 0.0f;
      float sina;
      float cosa;

      for(int count = 0; count < e; ++count)
      {
          sina = std::sin(a);
          cosa = std::cos(a);

          // panel (polygon)
          (*vertices)[e * 4 - count - 1].set( w - r + r * sina + x,  h - r + r * cosa + y, z);
          (*vertices)[e * 3 - count - 1].set( w - r + r * cosa + x, -h + r - r * sina + y, z);
          (*vertices)[e * 2 - count - 1].set(-w + r - r * sina + x, -h + r - r * cosa + y, z);
          (*vertices)[e * 1 - count - 1].set(-w + r - r * cosa + x,  h - r + r * sina + y,  z);

          // border (tri-stripes)
          // inner vertices (same as panel vertices)
          (*vertices)[e * 12 - 2 * count].set( w - r + r * sina + x,  h - r + r * cosa + y,  z);
          (*vertices)[e * 10 - 2 * count].set( w - r + r * cosa + x, -h + r - r * sina + y, z);
          (*vertices)[e * 8 - 2 * count].set(-w + r - r * sina + x, -h + r - r * cosa + y, z);
          (*vertices)[e * 6 - 2 * count].set(-w + r - r * cosa + x,  h - r + r * sina + y, z);

          // outer vertices
          (*vertices)[e * 12 - 2 * count + 1].set( w - r + br * sina + x,  h - r + br * cosa + y, z);
          (*vertices)[e * 10 - 2 * count + 1].set( w - r + br * cosa + x, -h + r - br * sina + y, z);
          (*vertices)[e * 8 - 2 * count + 1].set(-w + r - br * sina + x, -h + r - br * cosa + y, z);
          (*vertices)[e * 6 - 2 * count + 1].set(-w + r - br * cosa + x,  h - r + br * sina + y, z);

          a += step;
      }
  }
  else  // EdgeSmooth == 1 : Rectangle
  {
      // panel (polygon)
      (*vertices)[3].set( w + x,  h + y, z);
      (*vertices)[2].set( w + x, -h + y, z);
      (*vertices)[1].set(-w + x, -h + y, z);
      (*vertices)[0].set(-w + x,  h + y, z);

      // border (tri-stripes)
      // inner rectangle
      (*vertices)[7].set( w + x,  h + y, z);
      (*vertices)[9].set( w + x, -h + y, z);
      (*vertices)[11].set(-w + x, -h + y, z);
      (*vertices)[13].set(-w + x,  h + y, z);

      // outer rectangle
      (*vertices)[6].set( w + b + x,  h + b + y, z);
      (*vertices)[8].set( w + b + x, -h - b + y, z);
      (*vertices)[10].set(-w - b + x, -h - b + y, z);
      (*vertices)[12].set(-w - b + x,  h + b + y, z);
  }

  // close tristrip
  (*vertices)[e * 4] = (*vertices)[e * 12];
  (*vertices)[e * 4 + 1] = (*vertices)[e * 12 + 1];

  // geometry
  getOsgGeometry()->setVertexArray(vertices.get());

  // normals
  ::osg::ref_ptr< ::osg::Vec3Array> normals = new ::osg::Vec3Array;
  normals->push_back(::osg::Vec3(0,0,1));

  getOsgGeometry()->setNormalArray(normals.get());
  getOsgGeometry()->setNormalBinding(::osg::Geometry::BIND_OVERALL);

  updateColor();

  // set up drawable
  if(getOsgGeometry()->getPrimitiveSetList().size() > 0)
  {
    getOsgGeometry()->removePrimitiveSet(0,2); //remove prims on panel recreation
  }
  getOsgGeometry()->addPrimitiveSet(new ::osg::DrawArrays(::osg::PrimitiveSet::POLYGON,0,e * 4));
  getOsgGeometry()->addPrimitiveSet(new ::osg::DrawArrays(::osg::PrimitiveSet::TRIANGLE_STRIP,e * 4,e * 8 + 2));
}

void
av::osg::Panel::updateColor()
{
  ::osg::ref_ptr< ::osg::Vec4Array> color = new ::osg::Vec4Array;
  color->push_back(PanelColor.getValue());
  color->push_back(BorderColor.getValue());

  getOsgGeometry()->setColorArray(color.get());
  getOsgGeometry()->setColorBinding(::osg::Geometry::BIND_PER_PRIMITIVE_SET);
}
