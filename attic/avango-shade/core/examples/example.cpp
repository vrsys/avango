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

#include "example.h"
#include "render-teapot.h"
#include "ppm.h"
#include "Texture.h"
#include <shade/GLSLInstance.h>
#include <shade/GLSLTexture.h>
#include <boost/lexical_cast.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <cstdlib>

using namespace example;

namespace
{
boost::function<void(void)> display_func;

double distance = 2.2;
double angle = 0.;
double rot_speed = 0.25;

int frame_count = 0;
bool turntable = false;
bool manual_control = false;
int last_mouse_x;

void idle(void)
{
    if(manual_control)
        return;

    angle += rot_speed;
    glutPostRedisplay();
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45., static_cast<GLfloat>(width) / static_cast<GLfloat>(height), 0.01, 100.);
    glutPostRedisplay();
}

void display(void)
{
    if(display_func)
        display_func();

    if(turntable)
    {
        glFinish();
        glReadBuffer(GL_FRONT);
        save_ppm(std::string("img_") + boost::lexical_cast<std::string>(frame_count++) + ".ppm");
        if(frame_count == 479)
            std::exit(0);
    }

    glutSwapBuffers();
}

void init_gl(void)
{
    GLenum err = glewInit();
    if(GLEW_OK != err)
    {
        std::cerr << "GLEW initialization failed!" << std::endl;
        ::exit(1);
    };

    glClearColor(0.4, 0.4, 0.4, 0.);
    glEnable(GL_DEPTH_TEST);
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
    case ' ':
        manual_control = !manual_control;
        break;
    case 27:
        std::exit(0);
        break;
    }
}

void mouse(int button, int state, int x, int y) { last_mouse_x = x; }

void motion(int x, int y)
{
    if(!manual_control)
        return;

    angle += 0.1 * static_cast<double>(x - last_mouse_x);
    glutPostRedisplay();

    last_mouse_x = x;
}

} // namespace

void example::setup_camera(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0., -0.1, -distance);
    glRotatef(angle, 0., 1., 0.);
}

void example::draw_default_scene(bool clear)
{
    if(clear)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render_teapot();
}

void example::set_display_func(boost::function<void(void)> func) { display_func = func; }

void example::set_distance(double dist) { distance = dist; }

void example::init(int argc, char* argv[], std::string name, int width, int height)
{
    glutInit(&argc, argv);

    if((argc > 1) && (argv[1] == std::string("turntable")))
    {
        turntable = true;
        rot_speed = 0.75;
    }
    else if((argc > 1) && (argv[1] == std::string("still")))
    {
        rot_speed = 0.;
    }

    unsigned int mode = turntable ? GLUT_SINGLE : GLUT_DOUBLE;
    glutInitDisplayMode(mode | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow(name.c_str());
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    init_gl();
    reshape(width, height);
}

void example::run_main_loop(void) { glutMainLoop(); }

void example::set_still_camera(void) { rot_speed = 0.; }

boost::shared_ptr<shade::GLSLTexture> example::make_texture(const std::string& fname)
{
    boost::shared_ptr<shade::GLSLTexture> result(new shade::GLSLTexture(GL_TEXTURE_2D));
    result->bind();
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Texture texture(fname.c_str());
    texture.upload();

    return result;
}
