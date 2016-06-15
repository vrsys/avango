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
#include <avango/Type.h>
#include <avango/Field.h>
#include <avango/StandardFields.h>
#include <avango/python/register_field.h>
#include <vector>
#include <string>

#include "InputStream.h"

using namespace boost::python;
using namespace av::python;

namespace
{
class StringInputStream : public av::InputStream
{
private:
  std::stringstream strstream;

public:
  StringInputStream()
  {
    rdbuf(strstream.rdbuf());
  }

  std::string get_buffer()
  {
    return strstream.str();
  }

  void set_buffer(const std::string buffer)
  {
    strstream << buffer;
  }
};

}

void init_InputStream(void)
{
  class_<av::InputStream, boost::noncopyable>("InputStream", "docstring");
  class_<StringInputStream, bases<av::InputStream>, boost::noncopyable>("StringInputStream", "docstring")
  .add_property("buffer", &StringInputStream::get_buffer, &StringInputStream::set_buffer)
  ;
}
