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

#if !defined(AVANGO_TIMESENSOR_H)
#define AVANGO_TIMESENSOR_H

#include <avango/FieldContainer.h>

namespace av
{
/**
 * Simple time sensor.
 * The reference time is initialized with the current real time on
 * instantiation but may be set to a user-defined value.
 *
 * RealTime is an adaptor for gettimeofday() always containg
 * the current time and is touched in evaluate().
 *
 * Time is updated in evaluate().
 *
 * \ingroup av
 */
class AV_DLL TimeSensor : public FieldContainer
{
    AV_FC_DECLARE();

  public:
    TimeSensor();

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~TimeSensor();

  public:
    /// Time in seconds since 1970-01-01 00:00:00 UTC
    SFDouble RealTime;

    /// Time reference value in seconds since 1970-01-01 00:00:00 UTC
    SFDouble ReferenceTime;

    /// Time in seconds since ReferenceTime
    SFDouble Time;

    virtual void evaluate();
};

} // namespace av

#endif // #if !defined(AVANGO_TIMESENSOR_H)
