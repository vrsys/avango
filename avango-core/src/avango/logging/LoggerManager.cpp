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

#include <avango/logging/LoggerManager.h>
#include <avango/logging/Logger.h>

namespace
{

  std::string getParentLoggerName(const std::string& name)
  {
    if (name.find(":::") != std::string::npos)
    {
      throw std::invalid_argument("av::logging::LoggerManager::getParentLoggerName: "
                                  "name must not contain ':::'");
    }
    if (name.rfind("::") == name.size()-2)
    {
      throw std::invalid_argument("av::logging::LoggerManager::getParentLoggerName: "
                                  "name must not end with '::'");
    }

    size_t parent_idx = name.rfind("::");
    if (parent_idx != std::string::npos)
    {
      return name.substr(0, parent_idx);
    }
    else
    {
      return "";
    }
  }

} // namespace

av::logging::LoggerManager::LoggerManager()
{
  // create root logger
  mRootLogger = new Logger();
  mLoggerMap[""] = mRootLogger;
}

av::logging::LoggerManager::~LoggerManager()
{
  LoggerMap::const_iterator it = mLoggerMap.begin();
  LoggerMap::const_iterator it_end = mLoggerMap.end();

  for (; it != it_end; ++it )
  {
    delete (it->second);
  }
  mRootLogger = 0;
}

av::logging::LoggerManager::LoggerMap&
av::logging::LoggerManager::getLoggerMap()
{
  return mLoggerMap;
}

av::logging::Logger&
av::logging::LoggerManager::getRootLogger()
{
  return *mRootLogger;
}

av::logging::Logger&
av::logging::LoggerManager::getLogger(const std::string& name)
{
  if (0 == mRootLogger->getName().compare(name))
  {
    return *mRootLogger;
  }

  // creates parent logger if necessary
  Logger *parent_logger(&getLogger(getParentLoggerName(name)));

  {
    // lock after creating parent logger to avoid recursive locking
    boost::mutex::scoped_lock lock(mLoggerMapMutex);

    auto it = mLoggerMap.find(name);

    if (it == mLoggerMap.end()) // not in map
    {
        Logger* logger = new Logger(name);
        logger->setParent(parent_logger);
        mLoggerMap[name] = logger;
        return *logger;
    }
    else
    {
      return *(it->second);
    }
  }
}
