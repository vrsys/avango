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
#include <avango/FieldContainer.h>
#include <avango/FieldContainerHelper.h>
#include <avango/ContainerPool.h>

#include "FieldContainer.h"

using namespace boost::python;

av::Link<av::FieldContainer> make_instance_by_name(std::string name)
{
  av::Link<av::FieldContainer> retval(dynamic_cast<av::FieldContainer*>(av::Type::createInstanceOfType(name)));
  return retval;
}

av::Field* make_field_by_name(std::string name)
{
  return dynamic_cast<av::Field*>(av::Type::createInstanceOfType(name));
}

av::Link<av::FieldContainer> ContainerPoll_get_instance_by_name(std::string name)
{
  return av::Link<av::FieldContainer>(av::ContainerPool::getInstanceByName(name));
}

bool Base_equal(const av::Base* a, const av::Base* b)
{
  return a == b;
}

bool Base_unequal(const av::Base* a, const av::Base* b)
{
  return a != b;
}

void FieldContainer_touch(av::FieldContainer* self)
{
  self->touch();
}

void init_FieldContainer(void)
{

  av::Field* (av::FieldContainer::*getField_uint)(unsigned int) = &av::FieldContainer::getField;
  av::Field* (av::FieldContainer::*getField_string)(const std::string&) = &av::FieldContainer::getField;
  void (av::FieldContainer::*addField)(av::Field*, const std::string&) = &av::FieldContainer::addDynamicField;

  class_<av::Base, av::Link<av::Base>, boost::noncopyable >("Base", "docstring", no_init)
    .def("__eq__", Base_equal)
    .def("__ne__", Base_unequal)
    ;

  class_<av::Distributed, av::Link<av::Distributed>, bases<av::Base>, boost::noncopyable >("Distributed", "docstring", no_init);

  class_<av::FieldContainer, av::Link<av::FieldContainer>, bases<av::Distributed>, boost::noncopyable >("FieldContainer", "docstring", no_init)
    .def("_get_num_fields", &av::FieldContainer::getNumFields)
    .def("_get_field_name", &av::FieldContainer::getFieldName, return_value_policy<copy_const_reference>())
    .def("_get_field", getField_uint, return_internal_reference<>())
    .def("_get_field", getField_string, return_internal_reference<>())
    .def("add_field", addField)
    .def("touch", FieldContainer_touch)
    .def("always_evaluate", &av::FieldContainer::alwaysEvaluate)
    .def("allow_scheduling", &av::FieldContainer::allowScheduling)
    .def("disconnect_all_fields", &av::FieldContainer::disconnectAllFields)
    .def("evaluate_dependency", &av::FieldContainer::evaluateDependency)
    .def("read", &av::FieldContainer::read)
    .def("write", &av::FieldContainer::write)
    ;

  def("_make_instance_by_name", make_instance_by_name);
  def("_make_field_by_name", make_field_by_name, return_internal_reference<>());
  def("get_instance_by_name", ContainerPoll_get_instance_by_name);
  def("disconnect_and_clear_all_fields", av::disconnectAndClearAllFields);
}
