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

#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <sys/types.h>
#include <sys/stat.h>

#include <avango/Logger.h>
#include <avango/daemon/HIDInput.h>
#include <avango/daemon/Station.h>
#include <avango/daemon/LinuxEvent.h>

#define TEST_BIT(bit, array) (array[bit / 8] & (1 << (bit % 8)))

namespace
{
av::Logger& logger(av::getLogger("av::daemon::HIDInput"));

const int KIND_BUTTON = 0;
const int KIND_VALUE = 1;
const int KIND_LED = 2;

// these two defines are used for packing two unsigned shorts into one unsigned int

// bits to left-shift hi short (16)
const unsigned int HIDINPUT_HI_SHIFT = sizeof(unsigned short) * 8;
// mask to filter lo unsigned short (0xFFFF)
const unsigned int HIDINPUT_LO_MASK = USHRT_MAX;

} // namespace

AV_BASE_DEFINE(av::daemon::HIDInput);

av::daemon::HIDInput::HIDInput() : mFd(-1) {}

av::daemon::HIDInput::~HIDInput() { stopDevice(); }

void av::daemon::HIDInput::initClass()
{
    if(!isTypeInitialized())
    {
        av::daemon::Device::initClass();
        AV_BASE_INIT(av::daemon::Device, av::daemon::HIDInput, true);
    }
}

bool av::daemon::HIDInput::open()
{
    close();

    if(-1 == (mFd = ::open(mTty.c_str(), O_RDWR | O_NONBLOCK)))
    {
        logger.error() << "open: failed for " << mTty.c_str() << " with reason: " << ::strerror(errno);
        return false;
    }
    logger.info() << "open: opened " << mTty.c_str() << " (fd: " << mFd << ")";

    struct input_id device_info;

    if(-1 == ioctl(mFd, EVIOCGID, &device_info))
    {
        logger.error() << "open: ioctl(EVIOCGID) failed with reason: " << ::strerror(errno);
        close();
        return false;
    }
    logger.info() << "open: found device " << std::hex << device_info.vendor << ":" << std::hex << device_info.product << " on " << mTty.c_str() << " (fd: " << mFd << ")";
    return true;
}

bool av::daemon::HIDInput::isOpen() const { return (-1 != mFd); }

bool av::daemon::HIDInput::close()
{
    if(isOpen())
    {
        (void)::close(mFd);
        logger.info() << "close: closed " << mTty.c_str() << " (fd: " << mFd << ")";
        mFd = -1;
    }

    return (-1 == mFd) ? true : false;
}

bool av::daemon::HIDInput::readEvent(struct input_event* event)
{
    if(isOpen() && event)
    {
        fd_set rfds;

        FD_ZERO(&rfds);
        FD_SET(mFd, &rfds);

        struct timeval timeout = {0, mTimeout * 1000};

        switch(::select(mFd + 1, &rfds, 0, 0, &timeout))
        {
        case 0:
            logger.trace() << "read: select(2) timed out after " << timeout.tv_sec << " sec for " << mTty.c_str();
            break;

        case -1:
            logger.warn() << "read: select(2) failed for " << mTty.c_str() << " with reason: " << ::strerror(errno) << " (" << errno << ")";
            close();
            break;

        default:
            if(sizeof(input_event) != ::read(mFd, event, sizeof(input_event)))
            {
                logger.warn() << "read: select(2) failed for " << mTty.c_str() << " with reason: " << ::strerror(errno) << " (" << errno << ")";
                close();
            }
            else
            {
                return true;
            }
            break;
        }
    }
    return false;
}

bool av::daemon::HIDInput::writeEvent(const struct input_event& event)
{
    if(isOpen())
    {
        ssize_t retval = ::write(mFd, &event, sizeof(input_event));

        switch(retval)
        {
        case sizeof(input_event):
            logger.trace() << "writeEvent: success for " << mTty.c_str();
            return true;
            break;
        default:
            logger.warn() << "writeEvent: write(2) failed for " << mTty.c_str() << " with reason: " << ::strerror(errno) << " (" << errno << ")";
            close();
            break;
        }
    }
    return false;
}

namespace av
{
namespace daemon
{
template <>
unsigned int HIDInput::scanForOptionalFeature<unsigned int>(const std::string& feature, unsigned int default_value)
{
    std::string feature_value = queryFeature(feature);
    unsigned int return_value = default_value;

    if(!scanUnsignedInteger(feature_value, return_value))
    {
        logger.info() << "scanForOptionalFeature: feature " << feature.c_str() << " not specified, using default of " << default_value;
        return default_value;
    }
    return return_value;
}

template <>
int HIDInput::scanForOptionalFeature<int>(const std::string& feature, int default_value)
{
    std::string feature_value = queryFeature(feature);
    int return_value = default_value;

    if(!scanInteger(feature_value, return_value))
    {
        logger.info() << "scanForOptionalFeature: feature " << feature.c_str() << " not specified, using default of " << default_value;
        return default_value;
    }
    return return_value;
}

template <>
bool HIDInput::scanForOptionalFeature<bool>(const std::string& feature, bool default_value)
{
    std::string feature_value = queryFeature(feature);
    unsigned int scanned_value = default_value;

    if(!scanUnsignedInteger(feature_value, scanned_value))
    {
        logger.info() << "scanForOptionalFeature: feature " << feature.c_str() << " not specified, using default of " << (default_value ? 1 : 0);
        return default_value;
    }
    return scanned_value == 0 ? false : true;
}

template <>
std::string HIDInput::scanForRequiredFeature<std::string>(const std::string& feature)
{
    std::string feature_value = queryFeature(feature);

    if(feature_value.empty())
    {
        logger.info() << "scanForRequiredFeature: feature " << feature.c_str() << " not specified.";
        throw 1;
    }
    return feature_value;
}

template <>
unsigned int HIDInput::scanForRequiredFeature<unsigned int>(const std::string& feature)
{
    std::string feature_value = queryFeature(feature);
    unsigned int return_value = 0;

    if(!scanUnsignedInteger(feature_value, return_value))
    {
        logger.info() << "scanForRequiredFeature: feature " << feature.c_str() << " not specified.";
        throw 1;
    }
    return return_value;
}
} // namespace daemon
} // namespace av

/*virtual*/ int av::daemon::HIDInput::parse_features()
{
    try
    {
        mTty = scanForRequiredFeature<std::string>("tty");
    }
    catch(int err)
    {
        return 0;
    }

    mNormAbs = scanForOptionalFeature<bool>("norm-abs", true);
    mAccumRel = scanForOptionalFeature<bool>("accum-rel-events", false);
    mResetRelCycle = scanForOptionalFeature<int>("reset-rel-values-cycle", 0);
    mTimeout = scanForOptionalFeature<unsigned int>("timeout", 200);

    return 1;
}

