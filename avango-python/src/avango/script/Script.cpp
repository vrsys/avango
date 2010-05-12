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


av::Type av::script::Script::sType;
object av::script::Script::sHandler;


av::script::Script::Script(PyObject* self, av::Type type) :
  mSelf(self),
  mType(type),
  mIsFloatingRef(false),
  mHasFieldHasChangedEnabled(false)
{
}

/*virtual*/ av::Type av::script::Script::getTypeId() const
{
  return mType;
}

::av::Type av::script::Script::getClassTypeId(void)
{
  return sType;
}

void av::script::Script::initClass(void)
{
  sType = av::Type::createAbstractType(av::FieldContainer::getClassTypeId(), "av::script::_Script", true);
}

/*virtual*/ void av::script::Script::refImpl()
{
  if (mIsFloatingRef)
  {
    mIsFloatingRef = false;
  }
  else
  {
    Py_INCREF(mSelf);
  }
}

/*virtual*/ void av::script::Script::unrefImpl()
{
  Py_DECREF(mSelf);
}

/*virtual*/ void av::script::Script::unrefWithoutDeletionImpl()
{
  // TODO fixme
}

/*virtual*/ int av::script::Script::refCountImpl()
{
  // TODO fixme
  return 42;
}

/*virtual*/ void av::script::Script::setFloatingRefImpl()
{
  Py_INCREF(mSelf);
  mIsFloatingRef = true;
}

/*virtual*/ void av::script::Script::evaluate()
{
  try
  {
    call_method<void>(mSelf, "evaluate");
  }
  catch(...)
  {
    handle_exception();
  }
}

/*virtual*/ void av::script::Script::fieldHasChanged(const Field& field)
{
  if (mHasFieldHasChangedEnabled)
    call_method<void>(mSelf, "_Script__fieldHasChanged", boost::ref(field));
}

void av::script::Script::enableFieldHasChanged(void)
{
  mHasFieldHasChangedEnabled = true;
}

PyObject* av::script::Script::getSelf(void) const
{
  return mSelf;
}

void av::script::Script::register_exception_handler(object handler)
{
  sHandler = handler;
}

void av::script::Script::handle_exception(void)
{
  // TODO This was called during method invocation or fieldHasChanged
  // callbacks. See if this still applies. Otherwise remove.
  if (!sHandler)
    throw_error_already_set();

  PyObject* type;
  PyObject* value;
  PyObject* traceback;
  PyErr_Fetch(&type, &value, &traceback);

  sHandler(handle<>(type), handle<>(value), handle<>(traceback));
}


namespace
{
  class ScriptCreator : public av::Create
  {
  public:

    ScriptCreator(object creator) :
      mCreator(creator)
    {
    }

    /*virtual*/ av::Typed* makeInstance() const
    {
      object sc(mCreator());
      av::Link<av::script::Script> script((extract<av::script::Script*>(sc)()));
      script->setFloatingReference();
      return script.getBasePtr();
    }

  private:
    object mCreator;
  };

  av::Type create_type(const std::string& name, object creator)
  {
    ScriptCreator* script_creator = new ScriptCreator(creator);
    av::Type type = av::Type::createType(av::script::Script::getClassTypeId(), name, script_creator, true);
    return type;
  }

  struct ScriptLinkConverter
  {
    static PyObject* convert(av::Link<av::script::Script> const& script)
    {
      PyObject* result(script->getSelf());
      Py_INCREF(result);
      return result;
    }
    static PyObject* convert(av::script::Script const& script)
    {
      PyObject* result(script.getSelf());
      Py_INCREF(result);
      return result;
    }
  };

}

namespace boost
{
  namespace python
  {
    template <class T> struct pointee<av::Link<T> >
    {
      typedef T type;
    };
    template <> struct has_back_reference<av::script::Script>
      : mpl::true_
    {
    };
  }
}

namespace
{
  int num_py_references(av::script::Script* self)
  {
    PyObject * sys = PyImport_ImportModule("sys");
    PyObject * method = PyString_FromString("getrefcount");
    PyObject * result = PyObject_CallMethodObjArgs(sys, method, self->getSelf(), NULL);
    int res = (int)PyInt_AsLong(result);
    if(PyErr_Occurred())
      return -1;
    else
      return res;
  }
}

void av::script::register_script(void)
{
  av::FieldContainer::initClass();
  Script::initClass();

  def("_create_type", create_type);
  def("register_exception_handler", &Script::register_exception_handler);

  class_<Script, bases<av::FieldContainer>, boost::noncopyable>
    ("_Script", "Internal base class for Script nodes", init<av::Type>())
    .def("_Script__enable_field_has_changed", &Script::enableFieldHasChanged)
    .def("reference_count", num_py_references)
    ;

  class_<av::Type>("_Type");

  to_python_converter<av::Link<Script>, ScriptLinkConverter>();
  to_python_converter<Script, ScriptLinkConverter>();
}
