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

#if !defined(REGISTER_MULTIVALUEFIELD_H)
#define REGISTER_MULTIVALUEFIELD_H

#include <boost/python.hpp>
#include <avango/Field.h>
#include <avango/FieldContainer.h>
#include <avango/Base.h>
#include <avango/Link.h>
#include <avango/python/exceptions.h>
#include <iostream>
#include <vector>
#include "MultiValueField.h"


namespace av
{
  namespace python
  {
    namespace detail
    {
      template<class Type> class MultiValueFieldWrapper
      {
      public:
        MultiValueFieldWrapper(Type& Field) :
          mField(Field)
        {
        }

      private:

        void validate_index(int index)
        {
          if ( (index < 0) || (index >= mField.getSize()) )
            throw index_exception();
        }

        // using ValueType = typename Type::ValueType;

      public:

        int getSize(void)
        {
          return mField.getSize();
        }

        typename Type::ValueType getItem(int index)
        {
          validate_index(index);
          return mField.get1Value(index);
        }

        void setItem(int index, boost::python::object value)
        {
          validate_index(index);
          mField.set1Value(boost::python::extract<typename Type::ValueType>(value), index);
        }

        void delItem(int index)
        {
          validate_index(index);
          mField.erase1Value(index);
        }

      private:
        Type& mField;
      };

      template<class Type> MultiValueFieldWrapper<Type> MultiValueField_get_all_values(Type& self)
      {
        return MultiValueFieldWrapper<Type>(self);
      }

      template<class Type> void MultiValueField_set_all_values(Type& self, boost::python::object value)
      {
        using value_type = typename Type::ValueType;
        int len = boost::python::extract<int>(value.attr("__len__")());
        std::vector<value_type> list(len);
        for (int i = 0; i != len; ++i)
        {
          boost::python::object v = value.attr("__getitem__")(i);
          list[i] = boost::python::extract<value_type>(v);
        }
        self.setAllValues(list);
      }

      template<class Type> void MultiValueField_set_some_values(Type& self, boost::python::object value, boost::python::object inds)
      {
        using value_type = typename Type::ValueType;

        int len = boost::python::extract<int>(value.attr("__len__")());
        std::vector<value_type> value_list(len);
        for (int i = 0; i != len; ++i)
        {
          boost::python::object v = value.attr("__getitem__")(i);
          value_list[i] = boost::python::extract<value_type>(v);
        }

        len = boost::python::extract<int>(inds.attr("__len__")());
        std::vector<unsigned int> inds_list(len);
        for(int i = 0; i != len; ++i)
        {
          boost::python::object indx = inds.attr("__getitem__")(i);
          inds_list[i] = boost::python::extract<unsigned int>(indx);
        }

        self.setSomeValues(value_list, inds_list);
      }

    }

    template<class Type> void register_multivaluefield(std::string name)
    {
      using namespace av::python::detail;

      // this declaration is necessary to avoid ambiguities because of function overloading
      void (Type::*set_1value_fptr)(const typename Type::ValueType&, unsigned int, bool) = &Type::set1Value;

      boost::python::class_<Type, boost::python::bases<av::Field> >(name.c_str())
        .def("get_all_values", MultiValueField_get_all_values<Type>)
        .def("set_all_values", MultiValueField_set_all_values<Type>)
        .def("set_some_values", MultiValueField_set_some_values<Type>)
        .def("get_1value", &Type::get1Value)
        .def("insert_1value", &Type::insert1Value)
        .def("erase_1value", &Type::erase1Value)
        .def("add_1value", &Type::add1Value)
        .def("remove_1value", &Type::remove1Value)
        .def("clear_all_values", &Type::clearAllValues)
        .def("get_size", &Type::getSize)
        .def("is_empty", &Type::isEmpty)
        .def("value_has_changed", &Type::valueHasChanged)
        .def("trigger_field_change", &Type::triggerFieldChange)
        .def("reset_changed_flags", &Type::resetChangedFlags)
        .def("set_1value", set_1value_fptr)
        .add_property("all_values", MultiValueField_get_all_values<Type>, MultiValueField_set_all_values<Type>);
      ;

      boost::python::class_<MultiValueFieldWrapper<Type> >((name+"_wrapper").c_str(), boost::python::no_init)
        .def("__len__", &MultiValueFieldWrapper<Type>::getSize)
        .def("__getitem__", &MultiValueFieldWrapper<Type>::getItem)
        .def("__setitem__", &MultiValueFieldWrapper<Type>::setItem)
        .def("__delitem__", &MultiValueFieldWrapper<Type>::delItem)
        ;
    }
  }
}


#endif
