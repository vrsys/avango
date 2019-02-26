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

#include <avango/osg/Quad.h>
#include <avango/Logger.h>
#include <boost/bind.hpp>

namespace
{
av::Logger& logger(av::getLogger("av::osg::Quad"));
}

AV_FC_DEFINE(av::osg::Quad);

AV_FIELD_DEFINE(av::osg::SFQuad);
AV_FIELD_DEFINE(av::osg::MFQuad);

av::osg::Quad::Quad() : Geometry(), mGeometryChanged(true), mColorChanged(true), mVertexArray(new ::osg::Vec3Array(4)), mNormals(new ::osg::Vec3Array(1)), mColors(new ::osg::Vec4Array(1))
{
    AV_FC_ADD_FIELD(Width, 1.0f);
    AV_FC_ADD_FIELD(Height, 1.0f);
    AV_FC_ADD_FIELD(Color, ::osg::Vec4(1, 1, 1, 1));
    AV_FC_ADD_FIELD(Position, ::osg::Vec3(0, 0, 0));

    // set up geometry
    getOsgGeometry()->setVertexArray(mVertexArray.get());
    (*mNormals)[0].set(0.0f, 0.0f, 1.0f);
    getOsgGeometry()->setNormalArray(mNormals.get());
    getOsgGeometry()->setNormalBinding(::osg::Geometry::BIND_OVERALL);
    getOsgGeometry()->addPrimitiveSet(new ::osg::DrawArrays(GL_QUADS, 0, 4));
    getOsgGeometry()->setColorArray(mColors.get());
    getOsgGeometry()->setColorBinding(::osg::Geometry::BIND_OVERALL);
}

/* virtual */
av::osg::Quad::~Quad() {}

/* static */ void av::osg::Quad::initClass()
{
    if(!isTypeInitialized())
    {
        av::osg::Geometry::initClass();

        AV_FC_INIT(av::osg::Geometry, av::osg::Quad, true);

        SFQuad::initClass("av::osg::SFQuad", "av::Field");
        MFQuad::initClass("av::osg::MFQuad", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

/* virtual */ void av::osg::Quad::fieldHasChangedLocalSideEffect(const av::Field& field)
{
    Geometry::fieldHasChangedLocalSideEffect(field);

    if(&field == &Width || &field == &Height || &field == &Position)
    {
        mGeometryChanged = true;
    }

    if(&field == &Color)
    {
        mColorChanged = true;
    }
}

/* virtual */ void av::osg::Quad::evaluateLocalSideEffect()
{
    Geometry::evaluateLocalSideEffect();

    if(mGeometryChanged)
    {
        updateGeometry();
        mGeometryChanged = false;
    }

    if(mColorChanged)
    {
        updateColor();
        mColorChanged = false;
    }
}

void av::osg::Quad::updateGeometry()
{
    ::osg::Vec3::value_type x = Position.getValue().x();
    ::osg::Vec3::value_type y = Position.getValue().y();
    ::osg::Vec3::value_type z = Position.getValue().z();

    (*mVertexArray)[0].set(x, y, z);                                         // top left
    (*mVertexArray)[1].set(x, -Height.getValue() + y, z);                    // bottom left
    (*mVertexArray)[2].set(Width.getValue() + x, -Height.getValue() + y, z); // bottom right
    (*mVertexArray)[3].set(Width.getValue() + x, y, z);                      // top right

    getOsgGeometry()->dirtyDisplayList();
    getOsgGeometry()->computeFastPathsUsed();
    getOsgGeometry()->dirtyBound();
}

void av::osg::Quad::updateColor()
{
    (*mColors)[0] = Color.getValue();
    getOsgGeometry()->dirtyDisplayList();
    getOsgGeometry()->computeFastPathsUsed();
}
