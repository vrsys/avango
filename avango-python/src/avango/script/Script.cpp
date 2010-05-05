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
    Script(av::Type type) :
      mType(type)
    {
    }

    /*virtual*/ av::Type getTypeId() const
    {
      return mType;
    }

    static ::av::Type getClassTypeId(void)
    {
      return sType;
    }

    static void initClass(void)
    {
      sType = av::Type::createAbstractType(av::FieldContainer::getClassTypeId(), "av::script::_Script", true);
    }

    static void register_exception_handler(object handler)
    {
      sHandler = handler;
    }

    static void handle_exception(void)
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

  private:

    av::Type mType;
    static av::Type sType;
    static object sHandler;
  };

  object Script::sHandler;
  av::Type Script::sType;

  class ScriptCreator : public av::Create
  {
  public:

    ScriptCreator(object creator) :
      mCreator(creator)
    {
    }

    /*virtual*/ av::Typed* makeInstance() const
    {
      av::Link<Script> script = extract<av::Link<Script> >(mCreator());
      script->setFloatingReference();
      return script.getBasePtr();
    }

  private:
    object mCreator;
  };

  av::Type create_type(const std::string& name, object creator)
  {
    ScriptCreator* script_creator = new ScriptCreator(creator);
    av::Type type = av::Type::createType(Script::getClassTypeId(), name, script_creator, true);
    return type;
  }

}

void av::script::register_script(void)
{
  av::FieldContainer::initClass();
  Script::initClass();

  def("_create_type", create_type);
  def("register_exception_handler", &Script::register_exception_handler);

  class_<Script, av::Link<Script>, bases<av::FieldContainer> >
    ("_Script", "Internal base class for Script nodes", init<av::Type>())
    ;

  class_<av::Type>("_Type");
}
