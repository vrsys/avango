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
#include <avango/gua/network/NetMatrixTransform.h>
#include <avango/NetNode.h>
#include <avango/FieldContainer.h>
#include <avango/Base.h>
#include <avango/Link.h>

using namespace boost::python;

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

namespace av
 {
  namespace python
    {
     namespace detail
      {

        void distributeFieldContainerHelper(av::gua::NetMatrixTransform& self, boost::python::object obj)
        {
          av::Base* av_value = boost::python::extract<av::Base*>(obj);

          av::Link<av::FieldContainer> tmp = dynamic_cast<FieldContainer*>(av_value);
          self.distributeFieldContainer(tmp);
        }

        void undistributeFieldContainerHelper(av::gua::NetMatrixTransform& self, boost::python::object obj)
        {
          av::Base* av_value = boost::python::extract<av::Base*>(obj);

          av::Link<av::FieldContainer> tmp = dynamic_cast<FieldContainer*>(av_value);
          self.undistributeFieldContainer(tmp);
        }

      } // namespace detail
    } // namespace python
 } // namespace av


void init_NetMatrixTransform(void)
{
  class_<av::gua::NetMatrixTransform, av::Link<av::gua::NetMatrixTransform>, bases<av::gua::TransformNode>, boost::noncopyable >("NetMatrixTransform", "docstring", no_init)
    //.def("distribute_object", &av::gua::NetMatrixTransform::distributeFieldContainer)
    .def("distribute_object", av::python::detail::distributeFieldContainerHelper)
    //.def("undistribute_object", &av::gua::NetMatrixTransform::undistributeFieldContainer)
    .def("undistribute_object", av::python::detail::undistributeFieldContainerHelper)
    ;
  def("set_ensemble_option", av::NetNode::setEnsOption);
}
