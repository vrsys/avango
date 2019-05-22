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
#include <shade/shaders/Surface.h>
#include <shade/shaders/Constant.h>
#include <shade/shaders/UVCoord.h>
#include <shade/shaders/Texture2D.h>
#include <shade/osg/Wrapper.h>
#include <shade/osg/Texture.h>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Vec3>
#include <osg/Array>
#include <osg/PrimitiveSet>
#include <osg/Texture2D>
#include <osg/Image>
#include <osgDB/ReadFile>
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

boost::shared_ptr<shade::Program> setup_shading(boost::shared_ptr<shade::GLSLWrapper> state, boost::shared_ptr<shade::Texture> texture)
{
    boost::shared_ptr<shade::shaders::Surface> shader(new shade::shaders::Surface);
    boost::shared_ptr<shade::Program> program(new shade::Program(shader, state));

    boost::shared_ptr<shade::shaders::Constant> constant(new shade::shaders::Constant);
    constant->color.set_value(shade::vec4<>(1., 0.4, 0.4, 1.));
    shader->material = constant;

    boost::shared_ptr<shade::shaders::Texture2D> tex(new shade::shaders::Texture2D);
    tex->texture_unit.set(texture);
    boost::shared_ptr<shade::shaders::UVCoord> uvcoord(new shade::shaders::UVCoord);
    tex->uv = uvcoord;
    constant->color = tex;

    return program;
}

osg::ref_ptr<osg::Geode> build_quad(void)
{
    osg::ref_ptr<osg::Geometry> geom(new osg::Geometry);

    osg::ref_ptr<osg::Vec3Array> v(new osg::Vec3Array);
    geom->setVertexArray(v.get());
    v->push_back(osg::Vec3(-1.f, -1.f, 0.f));
    v->push_back(osg::Vec3(1.f, -1.f, 0.f));
    v->push_back(osg::Vec3(1.f, 1.f, 0.f));
    v->push_back(osg::Vec3(-1.f, 1.f, 0.f));

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
    osg::ref_ptr<osg::Geode> quad(build_quad());

    osg::ref_ptr<osg::Image> image = osgDB::readImageFile("examples/pattern.dds");
    osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D;
    tex->setImage(image.get());
    boost::shared_ptr<shade::osg::Texture> shade_tex(new shade::osg::Texture(tex.get()));

    boost::shared_ptr<shade::GLSLWrapper> state(new shade::osg::Wrapper(quad->getOrCreateStateSet()));

    boost::shared_ptr<shade::Program> shader_program(setup_shading(state, shade_tex));
    shader_program->compile();
    state->make_current();
    shader_program->upload();

    run_viewer(quad.get());
}
