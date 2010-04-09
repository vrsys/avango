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

BOOST_PYTHON_MODULE(_utils)
{
  av::utils::Init::initClass();

  class_<av::utils::Bool2Or, av::Link<av::utils::Bool2Or>, bases<av::FieldContainer>, boost::noncopyable >("Bool2Or", "docstring", no_init);
  class_<av::utils::Bool2And, av::Link<av::utils::Bool2And>, bases<av::FieldContainer>, boost::noncopyable >("Bool2And", "docstring", no_init);
  class_<av::utils::Bool3Or, av::Link<av::utils::Bool3Or>, bases<av::FieldContainer>, boost::noncopyable >("Bool3Or", "docstring", no_init);
  class_<av::utils::Bool3And, av::Link<av::utils::Bool3And>, bases<av::FieldContainer>, boost::noncopyable >("Bool3And", "docstring", no_init);
}
