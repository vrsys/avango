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

#include <boost/python.hpp>

#include <avango/Config.h>

#include <avango/moving/Init.h>
#include <avango/moving/Trackball.h>
#include <avango/moving/DesktopFlyer.h>
#include <avango/python/register_field.h>

using namespace boost::python;
using namespace av::python;


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

BOOST_PYTHON_MODULE(_moving)
{
  av::moving::Init::initClass();

  register_field<av::moving::SFTrackball>("SFTrackball");
  register_multifield<av::moving::MFTrackball>("MFTrackball");

  register_field<av::moving::SFDesktopFlyer>("SFDesktopFlyer");
  register_multifield<av::moving::MFDesktopFlyer>("MFDesktopFlyer");

  class_<av::moving::Trackball, av::Link<av::moving::Trackball>, bases<av::osg::MatrixTransform>, boost::noncopyable >("Trackball", "Trackball mover", no_init);
  class_<av::moving::DesktopFlyer, av::Link<av::moving::DesktopFlyer>, bases<av::osg::MatrixTransform>, boost::noncopyable >("DesktopFlyer", "DesktopFlyer mover", no_init);
}
