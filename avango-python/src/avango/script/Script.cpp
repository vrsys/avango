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

#include <list>
#include <map>

#include "Script.h"

#include <avango/Create.h>
#include <avango/FieldContainer.h>
#include <avango/Type.h>

#include <boost/shared_ptr.hpp>
#include <boost/python.hpp>
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

namespace
{

  class Script : public av::FieldContainer
  {
  public:

    Script(av::Type type, object class_, object fields, dict callbacks) :
      mType(type)
    {
      mInstance = class_.attr("_new")();

      for (int i = 0; i != extract<int>(fields.attr("__len__")()); ++i)
      {
        std::string name = extract<std::string>(fields[i][0]);
        av::Field* field = extract<av::Field*>(fields[i][1]);

        av::Field* field_copy = field->clone();
        //mFields.add(name, boost::shared_ptr<av::Field>(field_copy));

        if (callbacks.has_key(name))
        {
          mCallbacks[field_copy] = extract<std::string>(callbacks[name]);
        }

        field_copy->bind(this, name, true);
      }

      if (mHasFieldHasChanged = callbacks.has_key(object()))
      {
        mFieldHasChangedCallback = extract<std::string>(callbacks[object()]);
      }

      // Now instance is setup, call initialization method
      this->reference();
      try
      {
        // The '__init__' method exists in the Boost base object, we therefore
        // cannot rely on '__getattr__' to get the correct function for us.
        mInstance.attr("__init__").attr("im_func")(av::Link<Script>(this));
      }
      catch(...)
      {
        this->unreferenceWithoutDeletion();

        // FIXME This assertion triggers almost always as python still holds
        // references to our unborn class
        assert((!this->referenceCount()));

        throw;
      }
      this->unreferenceWithoutDeletion();
    }

    /*virtual*/ av::Type getTypeId() const
    {
      return mType;
    }

    /*virtual*/ void evaluate(void)
    {
      try
      {
        object(av::Link<Script>(this)).attr("evaluate")();
      }
      catch(...)
      {
        handle_exception();
      }
    }

    /*virtual*/ void fieldHasChanged(const av::Field& field)
    {
      std::map<const av::Field*,std::string>::const_iterator result = mCallbacks.find(&field);
      if (result != mCallbacks.end())
      {
        try
        {
          object(av::Link<Script>(this)).attr(result->second.c_str())();
        }
        catch(...)
        {
          handle_exception();
        }
      }
      else if (mHasFieldHasChanged)
      {
        try
        {
          object(av::Link<Script>(this)).attr(mFieldHasChangedCallback.c_str())(field.getName(), boost::ref(field));
        }
        catch(...)
        {
          handle_exception();
        }
      }
    }

    object getObject(void)
    {
      return mInstance;
    }

    static void register_exception_handler(object handler)
    {
      sHandler = handler;
    }

    static void handle_exception(void)
    {
      if (!sHandler)
        throw_error_already_set();

      PyObject* type;
      PyObject* value;
      PyObject* traceback;
      PyErr_Fetch(&type, &value, &traceback);

      sHandler(handle<>(type), handle<>(value), handle<>(traceback));
    }


  private:

    av::Type mType;
    object mInstance;
    std::map< const av::Field*, std::string > mCallbacks;
    bool mHasFieldHasChanged;
    std::string mFieldHasChangedCallback;
    static object sHandler;
  };

  object Script::sHandler;

  class ScriptCreator : public av::Create
  {
  public:

    ScriptCreator(const std::string& name, object class_, object fields, dict callbacks) :
      mClass(class_),
      mFields(fields),
      mCallbacks(callbacks)
    {
      mType = av::Type::createType(av::FieldContainer::getClassTypeId(), name, this, true);
    }

    /*virtual*/ av::Typed* makeInstance() const
    {
      Script* script = new Script(mType, mClass, mFields, mCallbacks);
      return script;
    }

  private:

    av::Type mType;
    object mClass;
    object mFields;
    dict mCallbacks;
  };

  void create_type(const std::string& name, object class_, object fields, dict callbacks)
  {
    new ScriptCreator(name, class_, fields, callbacks);
  }

}

void av::script::register_script(void)
{
  def("_create_type", create_type);
  def("register_exception_handler", &Script::register_exception_handler);

  class_<Script, av::Link<Script>, bases<av::FieldContainer>, boost::noncopyable>
    ("_Script", "Internal base class for Script nodes", no_init)
    .def("_get_object", &Script::getObject)
    ;
}
