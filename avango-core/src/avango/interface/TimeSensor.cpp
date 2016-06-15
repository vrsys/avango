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

#include <avango/TimeSensor.h>



#if defined(_WIN32)
#include <sys/types.h>
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif

#include <boost/format.hpp>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::Logger::getLogger("av::TimeSensor"));

  void getRealTime(const av::SFDouble::GetValueEvent& event)
  {
#if defined(_WIN32)
    struct _timeb t;
    _ftime_s(&t);
    (*event.getValuePtr()) = t.time + (t.millitm/1000.0);
#else
    timeval t;
    gettimeofday(&t, 0);
    (*event.getValuePtr()) = t.tv_sec + (t.tv_usec/1000000.0);
#endif
  }

  void setRealTime(const av::SFDouble::SetValueEvent&)
  {
    AVANGO_LOG(logger, av::logging::WARN, "RealTime is read-only")
  }
}

AV_FC_DEFINE(av::TimeSensor);

av::TimeSensor::TimeSensor()
{
  AV_FC_ADD_ADAPTOR_FIELD(RealTime, &getRealTime, &setRealTime);
  AV_FC_ADD_FIELD(ReferenceTime, RealTime.getValue());
  AV_FC_ADD_FIELD(Time, 0);

  alwaysEvaluate(true);
}

av::TimeSensor::~TimeSensor()
{}

void
av::TimeSensor::initClass()
{
  if (!isTypeInitialized())
  {
    FieldContainer::initClass();
    AV_FC_INIT(av::FieldContainer, av::TimeSensor, true);
  }
}

/* virtual */ void
av::TimeSensor::evaluate()
{
  Time.setValue(RealTime.getValue()-ReferenceTime.getValue());
  RealTime.touch();
}
