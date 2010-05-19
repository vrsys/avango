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

#include <avango/osg/Init.h>

#include <avango/Logger.h>

#include <avango/osg/AbsoluteTransform.h>
#include <avango/osg/AutoTransform.h>
#include <avango/osg/Billboard.h>
#include <avango/osg/BlendFunc.h>
#include <avango/osg/BoundingBoxCalculator.h>
#include <avango/osg/BoundingSphere.h>
#include <avango/osg/Camera.h>
#include <avango/osg/CameraAttachment.h>
#include <avango/osg/ClipPlane.h>
#include <avango/osg/ClipNode.h>
#include <avango/osg/CullFace.h>
#include <avango/osg/Depth.h>
#include <avango/osg/Drawable.h>
#include <avango/osg/Fields.h>
#include <avango/osg/Fog.h>
#include <avango/osg/Shader.h>
#include <avango/osg/Uniform.h>
#include <avango/osg/Program.h>
#include <avango/osg/Light.h>
#include <avango/osg/LightSource.h>
#include <avango/osg/Geode.h>
#include <avango/osg/Geometry.h>
#include <avango/osg/Group.h>
#include <avango/osg/Image.h>
#include <avango/osg/ImageStream.h>
#include <avango/osg/Intersection.h>
#include <avango/osg/LayerGeode.h>
#include <avango/osg/LayerGroup.h>
#include <avango/osg/LineSegmentIntersector.h>
#include <avango/osg/LoadFile.h>
#include <avango/osg/LoadImage.h>
#include <avango/osg/LoadImageStream.h>
#include <avango/osg/MatrixTransform.h>
#include <avango/osg/Node.h>
#include <avango/osg/Panel.h>
#include <avango/osg/PolygonOffset.h>
#include <avango/osg/Projection.h>
#include <avango/osg/Quad.h>
#include <avango/osg/Sphere.h>
#include <avango/osg/Box.h>
#include <avango/osg/Capsule.h>
#include <avango/osg/StateAttribute.h>
#include <avango/osg/StateSet.h>
#include <avango/osg/Switch.h>
#include <avango/osg/Text.h>
#include <avango/osg/Texture.h>
#include <avango/osg/Texture1D.h>
#include <avango/osg/Texture2D.h>
#include <avango/osg/Texture2DArray.h>
#include <avango/osg/TextureCubeMap.h>
#include <avango/osg/TexturedQuad.h>
#include <avango/osg/Transform.h>
#include <avango/osg/TriangleContainer.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::Init"));
}

AV_TYPED_DEFINE_ABSTRACT(av::osg::Init);

/* static */ void
av::osg::Init::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::initFields();

    av::osg::AbsoluteTransform::initClass();
    av::osg::AutoTransform::initClass();
    av::osg::Billboard::initClass();
    av::osg::Camera::initClass();
    av::osg::CameraAttachment::initClass();
    av::osg::ClipPlane::initClass();
    av::osg::ClipNode::initClass();
    av::osg::CullFace::initClass();
    av::osg::Depth::initClass();
    av::osg::BlendFunc::initClass();
    av::osg::BoundingBoxCalculator::initClass();
    av::osg::BoundingSphere::initClass();
    av::osg::Drawable::initClass();
    av::osg::Fog::initClass();
    av::osg::Shader::initClass();
    av::osg::Program::initClass();
    av::osg::Uniform::initClass();
    av::osg::Light::initClass();
    av::osg::LightSource::initClass();
    av::osg::Geode::initClass();
    av::osg::Geometry::initClass();
    av::osg::Group::initClass();
    av::osg::Image::initClass();
    av::osg::ImageStream::initClass();
    av::osg::Intersection::initClass();
    av::osg::LayerGeode::initClass();
    av::osg::LayerGroup::initClass();
    av::osg::LineSegmentIntersector::initClass();
    av::osg::LoadFile::initClass();
    av::osg::LoadImage::initClass();
    av::osg::LoadImageStream::initClass();
    av::osg::MatrixTransform::initClass();
    av::osg::Node::initClass();
    av::osg::Panel::initClass();
    av::osg::PolygonOffset::initClass();
    av::osg::Projection::initClass();
    av::osg::Quad::initClass();
    av::osg::Sphere::initClass();
    av::osg::Box::initClass();
    av::osg::Capsule::initClass();
    av::osg::StateAttribute::initClass();
    av::osg::StateSet::initClass();
    av::osg::Switch::initClass();
    av::osg::Text::initClass();
    av::osg::Texture::initClass();
    av::osg::Texture1D::initClass();
    av::osg::Texture2D::initClass();
    av::osg::Texture2DArray::initClass();
    av::osg::TextureCubeMap::initClass();
    av::osg::TexturedQuad::initClass();
    av::osg::Transform::initClass();
    av::osg::TriangleContainer::initClass();

    AV_TYPED_INIT_ABSTRACT(av::Type::badType(), "av::osg::Init", true);
  }
}
