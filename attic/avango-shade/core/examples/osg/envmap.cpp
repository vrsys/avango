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
#include <shade/Type.h>
#include <shade/Program.h>
#include <shade/GLSLWrapper.h>
#include <shade/shaders/Surface.h>
#include <shade/shaders/Constant.h>
#include <shade/shaders/Texture2D.h>
#include <shade/shaders/IMaterial.h>
#include <shade/shaders/ICoordinateSystem.h>
#include <shade/shaders/PerStageGettable.h>
#include <shade/shaders/ObjectSpace.h>
#include <shade/osg/Wrapper.h>
#include <shade/osg/Texture.h>
#include <shade/utility.h>
#include <shade/types/objref.h>
#include <shade/types/const.h>
#include <shade/types/vec2.h>
#include <shade/types/vec3.h>
#include <shade/types/void.h>
#include <shade/types/Varying.h>
#include <shade/types/samplerCube.h>
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
#include <osg/NodeCallback>
#include <osg/FrontFace>
#include <osg/TextureCubeMap>
#include <boost/shared_ptr.hpp>
#include <cmath>

class MirrorCamera : public osg::NodeCallback
{
  public:
    MirrorCamera(osg::ref_ptr<osg::Camera> camera, osg::Matrix from_origin) : m_camera(camera), m_from_origin(from_origin) {}

    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        osg::Camera* mirror_camera = dynamic_cast<osg::Camera*>(node);
        mirror_camera->setProjectionMatrix(m_camera->getProjectionMatrix());

        osg::Matrix mirror;
        mirror.makeScale(1., -1., 1.);

        mirror_camera->setViewMatrix(osg::Matrix::inverse(m_from_origin) * mirror * m_from_origin * m_camera->getViewMatrix());
    }

  private:
    osg::ref_ptr<osg::Camera> m_camera;
    osg::Matrix m_from_origin;
};

class RotateCallback : public osg::NodeCallback
{
  public:
    RotateCallback(osg::Vec3 axis, double speed) : m_axis(axis), m_angle(0.), m_speed(speed) {}

    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        m_angle += m_speed;
        osg::Matrix rotation;
        rotation.makeRotate(m_angle, m_axis);

        osg::MatrixTransform* transform = dynamic_cast<osg::MatrixTransform*>(node);
        transform->setMatrix(rotation);
    }

  private:
    osg::Vec3 m_axis;
    double m_angle;
    double m_speed;
};

class ScreenCoord : public shade::ShaderBase<ScreenCoord, shade::shaders::Initializeable, shade::shaders::PerStageGettable<shade::vec2>>
{
  public:
    /*virtual*/ shade::void_<> init_vertex(void) { return invoke<shade::void_<>>("ScreenCoord_init_vertex_impl"); }

    /*virtual*/ shade::vec2<> get_fragment(void) { return invoke<shade::vec2<>>("ScreenCoord_get_fragment_impl"); }

  private:
    shade::Varying<shade::vec3> uv_coord;

    SHADE_MULTI_DERIVED_DECL(ScreenCoord, (shade::shaders::Initializeable)(shade::shaders::PerStageGettable<shade::vec2>))
};

SHADE_CLASS_INIT(ScreenCoord, "osg/ScreenCoord.glsl", SHADE_NONE, SHADE_DEFS((uv_coord)))

class Reflection : public shade::ShaderBase<Reflection, shade::shaders::IMaterial>
{
  public:
    /*virtual*/ shade::vec4<> shade(void) { return invoke<shade::vec4<>>("Reflection_shade_impl"); }

    shade::objref<boost::shared_ptr<shade::shaders::ICoordinateSystem>, shade::const_> coordinate_system;
    shade::samplerCube cube_map;

  private:
    SHADE_DERIVED_DECL(Reflection, shade::shaders::IMaterial)
};

SHADE_CLASS_INIT(Reflection, "osg/Reflection.glsl", SHADE_NONE, SHADE_DEFS((coordinate_system)(cube_map)))

osg::ref_ptr<osgViewer::Viewer> build_viewer(osg::Node* root)
{
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    viewer->setUpViewInWindow(-1, -1, 400, 400);
    osg::ref_ptr<osgGA::TrackballManipulator> trackball(new osgGA::TrackballManipulator());
    trackball->setHomePosition(osg::Vec3(0., 0., -10.), osg::Vec3(0., 0., 0.), osg::Vec3(0., 1., 0.));
    viewer->setCameraManipulator(trackball);
    viewer->addEventHandler(new osgViewer::WindowSizeHandler);
    viewer->addEventHandler(new osgViewer::StatsHandler);

    viewer->setSceneData(root);
    return viewer;
}

