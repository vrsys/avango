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

#include <avango/OutputStream.h>
#include <cstring>


av::OutputStream::OutputStream() :
  std::ostream(0),
  mWriteAction(0),
  mBinary(false)
{}

av::OutputStream::OutputStream(std::ostream& os) :
  std::ostream(os.rdbuf()),
  mWriteAction(0),
  mBinary(false)
{}

av::OutputStream::OutputStream(std::streambuf* sb) :
  std::ostream(sb),
  mWriteAction(0),
  mBinary(false)
{}

av::OutputStream&
av::OutputStream::operator=(std::ostream& os)
{
  (void) rdbuf(os.rdbuf());
  return *this;
}

av::OutputStream&
av::OutputStream::operator=(std::streambuf* sb)
{
  (void) rdbuf(sb);
  return *this;
}

void
av::OutputStream::enableBinary(bool on)
{
  mBinary = on;
}

bool
av::OutputStream::isBinaryEnabled() const
{
  return mBinary;
}

void
av::OutputStream::setWriteAction(av::WriteAction* writeAction)
{
  mWriteAction = writeAction;
}

av::WriteAction*
av::OutputStream::getWriteAction()
{
  return mWriteAction;
}

av::OutputStream&
av::endl(av::OutputStream& os)
{
  std::endl(os);
  return os;
}

av::OutputStream&
av::flush(av::OutputStream& os)
{
  std::flush(os);
  return os;
}

av::OutputStream& av::operator<<(av::OutputStream& os, const int16_t& value)
{
  if (os.isBinaryEnabled()) {
    os.write((const char*) &value, 4);
  } else {
    (std::ostream&) os << value;
  }
  return os;
}

av::OutputStream& av::operator<<(av::OutputStream& os, const uint16_t& value)
{
  if (os.isBinaryEnabled()) {
    os.write((const char*) &value, 4);
  } else {
    (std::ostream&) os << value;
  }
  return os;
}

av::OutputStream& av::operator<<(av::OutputStream& os, const int32_t& value)
{
  if (os.isBinaryEnabled()) {
    os.write((const char*) &value, 4);
  } else {
    (std::ostream&) os << value;
  }
  return os;
}

av::OutputStream& av::operator<<(av::OutputStream& os, const uint32_t& value)
{
  if (os.isBinaryEnabled()) {
    os.write((const char*) &value, 4);
  } else {
    (std::ostream&) os << value;
  }
  return os;
}

av::OutputStream& av::operator<<(av::OutputStream& os, const int64_t& value)
{
  if (os.isBinaryEnabled()) {
    os.write((const char*) &value, 8);
  } else {
    (std::ostream&) os << (int32_t) value;
  }
  return os;
}

av::OutputStream& av::operator<<(av::OutputStream& os, const uint64_t& value)
{
  if (os.isBinaryEnabled()) {
    os.write((const char*) &value, 8);
  } else {
    (std::ostream&) os << (uint32_t) value;
  }
  return os;
}

av::OutputStream& av::operator<<(av::OutputStream& os, const float& value)
{
  if (os.isBinaryEnabled()) {
    os.write((const char*) &value, sizeof(value));
  } else {
    (std::ostream&) os << value;
  }
  return os;
}

av::OutputStream& av::operator<<(av::OutputStream& os, const double& value)
{
  if (os.isBinaryEnabled()) {
    os.write((const char*) &value, sizeof(value));
  } else {
    (std::ostream&) os << value;
  }
  return os;
}

av::OutputStream& av::operator<<(av::OutputStream& os, const bool& value)
{
  if (os.isBinaryEnabled()) {
    os.write((const char*) &value, sizeof(value));
  } else {
    (std::ostream&) os << value;
  }
  return os;
}

av::OutputStream& av::operator<<(av::OutputStream& os, const std::string& value)
{
  if (os.isBinaryEnabled()) {
    if (value.c_str()) {
      int length = std::strlen(value.c_str());
      os.write((char*) &length, sizeof(length));
      os.write(value.c_str(), length);
    } else {
      int length = 0;
      os.write((char*) &length, sizeof(length));
    }
  } else {
    (std::ostream&)os << '"';
    if (value.c_str())
      os.write(value.c_str(), std::strlen(value.c_str()));
    (std::ostream&)os << '"';
  }
  return os;
}
