// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* AVANGO is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* AVANGO is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
\************************************************************************/

#include <avango/Config.h>
#include <boost/python.hpp>
#include <avango/Type.h>
#include <avango/logging/Logger.h>
#include <avango/logging/FileAppender.h>
#include <avango/Application.h>
#include <avango/FieldContainer.h>
#include <avango/StandardFields.h>
#include <avango/python/exceptions.h>
#include <avango/python/register_field.h>

#ifdef AVANGO_DISTRIBUTION_SUPPORT
#include <avango/NetLock.h>
#endif

#include "FieldContainer.h"
#include "Field.h"
#include "InputStream.h"
#include "OutputStream.h"

using namespace boost::python;
using namespace av::python;

namespace
{
  void enable_logging(int level, std::string logfile)
  {
    boost::shared_ptr<av::logging::FileAppender> fileapp(new av::logging::FileAppender(logfile));
    av::getRootLogger().addAppender(fileapp);
    av::getRootLogger().setLevel(static_cast<av::logging::Level>(level));
  }

  void enable_logging(int level)
  {
    av::getRootLogger().addConsoleAppender();
    av::getRootLogger().setLevel(static_cast<av::logging::Level>(level));
  }

  void enable_logging(void)
  {
    av::getRootLogger().addConsoleAppender();
    av::getRootLogger().setLevel(av::logging::DEBUG);
  }

  void (*logging0)(void) = &enable_logging;
  void (*logging1)(int) = &enable_logging;
  void (*logging2)(int, std::string) = &enable_logging;

  void python_log(std::string logger, std::string message)
  {
    // TODO add proper severity level setting from python
    av::logging::Logger::getLogger(logger)(av::logging::DEBUG) << message;
  }

  void application_evaluate(void)
  {
    av::ApplicationInstance::get().evaluate();
  }

  bool application_running(void)
  {
    return av::ApplicationInstance::get().running();
  }

  bool application_start(void)
  {
    return av::ApplicationInstance::get().start();
  }

  bool application_stop(void)
  {
    return av::ApplicationInstance::get().stop();
  }

  void application_cleanup(void)
  {
    return av::ApplicationInstance::get().exit(false);
  }

  void application_exit(void)
  {
    return av::ApplicationInstance::get().exit(true);
  }

  void register_exception_translation(void)
  {
    // TODO This is _very_ expensive. Double check if really needed
    boost::python::register_exception_translator<av::python::detail::index_exception>(&av::python::detail::translate);
  }
}

BOOST_PYTHON_MODULE(_avango)
{
  // Enable avango
  av::ApplicationInstance::get();

  // Define further classes
  register_exception_translation();
  init_FieldContainer();
  init_Field();
  init_InputStream();
  init_OutputStream();

#ifdef AVANGO_DISTRIBUTION_SUPPORT
  av::NetLock::initClass();
  class_<av::NetLock, av::Link<av::NetLock>, bases<av::FieldContainer>, boost::noncopyable>("NetLock", "Network-wide Lock object", no_init);
#endif

  def("enable_logging", logging0);
  def("enable_logging", logging1);
  def("enable_logging", logging2);
  def("log", python_log);

  def("evaluate", application_evaluate);
  def("running", application_running);
  def("start", application_start);
  def("stop", application_stop);
  def("cleanup", application_cleanup);
  def("exit", application_exit);
}
