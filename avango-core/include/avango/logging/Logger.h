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

#if !defined(AVANGO_LOGGING_LOGGER_H)
#define AVANGO_LOGGING_LOGGER_H

/**
 * \file
 * \ingroup av_logging
 */

// includes, system

#include <set>
#include <string>
#include <sstream>

#include <boost/format.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

// includes, project

#include <avango/Assert.h>
#include <avango/Config.h>
#include <avango/logging/Level.h>
#include <avango/logging/LoggerManager.h>
#include <avango/windows_specific.h>

namespace av
{
  namespace logging
  {

    class Appender;
    class LoggingEvent;

    // types, exported (class, enum, struct, union, typedef)

    /**
     * Hierarchical logging facility.
     *
     * Loggers are hierarchically sorted by their names with "::" as divider.
     *
     * \ingroup av_logging
     */
    class Logger
    {

    public:

      friend class LoggerManager;
      friend class Stream;

      /**
       * A stream attached to a specific logger with a fixed log
       * level which is assigned to messages passed to the stream.
       * \todo Make it thread-safe.
       */
      class Stream
      {

        friend class Logger;

      public:

        /**
         * Create logger stream for a specific logger and log level
         *
         * \param logger Target logger for all messages passed to the stream
         * \param level log level of the stream.
         */
        AV_DLL Stream(Logger& logger, Level level);

        /**
         * Destructor.
         * Flushes the Stream.
         */
        AV_DLL ~Stream();

        /**
         * Messages passed with << to the Stream are logged to the
         * underlying logger with the current log level.
         */
        template<typename T> Stream& operator<<(const T& t);

        /**
         * Pass values for parameters to Stream. These are used for
         * %-style placeholders with boost::format syntax.
         *
         * operator<<() calls are not allowed after operator,().
         *
         * \note not using boost::format syntax because of operator%-precedence
         */
        template<typename T> Stream& operator,(const T& t);

        /**
         * Flushes the Stream.
         */
        AV_DLL void flush();

      private:

        /**
         * Made private to prevent copy construction
         */
        Stream(const Stream&);

        /**
         * Made private to prevent assignment
         */
        Stream& operator=(const Stream&);

        Logger& mLogger;
        Level mLevel;
        std::ostringstream mBuf;
        boost::format mFormatter;
      };

      /**
       * Create specific logger instance with the given name
       *
       * \param name Name in logger hierarchy, usually the class name, e.g. "av::Logger".
       */
      AV_DLL Logger(const std::string& name = "", Level level = INFO);

      /**
       * Return specific logger with the given name.
       * If it does not yet exist, it is automatically created.
       *
       * \param name Name in logger hierarchy, usually the class name, e.g. "av::Logger".
       *
       * \throw std::invalid_argument if the logger name contains ':::' or ends with '::'
       */
      AV_DLL static Logger& getLogger(const std::string& name);

      /**
       * Return the root logger, which gets all messages.
       */
      AV_DLL static Logger& getRootLogger();

      /**
       * Use Logger as functor to stream message into log.
       * This produces slightly more overhead than calling log() directly.
       */
      AV_DLL Stream operator()(Level level);

      /**
       * Log a message with given log level.
       * \param level Log level of this message.
       * \param msg Message to log.
       */
      AV_DLL void log(Level level, const std::string& msg);

      /**
       * Return stream for loging messages with fatal level.
       */
      AV_DLL Stream fatal();

      /**
       * Log a message with fatal level
       * \param msg Message to log.
       */
      AV_DLL void fatal(const std::string& msg);

      /**
       * Return stream for loging messages with error level.
       */
      AV_DLL Stream error();

      /**
       * Log a message with error level
       * \param msg Message to log.
       */
      AV_DLL void error(const std::string& msg);

      /**
       * Return stream for loging messages with warn level.
       */
      AV_DLL Stream warn();

      /**
       * Log a message with warn level
       * \param msg Message to log.
       */
      AV_DLL void warn(const std::string& msg);

      /**
       * Return stream for loging messages with info level.
       */
      AV_DLL Stream info();

      /**
       * Log a message with info level
       * \param msg Message to log.
       */
      AV_DLL void info(const std::string& msg);

      /**
       * Return stream for loging messages with debug level.
       */
      AV_DLL Stream debug();

      /**
       * Log a message with debug level
       * \param msg Message to log.
       */
      AV_DLL void debug(const std::string& msg);

      /**
       * Return stream for loging messages with trace level.
       */
      AV_DLL Stream trace();

      /**
       * Log a message with trace level
       * \param msg Message to log.
       */
      AV_DLL void trace(const std::string& msg);

