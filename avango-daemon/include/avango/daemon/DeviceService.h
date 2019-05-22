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

#if !defined(AV_DAEMON_DEVICESERVICE_H)
#define AV_DAEMON_DEVICESERVICE_H

/**
 * \file
 * \ingroup av_daemon
 */

#include <avango/FieldContainer.h>
#include <avango/Singleton.h>
#include <avango/daemon/windows_specific_daemon.h>

#include <gua/math.hpp>

namespace av
{
namespace daemon
{
class Station;
class StationSegment;

/**
 * DeviceService, handles shared memory segment used to communicate with
 * Avango Daemon instance.
 *
 * \ingroup av_daemon
 */
class AV_DAEMON_DLL DeviceService : public Base /*, public Singleton<DeviceService>*/
{
    AV_BASE_DECLARE();

  public:
    /**
     * Public Constructor, to provide ability to construct own customized services.
     */
    DeviceService();
    virtual ~DeviceService();

    /**
     * Connects to the shared memory segment.
     */
    void connectDaemon();

    /**
     * Disconnects from the shared memory segment.
     */
    void disconnectDaemon();

    /**
     * Get matrix of a specified station.
     */
    const ::gua::math::mat4& getMatrix(const char* station);

    /**
     * Get button state by specified station and number.
     */
    int getButton(const char* station, int which);

    /**
     * Get value state by specified station and number.
     */
    float getValue(const char* station, int which);

    /**
     * Get LED state by specified station and number.
     */
    bool getLED(const char* station, int which);

    /**
     * Set matrix of a specified station.
     */
    void setMatrix(const char* station, const ::gua::math::mat4& value);

    /**
     * Set a specific button of a specific station.
     */
    void setButton(const char* station, int which, bool value);

    /**
     * Set a specfic value of a specific station.
     */
    void setValue(const char* station, int which, float value);

    /**
     * Set LED state of a specific station.
     */
    void setLED(const char* station, int which, bool value);

    bool getMatrixUsed(const char* station);
    int getButtonsUsed(const char* station);
    int getValuesUsed(const char* station);
    int getLEDsUsed(const char* station);

  protected:
    const ::gua::math::mat4* mIdentityMatrix;
    std::string mCachedStationName;
    Station* mCachedStation;
    StationSegment* mStationSegment;

    Station* lookupCachedStation(const char* name);
    void clearStationCache();
};

using SFDeviceService = SingleField<Link<DeviceService>>;
using MFDeviceService = MultiField<Link<DeviceService>>;
using DevService = Singleton<DeviceService>;
} // namespace daemon

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_DAEMON_DLL SingleField<Link<daemon::DeviceService>>;
template class AV_DAEMON_DLL MultiField<Link<daemon::DeviceService>>;
#endif

} // namespace av

#endif // #if !defined(AV_DAEMON_DEVICESERVICE_H)
