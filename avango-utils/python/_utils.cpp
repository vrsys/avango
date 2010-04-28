// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
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

#include <boost/python.hpp>
#include <avango/Link.h>

#include <osg/Matrix>

#include "../include/avango/utils/Init.h"
#include "../include/avango/utils/Bool2Or.h"
#include "../include/avango/utils/Bool2And.h"
#include "../include/avango/utils/Bool3Or.h"
#include "../include/avango/utils/Bool3And.h"

using namespace boost::python;

// TODO can we move this somewhere else?
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

osg::Vec3 CalcHpr(const osg::Matrix& mat)
{
  osg::Vec3 hpr;
  if(mat(1,0)>0.998) {
    float heading = atan2(mat(0,2),mat(2,2));
    float attitude = 3.14/2.0;
    float bank =0;
    hpr = osg::Vec3(heading,attitude,bank);
    return hpr;
  }

  if(mat(1,0)<-0.998) {
    float heading = atan2(mat(0,2),mat(2,2));
    float attitude = -3.14/2.0;
    float bank =0;
    hpr = osg::Vec3(heading,attitude,bank);
    return hpr;
  }

  float heading = atan2(-mat(2,0),mat(0,0));
  float attitude = atan2(-mat(1,2),mat(1,1));
  float bank = sin(mat(1,0));
  hpr = osg::Vec3(heading,attitude,bank);
  return hpr;
}

BOOST_PYTHON_MODULE(_utils)
{
  av::utils::Init::initClass();

  class_<av::utils::Bool2Or, av::Link<av::utils::Bool2Or>, bases<av::FieldContainer>, boost::noncopyable >("Bool2Or", "docstring", no_init);
  class_<av::utils::Bool2And, av::Link<av::utils::Bool2And>, bases<av::FieldContainer>, boost::noncopyable >("Bool2And", "docstring", no_init);
  class_<av::utils::Bool3Or, av::Link<av::utils::Bool3Or>, bases<av::FieldContainer>, boost::noncopyable >("Bool3Or", "docstring", no_init);
  class_<av::utils::Bool3And, av::Link<av::utils::Bool3And>, bases<av::FieldContainer>, boost::noncopyable >("Bool3And", "docstring", no_init);

  def("calc_hpr", CalcHpr);
}