      /**
       * Returns the name of this logger.
       */
      AV_DLL const std::string& getName() const;

      /**
       * Returns true if the logger has a parent logger.
       */
      AV_DLL bool hasParent() const;

      /**
       * Returns the parent logger of this logger.
       * \throw std::logic_error If the logger has no parents (which means it is the root logger).
       */
      AV_DLL Logger& getParent();

      /**
       * Returns the parent logger of this logger.
       * \throw std::logic_error If the logger has no parents (which means it is the root logger).
       */
      AV_DLL const Logger& getParent() const;

      /**
       * Get level of this logger. Only messages >= this level are logged
       */
      AV_DLL Level getLevel() const;

      /**
       * Set level of this logger. Only messages >= level are logged
       */
      AV_DLL void setLevel(Level level);

      /**
       * Adds an appender to this logger.
       * If it has been added before, nothing will happen.
       */
      AV_DLL void addAppender(boost::shared_ptr<Appender> appender);

      /**
       * Adds the console appender to this logger.
       * If it has been added before, nothing will happen.
       */
      AV_DLL void addConsoleAppender();

      /**
       * Removes an appender from this logger.
       * If it has not been added before, nothing will happen.
       */
      AV_DLL void removeAppender(boost::shared_ptr<Appender> appender);

      /**
       * Removes the console appender from this logger.
       * If it has not been added before, nothing will happen.
       */
      AV_DLL void removeConsoleAppender();

      /**
       * Removes all appenders from this logger.
       */
      AV_DLL void removeAllAppenders();

      /**
       * Check if this logger (or one of its parents) has at least one appender
       */
      AV_DLL bool isActive(Level level) const;

      /**
       * Functor used to compare two boost::shared_ptrs
       */
      struct AV_DLL compareSharedPtrs
       {
         bool operator()(const boost::shared_ptr<Appender> p1, const boost::shared_ptr<Appender> p2) const
         {
           return p1.get() < p2.get();
         }
       };

      /**
       * Returns a list of already added appenders
       */
      AV_DLL const std::set<boost::shared_ptr<Appender>, compareSharedPtrs >& getAppenders() const;

    private:

      /**
       * Made private to prevent copy construction
       */
      Logger(const Logger&);

      /**
       * Made private to prevent assignment
       */
      Logger& operator=(const Logger&);

      /**
       * Log a LoggingEvent created by ourselves or a child logger.
       */
      void passMessage(LoggingEvent& event);

      /**
       * Set the parent logger.
       * Used only by LoggerManager.
       */
      void setParent(Logger* parent);

      std::string mName;
      Level mLevel;
      Logger* mParent;
      std::set<boost::shared_ptr<Appender>, compareSharedPtrs > mAppenders;
      boost::mutex mAppenderMutex;
    };

    // variables, exported (extern)

    template<typename T> inline Logger::Stream&Logger::Stream::operator<<(const T& t)
    {
      if (mLevel <= mLogger.mLevel) {
        AV_ASSERT(0 == mFormatter.size());
        mBuf << t;
      }
      return *this;
    }

    template<typename T> inline Logger::Stream&
    Logger::Stream::operator,(const T& t)
    {
      if (mLevel <= mLogger.mLevel)
      {
        if (0 == mFormatter.size())
        {
          mFormatter.parse(mBuf.str());
        }

        mFormatter % t;
      }
      return *this;
    }

  } // namespace logging

  // convenience definitions

  /**
   * Convenience definition, shorthand for logging::Logger.
   * \ingroup av_logging
   */
  typedef logging::Logger Logger;

  /**
   * Convenience definition, shorthand for Logger::getLogger.
   * \ingroup av_logging
   */
  AV_DLL inline Logger& getLogger(const std::string& name)
  {
    return logging::LoggerManagerInstance::get().getLogger(name);
  }

  /**
   * Convenience definition, shorthand for Logger::getRootLogger.
   * \ingroup av_logging
   */
  AV_DLL inline Logger& getRootLogger()
  {
    return logging::LoggerManagerInstance::get().getRootLogger();
  }

} // namespace av

#ifdef TRACE_LOGGING
#define LOG_TRACE(trace_logger)\
  (trace_logger).trace()
#else
#define LOG_TRACE(trace_logger)\
  if (false) (trace_logger).trace()
#endif

#define AVANGO_LOG(logger, level, message)\
  if ((level <= AVANGO_LOG_LEVEL) && (logger.isActive(level)))\
    logger.log(level, message);

#endif // #if !defined(AVANGO_LOGGING_LOGGER_H)
