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

#include <avango/osg/TriangleContainer.h>

#include <avango/Logger.h>
#include <iostream>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::TriangleContainer"));
}

AV_FC_DEFINE(av::osg::TriangleContainer);

AV_FIELD_DEFINE(av::osg::SFTriangleContainer);
AV_FIELD_DEFINE(av::osg::MFTriangleContainer);

av::osg::TriangleContainer::TriangleContainer() :
  mVecFieldHasChanged(false),
  mColorFieldHasChanged(false),
  mModeFieldHasChanged(false),
  mNormalFieldHasChanged(false)
{
  AV_FC_ADD_FIELD(Vertices, MFVec3::ContainerType());
  AV_FC_ADD_FIELD(Colors, MFVec4::ContainerType());
  AV_FC_ADD_FIELD(Normals, MFVec3::ContainerType());
  AV_FC_ADD_FIELD(Mode, GL_TRIANGLES);
  AV_FC_ADD_FIELD(ModeToggle, false);
  AV_FC_ADD_FIELD(PointSize, 1.0);
}

av::osg::TriangleContainer::~TriangleContainer()
{}

void
av::osg::TriangleContainer::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Geometry::initClass();

    AV_FC_INIT(av::osg::Geometry, av::osg::TriangleContainer, true);

    SFTriangleContainer::initClass("av::osg::SFTriangleContainer", "av::Field");
    MFTriangleContainer::initClass("av::osg::MFTriangleContainer", "av::Field");
  }
}

void
av::osg::TriangleContainer::updateVertexBuffer()
{
  unsigned int numEle = Vertices.getValue().size();

  //check if we have enough points for a triangle
  if (numEle < 3)
  {
    logger.warn("Not enough points to create triangle");
    return;
  }
  ::osg::ref_ptr< ::osg::Vec3Array> vertexBuffer = new ::osg::Vec3Array(numEle);
  MFVec3::ContainerType::const_iterator it = Vertices.getValue().begin();

  unsigned int count = 0;
  for( ; it != Vertices.getValue().end(); ++it)
  {
    (*vertexBuffer)[count] = (*it);
    //std::cout << "#: " << count << ' ' << (*it).x() << ' ' << (*it).y() << ' ' << (*it).z() << std::endl;
    count++;
  }
  mTriangleCount = count / 3;

  getOsgGeometry()->setVertexArray(vertexBuffer.get());

  if (getOsgGeometry()->getNumPrimitiveSets()) //first geometry?
    getOsgGeometry()->setPrimitiveSet(0,new ::osg::DrawArrays(Mode.getValue(),0, numEle));
  else
    getOsgGeometry()->addPrimitiveSet(new ::osg::DrawArrays(Mode.getValue(),0, numEle));

  ::osg::StateSet* state_set = getOsgGeometry()->getOrCreateStateSet();

  ::osg::Point* osg_point = new ::osg::Point();
  osg_point->setSize(PointSize.getValue());

  state_set->setAttribute(osg_point);

  mVecFieldHasChanged = false;
}

void
av::osg::TriangleContainer::updateNormal()
{
  if(Normals.getValue().size() == 0 )
  {
    mNormalFieldHasChanged = false;
    getOsgGeometry()->setNormalBinding(::osg::Geometry::BIND_OFF);
    return; //take default opengl face normal for backface culling
  }

  ::osg::ref_ptr< ::osg::Vec3Array> normalBuffer = new ::osg::Vec3Array(Normals.getValue().size());
  MFVec3::ContainerType::const_iterator it = Normals.getValue().begin();
  MFVec3::ContainerType::const_iterator itEnd = Normals.getValue().end();

  unsigned count = 0;
  for ( ; it != itEnd; ++it)
  {
    (*normalBuffer)[count] = (*it);
    count++;
  }
  mNormalCount = count;
  getOsgGeometry()->setNormalArray(normalBuffer.get());

  //
  //  if(Normals.getValue().size() == Vertices.getValue().size())
  //  {
  //      getOsgGeometry()->setNormalBinding(::osg::Geometry::BIND_PER_VERTEX);
  //      std::cout << "per vertex" << std::endl;
  //  }
  //
  //  else if ( Normals.getValue().size() == ( Vertices.getValue().size() / 3) )
  //  {
  //    getOsgGeometry()->setNormalBinding(::osg::Geometry::BIND_PER_PRIMITIVE);
  //    std::cout << "per prim" << std::endl;
  //  }
  //
  //  else
  //  {
  //    getOsgGeometry()->setNormalBinding(::osg::Geometry::BIND_OFF);
  //    std::cout << "bind off" << std::endl;
  //  }

  getOsgGeometry()->setNormalBinding(::osg::Geometry::BIND_PER_PRIMITIVE);
  mNormalFieldHasChanged = false;
}

