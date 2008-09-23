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

#include <avango/InputStream.h>

#include <avango/Assert.h>

av::InputStream::InputStream() :
  std::istream(0),
  mBinary(false),
  mReader(0)
{}

av::InputStream::InputStream(std::istream& is) :
  std::istream(is.rdbuf()),
  mBinary(false),
  mReader(0)
{}

av::InputStream::InputStream(std::streambuf* sb) :
  std::istream(sb),
  mBinary(false),
  mReader(0)
{}

av::InputStream&
av::InputStream::operator=(std::istream& is)
{
  (void) rdbuf(is.rdbuf());
  return *this;
}

av::InputStream&
av::InputStream::operator=(std::streambuf* sb)
{
  (void) rdbuf (sb);
  return *this;
}

void
av::InputStream::enableBinary(bool on)
{
  mBinary = on;
}

bool
av::InputStream::isBinaryEnabled() const
{
  return mBinary;
}

void
av::InputStream::setReader(av::Reader* reader)
{
  mReader = reader;
}

av::Reader*
av::InputStream::getReader()
{
  return mReader;
}

av::InputStream& av::operator>>(av::InputStream& is, int16_t& value)
{
  if (is.isBinaryEnabled()) {
    is.read((char*) &value, 4);
  } else {
    (std::istream&) is >> value;
  }
  return is;
}

av::InputStream& av::operator>>(av::InputStream& is, uint16_t& value)
{
  if (is.isBinaryEnabled()) {
    is.read((char*) &value, 4);
  } else {
    (std::istream&) is >> value;
  }
  return is;
}

av::InputStream& av::operator>>(av::InputStream& is, int32_t& value)
{
  if (is.isBinaryEnabled()) {
    is.read((char*) &value, 4);
  } else {
    (std::istream&) is >> value;
  }
  return is;
}

av::InputStream& av::operator>>(av::InputStream& is, uint32_t& value)
{
  if (is.isBinaryEnabled()) {
    is.read((char*) &value, 4);
  } else {
    (std::istream&) is >> value;
  }
  return is;
}

av::InputStream& av::operator>>(av::InputStream& is, int64_t& value)
{
  if (is.isBinaryEnabled()) {
    is.read((char*) &value, 8);
  } else {
    int32_t tmp;
    (std::istream&) is >> tmp;
    value = tmp;
  }
  return is;
}

av::InputStream& av::operator>>(av::InputStream& is, uint64_t& value)
{
  if (is.isBinaryEnabled()) {
    is.read((char*) &value, 8);
  } else {
    uint32_t tmp;
    (std::istream&) is >> tmp;
    value = tmp;
  }
  return is;
}

av::InputStream& av::operator>>(av::InputStream& is, float& value)
{
  if (is.isBinaryEnabled()) {
    is.read((char*) &value, sizeof(value));
  } else {
    (std::istream&) is >> value;
  }
  return is;
}

av::InputStream& av::operator>>(av::InputStream& is, double& value)
{
  if (is.isBinaryEnabled()) {
    is.read((char*) &value, sizeof(value));
  } else {
    (std::istream&) is >> value;
  }
  return is;
}

av::InputStream& av::operator>>(av::InputStream& is, bool& value)
{
  if (is.isBinaryEnabled()) {
    is.read((char*) &value, sizeof(value));
  } else {
    (std::istream&) is >> value;
  }
  return is;
}

av::InputStream&
av::operator>>(av::InputStream& is, std::string& value)
{
  char str[1024];
  if (is.isBinaryEnabled())
  {
    int size = 0;
    is.read((char*) &size, sizeof(int));
    if (is && size)
      is.read(str, size);
    str[size] = '\0';
    value = str;
  }
  else
  {
    char chr;
    (std::istream&)is >> chr;
    AV_ASSERT(chr == '"');
    int size = -1;
    do {
      (std::istream&)is >> chr;
      if (!is) break;
      size ++;
      str[size] = chr;
    } while (chr != '"');
    str[size] = '\0';
    value = str;
  }
  return is;
}
