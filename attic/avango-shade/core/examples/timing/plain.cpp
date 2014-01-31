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

double angle = 0.;
double rot_speed = 0.25;

class ShaderCompiler
{
public:

  ShaderCompiler(GLenum type, const std::string& fname)
  {
    shader_ = glCreateShader(type);

    std::ifstream file(fname.c_str());
    std::ostringstream contents;
    contents << file.rdbuf();
    source_ = contents.str();
  }

  void compile(void)
  {
    const char* source_str = source_.c_str();
    glShaderSource(shader_, 1, &source_str, 0);

    glCompileShader(shader_);
  }

  GLuint get_shader(void)
  {
    return shader_;
  }

private:

  GLuint shader_;
  std::string source_;
};

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

  ShaderCompiler vertex_shader(GL_VERTEX_SHADER, "vertex.glsl");
  ShaderCompiler fragment_shader(GL_FRAGMENT_SHADER, "fragment.glsl");
  ShaderCompiler geometry_shader(GL_GEOMETRY_SHADER_EXT, "geomerety.glsl");

  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader.get_shader());
  glAttachShader(program, fragment_shader.get_shader());
  glAttachShader(program, geometry_shader.get_shader());

  vertex_shader.compile();
  fragment_shader.compile();
  geometry_shader.compile();

  struct timeval tv0, tv1;
  gettimeofday(&tv0, 0);

  vertex_shader.compile();
  fragment_shader.compile();
  geometry_shader.compile();
  glLinkProgram(program);

  gettimeofday(&tv1, 0);
  double timediff = get_time(tv1) - get_time(tv0);
  std::cout << timediff << std::endl;

  glUseProgram(program);

  GLint surface_color = glGetUniformLocation(program, "obj_0x7d2690_color_");
  GLint surface_diffuse = glGetUniformLocation(program, "obj_0x7d2690_diffuse_");
  GLint surface_specular = glGetUniformLocation(program, "obj_0x7d2690_specular_");
  GLint light_color = glGetUniformLocation(program, "obj_0x7d34f0_color");
  GLint light_position = glGetUniformLocation(program, "obj_0x7d34f0_position");

  glUniform3f(surface_color, 1., 1., 1.);
  glUniform1f(surface_diffuse, 0.4);
  glUniform1f(surface_specular, 0.6);
  glUniform3f(light_color, 1., 1., 1.);
  glUniform3f(light_position, 10., 0., 0.);
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
