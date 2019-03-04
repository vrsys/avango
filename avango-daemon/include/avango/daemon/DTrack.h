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

#if !defined(AV_DAEMON_DTRACK_H)
#define AV_DAEMON_DTRACK_H

#include <avango/daemon/Device.h>

/**
 * \file
 * \ingroup av_daemon
 */

/**
 * Foreward declaration of the DTrackSDK class
 */
class DTrackSDK;

namespace av
{
namespace daemon
{
/**
 * An Avango NG device for processing DTrack udp packets (ASCII protocol).
 * (from A.R.T. GmbH)
 *
 * \ingroup av_daemon
 */
class AV_DAEMON_DLL DTrack : public Device
{
    AV_BASE_DECLARE();

  public:
    /**
     * Constructor
     */
    DTrack();

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~DTrack();

    /**
     * Inherited from base class, implements the initialization of this device.
     */
    void startDevice();

    /**
     * Inherited from base class, implements the loop in which the device is read out.
     */
    void readLoop();

    /**
     * Inherited from base class, implements the closing operation of this device.
     */
    void stopDevice();

    /**
     * Inherited from base class, returns a list of settable features.
     */
    const std::vector<std::string>& queryFeatures();

  private:
    ::std::shared_ptr<::DTrackSDK> mDTrackSDK;
    ::std::vector<::std::string> mRequiredFeatures;
    size_t mPort;

    bool parseFeatures();
    unsigned long int convert_charpointer_to_ulong(const char* arg) const;
};
} // namespace daemon
} // namespace av

#endif
