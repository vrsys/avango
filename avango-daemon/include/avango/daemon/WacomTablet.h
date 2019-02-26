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

#if !defined(AV_DAEMON_WACOMTABLET_H)
#define AV_DAEMON_WACOMTABLET_H

/**
 * \file
 * \ingroup av_daemon
 */

#include <avango/daemon/HIDInput.h>
#include <gua/math.hpp>
//#include <avango/gua/Object.h>
#include <avango/gua/Fields.hpp>
//#include <avango/gua/MatrixTransform.h>

namespace av
{
namespace daemon
{
/**
 * An Avango NG device for communication with a Wacom tablet.
 * the Linux event system will propagate the inputs via /dev/input/wacom.
 */
class WacomTablet : public HIDInput
{
    AV_BASE_DECLARE();

  public:
    /**
     * Constructor
     */
    WacomTablet();

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~WacomTablet();

    /**
     * Inherited from interface av::daemon::Device, used to initialize the device.
     */
    void startDevice();

    /**
     * overrides readloop() from av::daemon::HIDInput, used to create a transformation Matrix
     * from pen input
     */
    void readLoop();

    /**
     * overrides normalizeAbsValue from HIDInput, used to perform custom normalization
     * of some values
     */
    float normalizeAbsValue(const input_event& event) const;

    /**
     * Retrieves aspect ratio of tablet from maximum absolute values, Ratio is written in Value12
     */
    void retrieveAspectRatio();

    /**
     * overrides HIDInput::parse_features() to add toggle_reset property
     */
    int parse_features();

    /**
     * toggle switch to reset absolute axes when pen is lifted up to zero
     */
    bool mToggleReset;
};
} // namespace daemon
} // namespace av

#endif
