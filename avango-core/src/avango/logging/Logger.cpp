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

#include <avango/logging/Logger.h>

#include <algorithm>
#include <boost/bind.hpp>
#include <map>
#include <stdexcept>

#include <avango/logging/Appender.h>
#include <avango/logging/ConsoleAppender.h>
#include <avango/logging/LoggingEvent.h>
#include <avango/logging/LoggerManager.h>

av::logging::Logger&
av::logging::Logger::getRootLogger()
{
  return LoggerManagerInstance::get().getRootLogger();
}

av::logging::Logger&
av::logging::Logger::getLogger(const std::string& name)
{
  return LoggerManagerInstance::get().getLogger(name);
}

av::logging::Logger::Logger(const std::string& name, Level level) :
  mName(name),
  mLevel(level),
  mParent(0)
{}

void
av::logging::Logger::setParent(Logger* parent)
{
  AV_ASSERT(parent);
  mParent = parent;
}

av::logging::Logger::Stream
av::logging::Logger::operator()(Level level)
{
  return Stream(*this, level);
}

void
av::logging::Logger::log(Level level, const std::string& msg)
{
  LoggingEvent event(*this, level, msg);
  passMessage(event);
}

void
av::logging::Logger::fatal(const std::string& msg)
{
  log(FATAL, msg);
}

av::logging::Logger::Stream av::logging::Logger::fatal()
{
  return Stream(*this, FATAL);
}

void
av::logging::Logger::error(const std::string& msg)
{
  log(ERROR, msg);
}

av::logging::Logger::Stream av::logging::Logger::error()
{
  return Stream(*this, ERROR);
}

void
av::logging::Logger::warn(const std::string& msg)
{
  log(WARN, msg);
}

av::logging::Logger::Stream av::logging::Logger::warn()
{
  return Stream(*this, WARN);
}

void
av::logging::Logger::info(const std::string& msg)
{
  log(INFO, msg);
}

av::logging::Logger::Stream av::logging::Logger::info()
{
  return Stream(*this, INFO);
}

void
av::logging::Logger::debug(const std::string& msg)
{
  log(DEBUG, msg);
}

av::Logger::Stream av::logging::Logger::debug()
{
  return Stream(*this, DEBUG);
}

void
av::logging::Logger::trace(const std::string& msg)
{
  log(TRACE, msg);
}

av::logging::Logger::Stream av::logging::Logger::trace()
{
  return Stream(*this, TRACE);
}

void
av::logging::Logger::passMessage(LoggingEvent& event)
{
  size_t listSize = mAppenders.size();
  if (event.getLevel() <= mLevel)
  {
    boost::mutex::scoped_lock lock(mAppenderMutex);
    if (listSize == mAppenders.size())
    {
      std::for_each(mAppenders.begin(), mAppenders.end(),
                    boost::bind(&Appender::doAppend, _1, event));
    }
  }
  if (mParent)
  {
    mParent->passMessage(event);
  }
}

const std::string&
av::logging::Logger::getName() const
{
  return mName;
}

bool
av::logging::Logger::hasParent() const
{
  return mParent;
}

const av::logging::Logger&
av::logging::Logger::getParent() const
{
  if (mParent)
  {
    return *mParent;
  }
  else
  {
    AV_ASSERT(this == &getRootLogger());
    throw std::logic_error("av::logging::Logger::getParent: Logger has no parent.");
    return *this;
  }
}

av::logging::Logger&
av::logging::Logger::getParent()
{
  if (mParent)
  {
    return *mParent;
  }
  else
  {
    AV_ASSERT(this == &getRootLogger());
    throw std::logic_error("av::logging::Logger::getParent: Logger has no parent.");
    return *this;
  }
}

av::logging::Level
av::logging::Logger::getLevel() const
{
  return mLevel;
}

void
av::logging::Logger::setLevel(Level level)
{
  mLevel = level;
}

void
av::logging::Logger::addAppender(boost::shared_ptr<Appender> appender)
{
  boost::mutex::scoped_lock lock(mAppenderMutex);
  mAppenders.insert(appender);
}

void
av::logging::Logger::addConsoleAppender()
{
  addAppender(ConsoleAppenderInstance::getAsSharedPtr());
}

void
av::logging::Logger::removeAppender(boost::shared_ptr<Appender> appender)
{
  boost::mutex::scoped_lock lock(mAppenderMutex);
  mAppenders.erase(appender);
}

void
av::logging::Logger::removeConsoleAppender()
{
  removeAppender(ConsoleAppenderInstance::getAsSharedPtr());
}

void
av::logging::Logger::removeAllAppenders()
{
  boost::mutex::scoped_lock lock(mAppenderMutex);
  mAppenders.clear();
}

bool
av::logging::Logger::isActive(Level level) const
{
  return ( (!mAppenders.empty() && level <= mLevel) || ( hasParent() && getParent().isActive(level)));
}

const std::set<boost::shared_ptr<av::logging::Appender>, av::logging::Logger::compareSharedPtrs >&
av::logging::Logger::getAppenders() const
{
  return mAppenders;
}

av::logging::Logger::Stream::Stream(Logger& logger, Level level) :
  mLogger(logger),
  mLevel(level),
  mBuf()
{}

av::logging::Logger::Stream::~Stream()
{
  flush();
}

void
av::logging::Logger::Stream::flush()
{
  if (0 == mFormatter.size())
  {
    if (0 != mBuf.str().size())
    {
      mLogger.log(mLevel, mBuf.str());
    }
  }
  else
  {
    mLogger.log(mLevel, mFormatter.str());
  }
  mBuf.clear();
  mBuf.str("");
  mFormatter.clear();
}
