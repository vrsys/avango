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

#if !defined(AV_SCRIPT_SCRIPT_H)
#define AV_SCRIPT_SCRIPT_H

#include <avango/FieldContainer.h>
#include <boost/python.hpp>

namespace av
{
  namespace script
  {

    class Script : public av::FieldContainer, public boost::python::wrapper<Script>
    {
    public:
      Script(PyObject* self, av::Type type);

      /*virtual*/ av::Type getTypeId() const;
      static ::av::Type getClassTypeId(void);

      static void initClass(void);

      /*virtual*/ void refImpl();
      /*virtual*/ void unrefImpl();
      /*virtual*/ void unrefWithoutDeletionImpl();
      /*virtual*/ int refCountImpl();
      /*virtual*/ void setFloatingRefImpl();

      PyObject* getSelf(void) const;

      static void register_exception_handler(boost::python::object handler);
      static void handle_exception(void);

    private:

      /* Forbid copy constructor and operator */
      Script(const Script&) {}
      void operator=(const Script&) {}

      PyObject* mSelf;
      av::Type mType;
      bool mIsFloatingRef;
      static av::Type sType;
      static boost::python::object sHandler;
    };

   void register_script(void);

  }
}

#endif // #if !defined(AV_SCRIPT_SCRIPT_H)

