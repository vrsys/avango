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

#if !defined(AV_DAEMON_OCULUS_TRACK_H)
#define AV_DAEMON_OCULUS_TRACK_H

#include <avango/daemon/Device.h>

/**
 * \file
 * \ingroup av_daemon
 */

namespace av
{
namespace daemon
{
class AV_DAEMON_DLL OculusTrack : public Device
{
    AV_BASE_DECLARE();

  public:
    /**
     * Constructor
     */
    OculusTrack();

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~OculusTrack() {}

    /**
     * Inherited from base class, implements the initialization of this device.
     */
    void startDevice() override;

    /**
     * Inherited from base class, implements the loop in which the device is read out.
     */
    void readLoop() override;

    /**
     * Inherited from base class, implements the closing operation of this device.
     */
    void stopDevice() override;

    /**
     * Inherited from base class, returns a list of settable features.
     */
    const std::vector<std::string>& queryFeatures() override;

  private:
    ::std::vector<::std::string> mRequiredFeatures;
    std::string mPort;
    std::string mServer;
    bool parseFeatures();
};
} // namespace daemon
} // namespace av

#endif