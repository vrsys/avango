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

#if !defined(AVANGO_LOGGING_LOGGERMANAGER_H)
#define AVANGO_LOGGING_LOGGERMANAGER_H

#include <boost/thread/mutex.hpp>
#include <map>

#include <avango/Singleton.h>
#include <avango/windows_specific.h>

namespace av
{
namespace logging
{
class Logger;
// class LoggerManager;

/**
 * Responsible for creating and destroying logger instancies.
 */

class AV_DLL LoggerManager /* : public SingletonThreadSafe*/
{
    // friend class SingletonThreadSafe;//<LoggerManager>;

  public:
    /**
     * Return the root logger, which gets all messages.
     */
    Logger& getRootLogger();

    /**
     * Return specific logger with the given name.
     * If it does not yet exist, it is automatically created.
     *
     * \param name Name in logger hierarchy, usually the class name, e.g. "av::Logger".
     *
     * \throw std::invalid_argument if the logger name contains ':::' or ends with '::'
     */
    Logger& getLogger(const std::string& name);

    LoggerManager();
    virtual ~LoggerManager();
    LoggerManager& operator=(const LoggerManager&);

  private:
    using LoggerMap = std::map<std::string, Logger*>;
    LoggerMap& getLoggerMap();

    boost::mutex mLoggerMapMutex;
    Logger* mRootLogger;
    LoggerMap mLoggerMap;
};

using LoggerManagerInstance = Singleton<LoggerManager, CreateUsingNew, DefaultLifeTime, MultiThreaded>;

} // namespace logging

} // namespace av

#endif // #if !defined(AVANGO_LOGGING_LOGGERMANAGER_H)
