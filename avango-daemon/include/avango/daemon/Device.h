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

#if !defined(AV_DAEMON_DEVICE_H)
#define AV_DAEMON_DEVICE_H

#include <map>
#include <thread>
#include <memory>
#include <avango/Base.h>
#include <avango/daemon/Station.h>

/**
 * \file
 * \ingroup av_daemon
 */

namespace av
{
namespace daemon
{
/**
 * Abstract base class of all devices. Devices are used by the DeviceDaemon to interface
 * to external devices like tracking cameras or input devices. The DeviceDaemon communicates
 * via shared memory with an Avango NG application.
 *
 * \ingroup av_daemon
 */
class AV_DAEMON_DLL Device : public Base
{
    AV_BASE_DECLARE_ABSTRACT();

  public:
    using NumStationMap = std::map<int, Station*, std::less<int>>;
    using StringStringMap = std::map<std::string, std::string, std::less<std::string>>;

    /**
     * Constructor.
     */
    Device();

    /**
     * Add a station to this device identified by a station number. Stations are the basic
     * entities which are communicated to interested processes via shared memory.
     */
    virtual void addStation(int station_number, Station* station);

    /**
     * Device specific parameters can be passed via this method in a generic way. The HIDInput
     * device for example accepts the features 'tty', 'vendor' and 'product'. Features
     * are evaluated on startup.
     */
    virtual void configureFeature(const std::string& feature, const std::string& value);

    /**
     * Removes the given feature from the feature map.
     */
    virtual void unconfigureFeature(const std::string& feature);

    /**
     * This function returns the value for a specific feature. If the requested feature has
     * not previously been set, the empty string is returned.
     */
    virtual std::string queryFeature(const std::string& feature);

    /**
     * Returns the name of a station by a given ID, if not exists an empty string is returned.
     */
    virtual std::string getStationName(int id);

    /**
     * Starts up the device. First the features are queried and evaluated. Then a separate
     * process is started using av_thread::create. This process updates the station data
     * in an endless loop.
     */
    bool startUp();

    /**
     * Shuts the device down.
     */
    bool shutDown();

    /**
     * Returns true if the device is running.
     */
    bool isDeviceRunning();

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    ~Device() = default;

    /**
     * This function is called once before the separate thread is created. It is
     * supposed to be overloaded by derived classes.
     */
    virtual void startDevice() = 0;

    /**
     * This function is called once in the created thread. It is supposed to be
     * overloaded by derived classes and should contain an endless loop reading
     * device data and updating the the station records.
     * The loop should look like this: while (mKeepRunning) { ... }
     */
    virtual void readLoop() = 0;

    /**
     * This function is called after the thread is terminated. It is
     * supposed to be overloaded by derived classes.
     */
    virtual void stopDevice() = 0;

    /**
     * This function should return a list of all available features the can be set.
     */
    virtual const std::vector<std::string>& queryFeatures() = 0;

    /**
     * A map of stations of this device.
     */
    NumStationMap mStations;

    /**
     * A map of features of this device.
     */
    StringStringMap mFeatures;

    /**
     * Variable used to trigger the loop
     */
    bool mKeepRunning;

  private:
    static void threadFunction(Device* device);

    const std::string mEmptyFeature;

    std::shared_ptr<std::thread> mThread;
    bool mRunning;
};
} // namespace daemon
} // namespace av

#endif
