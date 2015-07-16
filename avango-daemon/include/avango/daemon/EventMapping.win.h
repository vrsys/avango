// -*- Mode:C++ -*-

#if !defined(AV_DAEMON_EVENTMAPPING_WIN_H)
#define AV_DAEMON_EVENTMAPPING_WIN_H

#include <windows.h>
#include <string>
#include <map>
#include <avango/daemon/windows_specific_daemon.h>


/**
 * \file
 * \ingroup av_daemon
 */

namespace av
{
  namespace daemon
  {
    /**
     * \ingroup av_daemon
     */
    class AV_DAEMON_DLL WindowsEventMapping
    {
    public:
      /**
       * Constructor
       */
      WindowsEventMapping();

      using EventPair = std::pair<unsigned short, unsigned short>;

      static unsigned long mapKeyboardEvent(USHORT VKey);

      static bool mapHidButtonEvent(unsigned short usagePage, unsigned short usageId,
                                    unsigned short appUsagePage, unsigned short appUsageId,
                                    EventPair& ev);

      static bool mapHidValueEvent(unsigned short usagePage, unsigned short usageId,
                                    unsigned short appUsagePage, unsigned short appUsageId,
                                    EventPair& ev);
    private:

      ::std::map<USHORT, unsigned long> mKeyboardVkey;
      ::std::map<unsigned short, EventPair > mHidMappingConsumer;
      ::std::map<std::pair<unsigned short, unsigned short>, EventPair > mHidMappingValue;

    };
  }
}

#endif