/* virtual */ void av::daemon::HIDInput::retrieve_abs_info()
{
    uint8_t bitmask[ABS_MAX / CHAR_BIT + 1] = {0};
    input_absinfo abs_info;

    if(-1 == ioctl(mFd, EVIOCGBIT(EV_ABS, sizeof(bitmask)), bitmask))
    {
        logger.warn() << "retrieve_abs_info: ioctl(EVIOCGBIT(EV_ABS)) failed with reason: " << ::strerror(errno);
        return;
    }

    for(int loop = 0; loop < ABS_MAX; loop++)
    {
        if(TEST_BIT(loop, bitmask))
        {
            // test is o.k. -> feature available
            // retrieve information
            if(-1 == ioctl(mFd, EVIOCGABS(loop), &abs_info))
            {
                logger.warn() << "retrieve_abs_info: ioctl(EVIOCGABS) failed with reason: " << ::strerror(errno);
            }
            else
            {
                // store information in map
                mAbsInfoMap[loop] = abs_info;
            }
        }
    }
}

/* virtual */ void av::daemon::HIDInput::startDevice() {}

/* virtual */ void av::daemon::HIDInput::readLoop()
{
    if(!parse_features())
    {
        logger.warn() << "startDevice: required features missing, not started.";
        return;
    }

    NumStationMap::iterator ns;
    struct input_event event;
    int reset_wait_counter(0);

    for(;;)
    {
        while(!isOpen())
        {
            if(open())
            {
                retrieve_abs_info();
                stopLEDs();
                startLEDs();
            }
            else
            {
                logger.warn() << "readLoop: HID device is not open.";
                // sleep 5 seconds
                struct timeval timeout = {5, 0};
                while(0 != ::select(0, 0, 0, 0, &timeout))
                {
                }
            }
        }

        updateLEDs();
        if(readEvent(&event))
        {
            reset_wait_counter = 0;
            logger.trace() << "readLoop: time " << event.time.tv_sec << "." << event.time.tv_usec << ", type " << event.type << ", code " << event.code << ", value " << event.value;

            for(ns = mStations.begin(); ns != mStations.end(); ++ns)
            {
                int station_index = ns->first;
                if(!stationLooksForEvent(station_index, event))
                {
                    continue;
                }
                applyEventToStation(station_index, event);
            }
        }
        else
        { // timeout
            ++reset_wait_counter;

            // reset all EV_REL mappings to 0.0 if not accumulating relative values
            if(!mAccumRel && (mResetRelCycle >= 0) && (reset_wait_counter >= mResetRelCycle))
            {
                reset_wait_counter = 0;
                clearRelativeStationValues();
                logger.trace() << "readLoop: resetting all EV_REL mappings.";
            }
        }
    }
}

void av::daemon::HIDInput::stopDevice()
{
    if(isOpen())
    {
        stopLEDs();
        close();
        logger.info() << "stopDevice: done.";
    }
}

/* virtual */ void av::daemon::HIDInput::addStation(int station_number, Station* station)
{
    // add station normally
    Device::addStation(station_number, station);

    // create mapping lookups for station
    mStationHIDMappingLookup.insert(StationHIDMappingLookup::value_type(station_number, mDefaultHIDMapping));
    mStationHIDLEDMappingLookup.insert(StationHIDLEDMappingLookup::value_type(station_number, mDefaultHIDLEDMapping));

    // create last led state lookup for station
    mLastLEDStateLookup.insert(LastLEDStateLookup::value_type(station, station->getLEDs()));
}

/* virtual */
std::string av::daemon::HIDInput::getFirstStation()
{
    if(mStations.size() > 0)
        return mStations.begin()->second->getName();
    else
        return "";
}

/* virtual */ void av::daemon::HIDInput::configureFeature(const std::string& feature, const std::string& value)
{
    // we have to check if this is a mapping request
    // feature must be "station::kind::index" and
    // value must be "eventType::eventCode"
    std::string::size_type pos1 = feature.find("::");
    std::string::size_type pos2 = feature.rfind("::");
    bool is_mapping_feature = (pos1 != std::string::npos) && (pos2 != std::string::npos) && (pos2 >= pos1 + 2);
    std::string::size_type pos3 = value.find("::");
    bool is_mapping_value = pos3 != std::string::npos;

    if(is_mapping_feature && is_mapping_value)
    {
        logger.debug() << "configureFeature: configuring feature " << feature.c_str() << " =  " << value.c_str();
        processMappingRequest(feature, value);
    }
    else if(feature == "call")
    {
        if(value == "print-maps")
            printMaps();
        else if(value == "print-abs")
            printAbsInfo();
        else
            logger.warn() << "configureFeature: unknown call " << value.c_str();
    }
    else
    {
        Device::configureFeature(feature, value);
    }
}

/* virtual */ std::string av::daemon::HIDInput::queryFeature(const std::string& feature)
{
    if(feature == "status")
    {
        static std::string status;
        status = (isOpen() ? "RUNNING" : "NOT RUNNING");
        return status;
    }
    else
    {
        return Device::queryFeature(feature);
    }
}

const std::vector<std::string>& av::daemon::HIDInput::queryFeatures() { return mFeatures; }

