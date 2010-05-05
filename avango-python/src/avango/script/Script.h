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

      void enableFieldHasChanged(void);

      static void register_exception_handler(boost::python::object handler);

    protected:

      /*virtual*/ void evaluate();
      /*virtual*/ void fieldHasChanged(const Field& field);

    private:

      static void handle_exception(void);

      /* Forbid copy constructor and operator */
      Script(const Script&) {}
      void operator=(const Script&) {}

      PyObject* mSelf;
      av::Type mType;
      bool mIsFloatingRef;
      bool mHasFieldHasChangedEnabled;
      static av::Type sType;
      static boost::python::object sHandler;
    };

   void register_script(void);

  }
}

#endif // #if !defined(AV_SCRIPT_SCRIPT_H)

