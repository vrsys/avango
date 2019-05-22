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
#include <shade/shaders/PointLight.h>
#include <shade/shaders/Surface.h>
#include <shade/shaders/Plastic.h>
#include <shade/GLSLInstance.h>
#include <boost/shared_ptr.hpp>
#include <GL/glew.h>
#include "RaytracedSphereFrame.h"
#include "example.h"

namespace shaders = shade::shaders;

namespace
{
boost::shared_ptr<shaders::Surface> shader;
boost::shared_ptr<shade::Program> program;
boost::shared_ptr<shade::GLSLWrapper> state;
} // namespace

void setup_raytraced_sphere(void)
{
    boost::shared_ptr<shade::shaders::Plastic> specular(new shade::shaders::Plastic(0.4, .6));
    boost::shared_ptr<RaytracedSphereFrame> sphere_space(new RaytracedSphereFrame);
    specular->color.set_value(shade::vec4<>(1., 0.4, 0.4, 1.));
    specular->coordinate_system = sphere_space;
    shader->material = specular;
    {
        shaders::IlluminatedMaterial::LightList::Accessor accessor(specular->lights);

        boost::shared_ptr<shaders::PointLight> light(new shaders::PointLight);
        light->position.set_value(shade::vec3<>(30., 15., 10.));
        light->color.set_value(shade::vec3<>(1., 1., 1.));
        accessor->push_back(light);

        boost::shared_ptr<shaders::PointLight> light2(new shaders::PointLight);
        light2->position.set_value(shade::vec3<>(-15., -3, 0.));
        light2->color.set_value(shade::vec3<>(1., 1., 1.));
        accessor->push_back(light2);
    }
}

void init(void)
{
    shader = boost::shared_ptr<shaders::Surface>(new shaders::Surface);
    state = shade::create_GLSL_wrapper();
    program = boost::shared_ptr<shade::Program>(new shade::Program(shader, state));

    setup_raytraced_sphere();

    example::set_distance(4.);

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
}

void display(void)
{
    if(program->requires_compilation())
        program->compile();

    state->make_current();

    if(program->requires_upload())
        program->upload();

    example::setup_camera();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_QUADS);
    /* Front */
    glNormal3f(0., 0., +1.);
    glVertex3f(-1, -1, +1);
    glVertex3f(+1, -1, +1);
    glVertex3f(+1, +1, +1);
    glVertex3f(-1, +1, +1);
    /* Right */
    glNormal3f(+1., 0., 0.);
    glVertex3f(+1., -1, -1);
    glVertex3f(+1., +1, -1);
    glVertex3f(+1., +1, +1);
    glVertex3f(+1., -1, +1);
    /* Back */
    glNormal3f(0., 0., -1.);
    glVertex3f(-1, -1, -1);
    glVertex3f(-1, +1, -1);
    glVertex3f(+1, +1, -1);
    glVertex3f(+1, -1, -1);
    /* Left */
    glNormal3f(-1., 0., 0.);
    glVertex3f(-1., -1, -1);
    glVertex3f(-1., -1, +1);
    glVertex3f(-1., +1, +1);
    glVertex3f(-1., +1, -1);
    glEnd();
}

int main(int argc, char* argv[])
{
    example::init(argc, argv, "SHADE Raytraced Sphere");
    example::set_display_func(display);

    init();

    example::run_main_loop();
    return 0;
}
