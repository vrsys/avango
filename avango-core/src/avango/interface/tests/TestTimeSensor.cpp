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

#ifdef _WIN32
#include <windows.h>
#include <winbase.h>
#else
#include <unistd.h>
#endif

#include <avango/Application.h>
#include <avango/Link.h>
#include <avango/Logger.h>
#include <avango/TimeSensor.h>

#include <avango/UnitTest++/UnitTest++.h>

namespace
{
  double real_time = 0;

  void realTimeChanged(const av::SFDouble::ChangedEvent& event)
  {
    real_time = dynamic_cast<const av::SFDouble*>(event.getField())->getValue();
    AV_ASSERT(real_time);
    av::getRootLogger().info() << "real_time: " << real_time;
  }

  TEST(TimeSensor)
  {
    av::TimeSensor::initClass();
    av::Link<av::TimeSensor> time_sensor(new av::TimeSensor);
    time_sensor->RealTime.addChangedCallback(&realTimeChanged);
    av::ApplicationInstance::get().evaluate();
    CHECK(real_time > 0);
    double old_time = real_time;
#ifdef _WIN32
    Sleep(50);
#else
    usleep(50000);
#endif
    av::ApplicationInstance::get().evaluate();
    av::getRootLogger().info() << "real_time - old_time : " << real_time - old_time;
    CHECK(old_time < real_time);
  }
}

int main()
{
  av::getRootLogger().addConsoleAppender();
  av::ApplicationInstance::get();

  return UnitTest::RunAllTests();
}
