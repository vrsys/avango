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
#include <avango/gua/network/NetTransform.h>
#include <avango/gua/renderer/Material.hpp>
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

        void distributeFieldContainerHelper(av::gua::NetTransform& self, object& obj)
        {
          av::Base* av_value = boost::python::extract<av::Base*>(obj);

          {
            av::Link<av::gua::Node> tmp = dynamic_cast<av::gua::Node*>(av_value);
            if (tmp.isValid()) {
              tmp->on_distribute(self);
              self.distributeFieldContainer(tmp);
              return;
            }
          }

          {
            av::Link<av::gua::Material> tmp = dynamic_cast<av::gua::Material*>(av_value);
            if (tmp.isValid()) {
              tmp->on_distribute(self);
              self.distributeFieldContainer(tmp);
              return;
            }
          }

        }

        void undistributeFieldContainerHelper(av::gua::NetTransform& self, object& obj)
        {
          av::Base* av_value = boost::python::extract<av::Base*>(obj);
          
          {
            av::Link<av::gua::Node> tmp = dynamic_cast<av::gua::Node*>(av_value);
            if (tmp.isValid()) {
              tmp->on_undistribute(self);
              self.undistributeFieldContainer(tmp);
              return;
            }
          }

          {
            av::Link<av::gua::Material> tmp = dynamic_cast<av::gua::Material*>(av_value);
            if (tmp.isValid()) {
              tmp->on_undistribute(self);
              self.undistributeFieldContainer(tmp);
              return;
            }
          }
        }

      } // namespace detail
    } // namespace python
 } // namespace av


void init_NetTransform(void)
{
  class_<av::gua::NetTransform, av::Link<av::gua::NetTransform>, bases<av::gua::TransformNode>, boost::noncopyable >("NetTransform", "docstring", no_init)
    //.def("distribute_object", &av::gua::NetTransform::distributeFieldContainer)
    .def("distribute_object", av::python::detail::distributeFieldContainerHelper)
    //.def("undistribute_object", &av::gua::NetTransform::undistributeFieldContainer)
    .def("undistribute_object", av::python::detail::undistributeFieldContainerHelper)
    ;
  def("set_ensemble_option", av::NetNode::setEnsOption);
}