void setup_plane_shading(osg::ref_ptr<osg::StateSet> osg_state, boost::shared_ptr<shade::Texture> plane_map)
{
    boost::shared_ptr<shade::GLSLWrapper> state(new shade::osg::Wrapper(osg_state));

    boost::shared_ptr<shade::shaders::Surface> shader(new shade::shaders::Surface);
    boost::shared_ptr<shade::Program> program(new shade::Program(shader, state));

    boost::shared_ptr<shade::shaders::Constant> plane_material(new shade::shaders::Constant());
    plane_material->color.set_value(shade::vec4<>(1., 0.4, 0.4, 1.));
    shader->material = plane_material;

    boost::shared_ptr<shade::shaders::Texture2D> texture(new shade::shaders::Texture2D);
    texture->texture_unit.set(plane_map);
    boost::shared_ptr<ScreenCoord> uvcoord(new ScreenCoord);
    texture->uv = uvcoord;
    plane_material->color = texture;

    program->compile();
    state->make_current();
    program->upload();
}

void setup_sphere_shading(osg::ref_ptr<osg::StateSet> osg_state, boost::shared_ptr<shade::Texture> cube_map)
{
    boost::shared_ptr<shade::GLSLWrapper> state(new shade::osg::Wrapper(osg_state));

    boost::shared_ptr<shade::shaders::Surface> shader(new shade::shaders::Surface);
    boost::shared_ptr<shade::Program> program(new shade::Program(shader, state));

    boost::shared_ptr<Reflection> reflection(new Reflection());
    reflection->cube_map.set(cube_map);
    boost::shared_ptr<shade::shaders::ObjectSpace> object_space(new shade::shaders::ObjectSpace);
    reflection->coordinate_system = object_space;
    shader->material = reflection;

    program->compile();
    state->make_current();
    program->upload();
}

osg::ref_ptr<osg::Geode> build_sphere(double radius = 1., osg::Vec4 color = osg::Vec4(1., 1., 1., 1.))
{
    osg::ref_ptr<osg::Sphere> sphere(new osg::Sphere);
    sphere->setRadius(radius);

    osg::ref_ptr<osg::ShapeDrawable> shape_drawable(new ::osg::ShapeDrawable(sphere.get()));
    shape_drawable->setColor(color);

    osg::ref_ptr<osg::Geode> result(new osg::Geode);
    result->addDrawable(shape_drawable.get());

    return result;
}

