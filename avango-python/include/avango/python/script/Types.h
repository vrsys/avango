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

#if !defined(AVANGO_PYTHON_TYPES_H)
#define AVANGO_PYTHON_TYPES_H

/**
 * \namespace av::script
 * Avango Python Library
 *
 * \ingroup av_python
 */

/**
 * \file
 * \ingroup av_python
 */

#include <boost/python.hpp>

#include <avango/Config.h>
#include <avango/SingleField.h>
#include <avango/MultiField.h>
#include <avango/InputStream.h>
#include <avango/OutputStream.h>

namespace av
{

#if defined(AVANGO_DISTRIBUTION_SUPPORT)

  class Msg;

  void av_popMsg(av::Msg&, boost::python::api::object&);
  void av_pushMsg(av::Msg&, boost::python::api::object&);

#endif

  InputStream& operator>>(InputStream&, boost::python::api::object&);
  OutputStream& operator<<(OutputStream&, const boost::python::api::object&);

  namespace script
  {
    typedef SingleField<boost::python::object> SFObject;
    typedef MultiField<boost::python::object> MFObject;
  }
} // namespace av

#endif // #if !defined(AVANGO_PYTHON_TYPES_H)
