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

#include <avango/osg/TexturedQuad.h>
#include <avango/Logger.h>
#include <boost/bind.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::TexturedQuad"));
}

AV_FC_DEFINE(av::osg::TexturedQuad);

AV_FIELD_DEFINE(av::osg::SFTexturedQuad);
AV_FIELD_DEFINE(av::osg::MFTexturedQuad);

av::osg::TexturedQuad::TexturedQuad() :
  Geometry(),
  mGeometryChanged(true),
  mColorChanged(true),
  mFilenameChanged(false),
  mTextureChanged1D(false),
  mTextureChanged2D(false),
  mTexCoordsChanged(false),
  mVertexArray(new ::osg::Vec3Array(4)),
  mNormals(new ::osg::Vec3Array(1)),
  mColors(new ::osg::Vec4Array(1)),
  mTexCoords(new ::osg::Vec2Array(4))
{
  av::osg::Texture1D *texture1D = new av::osg::Texture1D();
  av::osg::Texture2D *texture2D = new av::osg::Texture2D();

  AV_FC_ADD_FIELD(Width, 1.0f);
  AV_FC_ADD_FIELD(Height, 1.0f);
  AV_FC_ADD_FIELD(Color, ::osg::Vec4(1,1,1,1));
  AV_FC_ADD_FIELD(Position, ::osg::Vec3(0,0,0));
  AV_FC_ADD_FIELD(UseFilename, true);
  AV_FC_ADD_FIELD(Filename, "");
  AV_FC_ADD_FIELD(Texture1D, texture1D);
  AV_FC_ADD_FIELD(Texture2D, texture2D);
  AV_FC_ADD_FIELD(TexCoord01, ::osg::Vec2(0.0f,1.0f));
  AV_FC_ADD_FIELD(TexCoord00, ::osg::Vec2(0.0f,0.0f));
  AV_FC_ADD_FIELD(TexCoord10, ::osg::Vec2(1.0f,0.0f));
  AV_FC_ADD_FIELD(TexCoord11, ::osg::Vec2(1.0f,1.0f));

  SFVec3 TexCoord01;
  SFVec3 TexCoord00;
  SFVec3 TexCoord10;
  SFVec3 TexCoord11;

  AV_FC_ADD_ADAPTOR_FIELD(MinFilter,
                          boost::bind(&TexturedQuad::getMinFilterCB, this, _1),
                          boost::bind(&TexturedQuad::setMinFilterCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(MagFilter,
                          boost::bind(&TexturedQuad::getMagFilterCB, this, _1),
                          boost::bind(&TexturedQuad::setMagFilterCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(InternalFormatMode,
                          boost::bind(&TexturedQuad::getInternalFormatModeCB, this, _1),
                          boost::bind(&TexturedQuad::setInternalFormatModeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(MaxAnisotropy,
                          boost::bind(&TexturedQuad::getMaxAnisotropyCB, this, _1),
                          boost::bind(&TexturedQuad::setMaxAnisotropyCB, this, _1));

  // set up geometry
  getOsgGeometry()->setVertexArray(mVertexArray.get());
  (*mNormals)[0].set(0.0f,0.0f,1.0f);
  getOsgGeometry()->setNormalArray(mNormals.get());
  getOsgGeometry()->setNormalBinding(::osg::Geometry::BIND_OVERALL);
  getOsgGeometry()->addPrimitiveSet(new ::osg::DrawArrays(GL_QUADS,0,4));
  getOsgGeometry()->setColorArray(mColors.get());
  getOsgGeometry()->setColorBinding(::osg::Geometry::BIND_OVERALL);
  (*mTexCoords)[0].set(0.0f,1.0f);
  (*mTexCoords)[1].set(0.0f,0.0f);
  (*mTexCoords)[2].set(1.0f,0.0f);
  (*mTexCoords)[3].set(1.0f,1.0f);
  getOsgGeometry()->setTexCoordArray(0,mTexCoords.get());

  // set up texture
  Texture2D.getValue()->getOsgTexture2D()->setWrap(::osg::Texture::WRAP_S, ::osg::Texture::CLAMP_TO_EDGE);
  Texture2D.getValue()->getOsgTexture2D()->setWrap(::osg::Texture::WRAP_T, ::osg::Texture::CLAMP_TO_EDGE);
}

/* virtual */
av::osg::TexturedQuad::~TexturedQuad()
{}

/* static */ void
av::osg::TexturedQuad::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Geometry::initClass();

    AV_FC_INIT(av::osg::Geometry, av::osg::TexturedQuad, true);

    SFTexturedQuad::initClass("av::osg::SFTexturedQuad", "av::Field");
    MFTexturedQuad::initClass("av::osg::MFTexturedQuad", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

/* virtual */ void
av::osg::TexturedQuad::fieldHasChangedLocalSideEffect(const av::Field& field)
{
  Geometry::fieldHasChangedLocalSideEffect(field);

  if (&field == &Width ||
      &field == &Height ||
      &field == &Position)
  {
    mGeometryChanged = true;
  }

  if (&field == &Color)
  {
    mColorChanged = true;
  }

  if (&field == &Filename)
  {
    mFilenameChanged = true;
  }

  if (&field == &Texture1D)
  {
    mTextureChanged1D = true;
  }

  if (&field == &Texture2D)
  {
    mTextureChanged2D = true;
  }

  if (&field == &TexCoord01 || &field == &TexCoord00 || &field == &TexCoord10 || &field == &TexCoord11 )
  {
    mTexCoordsChanged = true;
  }
}

/* virtual */ void
av::osg::TexturedQuad::evaluateLocalSideEffect()
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

  if (mFilenameChanged && UseFilename.getValue())
  {
    updateTexture();
    mFilenameChanged = false;
  }

  if (mTextureChanged1D && !UseFilename.getValue())
  {
    getOsgGeometry()->getOrCreateStateSet()->setTextureAttributeAndModes(0,Texture1D.getValue()->getOsgTexture1D(),::osg::StateAttribute::ON);
    mTextureChanged1D = false;
  }

  if (mTextureChanged2D && !UseFilename.getValue())
  {
    getOsgGeometry()->getOrCreateStateSet()->setTextureAttributeAndModes(0,Texture2D.getValue()->getOsgTexture2D(),::osg::StateAttribute::ON);
    mTextureChanged2D = false;
  }

  if (mTexCoordsChanged)
  {
    (*mTexCoords)[0] = TexCoord01.getValue();
    (*mTexCoords)[1] = TexCoord00.getValue();
    (*mTexCoords)[2] = TexCoord10.getValue();
    (*mTexCoords)[3] = TexCoord11.getValue();
    getOsgGeometry()->setTexCoordArray(0,mTexCoords.get());
    mTexCoordsChanged = false;
  }
}

::osg::ref_ptr< ::osg::Texture2D>
av::osg::TexturedQuad::getOsgTexture() const {
  ::osg::ref_ptr< ::osg::Texture2D> tex = Texture2D.getValue()->getOsgTexture2D();
  return tex;
}

void
av::osg::TexturedQuad::updateGeometry()
{
  ::osg::Vec3::value_type x = Position.getValue().x();
  ::osg::Vec3::value_type y = Position.getValue().y();
  ::osg::Vec3::value_type z = Position.getValue().z();

  (*mVertexArray)[0].set(x,y,z); // top left
  (*mVertexArray)[1].set(x,-Height.getValue() + y,z); // bottom left
  (*mVertexArray)[2].set(Width.getValue() + x,-Height.getValue() + y,z); // bottom right
  (*mVertexArray)[3].set(Width.getValue() + x,y,z); // top right

  getOsgGeometry()->dirtyDisplayList();
  getOsgGeometry()->computeFastPathsUsed();
  getOsgGeometry()->dirtyBound();
}

void
av::osg::TexturedQuad::updateTexture()
{
  if(Filename.getValue().size() != 0 && UseFilename.getValue())
  {
    Texture2D.getValue()->getOsgTexture2D()->setImage(::osgDB::readImageFile(Filename.getValue()));
    getOsgGeometry()->getOrCreateStateSet()->setTextureAttributeAndModes(0,Texture2D.getValue()->getOsgTexture2D(),::osg::StateAttribute::ON);
  }
}

void
av::osg::TexturedQuad::updateColor()
{
  (*mColors)[0] = Color.getValue();
  getOsgGeometry()->dirtyDisplayList();
  getOsgGeometry()->computeFastPathsUsed();
}

/* virtual */ void
av::osg::TexturedQuad::getMinFilterCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = Texture2D.getValue()->getOsgTexture2D()->getFilter(::osg::Texture2D::MIN_FILTER);
}

/* virtual */ void
av::osg::TexturedQuad::setMinFilterCB(const av::SFInt::SetValueEvent& event)
{
	Texture2D.getValue()->getOsgTexture2D()->setFilter(::osg::Texture2D::MIN_FILTER,(::osg::Texture2D::FilterMode)(event.getValue()));
}

/* virtual */ void
av::osg::TexturedQuad::getMagFilterCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = Texture2D.getValue()->getOsgTexture2D()->getFilter(::osg::Texture2D::MAG_FILTER);
}

/* virtual */ void
av::osg::TexturedQuad::setMagFilterCB(const av::SFInt::SetValueEvent& event)
{
  Texture2D.getValue()->getOsgTexture2D()->setFilter(::osg::Texture2D::MAG_FILTER,(::osg::Texture2D::FilterMode)(event.getValue()));
}

/* virtual */ void
av::osg::TexturedQuad::getInternalFormatModeCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = Texture2D.getValue()->getOsgTexture2D()->getInternalFormatMode();
}

/* virtual */ void
av::osg::TexturedQuad::setInternalFormatModeCB(const av::SFInt::SetValueEvent& event)
{
  Texture2D.getValue()->getOsgTexture2D()->setInternalFormatMode((::osg::Texture2D::InternalFormatMode)(event.getValue()));
}

/* virtual */ void
av::osg::TexturedQuad::getMaxAnisotropyCB(const av::SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = Texture2D.getValue()->getOsgTexture2D()->getMaxAnisotropy();
}

/* virtual */ void
av::osg::TexturedQuad::setMaxAnisotropyCB(const av::SFFloat::SetValueEvent& event)
{
  Texture2D.getValue()->getOsgTexture2D()->setMaxAnisotropy(event.getValue());
}