void av::daemon::HIDInput::processMappingRequest(const std::string& feature, const std::string& value)
{
    std::string station;
    std::string kind;
    std::string index;
    std::string type;
    std::string code;

    bool valid = true;

    logger.debug() << "configureFeature: got feature " << feature.c_str() << " = " << value.c_str() << " for processing.";

    try
    {
        // feature
        std::string::size_type feature_lower = 0;
        std::string::size_type feature_upper = feature.find("::");
        if(feature_upper == std::string::npos)
            throw 1;

        station = feature.substr(feature_lower, feature_upper);
        feature_lower = feature_upper + 2;
        feature_upper = feature.find("::", feature_lower);
        if(feature_upper == std::string::npos)
            throw 1;

        kind = feature.substr(feature_lower, feature_upper - feature_lower);
        feature_lower = feature_upper + 2;
        index = feature.substr(feature_lower);

        // value
        std::string::size_type value_lower = 0;
        std::string::size_type value_upper = value.find("::");
        if(value_upper == std::string::npos)
            throw 1;

        type = value.substr(value_lower, value_upper);
        value_lower = value_upper + 2;
        code = value.substr(value_lower);
    }
    catch(int err)
    {
        valid = false;
    }

    valid = valid && !station.empty() && !kind.empty() && !index.empty() && !type.empty() && !code.empty();

    if(!valid)
        logger.warn() << "processMappingRequest: " << feature.c_str() << " , " << value.c_str() << " invalid.";
    else
    {
        LOG_TRACE(logger) << "processMappingRequest: before conv. t: " << type.c_str() << ", c: " << code.c_str() << ", k: " << kind.c_str() << ", i: " << index.c_str();

        unsigned short eventType = LinuxEvent::getEventCode(type.c_str());
        unsigned short eventCode = LinuxEvent::getEventCode(code.c_str());
        unsigned short kind_value = LinuxEvent::getEventCode(kind.c_str());
        unsigned short index_value = convert_charpointer_to_ulong(index.c_str());
        unsigned int eventPair = pack_into_uint(eventType, eventCode);
        unsigned int stationPair = pack_into_uint(kind_value, index_value);

        LOG_TRACE(logger) << "processMappingRequest: after conv. t: " << eventType << ", c: " << eventCode << ", k: " << kind_value << ", i: " << index_value;

        // sanity check
        switch(kind_value)
        {
        case KIND_BUTTON:
            if(index_value >= Station::sMaxButtons)
            {
                logger.warn() << "processMappingRequest: Button index must be in range [0," << av::daemon::Station::sMaxButtons - 1 << "]!";
                return;
            }
            break;
        case KIND_VALUE:
            if(index_value >= Station::sMaxValues)
            {
                logger.warn() << "processMappingRequest: Value index must be in range [0," << av::daemon::Station::sMaxValues - 1 << "]!";
                return;
            }
            break;
        case KIND_LED:
            if(index_value >= Station::sMaxLeds)
            {
                logger.warn() << "processMappingRequest: LED index must be in range [0," << av::daemon::Station::sMaxLeds - 1 << "]!";
                return;
            }
            if(eventType != EV_LED)
            {
                logger.warn() << "processMappingRequest: event type for STATION_LED must be EV_LED";
                return;
            }
            break;
        default:
        {
            logger.warn() << "processMappingRequest: Kind must be either Button (0) or Value (1) !";
            return;
        }
        }

        logger.debug() << "processMappingRequest: '" << station.c_str() << "::" << kind_value << "::" << index_value << "' <- '" << eventType << "::" << eventCode << "'";

        if(station == "default")
        {
            // insert mapping into default mapping table
            if(kind_value == KIND_LED)
            {
                // doing reversed lookups for LEDs
                if(mDefaultHIDLEDMapping.find(stationPair) != mDefaultHIDLEDMapping.end())
                    mDefaultHIDLEDMapping.erase(stationPair);
                mDefaultHIDLEDMapping.insert(HIDLEDMapping::value_type(stationPair, eventPair));
            }
            else
            {
                if(mDefaultHIDMapping.find(eventPair) != mDefaultHIDMapping.end())
                    mDefaultHIDMapping.erase(eventPair);
                mDefaultHIDMapping.insert(HIDMapping::value_type(eventPair, stationPair));
            }
        }
        else
        {
            // insert mapping into station mapping table
            // find mapping lookup for the station (by name)
            NumStationMap::iterator ns;
            int station_id = -1;

            for(ns = mStations.begin(); ns != mStations.end(); ++ns)
            {
                if(0 == std::strcmp((*ns).second->getName(), station.c_str()))
                {
                    station_id = (*ns).first;
                    break;
                }
            }

            if(station_id >= 0)
            {
                if(kind_value == KIND_LED)
                {
                    // doing reversed lookups for LEDs
                    StationHIDLEDMappingLookup::iterator i = mStationHIDLEDMappingLookup.find(station_id);
                    if(i == mStationHIDLEDMappingLookup.end())
                        logger.warn() << "processMappingRequest: can't get HIDLEDMappingLookup for station " << station_id << " (" << station.c_str() << ")";
                    else
                    {
                        // we found the mapping
                        HIDLEDMapping& hidmapping = (*i).second;
                        if(hidmapping.find(stationPair) != hidmapping.end())
                            hidmapping.erase(stationPair);
                        hidmapping.insert(HIDLEDMapping::value_type(stationPair, eventPair));
                    }
                }
                else
                {
                    StationHIDMappingLookup::iterator i = mStationHIDMappingLookup.find(station_id);
                    if(i == mStationHIDMappingLookup.end())
                        logger.warn() << "processMappingRequest: can't get  HIDMappingLookup for station " << station_id << " (" << station.c_str() << ")";
                    else
                    {
                        // we found the mapping
                        HIDMapping& hidmapping = (*i).second;
                        if(hidmapping.find(eventPair) != hidmapping.end())
                            hidmapping.erase(eventPair);
                        hidmapping.insert(HIDMapping::value_type(eventPair, stationPair));
                    }
                }
            }
            else
            {
                logger.warn() << "processMappingRequest: can't find station " << station.c_str();
            }
        }
    }
}

void av::daemon::HIDInput::printAbsInfo() const
{
    if(0 == mAbsInfoMap.size())
    {
        logger.warn() << "printAbsInfo: No Absolute Axes available.";
        return;
    }

    logger.info() << "printAbsInfo: Absolute Axes: ";

    AbsInfoMap::const_iterator current = mAbsInfoMap.begin();
    while(current != mAbsInfoMap.end())
    {
        const input_absinfo& info = (*current).second;
        logger.info() << eventPairToString(EV_ABS, (*current).first).c_str() << " : " << info.value << " " << info.minimum << " " << info.maximum << " " << info.fuzz << " " << info.flat;
        ++current;
    }
}

void av::daemon::HIDInput::printHIDMapping(const HIDMapping& hidmapping) const
{
    logger.info() << "printHIDMapping: HID->StationSegment map: ";

    HIDMapping::const_iterator current = hidmapping.begin();
    unsigned short evType;
    unsigned short evCode;
    unsigned short kind;
    unsigned short index;

    while(current != hidmapping.end())
    {
        unpack_from_uint((*current).first, evType, evCode);
        unpack_from_uint((*current).second, kind, index);
        logger.info() << eventPairToString(evType, evCode).c_str() << " -> " << stationPairToString(kind, index).c_str() << " ('" << evType << "::" << evCode << " -> " << kind << "::" << index
                      << "')";
        ++current;
    }
}

void av::daemon::HIDInput::printStatus() const { logger.info() << "printStatus: " << (isOpen() ? "OK" : "NOT RUNNING"); }

// packs two ushorts into one uint
unsigned int av::daemon::HIDInput::pack_into_uint(unsigned short v1, unsigned short v2) const { return ((v1 << HIDINPUT_HI_SHIFT) | v2); }
// unpacks one uint into two ushorts
void av::daemon::HIDInput::unpack_from_uint(unsigned int v, unsigned short& v1, unsigned short& v2) const
{
    v1 = v >> HIDINPUT_HI_SHIFT;
    v2 = v & HIDINPUT_LO_MASK;
}

unsigned long int av::daemon::HIDInput::convert_charpointer_to_ulong(const char* arg) const
{
    unsigned long int result = 0;
    errno = 0;

    if(arg)
    {
        // man strtoul:
        // ...
        // The string must begin with an arbitrary amount of white space (as determined by isspace(3))
        // followed by a single optional '+' or '-' sign.  If base is zero  or 16, the string may then
        // include a '0x' prefix, and the number will be read in base 16; otherwise, a zero base is
        // taken as 10 (decimal) unless the next character is '0', in which case it is taken as 8
        // (octal).
        static int base_magic(0);

        result = ::strtoul(arg, 0, base_magic);

        if(errno)
        {
            logger.warn() << "convert_charpointer_to_ulong: unable to convert arg '%s' to int; errno: '%s' ", arg, ::strerror(errno);
            result = 0;
        }
        else
            LOG_TRACE(logger) << "convert_charpointer_to_ulong: converted arg '%s' to %ld", arg, result;
    }
    else
        logger.warn() << "convert_charpointer_to_ulong: got null pointer for arg conversion";

    return result;
}

