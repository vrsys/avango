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
#include <shade/shaders/ShadowPointLight.h>
#include <shade/shaders/PointLight.h>
#include <shade/shaders/GLLightPosition.h>
#include <shade/shaders/Vec4ToVec3.h>
#include <shade/shaders/Surface.h>
#include <shade/shaders/Plastic.h>
#include <shade/shaders/ObjectSpace.h>
#include <shade/osg/Wrapper.h>
#include <shade/osg/Texture.h>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/ViewerEventHandlers>
#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/MatrixTransform>
#include <osg/Vec3>
#include <osg/Vec4>
#include <boost/shared_ptr.hpp>
#include <math.h>


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

  boost::shared_ptr<shade::shaders::Plastic> specular(new shade::shaders::Plastic(1., 0.));
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
  }

  return program;
}

boost::shared_ptr<shade::Program> setup_shadowed_shading(boost::shared_ptr<shade::GLSLWrapper> state, osg::Matrix view_matrix, boost::shared_ptr<shade::Texture> shadow_map)
{
  boost::shared_ptr<shade::shaders::Surface> shader(new shade::shaders::Surface);
  boost::shared_ptr<shade::Program> program(new shade::Program(shader, state));

  boost::shared_ptr<shade::shaders::Plastic> specular(new shade::shaders::Plastic(1., 0.));
  boost::shared_ptr<shade::shaders::ObjectSpace> object_space(new shade::shaders::ObjectSpace);
  specular->color.set_value(shade::vec4<>(1., 1., 1., 1.));
  specular->coordinate_system = object_space;
  shader->material = specular;
  {
    shade::shaders::IlluminatedMaterial::LightList::Accessor accessor(specular->lights);

    boost::shared_ptr<shade::shaders::ShadowPointLight> shadow_light(new shade::shaders::ShadowPointLight);
    boost::shared_ptr<shade::shaders::GLLightPosition> gl_light_pos(new shade::shaders::GLLightPosition);
    boost::shared_ptr<shade::shaders::Vec4ToVec3> light_pos(new shade::shaders::Vec4ToVec3);
    gl_light_pos->index.set(1);
    light_pos->value = gl_light_pos;
    shadow_light->position = light_pos;
    shadow_light->color.set_value(shade::vec3<>(1., 1., 1.));
    shadow_light->shadow_map.set(shadow_map);
    float matrix[16];
    for (int i = 0; i != 16; ++i)
    {
      matrix[i] = view_matrix.ptr()[i];
      std::cout << matrix[i] << std::endl;
    }
    shadow_light->modelview.set(matrix);
    accessor->push_back(shadow_light);
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

osg::ref_ptr<osg::Geode> build_quad(void)
{
  osg::ref_ptr<osg::Geometry> geom(new osg::Geometry);

  osg::ref_ptr<osg::Vec3Array> v(new osg::Vec3Array);
  geom->setVertexArray(v.get());
  v->push_back(osg::Vec3(-10.f, 0.f, -10.f));
  v->push_back(osg::Vec3(10.f, 0.f, -10.f));
  v->push_back(osg::Vec3(10.f, 0.f, 10.f));
  v->push_back(osg::Vec3(-10.f, 0.f, 10.f));

  osg::ref_ptr<osg::Vec3Array> n(new osg::Vec3Array);
  geom->setNormalArray(n.get());
  geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
  n->push_back(osg::Vec3(0.f, 1.f, 0.f));
  n->push_back(osg::Vec3(0.f, 1.f, 0.f));
  n->push_back(osg::Vec3(0.f, 1.f, 0.f));
  n->push_back(osg::Vec3(0.f, 1.f, 0.f));

  osg::ref_ptr<osg::Vec2Array> vt(new osg::Vec2Array);
  geom->setTexCoordArray(0, vt.get());
  vt->push_back(osg::Vec2(0.f, 0.f));
  vt->push_back(osg::Vec2(1.f, 0.f));
  vt->push_back(osg::Vec2(1.f, 1.f));
  vt->push_back(osg::Vec2(0.f, 1.f));

  geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

  osg::ref_ptr<osg::Geode> result(new osg::Geode);
  result->addDrawable(geom.get());

  return result;
}

int main(int argc, char* argv[])
{
  osg::ref_ptr<osg::Group> root(new osg::Group);
  osg::ref_ptr<osg::Geode> sphere(build_sphere());
  root->addChild(sphere.get());

  osg::ref_ptr<osg::Geode> quad(build_quad());
  osg::ref_ptr<osg::MatrixTransform> quad_transform(new osg::MatrixTransform);
  quad_transform->addChild(quad.get());
  osg::Matrix quad_matrix;
  quad_matrix.makeTranslate(osg::Vec3(0., -2., 0.));
  quad_transform->setMatrix(quad_matrix);
  root->addChild(quad_transform.get());

  osg::ref_ptr<osg::Light> light(new osg::Light);
  light->setLightNum(1);
  light->setPosition(osg::Vec4(0., 0., 0., 1.));
  osg::ref_ptr<osg::LightSource> light_source(new osg::LightSource);
  light_source->setLight(light.get());
  osg::ref_ptr<osg::MatrixTransform> light_transform(new osg::MatrixTransform);
  light_transform->addChild(light_source.get());
  osg::Matrix light_trans_matrix;
  light_trans_matrix.makeTranslate(osg::Vec3(0., 5., 0.));
  osg::Matrix light_rot_matrix;
  light_rot_matrix.makeRotate(-M_PI/2., 1., 0., 0.);
  light_transform->setMatrix(light_rot_matrix * light_trans_matrix);
  root->addChild(light_transform.get());

  osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D;
  tex->setTextureSize(1024, 1024);
  tex->setInternalFormat(GL_DEPTH_COMPONENT);
  tex->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
  tex->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
  tex->setShadowComparison(true);
  tex->setShadowCompareFunc(osg::Texture::LEQUAL);
  tex->setShadowTextureMode(osg::Texture::ALPHA);
  boost::shared_ptr<shade::osg::Texture> shade_tex(new shade::osg::Texture(tex.get()));

  osg::ref_ptr<osg::Camera> shadow_camera = new osg::Camera;
  shadow_camera->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  shadow_camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
  shadow_camera->setProjectionMatrixAsPerspective(90., 1., 0.1, 10.);
  osg::Matrix shadow_view_matrix;
  shadow_view_matrix.invert(light_transform->getMatrix());
  shadow_camera->setViewMatrix(shadow_view_matrix);
  shadow_camera->setViewport(0, 0, 1024, 1024);
  shadow_camera->setRenderOrder(osg::Camera::PRE_RENDER);
  shadow_camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT, osg::Camera::FRAME_BUFFER);
  shadow_camera->attach(osg::Camera::DEPTH_BUFFER, tex.get(), 0, 0, false);

  shadow_camera->addChild(sphere);
  root->addChild(shadow_camera);

  boost::shared_ptr<shade::GLSLWrapper> quad_state(new shade::osg::Wrapper(quad->getOrCreateStateSet()));
  boost::shared_ptr<shade::Program> quad_shader_program(setup_shadowed_shading(quad_state, shadow_view_matrix*shadow_camera->getProjectionMatrix(), shade_tex));
  quad_shader_program->compile();
  quad_state->make_current();
  quad_shader_program->upload();

  boost::shared_ptr<shade::GLSLWrapper> sphere_state(new shade::osg::Wrapper(sphere->getOrCreateStateSet()));
  boost::shared_ptr<shade::Program> sphere_shader_program(setup_shading(sphere_state));
  sphere_shader_program->compile();
  sphere_state->make_current();
  sphere_shader_program->upload();

  run_viewer(root.get());
}
