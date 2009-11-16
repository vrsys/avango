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
#include <avango/Type.h>
#include <avango/Field.h>
#include <avango/StandardFields.h>
#include <avango/python/register_field.h>
#include <vector>
#include <string>
#include <boost/bind.hpp>

#include "Field.h"

using namespace boost::python;
using namespace av::python;

namespace
{
  std::string get_type_name(av::Field* self)
  {
    return self->getTypeId().getName();
  }

  void connect_from_dependant(av::Field* self, av::Field* other)
  {
    self->connectFrom(other, true);
  }

  void connect_weak_from(av::Field* self, av::Field* other)
  {
    self->connectFrom(other, false);
  }

  list
  FieldGetConnectedFields(av::Field* self)
  {
    list py_connected_fields;
    for (unsigned int field = 0u; field < self->getNumberOfConnectedFields(); ++field)
      py_connected_fields.append(boost::ref(*self->getConnectedField(field)));
    return py_connected_fields;
  }


}

void init_Field(void)
{
  void (av::Field::*disconnect_void)(void) = &av::Field::disconnect;
  void (av::Field::*disconnect_from)(av::Field*) = &av::Field::disconnectFrom;
  void (av::Field::*disconnect_auditors)(void) = &av::Field::disconnectAuditors;

  class_<av::Field, boost::noncopyable>("Field", "docstring", no_init)
    .def("_get_index", &av::Field::getIndex)
    .def("_get_name", &av::Field::getName, return_value_policy<copy_const_reference>())
    .def("_get_type", get_type_name)
    .def("get_container", &av::Field::getContainer, return_internal_reference<>())
    .def("connect_from", &av::Field::connectFrom)
    .def("connect_from", connect_from_dependant)
    .def("connect_weak_from", connect_weak_from)
    .def("disconnect", disconnect_void)
    .def("disconnect_from", disconnect_from)
    .def("disconnect_auditors", disconnect_auditors)
    .def("get_connected_fields", FieldGetConnectedFields)
    .def("enable_dependency", &av::Field::enableDependency)
    .def("dependency_enabled", &av::Field::dependencyEnabled)
    .def("enable_notify", &av::Field::enableNotify)
    .def("notify_enabled", &av::Field::notifyEnabled)
    .def("touch", &av::Field::touch)
    .def("get_number_of_connected_fields", &av::Field::getNumberOfConnectedFields)
    .def("read", &av::Field::read)
    .def("write", &av::Field::write)
    ;

  register_field<av::SFBool>("SFBool");
  register_field<av::SFDouble>("SFDouble");
  register_field<av::SFFloat>("SFFloat");
  register_field<av::SFInt>("SFInt");
  register_field<av::SFLong>("SFLong");
  register_field<av::SFUInt>("SFUInt");
  register_field<av::SFULong>("SFULong");
  register_field<av::SFString>("SFString");

  register_multifield<av::MFBool>("MFBool");
  register_multifield<av::MFDouble>("MFDouble");
  register_multifield<av::MFFloat>("MFFloat");
  register_multifield<av::MFInt>("MFInt");
  register_multifield<av::MFLong>("MFLong");
  register_multifield<av::MFUInt>("MFUInt");
  register_multifield<av::MFULong>("MFULong");
  register_multifield<av::MFString>("MFString");

  register_field<av::SFContainer>("SFContainer");
  register_multifield<av::MFContainer>("MFContainer");
}
