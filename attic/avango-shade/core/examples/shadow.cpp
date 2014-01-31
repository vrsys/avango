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
#include <shade/shaders/ShadowPointLight.h>
#include <shade/shaders/Surface.h>
#include <shade/shaders/Plastic.h>
#include <shade/shaders/ObjectSpace.h>
#include <shade/GLSLInstance.h>
#include <boost/shared_ptr.hpp>
#include <GL/glew.h>
#include "Texture.h"
#include "example.h"


namespace shaders=shade::shaders;

namespace
{
  boost::shared_ptr<shaders::Surface> shader;
  boost::shared_ptr<shade::Program> program;
  boost::shared_ptr<shade::GLSLWrapper> state;
  boost::shared_ptr<shaders::ShadowPointLight> shadow_light;
  boost::shared_ptr<shade::GLSLTexture> shadow_map;

  GLuint shadow_framebuffer;

  const GLuint shadow_map_size = 1024;
}

void setup_shadow(boost::shared_ptr<shade::GLSLTexture> shadow_map)
{
  boost::shared_ptr<shade::shaders::Plastic> specular(new shade::shaders::Plastic(1., 0.));
  boost::shared_ptr<shade::shaders::ObjectSpace> object_space(new shade::shaders::ObjectSpace);
  specular->color.set_value(shade::vec4<>(1., 0.4, 0.4, 1.));
  specular->coordinate_system = object_space;
  shader->material = specular;
  {
    shaders::IlluminatedMaterial::LightList::Accessor accessor(specular->lights);

    shadow_light = boost::shared_ptr<shaders::ShadowPointLight>(new shaders::ShadowPointLight);
    shadow_light->color.set_value(shade::vec3<>(1., 1., 1.));
    shadow_light->shadow_map.set(shadow_map);
    accessor->push_back(shadow_light);
  }
}

void init(void)
{
  shadow_map = boost::shared_ptr<shade::GLSLTexture>(new shade::GLSLTexture(GL_TEXTURE_2D));
  shadow_map->bind();
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadow_map_size, shadow_map_size, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_ALPHA);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

  glGenFramebuffersEXT(1, &shadow_framebuffer);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, shadow_framebuffer);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, shadow_map->get_name(), 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  shader = boost::shared_ptr<shaders::Surface> (new shaders::Surface);
  state = shade::create_GLSL_wrapper();
  program = boost::shared_ptr<shade::Program>(new shade::Program(shader, state));

  setup_shadow(shadow_map);
}

void display(void)
{
  const float shadow_pos[4] = { 2, 2., 6., 1.};
  static bool once = false;

  if (!once)
  {
    once = true;

    // Draw shadow

    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, shadow_framebuffer);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluPerspective(30., 1., 2., 10.);
    glRotatef(+15., 1., 0., 0.);
    glRotatef(-15., 0., 1., 0.);
    glTranslatef(-shadow_pos[0], -shadow_pos[1], -shadow_pos[2]);
    float matrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    shadow_light->modelview.set(matrix);

    glViewport(0, 0, shadow_map_size, shadow_map_size);
    glClear(GL_DEPTH_BUFFER_BIT);
    glColor3f(1., 0., 0.);
    // Push data quite a bit back to avoid self shadowing artifacts
    glPolygonOffset(5., 5000.);
    glEnable(GL_POLYGON_OFFSET_FILL);
    example::draw_default_scene();
    glDisable(GL_POLYGON_OFFSET_FILL);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    shadow_map->bind();
  }

  // Draw normal scene
  if (program->requires_compilation())
    program->compile();

  state->make_current();

  example::setup_camera();

  // Light position is given in camera space
  float camera_matrix[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, camera_matrix);
  float trans_shadow_pos[3];
  for (int i=0; i != 3; ++i)
  {
    trans_shadow_pos[i] = 0.;
    for (int j=0; j != 4; ++j)
    {
      trans_shadow_pos[i] += camera_matrix[4*j+i] * shadow_pos[j];
    }
  }
  shadow_light->position.set_value(shade::vec3<>(trans_shadow_pos[0], trans_shadow_pos[1], trans_shadow_pos[2]));

  if (program->requires_upload())
    program->upload();

  // Draw teapot ...
  example::draw_default_scene();

  // ... and a floor
  glBegin(GL_TRIANGLE_FAN);
  glColor3f(0.3, 0.3, 0.3);
  glNormal3f(0., 1., 0.);
  glVertex3f( 40., -0.28, -40.);
  glVertex3f( 40., -0.28,  40.);
  glVertex3f(-40., -0.28,  40.);
  glVertex3f(-40., -0.28, -40.);
  glEnd();
}


int main(int argc, char* argv[])
{
  example::init(argc, argv, "SHADE Shadow Light Source");
  example::set_display_func(display);

  init();

  example::run_main_loop();
  return 0;
}
