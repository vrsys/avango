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

#ifndef example_H
#define example_H example_H

#include <shade/GLSLTexture.h>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

namespace example
{
void init(int argc, char* argv[], std::string name, int width = 640, int height = 480);
void setup_camera(void);
void draw_default_scene(bool clear = true);
void set_display_func(boost::function<void(void)> func);
void set_distance(double dist);
void set_still_camera(void);
void run_main_loop(void);

boost::shared_ptr<shade::GLSLTexture> make_texture(const std::string& fname);
} // namespace example

#endif