std::string av::daemon::HIDInput::stationPairToString(unsigned short kind, unsigned short index) const
{
    std::ostringstream out_stream;

    switch(kind)
    {
    case KIND_BUTTON:
        out_stream << "Button[" << index << "]" << std::ends;
        break;
    case KIND_VALUE:
        out_stream << "Value[" << index << "]" << std::ends;
        break;
    default:
        out_stream << "INVALID_KIND[" << index << "]" << std::ends;
        break;
    }
    return out_stream.str();
}

std::string av::daemon::HIDInput::eventPairToString(unsigned short eventType, unsigned short eventCode) const
{
    unsigned short type(eventType);
    unsigned short code(eventCode);
    std::string capability("");
    std::string type_s("");
    std::ostringstream codestrbuf;

    // more verbose output
    codestrbuf << "[code=" << std::setw(3) << std::setfill('0') << std::dec << code << "/0x" << std::setw(3) << std::setfill('0') << std::hex << code << "]" << std::ends;

    switch(type)
    {
    case EV_KEY:
        type_s = "EV_KEY";

        switch(code)
        {
        case KEY_RESERVED:
            capability += "(Reserved)";
            break;
        case KEY_ESC:
            capability += "(Escape)";
            break;
        case KEY_1:
            capability += "(1)";
            break;
        case KEY_2:
            capability += "(2)";
            break;
        case KEY_3:
            capability += "(3)";
            break;
        case KEY_4:
            capability += "(4)";
            break;
        case KEY_5:
            capability += "(5)";
            break;
        case KEY_6:
            capability += "(6)";
            break;
        case KEY_7:
            capability += "(7)";
            break;
        case KEY_8:
            capability += "(8)";
            break;
        case KEY_9:
            capability += "(9)";
            break;
        case KEY_0:
            capability += "(0)";
            break;
        case KEY_MINUS:
            capability += "(-)";
            break;
        case KEY_EQUAL:
            capability += "(=)";
            break;
        case KEY_BACKSPACE:
            capability += "(Backspace)";
            break;
        case KEY_TAB:
            capability += "(Tab)";
            break;
        case KEY_Q:
            capability += "(Q)";
            break;
        case KEY_W:
            capability += "(W)";
            break;
        case KEY_E:
            capability += "(E)";
            break;
        case KEY_R:
            capability += "(R)";
            break;
        case KEY_T:
            capability += "(T)";
            break;
        case KEY_Y:
            capability += "(Y)";
            break;
        case KEY_U:
            capability += "(U)";
            break;
        case KEY_I:
            capability += "(I)";
            break;
        case KEY_O:
            capability += "(O)";
            break;
        case KEY_P:
            capability += "(P)";
            break;
        case KEY_LEFTBRACE:
            capability += "([)";
            break;
        case KEY_RIGHTBRACE:
            capability += "(])";
            break;
        case KEY_ENTER:
            capability += "(Enter)";
            break;
        case KEY_LEFTCTRL:
            capability += "(Ctrl Left)";
            break;
        case KEY_A:
            capability += "(A)";
            break;
        case KEY_S:
            capability += "(S)";
            break;
        case KEY_D:
            capability += "(D)";
            break;
        case KEY_F:
            capability += "(F)";
            break;
        case KEY_G:
            capability += "(G)";
            break;
        case KEY_H:
            capability += "(H)";
            break;
        case KEY_J:
            capability += "(J)";
            break;
        case KEY_K:
            capability += "(K)";
            break;
        case KEY_L:
            capability += "(L)";
            break;
        case KEY_SEMICOLON:
            capability += "(;)";
            break;
        case KEY_APOSTROPHE:
            capability += "(')";
            break;
        case KEY_GRAVE:
            capability += "(`)";
            break;
        case KEY_LEFTSHIFT:
            capability += "(Shift Left)";
            break;
        case KEY_BACKSLASH:
            capability += "(\\)";
            break;
        case KEY_Z:
            capability += "(Z)";
            break;
        case KEY_X:
            capability += "(X)";
            break;
        case KEY_C:
            capability += "(C)";
            break;
        case KEY_V:
            capability += "(V)";
            break;
        case KEY_B:
            capability += "(B)";
            break;
        case KEY_N:
            capability += "(N)";
            break;
        case KEY_M:
            capability += "(M)";
            break;
        case KEY_COMMA:
            capability += "(,)";
            break;
        case KEY_DOT:
            capability += "(.)";
            break;
        case KEY_SLASH:
            capability += "(/)";
            break;
        case KEY_RIGHTSHIFT:
            capability += "(Shift Right)";
            break;
        case KEY_KPASTERISK:
            capability += "(*)";
            break;
        case KEY_LEFTALT:
            capability += "(Alt Left)";
            break;
        case KEY_SPACE:
            capability += "(Space)";
            break;
        case KEY_CAPSLOCK:
            capability += "(CapsLock)";
            break;
        case KEY_F1:
            capability += "(F1)";
            break;
        case KEY_F2:
            capability += "(F2)";
            break;
        case KEY_F3:
            capability += "(F3)";
            break;
        case KEY_F4:
            capability += "(F4)";
            break;
        case KEY_F5:
            capability += "(F5)";
            break;
        case KEY_F6:
            capability += "(F6)";
            break;
        case KEY_F7:
            capability += "(F7)";
            break;
        case KEY_F8:
            capability += "(F8)";
            break;
        case KEY_F9:
            capability += "(F9)";
            break;
        case KEY_F10:
            capability += "(F10)";
            break;
        case KEY_NUMLOCK:
            capability += "(NumLock)";
            break;
        case KEY_SCROLLLOCK:
            capability += "(ScrollLock)";
            break;
        case KEY_KP7:
            capability += "(KeyPad 7)";
            break;
        case KEY_KP8:
            capability += "(KeyPad 8)";
            break;
        case KEY_KP9:
            capability += "(Keypad 9)";
            break;
        case KEY_KPMINUS:
            capability += "(KeyPad Minus)";
            break;
        case KEY_KP4:
            capability += "(KeyPad 4)";
            break;
        case KEY_KP5:
            capability += "(KeyPad 5)";
            break;
        case KEY_KP6:
            capability += "(KeyPad 6)";
            break;
        case KEY_KPPLUS:
            capability += "(KeyPad Plus)";
            break;
        case KEY_KP1:
            capability += "(KeyPad 1)";
            break;
        case KEY_KP2:
            capability += "(KeyPad 2)";
            break;
        case KEY_KP3:
            capability += "(KeyPad 3)";
            break;
        case KEY_KPDOT:
            capability += "(KeyPad decimal point)";
            break;
#if defined(KEY_103RD)
        case KEY_103RD:
            capability += "(KEY_103RD)";
            break;
#endif
        case KEY_F13:
            capability += "(F13)";
            break;
        case KEY_102ND:
            capability += "(KEY_102ND)";
            break;
        case KEY_F11:
            capability += "(F11)";
            break;
        case KEY_F12:
            capability += "(F12)";
            break;
        case KEY_F14:
            capability += "(F14)";
            break;
        case KEY_F15:
            capability += "(F15)";
            break;
        case KEY_F16:
            capability += "(F16)";
            break;
        case KEY_F17:
            capability += "(F17)";
            break;
        case KEY_F18:
            capability += "(F18)";
            break;
        case KEY_F19:
            capability += "(F19)";
            break;
        case KEY_F20:
            capability += "(F20)";
            break;
        case KEY_KPENTER:
            capability += "(Keypad Enter)";
            break;
        case KEY_RIGHTCTRL:
            capability += "(Ctrl Right)";
            break;
        case KEY_KPSLASH:
            capability += "(KeyPad Forward Slash)";
            break;
        case KEY_SYSRQ:
            capability += "(System Request)";
            break;
        case KEY_RIGHTALT:
            capability += "(Alt Right)";
            break;
        case KEY_LINEFEED:
            capability += "(Line Feed)";
            break;
        case KEY_HOME:
            capability += "(Home)";
            break;
        case KEY_UP:
            capability += "(Up)";
            break;
        case KEY_PAGEUP:
            capability += "(Page Up)";
            break;
        case KEY_LEFT:
            capability += "(Left)";
            break;
        case KEY_RIGHT:
            capability += "(Right)";
            break;
        case KEY_END:
            capability += "(End)";
            break;
        case KEY_DOWN:
            capability += "(Down)";
            break;
        case KEY_PAGEDOWN:
            capability += "(Page Down)";
            break;
        case KEY_INSERT:
            capability += "(Insert)";
            break;
        case KEY_DELETE:
            capability += "(Delete)";
            break;
        case KEY_MACRO:
            capability += "(Macro)";
            break;
        case KEY_MUTE:
            capability += "(Mute)";
            break;
        case KEY_VOLUMEDOWN:
            capability += "(Volume Down)";
            break;
        case KEY_VOLUMEUP:
            capability += "(Volume Up)";
            break;
        case KEY_POWER:
            capability += "(Power)";
            break;
        case KEY_KPEQUAL:
            capability += "(KeyPad Equal)";
            break;
        case KEY_KPPLUSMINUS:
            capability += "(KeyPad +/-)";
            break;
        case KEY_PAUSE:
            capability += "(Pause)";
            break;
        case KEY_F21:
            capability += "(F21)";
            break;
        case KEY_F22:
            capability += "(F22)";
            break;
        case KEY_F23:
            capability += "(F23)";
            break;
        case KEY_F24:
            capability += "(F24)";
            break;
        case KEY_KPCOMMA:
            capability += "(KeyPad comma)";
            break;
        case KEY_LEFTMETA:
            capability += "(Meta Left)";
            break;
        case KEY_RIGHTMETA:
            capability += "(Meta Left)";
            break;
        case KEY_COMPOSE:
            capability += "(Compose)";
            break;
        case KEY_STOP:
            capability += "(Stop)";
            break;
        case KEY_AGAIN:
            capability += "(Again)";
            break;
        case KEY_PROPS:
            capability += "(Properties)";
            break;
        case KEY_UNDO:
            capability += "(Undo)";
            break;
        case KEY_FRONT:
            capability += "(Front)";
            break;
        case KEY_COPY:
            capability += "(Copy)";
            break;
        case KEY_OPEN:
            capability += "(Open)";
            break;
        case KEY_PASTE:
            capability += "(Paste)";
            break;
        case KEY_FIND:
            capability += "(Find)";
            break;
        case KEY_CUT:
            capability += "(Cut)";
            break;
        case KEY_HELP:
            capability += "(Help)";
            break;
        case KEY_MENU:
            capability += "(Menu)";
            break;
        case KEY_CALC:
            capability += "(Calculator)";
            break;
        case KEY_SETUP:
            capability += "(Setup)";
            break;
        case KEY_SLEEP:
            capability += "(Sleep)";
            break;
        case KEY_WAKEUP:
            capability += "(Wakeup)";
            break;
        case KEY_FILE:
            capability += "(File)";
            break;
        case KEY_SENDFILE:
            capability += "(Send File)";
            break;
        case KEY_DELETEFILE:
            capability += "(Delete File)";
            break;
        case KEY_XFER:
            capability += "(Transfer)";
            break;
        case KEY_PROG1:
            capability += "(Program 1)";
            break;
        case KEY_PROG2:
            capability += "(Program 2)";
            break;
        case KEY_WWW:
            capability += "(Web Browser)";
            break;
        case KEY_MSDOS:
            capability += "(DOS mode)";
            break;
        case KEY_COFFEE:
            capability += "(Coffee)";
            break;
        case KEY_DIRECTION:
            capability += "(Direction)";
            break;
        case KEY_CYCLEWINDOWS:
            capability += "(Window cycle)";
            break;
        case KEY_MAIL:
            capability += "(Mail)";
            break;
        case KEY_BOOKMARKS:
            capability += "(Book Marks)";
            break;
        case KEY_COMPUTER:
            capability += "(Computer)";
            break;
        case KEY_BACK:
            capability += "(Back)";
            break;
        case KEY_FORWARD:
            capability += "(Forward)";
            break;
        case KEY_CLOSECD:
            capability += "(Close CD)";
            break;
        case KEY_EJECTCD:
            capability += "(Eject CD)";
            break;
        case KEY_EJECTCLOSECD:
            capability += "(Eject / Close CD)";
            break;
        case KEY_NEXTSONG:
            capability += "(Next Song)";
            break;
        case KEY_PLAYPAUSE:
            capability += "(Play and Pause)";
            break;
        case KEY_PREVIOUSSONG:
            capability += "(Previous Song)";
            break;
        case KEY_STOPCD:
            capability += "(Stop CD)";
            break;
        case KEY_RECORD:
            capability += "(Record)";
            break;
        case KEY_REWIND:
            capability += "(Rewind)";
            break;
        case KEY_PHONE:
            capability += "(Phone)";
            break;
        case KEY_ISO:
            capability += "(ISO)";
            break;
        case KEY_CONFIG:
            capability += "(Config)";
            break;
        case KEY_HOMEPAGE:
            capability += "(Home)";
            break;
        case KEY_REFRESH:
            capability += "(Refresh)";
            break;
        case KEY_EXIT:
            capability += "(Exit)";
            break;
        case KEY_MOVE:
            capability += "(Move)";
            break;
        case KEY_EDIT:
            capability += "(Edit)";
            break;
        case KEY_SCROLLUP:
            capability += "(Scroll Up)";
            break;
        case KEY_SCROLLDOWN:
            capability += "(Scroll Down)";
            break;
        case KEY_KPLEFTPAREN:
            capability += "(KeyPad Parenthesis Left)";
            break;
        case KEY_KPRIGHTPAREN:
            capability += "(KeyPad Parenthesis Right)";
            break;
#if defined(KEY_INTL1)
        case KEY_INTL1:
            capability += "(Intl 1)";
            break;
        case KEY_INTL2:
            capability += "(Intl 2)";
            break;
        case KEY_INTL3:
            capability += "(Intl 3)";
            break;
        case KEY_INTL4:
            capability += "(Intl 4)";
            break;
        case KEY_INTL5:
            capability += "(Intl 5)";
            break;
        case KEY_INTL6:
            capability += "(Intl 6)";
            break;
        case KEY_INTL7:
            capability += "(Intl 7)";
            break;
        case KEY_INTL8:
            capability += "(Intl 8)";
            break;
        case KEY_INTL9:
            capability += "(Intl 9)";
            break;
#endif
#if defined(KEY_LANG1)
        case KEY_LANG1:
            capability += "(Language 1)";
            break;
        case KEY_LANG2:
            capability += "(Language 2)";
            break;
        case KEY_LANG3:
            capability += "(Language 3)";
            break;
        case KEY_LANG4:
            capability += "(Language 4)";
            break;
        case KEY_LANG5:
            capability += "(Language 5)";
            break;
        case KEY_LANG6:
            capability += "(Language 6)";
            break;
        case KEY_LANG7:
            capability += "(Language 7)";
            break;
        case KEY_LANG8:
            capability += "(Language 8)";
            break;
        case KEY_LANG9:
            capability += "(Language 9)";
            break;
#endif
        case KEY_PLAYCD:
            capability += "(Play CD)";
            break;
        case KEY_PAUSECD:
            capability += "(Pause CD)";
            break;
        case KEY_PROG3:
            capability += "(Program 3)";
            break;
        case KEY_PROG4:
            capability += "(Program 4)";
            break;
        case KEY_SUSPEND:
            capability += "(Suspend)";
            break;
        case KEY_CLOSE:
            capability += "(Close)";
            break;
        case KEY_UNKNOWN:
            capability += "(Specifically unknown)";
            break;
#if defined(KEY_BRIGHTNESSDOWN)
        case KEY_BRIGHTNESSDOWN:
            capability += "(Brightness Down)";
            break;
#endif
#if defined(KEY_BRIGHTNESSUP)
        case KEY_BRIGHTNESSUP:
            capability += "(Brightness Up)";
            break;
#endif
        case BTN_0:
            capability += "(Button 0)";
            break;
        case BTN_1:
            capability += "(Button 1)";
            break;
        case BTN_2:
            capability += "(Button 2)";
            break;
        case BTN_3:
            capability += "(Button 3)";
            break;
        case BTN_4:
            capability += "(Button 4)";
            break;
        case BTN_5:
            capability += "(Button 5)";
            break;
        case BTN_6:
            capability += "(Button 6)";
            break;
        case BTN_7:
            capability += "(Button 7)";
            break;
        case BTN_8:
            capability += "(Button 8)";
            break;
        case BTN_9:
            capability += "(Button 9)";
            break;
        case BTN_LEFT:
            capability += "(Left Button)";
            break;
        case BTN_RIGHT:
            capability += "(Right Button)";
            break;
        case BTN_MIDDLE:
            capability += "(Middle Button)";
            break;
        case BTN_SIDE:
            capability += "(Side Button)";
            break;
        case BTN_EXTRA:
            capability += "(Extra Button)";
            break;
        case BTN_FORWARD:
            capability += "(Forward Button)";
            break;
        case BTN_BACK:
            capability += "(Back Button)";
            break;
        case BTN_TRIGGER:
            capability += "(Trigger Button)";
            break;
        case BTN_THUMB:
            capability += "(Thumb Button)";
            break;
        case BTN_THUMB2:
            capability += "(2nd Thumb Button)";
            break;
        case BTN_TOP:
            capability += "(Top Button)";
            break;
        case BTN_TOP2:
            capability += "(2nd Top Button)";
            break;
        case BTN_PINKIE:
            capability += "(Pinkie Button)";
            break;
        case BTN_BASE:
            capability += "(Base Button)";
            break;
        case BTN_BASE2:
            capability += "(2nd Base Button)";
            break;
        case BTN_BASE3:
            capability += "(3rd Base Button)";
            break;
        case BTN_BASE4:
            capability += "(4th Base Button)";
            break;
        case BTN_BASE5:
            capability += "(5th Base Button)";
            break;
        case BTN_BASE6:
            capability += "(6th Base Button)";
            break;
        case BTN_DEAD:
            capability += "(Dead Button)";
            break;
        case BTN_A:
            capability += "(Button A)";
            break;
        case BTN_B:
            capability += "(Button B)";
            break;
        case BTN_C:
            capability += "(Button C)";
            break;
        case BTN_X:
            capability += "(Button X)";
            break;
        case BTN_Y:
            capability += "(Button Y)";
            break;
        case BTN_Z:
            capability += "(Button Z)";
            break;
        case BTN_TL:
            capability += "(Thumb Left Button)";
            break;
        case BTN_TR:
            capability += "(Thumb Right Button )";
            break;
        case BTN_TL2:
            capability += "(2nd Thumb Left Button)";
            break;
        case BTN_TR2:
            capability += "(2nd Thumb Right Button )";
            break;
        case BTN_SELECT:
            capability += "(Select Button)";
            break;
        case BTN_MODE:
            capability += "(Mode Button)";
            break;
        case BTN_THUMBL:
            capability += "(Another Left Thumb Button )";
            break;
        case BTN_THUMBR:
            capability += "(Another Right Thumb Button )";
            break;
        case BTN_TOOL_PEN:
            capability += "(Digitiser Pen Tool)";
            break;
        case BTN_TOOL_RUBBER:
            capability += "(Digitiser Rubber Tool)";
            break;
        case BTN_TOOL_BRUSH:
            capability += "(Digitiser Brush Tool)";
            break;
        case BTN_TOOL_PENCIL:
            capability += "(Digitiser Pencil Tool)";
            break;
        case BTN_TOOL_AIRBRUSH:
            capability += "(Digitiser Airbrush Tool)";
            break;
        case BTN_TOOL_FINGER:
            capability += "(Digitiser Finger Tool)";
            break;
        case BTN_TOOL_MOUSE:
            capability += "(Digitiser Mouse Tool)";
            break;
        case BTN_TOOL_LENS:
            capability += "(Digitiser Lens Tool)";
            break;
        case BTN_TOUCH:
            capability += "(Digitiser Touch Button)";
            break;
        case BTN_STYLUS:
            capability += "(Digitiser Stylus Button)";
            break;
        case BTN_STYLUS2:
            capability += "(Second Digitiser Stylus Button)";
            break;
        default:
            capability += "(Unknown Key)";
            break;
        }
        break;
    case EV_REL:
        type_s = "EV_REL";

        switch(code)
        {
        case REL_X:
            capability += "(X Axis)";
            break;
        case REL_Y:
            capability += "(Y Axis)";
            break;
        case REL_Z:
            capability += "(Z Axis)";
            break;
        case REL_HWHEEL:
            capability += "(Horizontal Wheel)";
            break;
        case REL_DIAL:
            capability += "(Dial)";
            break;
        case REL_WHEEL:
            capability += "(Vertical Wheel)";
            break;
        case REL_MISC:
            capability += "(Miscellaneous)";
            break;
        default:
            capability += "(Unknown relative feature)";
            break;
        }
        break;
    case EV_ABS:
        type_s = "EV_ABS";

        switch(code)
        {
        case ABS_X:
            capability += "(X Axis)";
            break;
        case ABS_Y:
            capability += "(Y Axis)";
            break;
        case ABS_Z:
            capability += "(Z Axis)";
            break;
        case ABS_RX:
            capability += "(X Rate Axis)";
            break;
        case ABS_RY:
            capability += "(Y Rate Axis)";
            break;
        case ABS_RZ:
            capability += "(Z Rate Axis)";
            break;
        case ABS_THROTTLE:
            capability += "(Throttle)";
            break;
        case ABS_RUDDER:
            capability += "(Rudder)";
            break;
        case ABS_WHEEL:
            capability += "(Wheel)";
            break;
        case ABS_GAS:
            capability += "(Accelerator)";
            break;
        case ABS_BRAKE:
            capability += "(Brake)";
            break;
        case ABS_HAT0X:
            capability += "(Hat zero, x axis)";
            break;
        case ABS_HAT0Y:
            capability += "(Hat zero, y axis)";
            break;
        case ABS_HAT1X:
            capability += "(Hat one, x axis)";
            break;
        case ABS_HAT1Y:
            capability += "(Hat one, y axis)";
            break;
        case ABS_HAT2X:
            capability += "(Hat two, x axis)";
            break;
        case ABS_HAT2Y:
            capability += "(Hat two, y axis)";
            break;
        case ABS_HAT3X:
            capability += "(Hat three, x axis)";
            break;
        case ABS_HAT3Y:
            capability += "(Hat three, y axis)";
            break;
        case ABS_PRESSURE:
            capability += "(Pressure)";
            break;
        case ABS_DISTANCE:
            capability += "(Distance)";
            break;
        case ABS_TILT_X:
            capability += "(Tilt, X axis)";
            break;
        case ABS_TILT_Y:
            capability += "(Tilt, Y axis)";
            break;
        case ABS_MISC:
            capability += "(Miscellaneous)";
            break;
        default:
            capability += "(Unknown absolute feature)";
            break;
        }
        break;
    case EV_MSC:
        type_s = "EV_MSC";
        break;
    case EV_LED:
        type_s = "EV_LED";

        switch(code)
        {
        case LED_NUML:
            capability += "(Num Lock)";
            break;
        case LED_CAPSL:
            capability += "(Caps Lock)";
            break;
        case LED_SCROLLL:
            capability += "(Scroll Lock)";
            break;
        case LED_COMPOSE:
            capability += "(Compose)";
            break;
        case LED_KANA:
            capability += "(Kana)";
            break;
        case LED_SLEEP:
            capability += "(Sleep)";
            break;
        case LED_SUSPEND:
            capability += "(Suspend)";
            break;
        case LED_MISC:
            capability += "(Miscellaneous)";
            break;
        case LED_MUTE:
            capability += "(Mute)";
            break;
#if defined(LED_MAIL)
        case LED_MAIL:
            capability += "(Mail)";
            break;
#endif
#if defined(LED_CHARGING)
        case LED_CHARGING:
            capability += "(Charging)";
            break;
#endif
        case LED_MAX:
            capability += "(Max)";
            break;
        default:
            capability += "(Unknown LED type)";
            break;
        }
        break;
    case EV_SND:
        type_s = "EV_SND";
        break;
    case EV_REP:
        type_s = "EV_REP";
        break;
    case EV_FF:
        type_s = "EV_FF";
        break;
    default:
        type_s = "UNKNOWN_EVENT_TYPE";
        break;
    }

    return (type_s + "::" + capability + codestrbuf.str());
}

