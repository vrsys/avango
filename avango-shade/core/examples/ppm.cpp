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

#include "ppm.h"
#include <GL/gl.h>
#include <fstream>

namespace
{
  void save_ppm_data(const std::string& name, GLint width, GLint height, GLubyte data[])
  {
    std::ofstream hout(name.c_str());
    hout << "P6 " << width << " " << height << " 255 ";
    const char* raw_data = reinterpret_cast<char*>(data);
    for (GLint i = 0; i != height; ++i)
      hout.write(&raw_data[width*(height-i-1)*3], width*3);
  }
}

void save_ppm(const std::string& name)
{
  // Initialize OpenGL context
  GLint width = 640;
  GLint height = 480;

  // Allocte enought space for RGB image
  GLubyte data[width*height*3];

  // Copy image to buffer
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, &data);

  // Store ppm image
  save_ppm_data(name, width, height, data);
}

void save_texture_ppm(const std::string& name)
{
  // Initialize OpenGL context
  GLint width = 0;
  GLint height = 0;
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

  // Allocte enought space for RGB image
  GLubyte data[width*height*3];

  // Copy image to buffer
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

  // Store ppm image
  save_ppm_data(name, width, height, data);
}
