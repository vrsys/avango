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

#if !defined(AV_DAEMON_HIDINPUT_H)
#define AV_DAEMON_HIDINPUT_H

/**
 * \file
 * \ingroup av_daemon
 */

#include <climits>
#include <functional>
#include <linux/input.h>
#include <map>
#include <string>

#include <avango/daemon/Device.h>

namespace av
{
  namespace daemon
  {
    /**
     * An Avango NG device to communicate with any devices that are registered
     * via Linux event system (i.e. /dev/input/something using linux/input.h) such as
     * mice, keyboards, joysticks and gamepads.
     *
     * \ingroup av_daemon
     */
    class HIDInput : public Device
    {

    AV_BASE_DECLARE();

    public:

      using HIDMapping = std::map<unsigned int, unsigned int, std::less<unsigned int> >;
      using HIDLEDMapping = std::map<unsigned int, unsigned int, std::less<unsigned int> >;
      using StationHIDMappingLookup = std::map<int, HIDMapping, std::less<int> >;
      using StationHIDLEDMappingLookup = std::map<int, HIDLEDMapping, std::less<int> >;
      using LastLEDStateLookup = std::map<Station*, std::vector<bool> >;

      /**
       * Constructor
       */
      HIDInput();

      /**
       * Overrides addStation of av::daemon::Device
       */
      virtual void addStation(int station_number, Station* station);

      /**
       * Overrides configureFeature of av::daemon::Device
       */
      virtual void configureFeature(const std::string& feature, const std::string& value);

      /**
       * Overrides queryFeature of av::daemon::Device
       */
      virtual std::string queryFeature(const std::string& feature);

      /**
       * Returns the name of the first station as string. Used for the Python binding.
       */
      virtual std::string getFirstStation();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~HIDInput();

      /**
       * Inherited from interface av::daemon::Device, used to initialize the device.
       */
      void startDevice();

      /**
       * Inherited from interface av::daemon::Device, used to read out events.
       */
      void readLoop();

      /**
       * Inherited from interface av::daemon::Device, used to stop device.
       */
      void stopDevice();

      /**
       * Inherited from interface av::daemon::Device, returns a list of settable features.
       */
      const std::vector<std::string>& queryFeatures();

      /**
       * Open device.
       */
      bool open();

      /**
       * Returns true if device is open.
       */
      bool isOpen() const;

      /**
       * Close device.
       */
      bool close();

      /**
       * Read input event.
       */
      bool readEvent(struct input_event* event);

      /**
       * Write event.
       */
      bool writeEvent(const struct input_event& event);

      /**
       * Processes a mapping request. Maps Linux events (e.g. EV_REL::REL_X) to a
       * specific field (e.g. Value0) of the registered station.
       */
      void processMappingRequest(const std::string& feature,const std::string& value);

      /**
       * Convenience method for debug printing.
       */
      void printHIDMapping(const HIDMapping& hidmapping) const;

      /**
       * Packs two ushorts into one uint.
       */
      unsigned int pack_into_uint(unsigned short v1,unsigned short v2) const;

      /**
       * Unpacks one uint into two ushorts.
       */
      void unpack_from_uint(unsigned int v,unsigned short& v1,unsigned short& v2) const;

      /**
       * Converts a char* to unsigned long.
       */
      unsigned long int convert_charpointer_to_ulong(const char* arg) const;

      /**
       * Convenience method for debug printing.
       */
      void printMaps() const;

      /**
       * Convenience method for debug printing.
       */
      void printAbsInfo() const;

      /**
       * Convenience method for debug printing.
       */
      void printStatus() const;

      std::string eventPairToString(unsigned short eventType,unsigned short eventCode) const;
      std::string stationPairToString(unsigned short eventType,unsigned short eventCode) const;

      /**
       * Before the device is started this method ensures that the required and
       * optional features are set properly. In case of an error the device will
       * not start.
       */
      int parse_features();

      void retrieve_abs_info();

      virtual void updateLED(unsigned short eventCode, bool on);
      virtual void updateLEDs(bool force = false);
      virtual void startLEDs();
      virtual void stopLEDs();

      // For every EV_ABS type we store the values (current, min, max, flat, fuzz)
      using AbsInfoMap = std::map<unsigned int, input_absinfo, std::less<unsigned int> >;

      HIDMapping                 mDefaultHIDMapping;
      HIDLEDMapping              mDefaultHIDLEDMapping;
      StationHIDMappingLookup    mStationHIDMappingLookup;
      StationHIDLEDMappingLookup mStationHIDLEDMappingLookup;
      LastLEDStateLookup         mLastLEDStateLookup;
      AbsInfoMap                 mAbsInfoMap;
      std::vector<std::string>   mFeatures;

      int         mFd;
      std::string mTty;
      bool        mNormAbs;       // normalize absolute values of EV_ABS in range [-1,1]
      bool        mAccumRel;      // if set, ALL(!) REL events are accumulated in the requested station
                                  // value!
      int         mResetRelCycle; // if >=0 reset EV_REL mapping to 0.0 if !_accumRel any _resetRel
                                  // cycles where no new data was read
      int         mTimeout;       // event timeout in milliseconds (default: 200)

      virtual float normalizeAbsValue(const struct input_event& event) const;

      bool stationLooksForEvent(int station_index, const struct input_event& event) const;

      void applyEventToStation(int station_index, const struct input_event& event);

      void clearRelativeStationValues();

      template <class ValueType> ValueType scanForOptionalFeature(const std::string& feature, ValueType default_value);


    private:

//      bool stationLooksForEvent(int station_index, const struct input_event& event) const;
      const HIDMapping&  getStationHIDMapping(int station_index) const;
      const HIDLEDMapping&  getStationHIDLEDMapping(int station_index) const;

      bool  scanUnsignedInteger(const std::string& int_string, unsigned int& return_value) const;
      bool  scanInteger(const std::string& int_string, int& return_value) const;

      template <class ValueType> ValueType scanForRequiredFeature(const std::string& feature);

//      void applyEventToStation(int station_index, const struct input_event& event);
//      void clearRelativeStationValues();

    };
  }
}

#endif
