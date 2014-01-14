// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 2007 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* AVANGO is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* AVANGO is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
\************************************************************************/

#include <shade/Shader.h>
#include <shade/Program.h>
#include <shade/GLSLWrapper.h>
#include <shade/shaders/DirectionalLight.h>
#include <shade/shaders/PointLight.h>
#include <shade/shaders/GLLightPosition.h>
#include <shade/shaders/Vec4ToVec3.h>
#include <shade/shaders/Surface.h>
#include <shade/shaders/Plastic.h>
#include <shade/shaders/ObjectSpace.h>
#include <shade/osg/Wrapper.h>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/ViewerEventHandlers>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/MatrixTransform>
#include <osg/Vec3>
#include <osg/Vec4>
#include <boost/shared_ptr.hpp>


void run_viewer(osg::Node* root)
{
  osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

  viewer->setUpViewInWindow(-1, -1, 400, 400);
  osg::ref_ptr<osgGA::TrackballManipulator> trackball(new osgGA::TrackballManipulator());
  trackball->setHomePosition(osg::Vec3(0., 0., -10.), osg::Vec3(0., 0., 0.), osg::Vec3(0., 1., 0.));
  viewer->setCameraManipulator(trackball);
  viewer->addEventHandler(new osgViewer::WindowSizeHandler);
  viewer->addEventHandler(new osgViewer::StatsHandler);

  viewer->setSceneData(root);
  viewer->run();
}

boost::shared_ptr<shade::Program> setup_shading(boost::shared_ptr<shade::GLSLWrapper> state)
{
  boost::shared_ptr<shade::shaders::Surface> shader(new shade::shaders::Surface);
  boost::shared_ptr<shade::Program> program(new shade::Program(shader, state));

  boost::shared_ptr<shade::shaders::Plastic> specular(new shade::shaders::Plastic(0.4, .6));
  boost::shared_ptr<shade::shaders::ObjectSpace> object_space(new shade::shaders::ObjectSpace);
  specular->color.set_value(shade::vec4<>(1., 0.4, 0.4, 1.));
  specular->coordinate_system = object_space;
  shader->material = specular;
  {
    shade::shaders::IlluminatedMaterial::LightList::Accessor accessor(specular->lights);

    boost::shared_ptr<shade::shaders::PointLight> light(new shade::shaders::PointLight);
    boost::shared_ptr<shade::shaders::GLLightPosition> gl_light_pos(new shade::shaders::GLLightPosition);
    boost::shared_ptr<shade::shaders::Vec4ToVec3> light_pos(new shade::shaders::Vec4ToVec3);
    gl_light_pos->index.set(1);
    light_pos->value = gl_light_pos;
    light->position = light_pos;
    light->color.set_value(shade::vec3<>(1., 1., 1.));
    accessor->push_back(light);

    boost::shared_ptr<shade::shaders::DirectionalLight> light2(new shade::shaders::DirectionalLight);
    light2->direction.set_value(shade::vec3<>(0., 1., 0.));
    light2->color.set_value(shade::vec3<>(1., 0., 0.));
    accessor->push_back(light2);
  }

  return program;
}

osg::ref_ptr<osg::Geode> build_sphere(void)
{
  osg::ref_ptr<osg::Sphere> sphere(new osg::Sphere);
  osg::ref_ptr<osg::ShapeDrawable> shape_drawable(new ::osg::ShapeDrawable(sphere.get()));

  osg::ref_ptr<osg::Geode> result(new osg::Geode);
  result->addDrawable(shape_drawable.get());

  return result;
}

int main(int argc, char* argv[])
{
  osg::ref_ptr<osg::Group> root(new osg::Group);
  osg::ref_ptr<osg::Geode> sphere(build_sphere());
  root->addChild(sphere.get());

  osg::ref_ptr<osg::Light> light(new osg::Light);
  light->setLightNum(1);
  light->setPosition(osg::Vec4(0., 0., 0., 1.));
  osg::ref_ptr<osg::LightSource> light_source(new osg::LightSource);
  light_source->setLight(light.get());
  osg::ref_ptr<osg::MatrixTransform> transform(new osg::MatrixTransform);
  transform->addChild(light_source.get());
  osg::Matrix matrix;
  matrix.makeTranslate(osg::Vec3(10., 0., 0.));
  transform->setMatrix(matrix);
  root->addChild(transform.get());

  boost::shared_ptr<shade::GLSLWrapper> state(new shade::osg::Wrapper(sphere->getOrCreateStateSet()));

  boost::shared_ptr<shade::Program> shader_program(setup_shading(state));
  shader_program->compile();
  state->make_current();
  shader_program->upload();

  run_viewer(root.get());
}