osg::ref_ptr<osg::Geode> build_quad(void)
{
    osg::ref_ptr<osg::Geometry> geom(new osg::Geometry);

    osg::ref_ptr<osg::Vec3Array> v(new osg::Vec3Array);
    geom->setVertexArray(v.get());
    v->push_back(osg::Vec3(-4.f, 0.f, -4.f));
    v->push_back(osg::Vec3(4.f, 0.f, -4.f));
    v->push_back(osg::Vec3(4.f, 0.f, 4.f));
    v->push_back(osg::Vec3(-4.f, 0.f, 4.f));

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

osg::ref_ptr<osg::Group> build_satellites(void)
{
    osg::ref_ptr<osg::Group> root(new osg::Group);

    struct
    {
        osg::Vec4 color;
        osg::Vec3 transform;
    } sphere[] = {{osg::Vec4(1., 0., 0., 1.), osg::Vec3(2., 0., 0.)},
                  {osg::Vec4(0., 1., 0., 1.), osg::Vec3(0., 0., 2.)},
                  {osg::Vec4(0., 0., 1., 1.), osg::Vec3(-2., 0., 0.)},
                  {osg::Vec4(1., 1., 0., 1.), osg::Vec3(0., 0., -2.)}};

    for(int i = 0; i != 4; ++i)
    {
        osg::ref_ptr<osg::MatrixTransform> transform(new osg::MatrixTransform);
        transform->addChild(build_sphere(0.5, sphere[i].color));

        osg::Matrix matrix;
        matrix.makeTranslate(sphere[i].transform);
        transform->setMatrix(matrix);

        root->addChild(transform.get());
    }

    return root;
}

int main(int argc, char* argv[])
{
    osg::ref_ptr<osg::Group> root(new osg::Group);

    osg::ref_ptr<osg::Geode> sphere(build_sphere());
    root->addChild(sphere.get());

    osg::ref_ptr<osg::Group> satellites_geometry(build_satellites());
    osg::ref_ptr<osg::MatrixTransform> satellites(new osg::MatrixTransform);
    satellites->addChild(satellites_geometry.get());
    satellites->setUpdateCallback(new RotateCallback(osg::Vec3(0., 1., 0.), 0.001));
    root->addChild(satellites.get());

    osg::ref_ptr<osg::Geode> plane(build_quad());
    osg::ref_ptr<osg::MatrixTransform> plane_transform(new osg::MatrixTransform);
    plane_transform->addChild(plane.get());
    osg::Matrix plane_transform_matrix;
    plane_transform_matrix.makeTranslate(0., -2., 0.);
    plane_transform->setMatrix(plane_transform_matrix);
    root->addChild(plane_transform.get());

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

    osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D;
    int env_width = 1024;
    int env_height = 1024;
    tex->setTextureSize(env_width, env_height);
    tex->setInternalFormat(GL_RGBA);
    tex->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
    tex->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
    boost::shared_ptr<shade::osg::Texture> shade_tex(new shade::osg::Texture(tex.get()));

    osg::ref_ptr<osg::Camera> env_camera = new osg::Camera;
    env_camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    env_camera->setClearColor(osg::Vec4(0.5, 0.5, 0.5, 1.));
    env_camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    env_camera->setViewport(0, 0, env_width, env_height);
    env_camera->setRenderOrder(osg::Camera::PRE_RENDER);
    env_camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT, osg::Camera::FRAME_BUFFER);
    env_camera->attach(osg::Camera::COLOR_BUFFER, tex, 0, 0, true);
    env_camera->addChild(sphere);
    env_camera->addChild(satellites);
    osg::ref_ptr<osg::StateSet> env_camera_state(env_camera->getOrCreateStateSet());
    osg::FrontFace* cf = new osg::FrontFace(osg::FrontFace::CLOCKWISE);
    env_camera_state->setAttributeAndModes(cf);

    root->addChild(env_camera);

    setup_plane_shading(plane->getOrCreateStateSet(), shade_tex);

    osg::ref_ptr<osg::TextureCubeMap> osg_cube_tex = new osg::TextureCubeMap;
    int cube_map_size = 512;
    osg_cube_tex->setTextureSize(cube_map_size, cube_map_size);
    osg_cube_tex->setInternalFormat(GL_RGBA);
    struct
    {
        osg::Vec3 center;
        osg::Vec3 up;
    } cube_cameras[] = {
        {osg::Vec3(+1., 0., 0.), osg::Vec3(0., -1., 0.)},
        {osg::Vec3(-1., 0., 0.), osg::Vec3(0., -1., 0.)},
        {osg::Vec3(0., +1., 0.), osg::Vec3(0., 0., +1.)},
        {osg::Vec3(0., -1., 0.), osg::Vec3(0., 0., +1.)},
        {osg::Vec3(0., 0., +1.), osg::Vec3(0., -1., 0.)},
        {osg::Vec3(0., 0., -1.), osg::Vec3(0., -1., 0.)},
    };
    for(int i = 0; i != 6; ++i)
    {
        osg::ref_ptr<osg::Camera> cm_camera = new osg::Camera;
        cm_camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cm_camera->setClearColor(osg::Vec4(0.3, 0.3, 0.3, 1.));
        cm_camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
        cm_camera->setViewport(0, 0, cube_map_size, cube_map_size);
        cm_camera->setRenderOrder(osg::Camera::PRE_RENDER);
        cm_camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT, osg::Camera::FRAME_BUFFER);
        cm_camera->attach(osg::Camera::COLOR_BUFFER, osg_cube_tex, 0, i, true);

        cm_camera->setProjectionMatrixAsPerspective(90., 1., 0.1, 10.);
        cm_camera->setViewMatrixAsLookAt(osg::Vec3(0., 0., 0.), cube_cameras[i].center, cube_cameras[i].up);

        cm_camera->addChild(satellites);
        root->addChild(cm_camera);
    }
    boost::shared_ptr<shade::osg::Texture> cube_tex(new shade::osg::Texture(osg_cube_tex.get()));

    setup_sphere_shading(sphere->getOrCreateStateSet(), cube_tex);

    osg::ref_ptr<osgViewer::Viewer> viewer(build_viewer(root.get()));

    env_camera->setDataVariance(osg::Object::DYNAMIC);
    env_camera->setUpdateCallback(new MirrorCamera(viewer->getCamera(), plane_transform_matrix));

    viewer->run();
}
