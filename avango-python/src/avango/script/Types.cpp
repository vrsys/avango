// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       *
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

#include <avango/python/script/Types.h>

#include <avango/Logger.h>

namespace
{
av::Logger& logger(av::getLogger("av::python::Types"));
}

// TODO: Do some useful stream and network stuff for boost::python::api::object

#if defined(AVANGO_DISTRIBUTION_SUPPORT)

#include <avango/Msg.h>

void av::av_popMsg(av::Msg&, boost::python::api::object&)
{
    // TODO: Implement proper Python pickle support
}

void av::av_pushMsg(av::Msg&, boost::python::api::object&)
{
    // TODO: Implement proper Python pickle support
}

#endif // #if defined(AVANGO_DISTRIBUTION_SUPPORT)

av::OutputStream& av::operator<<(av::OutputStream& os, const boost::python::api::object&) { return os; }

av::InputStream& av::operator>>(av::InputStream& is, boost::python::api::object&) { return is; }
