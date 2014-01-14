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

#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include <sys/time.h>
#include <time.h>
#include <shade/Shader.h>
#include <shade/Program.h>
#include <shade/shaders/PointLight.h>
#include <shade/shaders/Surface.h>
#include <shade/shaders/Plastic.h>
#include <shade/shaders/ObjectSpace.h>
#include <shade/GLSLInstance.h>
#include <boost/shared_ptr.hpp>

namespace shaders=shade::shaders;

double angle = 0.;
double rot_speed = 0.25;

namespace
{
  boost::shared_ptr<shaders::Surface> shader;
  boost::shared_ptr<shade::Program> program;
  boost::shared_ptr<shade::GLSLWrapper> state;
}

double get_time(const struct timeval& tv)
{
  return static_cast<double>(tv.tv_sec) + static_cast<double>(tv.tv_usec)/1000000.;
}

void init(void)
{
  glClearColor(0.4, 0.4, 0.4, 0.);
  glEnable(GL_DEPTH_TEST);

  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    std::cerr << "GLEW initialization failed!" << std::endl;
    ::exit(1);
  };

  shader = boost::shared_ptr<shaders::Surface> (new shaders::Surface);
  state = shade::create_GLSL_wrapper();
  program = boost::shared_ptr<shade::Program>(new shade::Program(shader, state));

  boost::shared_ptr<shade::shaders::Plastic> specular(new shade::shaders::Plastic(0.4, .6));
  boost::shared_ptr<shade::shaders::ObjectSpace> object_space(new shade::shaders::ObjectSpace);
  specular->color.set_value(shade::vec4<>(1., 0.4, 0.4, 1.));
  specular->coordinate_system = object_space;
  shader->material = specular;
  {
    shaders::IlluminatedMaterial::LightList::Accessor accessor(specular->lights);

    boost::shared_ptr<shaders::PointLight> light(new shaders::PointLight);
    light->position.set(30., 15., 10.);
    light->color.set(1., 1., 1.);
    accessor->push_back(light);

    boost::shared_ptr<shaders::PointLight> light2(new shaders::PointLight);
    light2->position.set(-15., -3, 0.);
    light2->color.set(1., 1., 1.);
    accessor->push_back(light2);
  }


  struct timeval tv0, tv1;
  gettimeofday(&tv0, 0);

  if (program->requires_compilation())
    program->compile();

  gettimeofday(&tv1, 0);
  double timediff = get_time(tv1) - get_time(tv0);
  std::cout << timediff << std::endl;

  //GLint surface_color = glGetUniformLocation(program, "obj_0x7d2690_color_");
  //GLint surface_diffuse = glGetUniformLocation(program, "obj_0x7d2690_diffuse_");
  //GLint surface_specular = glGetUniformLocation(program, "obj_0x7d2690_specular_");
  //GLint light_color = glGetUniformLocation(program, "obj_0x7d34f0_color");
  //GLint light_position = glGetUniformLocation(program, "obj_0x7d34f0_position");

  //glUniform3f(surface_color, 1., 1., 1.);
  //glUniform1f(surface_diffuse, 0.4);
  //glUniform1f(surface_specular, 0.6);
  //glUniform3f(light_color, 1., 1., 1.);
  //glUniform3f(light_position, 10., 0., 0.);
}

void idle(void)
{
  angle += rot_speed;
  glutPostRedisplay();
}

void reshape(int width, int height)
{
  glViewport( 0, 0, width, height );
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45., static_cast<GLfloat>(width)/static_cast<GLfloat>(height), 0.01, 100.);
  glutPostRedisplay();
}

void display(void)
{
  // Setup camera
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0., 0., -3);
  glRotatef(angle, 0., 1., 0.);

  if (program->requires_compilation())
    program->compile();

  state->make_current();

  if (program->requires_upload())
    program->upload();

  // Clear and draw model
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glutSolidTeapot(1.);

  glFlush();
  glutSwapBuffers();
}

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(600, 400);
  glutCreateWindow("SHADE Example");

  init();

  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutReshapeFunc(reshape);

  glutMainLoop();
  return 0;
}