bool av::daemon::HIDInput::scanUnsignedInteger(const std::string& int_string, unsigned int& return_value) const
{
    if(int_string.empty())
        return false;

#if 0
  // stringstream broken on linux; do not use!
  std::stringstream read_stream(int_string);

  if (int_string.size() > 0 && int_string[0] == '0') {
    // hex or octal
    if (int_string.size() > 1 && (int_string[1] == 'x' || int_string[1] == 'X')) {
      // hex
      read_stream >> std::hex >> return_value;
      return !read_stream.fail();
    } else {
      // octal
      read_stream >> std::oct >> r23eturn_value;
      return !read_stream.fail();
    }
  } else {
    // decimal
    read_stream >> return_value;
    return !read_stream.fail();
  }
#else
    // return_value = ::strtoul(int_string.c_str(), 0, 0);
    return_value = convert_charpointer_to_ulong(int_string.c_str());

    return true;
#endif
}

bool av::daemon::HIDInput::scanInteger(const std::string& int_string, int& return_value) const
{
    if(int_string.empty())
        return false;

    return_value = ::strtol(int_string.c_str(), 0, 0);
    return true;
}

bool av::daemon::HIDInput::stationLooksForEvent(int station_index, const struct input_event& event) const
{
    unsigned int type_code_packed = pack_into_uint(event.type, event.code);
    const HIDMapping& hidmapping = getStationHIDMapping(station_index);

    logger.trace() << "readLoop: event pair: " << type_code_packed << " found-el: " << hidmapping.count(type_code_packed);
    return hidmapping.find(type_code_packed) != hidmapping.end();
}