void
av::osg::TriangleContainer::updateColor()
{
  unsigned int numEle = Colors.getValue().size();
  unsigned int count = 0;

  if(!numEle)
    return;

  MFVec4::ContainerType::const_iterator it = Colors.getValue().begin();
  ::osg::ref_ptr< ::osg::Vec4Array> colorBuffer = new ::osg::Vec4Array(numEle);

  for( ; it != Colors.getValue().end(); ++it)
  {
    (*colorBuffer)[count] = (*it);
    count++;
  }
  //todo: this is just the case for a triangle. should be mode dependant
  getOsgGeometry()->setColorArray(colorBuffer.get());
  if(numEle == 1)
  {
    getOsgGeometry()->setColorBinding(::osg::Geometry::BIND_OVERALL);
  }
  else if (numEle == Vertices.getValue().size())
  {
    getOsgGeometry()->setColorBinding(::osg::Geometry::BIND_PER_VERTEX);
  }
  else if (numEle == (Vertices.getValue().size() / 3 ) )
  {
    getOsgGeometry()->setColorBinding(::osg::Geometry::BIND_PER_PRIMITIVE);
  }

  mColorFieldHasChanged = false;
}

void
av::osg::TriangleContainer::fieldHasChangedLocalSideEffect(const av::Field& field)
{
  Geometry::fieldHasChangedLocalSideEffect(field);

  if (&field == &Vertices )
    mVecFieldHasChanged = true;
  else if(&field == &Colors)
    mColorFieldHasChanged = true;
  else if(&field == &Mode)
    mModeFieldHasChanged = true;
  else if(&field == &Normals)
    mNormalFieldHasChanged = true;
}

void
av::osg::TriangleContainer::evaluateLocalSideEffect()
{
  Geometry::evaluateLocalSideEffect();

  if (mVecFieldHasChanged)
    updateVertexBuffer();
  if(mColorFieldHasChanged)
    updateColor();
  if(mModeFieldHasChanged)
    updateMode();
  if(mNormalFieldHasChanged)
    updateNormal();
}

void
av::osg::TriangleContainer::update()
{
  updateVertexBuffer();
  updateColor();
  updateNormal();
}

void
av::osg::TriangleContainer::updateMode()
{
  mModeFieldHasChanged = false;

  unsigned inMode = Mode.getValue();

  //todo: there should be a much more elegant way doing this. think!
  if(inMode == GL_TRIANGLES  || inMode == GL_TRIANGLE_STRIP ||
      inMode == GL_TRIANGLE_FAN || inMode == GL_POINTS ||
      inMode == GL_LINES  || inMode == GL_LINE_STRIP ||
      inMode == GL_LINE_LOOP || inMode == GL_QUADS ||
      inMode == GL_QUAD_STRIP || inMode == GL_POLYGON)
  {
    Mode.setValue(inMode);
    update();
  }
  else
    logger.warn("Mode type not supported");
}

unsigned
av::osg::TriangleContainer::getTriangleCount() const
{
  return mTriangleCount;
}

unsigned
av::osg::TriangleContainer::getNormalCount() const
{
  return mNormalCount;
}
