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

#include <avango/Link.h>

#include <cstdlib>

#include <avango/InputStream.h>
#include <avango/OutputStream.h>
#include <avango/Reader.h>
#include <avango/WriteAction.h>

namespace {
  av::Logger& logger(av::getLogger("av::Link"));
}

bool
av::AnyLink::isValid() const
{
  return mBasePtr != 0;
}

void
av::AnyLink::clear()
{
  AnyLink::setBasePtr(0);
}

av::Base*
av::AnyLink::getBasePtr() const
{
  return mBasePtr;
}

bool
av::AnyLink::setBasePtr(Base* base)
{
  if (base && !base->getTypeId().isOfType(mTypeId)) {
    return false;
  }

  if (mBasePtr)
  {
    mBasePtr->unreference();
    mBasePtr = 0;
  }
  mBasePtr = base;
  if (mBasePtr)
  {
    mBasePtr->reference();
  }

  return true;
}

av::AnyLink::AnyLink(av::Base* base, av::Type typeId)
  : mBasePtr(0), mTypeId(typeId)
{
  setBasePtr(base);
}

/* virtual */
av::AnyLink::~AnyLink()
{
  if (mBasePtr)
  {
    mBasePtr->unreference();
  }
}

bool
av::operator==(const av::AnyLink& a, const av::AnyLink& b)
{
  return a.getBasePtr() == b.getBasePtr();
}

bool
av::operator!=(const av::AnyLink& a, const av::AnyLink& b)
{
  return a.getBasePtr() != b.getBasePtr();
}

bool
av::operator<(const av::AnyLink& a, const av::AnyLink& b)
{
  return a.getBasePtr() < b.getBasePtr();
}

av::InputStream&
av::operator>>(av::InputStream& is, av::AnyLink& bl)
{
  if (is.isBinaryEnabled())
  {
    if (is.getReader())
    {
      char reference_flag;
      (std::istream&) is >> reference_flag;
      if (reference_flag == 'R') {
        // object is already there, resolve reference
        std::string obj_id;
        is >> obj_id;

        LOG_TRACE(logger) << "operator>>(av::InputStream&, av::AnyLink&): "
                                   << "resolving reference: "
                                   << obj_id;
        if (!bl.setBasePtr(is.getReader()->lookupObject(obj_id)))
        {
          logger.warn() << "operator>>(av::InputStream&, av::AnyLink&): type clash: "
                                 << obj_id;
        }
      }
      else
      {
        // object is new, load it
        LOG_TRACE(logger) << "operator>>(av::InputStream&, av::AnyLink&): reading object";
        if (!bl.setBasePtr(is.getReader()->readObject(is)))
        {
          logger.warn() << "operator>>(fpInputStream&, fpAnyLink&): type clash.";
        }
      }
    }
    //is.getReader()->addLink(&bl, obj_id);
  }
  return is;
}

av::OutputStream&
av::operator<<(av::OutputStream& os, const av::AnyLink& bl)
{
  Base* blptr;
  if (bl.isValid())
  {
    blptr = bl.getBasePtr();
  }
  else
  {
    blptr = 0;
  }

  if (os.isBinaryEnabled())
  {
    if (os.getWriteAction())
    {
      if (os.getWriteAction()->isObjectWritten(blptr))
      {
        // object is alraedy in file, only add reference
        (std::ostream&) os << 'R';
        os << os.getWriteAction()->lookupObjectId(blptr);
      }
      else
      {
        // object is not written, write it.
        (std::ostream&) os << 'O';
        // this will recurse write actions on base links
        os.getWriteAction()->traverse(blptr);
      }
    }
  }
  else
  {
    if (os.getWriteAction())
    {
      if (os.getWriteAction()->isObjectWritten(blptr))
      {
        // object is alraedy in file, only add reference
        (std::ostream&) os << 'R';
        (std::ostream&) os << " ";
        (std::ostream&) os << os.getWriteAction()->lookupObjectId(blptr).c_str();
      }
      else
      {
        // object is not written, write it.
        (std::ostream&) os << 'O';
        (std::ostream&) os << " ";
        // this will recurse write actions on base links
        os.getWriteAction()->traverse(blptr);
      }
    }
  }
  return os;
}