const av::daemon::HIDInput::HIDMapping& av::daemon::HIDInput::getStationHIDMapping(int station_index) const
{
    StationHIDMappingLookup::const_iterator iter = mStationHIDMappingLookup.find(station_index);
    if(iter == mStationHIDMappingLookup.end())
        return mDefaultHIDMapping;

    return (*iter).second;
}

float av::daemon::HIDInput::normalizeAbsValue(const input_event& event) const
{
    float normalized_value = event.value;

    HIDInput::AbsInfoMap::const_iterator iter(mAbsInfoMap.find(event.code));

    if(iter != mAbsInfoMap.end())
    {
        const input_absinfo& info = (*iter).second;
        // from /usr/src/linux/drivers/input/joydev.c
        int c0 = (info.maximum + info.minimum) / 2 - info.flat;
        int c1 = (info.maximum + info.minimum) / 2 + info.flat;
        int t = (info.maximum - info.minimum) / 2 - 2 * info.flat;

        if(t == 0)
        {
            logger.warn() << "readLoop: EV_ABS illegal ABS_INFO (t==0!)";
            // leave value untouched
        }
        else
        {
            int c2 = (1 << 29) / t;
            int val_i = event.value;

            val_i = val_i > c0 ? (val_i < c1 ? 0 : ((c2 * (val_i - c1)) >> 14)) : ((c2 * (val_i - c0)) >> 14);

            // value = 2.0f * (value - (float)info.minimum) /
            //  (float) (info.maximum - info.minimum) - 1.0f;
            normalized_value = val_i / 32767.0f;
            if(normalized_value < -1.0f)
                normalized_value = -1.0f;
            else if(normalized_value > 1.0f)
                normalized_value = 1.0f;
        }
    }

    return normalized_value;
}

