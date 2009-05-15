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

// include i/f header

#include "avango/Base.h"

// includes, system

#include <cstdlib>
#include <iostream>

// includes, project

#include <avango/logging/Logger.h>
#include <avango/WriteAction.h>

#include <avango/Config.h>

// internal unnamed namespace

namespace
{
  // types, internal

  // variables, internal

  bool delete_on_unref = 1;

  av::Logger& logger(av::getLogger("av::Base"));

  // functions, internal

} // namespace

// variables, exported

// functions, exported

AV_TYPED_DEFINE_ABSTRACT(av::Base);

av::Base::Base()
  : mRefCount(0), mHasFloatingRef(false)
{
#if defined(AVANGO_DEBUG)
  LOG_TRACE(logger) << "Base(): @0x%x", this;
#endif
}

/* virtual */
av::Base::~Base()
{
#if defined(AVAVNGO_DEBUG)
  LOG_TRACE(logger) << "~Base(): @0x%x", this;
#endif
}

/* static */ void
av::Base::initClass()
{
  if (!isTypeInitialized()) {
    av::Typed::initClass();

    AV_TYPED_INIT_ABSTRACT(av::Typed, av::Base, true);

    delete_on_unref = (0 == ::getenv("AVANGO_NO_DELETE_ON_UNREF"));

    if (!delete_on_unref)
    {
      logger.info() << "initClass: disabling 'delete this' when reference count goes to zero!";
    }
  }
}

av::Base*
av::Base::castTo(Type type)
{
  if (getTypeId().isOfType(type))
    return this;

  return 0;
}

/* static */ av::Base*
av::Base::castTo(Type type, Base* objectToCast)
{
  if (!objectToCast) {
    return 0;
  }
  return objectToCast->castTo(type);
}

void
av::Base::reference()
{
  refImpl();

#if defined(AVANGO_DEBUG)
  LOG_TRACE(logger) << "reference(): '%s' @0x%x refcount = %d after ref",
    getTypeId().getName().c_str(), this , referenceCount();
#endif
}

void
av::Base::unreference()
{
#if defined(AVANGO_DEBUG)
  LOG_TRACE(logger) << "reference(): '%s' @0x%x refcount = %d before unref",
    getTypeId().getName().c_str(), this, referenceCount();
#endif

  unrefImpl();
}

void
av::Base::unreferenceWithoutDeletion()
{
#if defined(AVANGO_DEBUG)
  LOG_TRACE(logger) << "reference(): '%s' @0x%x refcount = %d before unref",
    getTypeId().getName().c_str(), this, referenceCount();
#endif

  unrefWithoutDeletionImpl();
}

int
av::Base::referenceCount()
{
  return refCountImpl();
}

void
av::Base::setFloatingReference()
{
  setFloatingRefImpl();
}

/* virtual */ void
av::Base::refImpl()
{
  ++mRefCount;
  mHasFloatingRef = false;
}

/* virtual */ void
av::Base::unrefImpl()
{
  assert(mRefCount > 0);

  --mRefCount;

  if (delete_on_unref && !mHasFloatingRef && (mRefCount == 0))
  {
    delete this;
  }
}

/* virtual */ void
av::Base::unrefWithoutDeletionImpl()
{
  assert(mRefCount > 0);

  --mRefCount;
}

/* virtual */ int
av::Base::refCountImpl()
{
  return mRefCount;
}

/*virtual*/ void
av::Base::setFloatingRefImpl()
{
  mHasFloatingRef = true;
}

/* virtual */ void
av::Base::doAction(Action&)
{}

/* virtual */ void
av::Base::doWriteAction(WriteAction& action)
{
  if (action.getStream().isBinaryEnabled()) {
    writeToBinaryStream(action);
  } else {
    writeToASCIIStream(action);
  }
}

void
av::Base::writeToBinaryStream(WriteAction& action)
{
  action.getStream() << getTypeId().getName();
  action.getStream() << action.lookupObjectId(this);
  operator<<(action.getStream(), this);
}

void
av::Base::writeToASCIIStream(WriteAction& action)
{
  (std::ostream&)(action.getStream() << getTypeId().getName()) << " ";
  action.getStream() << action.lookupObjectId(this) << std::endl;
  operator<<(action.getStream(), this);
  action.getStream() << std::endl;
}

/* virtual */ void
av::Base::read(InputStream&)
{}

/* virtual */ void
av::Base::write(OutputStream&)
{}

av::InputStream&
av::operator>>(InputStream& is, Base* obj)
{
  obj->read(is);
  return is;
}

av::OutputStream&
av::operator<<(OutputStream& os, Base* obj)
{
  obj->write(os);
  return os;
}
