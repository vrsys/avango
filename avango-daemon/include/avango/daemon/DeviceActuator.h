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

#if !defined(AV_DAEMON_DEVICEACTUATOR_H)
#define AV_DAEMON_DEVICEACTUATOR_H

/**
 * \file
 * \ingroup av_daemon
 */

#include <string>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>
#include <avango/daemon/DeviceService.h>

namespace av
{
namespace daemon
{
/**
 * Communicates with DeviceService and provides shared data
 * of a device the associated DeviceService is connected with.
 * This class should be the base class for specific actuator
 * implementations used to send commands or set states of a
 * specific device. (An example can be found in WiimoteActuator.)
 *
 * \ingroup av_daemon
 */
class AV_DAEMON_DLL DeviceActuator : public FieldContainer
{
    AV_FC_DECLARE();

  public:
    DeviceActuator();
    ~DeviceActuator();

    /**
     * Name of device service to communicate with.
     */
    SFDeviceService DeviceService;

    /**
     * Name of station to connect with.
     */
    SFString Station;

    /**
     * Inherited from av::FieldContainer.
     */
    /* virtual */ void fieldHasChanged(const av::Field& field);

  protected:
    std::string mStation;
    av::Link<av::daemon::DeviceService> mService;
};
} // namespace daemon
} // namespace av

#endif