void av::daemon::HIDInput::printMaps() const
{
    logger.info() << "printMaps: HID->StationSegment Default map:";
    NumStationMap::const_iterator ns;

    for(ns = mStations.begin(); ns != mStations.end(); ++ns)
    {
        int station_index = (*ns).first;
        Station* station = (*ns).second;

        logger.info() << "printMaps: HID->StationSegment of station " << station_index << " '" << station->getName() << "'";

        StationHIDMappingLookup::const_iterator iter = mStationHIDMappingLookup.find(station_index);

        if(iter != mStationHIDMappingLookup.end())
            printHIDMapping((*iter).second);
        else
            logger.warn() << "printMaps: Can't find HIDMapping!";
    }
}

void av::daemon::HIDInput::applyEventToStation(int station_index, const input_event& event)
{
    HIDMapping::const_iterator matching_hid_entry = getStationHIDMapping(station_index).find(pack_into_uint(event.type, event.code));
    if(matching_hid_entry == getStationHIDMapping(station_index).end())
        return;

    unsigned int kind_index_pair = matching_hid_entry->second;
    unsigned short kind = 0;
    unsigned short index = 0;
    unpack_from_uint(kind_index_pair, kind, index);

    NumStationMap::iterator station_iter = mStations.find(station_index);
    if(station_iter == mStations.end())
        return;

    Station* station = station_iter->second;

    // leave this in for debugging
    logger.trace() << "applyEventToStation: station %d ('%s'): mapping '%s' (%d) -> '%s' ('%d::%d' -> '%d::%d')", station_index, station->getName(), eventPairToString(event.type, event.code).c_str(),
        event.value, stationPairToString(kind, index).c_str(), event.type, event.code, kind, index;

    switch(kind)
    {
    case KIND_BUTTON:
        station->setButton(index, event.value);
        logger.trace() << "applyEventToStation: current button of %d: %d", index, station->getButton(index);
        break;
    case KIND_VALUE:
        // if this is an EV_REL event and _accumRel is true we accumlate value in the station
        if(mAccumRel && event.type == EV_REL)
            station->setValue(index, event.value + station->getValue(index));
        else if(mNormAbs && event.type == EV_ABS)
            station->setValue(index, normalizeAbsValue(event));
        else
            station->setValue(index, event.value);
        logger.trace() << "applyEventToStation: current value of %d: %f", index, station->getValue(index);
        break;
    default:
        logger.fatal() << "applyEventToStation: unknown kind in mapping '%s' for station '%s' (#%d)!", eventPairToString(event.type, event.code).c_str(), station->getName(), station_index;
        break;
    }
}

