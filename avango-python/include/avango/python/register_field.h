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

#if !defined(REGISTER_FIELD_H)
#define REGISTER_FIELD_H

#include <boost/python.hpp>
#include <avango/Field.h>
#include <avango/FieldContainer.h>
#include <avango/Base.h>
#include <avango/Link.h>
#include <avango/MultiField.h>
#include <iostream>
#include <vector>
#include "exceptions.h"

/**
 * \namespace av::python
 * Avango Python Library
 *
 * \ingroup av_python
 */

/**
 * \file
 * \ingroup av_python
 */

namespace av
{
  namespace python
  {

    namespace detail
    {
      template<class Type> struct select_link_type
      {
        using type = Type;
        static Type get_value(boost::python::object obj)
        {
          return boost::python::extract<type>(obj);
        }
      };

      template<class Type> struct select_link_type< av::Link<Type> >
      {
        using type = Type;
        static av::Link<Type> get_value(boost::python::object obj)
        {
          av::Base* av_value = boost::python::extract<av::Base*>(obj);
          // FIXME Throw exception when dynamic cast fails
          return av::Link<Type>(dynamic_cast<Type*>(av_value));
        }
      };

      template<class Type> typename Type::ValueType Field_get_value(Type& self)
      {
        return self.getValue();
      }

      template<class Type> void Field_set_value(Type& self, boost::python::object value)
      {
        using value_type = typename Type::ValueType;
        value_type tmp(detail::select_link_type<value_type>::get_value(value));
        self.setValue(tmp);
      }
    }

    template<class Type> void register_field(std::string name)
    {
      boost::python::class_<Type, boost::python::bases<av::Field> >(name.c_str())
        .def("get_value", &Type::getValue, boost::python::return_value_policy<boost::python::copy_const_reference>())
        .def("set_value", detail::Field_set_value<Type>)
        .add_property("value", detail::Field_get_value<Type>, detail::Field_set_value<Type>);
    }


    namespace detail
    {
      template<class Type> class MultiFieldWrapper
      {
      public:
        MultiFieldWrapper(Type& Field) :
          mField(Field)
        {
        }

      private:

        void validate_index(int index)
        {
          if ( (index < 0) || (index >= mField.getSize()) )
            throw index_exception();
        }

        using ValueType = typename Type::ValueType;
        using SelectType = detail::select_link_type<ValueType>;

      public:

        int getSize(void)
        {
          return mField.getSize();
        }

        typename Type::ValueType getItem(int index)
        {
          validate_index(index);
          return mField.getValue()[index];
        }

        void setItem(int index, boost::python::object value)
        {
          validate_index(index);

          typename Type::ContainerType container = mField.getValue();
          container[index] = SelectType::get_value(value);
          mField.setValue(container);
        }

        void delItem(int index)
        {
          validate_index(index);

          typename Type::ContainerType container = mField.getValue();
          container.erase(container.begin()+index);
          mField.setValue(container);
        }

        void remove(boost::python::object value)
        {
          mField.remove1Value(SelectType::get_value(value));
        }

        void append(boost::python::object value)
        {
          mField.add1Value(SelectType::get_value(value));
        }

        void extend(boost::python::object values)
        {
          typename Type::ContainerType container = mField.getValue();
          int len = boost::python::extract<int>(values.attr("__len__")());
          for (int i = 0; i != len; ++i)
          {
            boost::python::object v = values.attr("__getitem__")(i);
            container.push_back(SelectType::get_value(v));
          }
          mField.setValue(container);
        }

        void insert(int index, boost::python::object value)
        {
          validate_index(index);

          typename Type::ContainerType container = mField.getValue();
          container.insert(container.begin()+index, SelectType::get_value(value));
          mField.setValue(container);
        }

      private:
        Type& mField;
      };

      template<class Type> MultiFieldWrapper<Type> MultiField_get_value(Type& self)
      {
        return MultiFieldWrapper<Type>(self);
      }

      template<class Type> void MultiField_set_value(Type& self, boost::python::object value)
      {
        using value_type = typename Type::ValueType;
        int len = boost::python::extract<int>(value.attr("__len__")());
        std::vector<value_type> list(len);
        for (int i = 0; i != len; ++i)
        {
          boost::python::object v = value.attr("__getitem__")(i);
          list[i] = detail::select_link_type<value_type>::get_value(v);
        }
        self.setValue(list);
      }

    }

    template<class Type> void register_multifield(std::string name)
    {
      using namespace av::python::detail;

      boost::python::class_<Type, boost::python::bases<av::Field> >(name.c_str())
        .def("get_value", MultiField_get_value<Type>)
        .def("set_value", MultiField_set_value<Type>)
        .add_property("value", MultiField_get_value<Type>, MultiField_set_value<Type>);
      ;

      boost::python::class_<MultiFieldWrapper<Type> >((name+"_wrapper").c_str(), boost::python::no_init)
        .def("__len__", &MultiFieldWrapper<Type>::getSize)
        .def("__getitem__", &MultiFieldWrapper<Type>::getItem)
        .def("__setitem__", &MultiFieldWrapper<Type>::setItem)
        .def("__delitem__", &MultiFieldWrapper<Type>::delItem)
        .def("remove", &MultiFieldWrapper<Type>::remove)
        .def("append", &MultiFieldWrapper<Type>::append)
        .def("extend", &MultiFieldWrapper<Type>::extend)
        .def("insert", &MultiFieldWrapper<Type>::insert)
        ;
    }

  }
}


#endif
