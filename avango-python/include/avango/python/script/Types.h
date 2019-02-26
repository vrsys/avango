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
#include <avango/python/windows_specific_python.h>

namespace av
{
#if defined(AVANGO_DISTRIBUTION_SUPPORT)

class Msg;

void av_popMsg(av::Msg&, boost::python::api::object&);
void av_pushMsg(av::Msg&, boost::python::api::object&);

#endif

AV_PYTHON_DLL InputStream& operator>>(InputStream&, boost::python::api::object&);
AV_PYTHON_DLL OutputStream& operator<<(OutputStream&, const boost::python::api::object&);

namespace script
{
using SFObject = SingleField<boost::python::object>;
using MFObject = MultiField<boost::python::object>;
} // namespace script

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_PYTHON_DLL SingleField<boost::python::object>;
template class AV_PYTHON_DLL MultiField<boost::python::object>;
#endif

} // namespace av

#endif // #if !defined(AVANGO_PYTHON_TYPES_H)
