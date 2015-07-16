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
#include <avango/FieldContainer.h>
#include <avango/FieldContainerHelper.h>
#include <avango/ContainerPool.h>
#include <avango/python/script/Script.h>

#include "FieldContainer.h"

using namespace boost::python;

std::string get_type_name(av::FieldContainer* self)
{
  return self->getTypeId().getName();
}

bool is_derived_from(av::FieldContainer* self, std::string name)
{
  return self->getTypeId().isDerivedFrom(av::Type::getByName(name));
}

av::Link<av::FieldContainer> make_instance_by_name(std::string name)
{
  av::Typed* instance(av::Type::createInstanceOfType(name));
  return av::Link<av::FieldContainer>(dynamic_cast<av::FieldContainer*>(instance));
}

av::Field* make_field_by_name(std::string name)
{
  return dynamic_cast<av::Field*>(av::Type::createInstanceOfType(name));
}

av::Link<av::FieldContainer> ContainerPoll_get_instance_by_name(std::string name)
{
  return av::Link<av::FieldContainer>(av::ContainerPool::getInstanceByName(name));
}

unsigned int getNumberOfContainersToEvaluate()
{
  return av::FieldContainer::getNumberOfContainersToEvaluate();
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

namespace
{
  class FieldContainerLink : public av::Link<av::FieldContainer>
  {
  public:

    FieldContainerLink(av::Link<av::FieldContainer> const& container) :
      mLink(container)
    {
    }

    av::FieldContainer* getPtr(void) const
    {
      return mLink.getPtr();
    }

  private:
    av::Link<av::FieldContainer> mLink;
  };

  av::FieldContainer* get_pointer(const FieldContainerLink& link)
  {
    return link.getPtr();
  }

  struct FieldContainerLinkConverter
  {
    static PyObject* convert(av::Link<av::FieldContainer> const& container)
    {
      av::script::Script* script(dynamic_cast<av::script::Script*>(container.getPtr()));
      if (script)
      {
        PyObject* result(script->getSelf());
        Py_INCREF(result);
        return script->getSelf();
      }

      object wrapped_container((FieldContainerLink(container)));
      PyObject* result(wrapped_container.ptr());
      Py_INCREF(result);
      return result;
    }
  };
}

namespace boost
{
  namespace python
  {
    template<> struct pointee<FieldContainerLink>
    {
      using type = av::FieldContainer;
    };
  }
}

void init_FieldContainer(void)
{

  av::Field* (av::FieldContainer::*getField_uint)(unsigned int) = &av::FieldContainer::getField;
  av::Field* (av::FieldContainer::*getField_string)(const std::string&) = &av::FieldContainer::getField;
  void (av::FieldContainer::*addField)(av::Field*, const std::string&) = &av::FieldContainer::addDynamicField;
  void (av::FieldContainer::*removeField)(const std::string&) = &av::FieldContainer::removeDynamicField;

  class_<av::Base, av::Link<av::Base>, boost::noncopyable >("Base", "docstring", no_init)
    .def("__eq__", Base_equal)
    .def("__ne__", Base_unequal)
    ;

  class_<av::Distributed, av::Link<av::Distributed>, bases<av::Base>, boost::noncopyable >("Distributed", "docstring", no_init);

  class_<av::FieldContainer, FieldContainerLink, bases<av::Distributed>, boost::noncopyable >("FieldContainer", "docstring", no_init)
    //Deprecated. Use the non underscore versions
    .def("_get_type", get_type_name)
    .def("_is_derived_from", is_derived_from)
    .def("_get_num_fields", &av::FieldContainer::getNumFields)
    .def("_get_field_name", &av::FieldContainer::getFieldName, return_value_policy<copy_const_reference>())
    .def("_get_field", getField_uint, return_internal_reference<>())
    .def("_get_field", getField_string, return_internal_reference<>())

    .def("get_type", get_type_name)
    .def("is_derived_from", is_derived_from)
    .def("get_num_fields", &av::FieldContainer::getNumFields)
    .def("get_field_name", &av::FieldContainer::getFieldName, return_value_policy<copy_const_reference>())
    .def("get_field", getField_uint, return_internal_reference<>())
    .def("get_field", getField_string, return_internal_reference<>())

    .def("has_field", &av::FieldContainer::hasField)
    .def("add_field", addField)
    .def("remove_field", removeField, return_internal_reference<>())
    .def("touch", FieldContainer_touch)
    .def("always_evaluate", &av::FieldContainer::alwaysEvaluate)
    .def("allow_scheduling", &av::FieldContainer::allowScheduling)
    .def("disconnect_all_fields", &av::FieldContainer::disconnectAllFields)
    .def("evaluate_dependency", &av::FieldContainer::evaluateDependency)
    .def("read", &av::FieldContainer::read)
    .def("write", &av::FieldContainer::write)
    .def("reference_count",&av::FieldContainer::referenceCount)
    ;

  to_python_converter<av::Link<av::FieldContainer>, FieldContainerLinkConverter>();

  def("_make_instance_by_name", make_instance_by_name);
  def("_make_field_by_name", make_field_by_name, return_internal_reference<>());
  def("get_instance_by_name", ContainerPoll_get_instance_by_name);
  def("disconnect_and_clear_all_fields", av::disconnectAndClearAllFields);
  def("get_number_of_containers_to_evaluate",getNumberOfContainersToEvaluate);
}
