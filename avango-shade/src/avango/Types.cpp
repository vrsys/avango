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

#include <avango/shade/Types.h>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::shade::Types"));
}

#if defined(AVANGO_DISTRIBUTION_SUPPORT)

#include <rpc/rpc.h>
#include <avango/Msg.h>

void
osg::av_pushMsg(av::Msg& netMsg, const ::osg::Object::DataVariance& buf)
{
  int value(static_cast<int>(buf));
  osg::av_pushMsg(netMsg, value);
}

void
osg::av_popMsg(av::Msg& netMsg, ::osg::Object::DataVariance& buf)
{
  int value;
  osg::av_popMsg(netMsg, value);
  buf = static_cast<::osg::Object::DataVariance>(value);
}

#endif // #if defined(AVANGO_DISTRIBUTION_SUPPORT)

av::OutputStream&
av::operator<<(av::OutputStream& os, const ::osg::Object::DataVariance& buf)
{
  int value(static_cast<int>(buf));
  os << value;

  return os;
}

av::InputStream&
av::operator>>(av::InputStream& is, ::osg::Object::DataVariance& buf)
{
  int value;
  is >> value;
  buf = static_cast< ::osg::Object::DataVariance >(value);

  return is;
}
