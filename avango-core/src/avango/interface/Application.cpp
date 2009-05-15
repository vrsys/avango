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

#include <avango/Application.h>

#include <algorithm>
#include <cstdlib>

#include <boost/bind.hpp>
#include <boost/format.hpp>

#include <avango/ContainerPool.h>
#include <avango/FieldContainer.h>
#include <avango/Link.h>
#include <avango/Logger.h>
#include <avango/Type.h>
#include <avango/Init.h>

namespace
{
  av::Logger& logger(av::getLogger("av::Application"));
}

av::Application::Application() :
  mCleanExit(false),
  mKeepRunning(false),
  mRealExit(true),
  mRunning(false)
{
  av::Init::initClass();
}

av::Application::~Application()
{
  // Nothing useful can be done here because we are statically allocated and are destructed
  // at some late point at application exit.
}

void
av::Application::evaluate()
{
  mPreEvaluateContainerCallbackSignal();
  FieldContainer::evaluateAllContainers();
  mPostEvaluateContainerCallbackSignal();
  mRenderCallbackSignal();
}

bool
av::Application::running()
{
  return mRunning;
}

bool
av::Application::start()
{
  if (mRunning) return false;

  mKeepRunning = true;
  mRunning = true;
  while (mKeepRunning)
  {
    evaluate();
  }
  mRunning = false;

  if (mCleanExit)
  {
    cleanExit();
  }

  return true;
}

void
av::Application::startInBackground()
{
  // ?! why boost::thread::thread ?!
  // mThread.reset(new boost::thread::thread(boost::bind(&av::Application::start, this)));
  mThread.reset(new boost::thread(boost::bind(&av::Application::start, this)));
}

bool
av::Application::stop()
{
  if (!mRunning) return false;

  mKeepRunning = false;
  return true;
}

void
av::Application::exit(bool realExit)
{
  mRealExit = realExit;
  if (mRunning)
  {
    // let start() call cleanExit()
    mCleanExit = true;
    stop();
  }
  else
  {
    // call cleanExit() ourselves
    cleanExit();
  }
}

av::Application::CallbackHandle
av::Application::addPreEvaluationContainerCallback(const VoidCallback& callback)
{
  return mPreEvaluateContainerCallbackSignal.connect(callback);
}

av::Application::CallbackHandle
av::Application::addPostEvaluationContainerCallback(const VoidCallback& callback)
{
  return mPostEvaluateContainerCallbackSignal.connect(callback);
}

av::Application::CallbackHandle
av::Application::addRenderCallback(const VoidCallback& callback)
{
  return mRenderCallbackSignal.connect(callback);
}

/* static */ void
av::Application::removeCallback(const CallbackHandle& handle)
{
  handle.disconnect();
}

void
av::Application::disconnectAllFields()
{
  ContainerPool::InstancePoolType pool(ContainerPool::getContainerPool());

  std::vector<Link<FieldContainer> > containers;
  containers.reserve(pool.size());
  ContainerPool::InstancePoolType::const_iterator pool_end(pool.end());
  for (ContainerPool::InstancePoolType::iterator pool_it = pool.begin();
       pool_it != pool_end; ++pool_it)
  {
    containers.push_back(pool_it->second);
  }

  for (std::vector<Link<FieldContainer> >::iterator container_it = containers.begin();
       container_it != containers.end(); ++container_it)
  {
    Link<FieldContainer> container(*container_it);
    AVANGO_LOG(logger, logging::DEBUG, boost::str(boost::format("cleanExit: disconnecting fields from container 0x%x (%s)")
          % container.getPtr()
          % (container->Name.getValue().empty()? "<unnamed>" : container->Name.getValue())))
    container->enableNotify(false);
    for_each(container->getFields().begin(), container->getFields().end(),
             boost::bind(&Field::disconnect, _1));
  }
}

void
av::Application::cleanExit()
{
  FieldContainer::unscheduleEvaluationForAllContainers();
  disconnectAllFields();

  ContainerPool::InstancePoolType pool(ContainerPool::getContainerPool());
  if (!pool.empty())
  {
    AVANGO_LOG(logger, logging::WARN, "cleanExit: container pool still not empty after cleanup:")
    ContainerPool::InstancePoolType::const_iterator pool_end(pool.end());
    for (ContainerPool::InstancePoolType::iterator pool_it = pool.begin();
         pool_it != pool_end; ++pool_it)
    {
      Link<FieldContainer> container(pool_it->second);
      AVANGO_LOG(logger, logging::DEBUG, boost::str(boost::format("           0x%x (%s)")
            % container.getPtr()
            % (container->Name.getValue().empty()? "<unnamed>" : container->Name.getValue())))
    }
  }

  if (mRealExit)
  {
    std::exit(0);
  }
}