void av::daemon::HIDInput::clearRelativeStationValues()
{
    for(NumStationMap::iterator ns = mStations.begin(); ns != mStations.end(); ++ns)
    {
        int station_index = (*ns).first;
        Station* station = (*ns).second;
        const HIDMapping& hidmapping = getStationHIDMapping(station_index);
        unsigned short evType;
        unsigned short evCode;
        unsigned short kind;
        unsigned short index;

        for(HIDMapping::const_iterator hidmapping_iter = hidmapping.begin(); hidmapping_iter != hidmapping.end(); ++hidmapping_iter)
        {
            unpack_from_uint(hidmapping_iter->first, evType, evCode);
            unpack_from_uint(hidmapping_iter->second, kind, index);

            if(kind == KIND_VALUE && evType == EV_REL && 0.0 != station->getValue(index))
            {
                logger.trace() << "clearRelativeStationValues: resetting current value of %d (%f) to 0.0", index, station->getValue(index);
                station->setValue(index, 0.0);
            }
            if(kind == KIND_BUTTON && evType == EV_REL && 0 != station->getButton(index))
            {
                logger.debug() << "clearRelativeStationValues: resetting current value of %d (%d) to 0", index, station->getButton(index);
                station->setButton(index, 0);
            }
        }
    }
}

/* virtual */ void av::daemon::HIDInput::updateLED(unsigned short eventCode, bool on)
{
    struct input_event event;

    event.type = EV_LED;
    event.code = eventCode;
    event.value = on;

    writeEvent(event);
}

/* virtual */ void av::daemon::HIDInput::updateLEDs(bool force)
{
    LastLEDStateLookup::iterator iter = mLastLEDStateLookup.begin();
    LastLEDStateLookup::iterator end = mLastLEDStateLookup.end();
    while(iter != end)
    {
        Station* station = iter->first;
        int station_id = -1;

        NumStationMap::const_iterator ns_iter = mStations.begin();
        NumStationMap::const_iterator ns_end = mStations.end();
        while(ns_iter != ns_end)
        {
            if(ns_iter->second == station)
            {
                station_id = ns_iter->first;
                break;
            }
            ++ns_iter;
        }

        for(size_t led = 0; led < iter->second.size(); ++led)
        {
            if((station->getLED(led) != iter->second[led]) || force)
            {
                unsigned int stationPair = pack_into_uint(KIND_LED, led);
                StationHIDLEDMappingLookup::iterator sthm_iter = mStationHIDLEDMappingLookup.find(station_id);
                if(sthm_iter != mStationHIDLEDMappingLookup.end())
                {
                    HIDLEDMapping& hidmapping = sthm_iter->second;
                    HIDLEDMapping::iterator hm_iter = hidmapping.find(stationPair);
                    if(hm_iter != hidmapping.end())
                    {
                        unsigned short eventType;
                        unsigned short eventCode;
                        unpack_from_uint(hm_iter->second, eventType, eventCode);
                        assert(eventType == EV_LED);
                        updateLED(eventCode, station->getLED(led));
                        logger.trace() << "updateLEDs: updated LED state '%s'", eventCode;
                    }
                }
            }
        }
        iter->second = station->getLEDs();
        ++iter;
    }
}

/* virtual */ void av::daemon::HIDInput::startLEDs()
{
    for(int i = 0; i <= LED_MAX; ++i)
        updateLED(i, false);

    updateLEDs(true);
}

/* virtual */ void av::daemon::HIDInput::stopLEDs()
{
    for(int i = 0; i <= LED_MAX; ++i)
        updateLED(i, false);
}
