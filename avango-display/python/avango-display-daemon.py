#!/usr/bin/python
# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of Avango.                                           #
#                                                                        #
# Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       #
# angewandten Forschung (FhG), Munich, Germany.                          #
#                                                                        #
# Avango is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, version 3.                  #
#                                                                        #
# Avango is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
# GNU General Public License for more details.                           #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with Avango. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
# Avango is a trademark owned by FhG.                                    #
#                                                                        #
##########################################################################

import avango.daemon
import sys
import getopt
 
try:
    opts, args = getopt.getopt(sys.argv[1:], "hn:l:d:o:",
                               ["help", "notify=", "log-file=", "display=", "option="])
except getopt.GetoptError, err:
    pass

notify_level = -1
notify_logfile = ''
display_type="Monitor"
options = {}
for opt, arg in opts:
    if opt in ("-h", "--help"):
        print "Usage: python %s [--notify <Level>] [--log-file <Filename>]" % sys.argv[0],
        print " [--display <Display>] [--option <Key>:<Value>]"
        print "Options and arguments:"
        print "--notify <Level>         : selects log-level"
        print "--log-file <Filename>    : selects log output file"
        print "--display <Display>      : selects Display setup"
        print "--option <Key>:<Value>   : set display specific option"
        sys.exit(0)
    elif opt in ("-n", "--notify"):
        notify_level = int(arg)
    elif opt in ("-l", "--log-file"):
        notify_logfile = arg
    elif opt in ("-d", "--display"):
        display_type = arg
    elif opt in ("-o", "--option"):
        try:
            key, value = arg.split(":")
            options[key] = value
        except:
            print "WARNING: Ignoring ill-formated option: '%s'" % arg

if notify_level > -1:
    if notify_logfile == '':
        avango.enable_logging(notify_level)
    else:
        avango.enable_logging(notify_level, notify_logfile)

devices = []

if display_type == "Monitor":
    device_key = "SpaceMouseDevice"
    if device_key in options:
        # create a station to propagate the input events
        station = avango.daemon.Station('spacemousestation')

        # configure a mouse device
        spacemouse = avango.daemon.HIDInput()
        spacemouse.station = station
        spacemouse.device = options[device_key]
        devices.append(spacemouse)

        # map incoming Spacemouse events to station values
        spacemouse.values[0] = "EV_ABS::ABS_X"   # SpaceMouse trans X
        spacemouse.values[1] = "EV_ABS::ABS_Y"   # SpaceMouse trans Y
        spacemouse.values[2] = "EV_ABS::ABS_Z"   # SpaceMouse trans Z
        spacemouse.values[3] = "EV_ABS::ABS_RX"  # SpaceMouse rotate X
        spacemouse.values[4] = "EV_ABS::ABS_RY"  # SpaceMouse rotate Y
        spacemouse.values[5] = "EV_ABS::ABS_RZ"  # SpaceMouse rotate Z

        spacemouse.values[6] = "EV_REL::REL_X"   # SpaceNavigator trans X
        spacemouse.values[7] = "EV_REL::REL_Y"   # SpaceNavigator trans Y
        spacemouse.values[8] = "EV_REL::REL_Z"   # SpaceNavigator trans Z
        spacemouse.values[9] = "EV_REL::REL_RX"  # SpaceNavigator rotate X
        spacemouse.values[10] = "EV_REL::REL_RY" # SpaceNavigator rotate Y
        spacemouse.values[11] = "EV_REL::REL_RZ" # SpaceNavigator rotate Z

        # map incoming button events to station buttons
        spacemouse.buttons[0] = "EV_KEY::BTN_BASE3"    # *
        spacemouse.buttons[1] = "EV_KEY::BTN_TRIGGER"  # 1
        spacemouse.buttons[2] = "EV_KEY::BTN_THUMB"    # 2
        spacemouse.buttons[3] = "EV_KEY::BTN_THUMB2"   # 3
        spacemouse.buttons[4] = "EV_KEY::BTN_TOP"      # 4
        spacemouse.buttons[5] = "EV_KEY::BTN_TOP2"     # 5
        spacemouse.buttons[6] = "EV_KEY::BTN_PINKIE"   # 6  
        spacemouse.buttons[7] = "EV_KEY::BTN_BASE"     # 7
        spacemouse.buttons[8] = "EV_KEY::BTN_BASE2"    # 8
        spacemouse.buttons[9] = "EV_KEY::BTN_0"        # Left SpaceNavigator Button
        spacemouse.buttons[10] = "EV_KEY::BTN_1"       # Right SpaceNavigator Button

# start daemon (will enter the main loop)
avango.daemon.run(devices)
