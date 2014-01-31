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
#include <shade/shaders/UVPointLight.h>
#include <shade/shaders/Surface.h>
#include <shade/shaders/UVPlastic.h>
#include <shade/shaders/ObjectSpace.h>
#include <shade/shaders/UVCoord.h>
#include <shade/shaders/Texture2D.h>
#include <shade/GLSLInstance.h>
#include <boost/shared_ptr.hpp>
#include <GL/glew.h>
#include <cmath>
#include "Texture.h"
#include "example.h"


namespace shaders=shade::shaders;

namespace
{
  boost::shared_ptr<shaders::Surface> shader;
  boost::shared_ptr<shade::Program> program;
  boost::shared_ptr<shade::GLSLWrapper> state;
  boost::shared_ptr<shaders::UVPointLight> uvlight;

  float light_angle = 0.;
}

void setup_uv(void)
{
  boost::shared_ptr<shade::shaders::UVPlastic> specular(new shade::shaders::UVPlastic(0.7, 0.3));
  boost::shared_ptr<shade::shaders::ObjectSpace> coordinate_system(new shade::shaders::ObjectSpace);
  boost::shared_ptr<shade::shaders::Texture2D> tex(new shade::shaders::Texture2D);
  tex->texture_unit.set(example::make_texture("examples/pattern.dds"));
  boost::shared_ptr<shade::shaders::UVCoord> uvcoord(new shade::shaders::UVCoord);
  tex->uv = uvcoord;
  specular->color.set_value(shade::vec4<>(1., 0.4, 0.4, 1.));
  specular->uv_color = tex;
  specular->coordinate_system = coordinate_system;
  shader->material = specular;
  {
    shaders::IlluminatedMaterial::LightList::Accessor accessor(specular->lights);

    boost::shared_ptr<shaders::PointLight> light(new shaders::PointLight);
    light->position.set_value(shade::vec3<>(-5., -3, 4.));
    light->color.set_value(shade::vec3<>(1., 1., 1.));
    accessor->push_back(light);

    uvlight = boost::shared_ptr<shaders::UVPointLight>(new shaders::UVPointLight);
    uvlight->position.set(30., 15., 10.);
    uvlight->color.set(1., 1., 1.);
    accessor->push_back(uvlight);
  }
}

void init(void)
{
  shader = boost::shared_ptr<shaders::Surface> (new shaders::Surface);
  state = shade::create_GLSL_wrapper();
  program = boost::shared_ptr<shade::Program>(new shade::Program(shader, state));

  setup_uv();
}

void display(void)
{
  if (program->requires_compilation())
    program->compile();

  state->make_current();

  light_angle += 0.02;
  uvlight->position.set(10.*cos(light_angle), 15., 10.*sin(light_angle));

  if (program->requires_upload())
    program->upload();

  example::setup_camera();

  example::draw_default_scene();
}


int main(int argc, char* argv[])
{
  example::init(argc, argv, "SHADE Light-dependent Material");
  example::set_display_func(display);
  example::set_still_camera();

  init();

  example::run_main_loop();
  return 0;
}
