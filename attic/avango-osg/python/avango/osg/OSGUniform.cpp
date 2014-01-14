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

#include <boost/python.hpp>
#include <boost/python/enum.hpp>
#include <osg/Uniform>
#include <avango/python/register_field.h>
#include <avango/osg/Uniform.h>
#include "OSGUniform.h"

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

void init_OSGUniform(void)
 {
  // wrapping osg::Uniform functionality
  register_field<av::osg::SFUniform>("SFUniform");
  register_multifield<av::osg::MFUniform>("MFUniform");
  class_<av::osg::Uniform, av::Link<av::osg::Uniform>, bases<av::osg::Object>, boost::noncopyable >("Uniform", "docstring", no_init);

  enum_<osg::Uniform::Type>("uniformtype")
    .value("FLOAT",::osg::Uniform::FLOAT)
    .value("FLOAT_VEC2",::osg::Uniform::FLOAT_VEC2)
    .value("FLOAT_VEC3",::osg::Uniform::FLOAT_VEC3)
    .value("FLOAT_VEC4",::osg::Uniform::FLOAT_VEC4)
    .value("INT",::osg::Uniform::INT)
    .value("INT_VEC2",::osg::Uniform::INT_VEC2)
    .value("INT_VEC3",::osg::Uniform::INT_VEC3)
    .value("INT_VEC4",::osg::Uniform::INT_VEC4)
    .value("SAMPLER_2D",::osg::Uniform::SAMPLER_2D)
    .value("FLOAT_MAT4",::osg::Uniform::FLOAT_MAT4)
    .export_values()
    ;
 }
