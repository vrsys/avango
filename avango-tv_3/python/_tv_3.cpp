// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* Avango is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* Avango is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with Avango. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
* Avango is a trademark owned by FhG.                                    *
*                                                                        *
\************************************************************************/
#include "TV_3Loader.hpp"
#include "TV_3SurfacePassDescription.hpp"
#include "TV_3VolumePassDescription.hpp"
#include "TV_3Node.hpp"
#include <avango/gua/tv_3/renderer/TV_3Loader.hpp>
#include <avango/gua/tv_3/renderer/TV_3SurfacePassDescription.hpp>
#include <avango/gua/tv_3/renderer/TV_3VolumePassDescription.hpp>
#include <avango/gua/tv_3/scenegraph/TV_3Node.hpp>
#include <avango/python/register_field.h>
#include <boost/python.hpp>

using namespace boost::python;
using namespace av::python;

namespace boost
 {
  namespace python
   {
    template <class T> struct pointee<av::Link<T> >
     {
      typedef T type;
     };
   }
 }

BOOST_PYTHON_MODULE(_tv_3)
{

  av::gua::tv_3::TV_3Loader::initClass();
  av::gua::tv_3::TV_3SurfacePassDescription::initClass();
  av::gua::tv_3::TV_3VolumePassDescription::initClass();
  av::gua::tv_3::TV_3Node::initClass();
  init_TV_3Node();
  init_TV_3SurfacePassDescription();
  init_TV_3VolumePassDescription();
  init_TV_3Loader();
  
}
