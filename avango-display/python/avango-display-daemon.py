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

def add_wiimote(device, station):
    wiimote = avango.daemon.Wiimote()
    wiimote.station = avango.daemon.Station(station)
    wiimote.device = device

    wiimote.buttons[ 0] = "EV_KEY::KEY_HOME"     # home
    wiimote.buttons[ 1] = "EV_KEY::KEY_UP"       # up
    wiimote.buttons[ 2] = "EV_KEY::KEY_DOWN"     # down
    wiimote.buttons[ 3] = "EV_KEY::KEY_LEFT"     # left
    wiimote.buttons[ 4] = "EV_KEY::KEY_RIGHT"    # right
    wiimote.buttons[ 5] = "EV_KEY::BTN_LEFT"     # A
    wiimote.buttons[ 6] = "EV_KEY::BTN_RIGHT"    # B
    wiimote.buttons[ 7] = "EV_KEY::KEY_PROG1"    # 1
    wiimote.buttons[ 8] = "EV_KEY::KEY_PROG2"    # 2
    wiimote.buttons[ 9] = "EV_KEY::KEY_BACK"     # -
    wiimote.buttons[10] = "EV_KEY::KEY_FORWARD"  # +
    
    return wiimote
    
 
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
        
elif display_type == "TwoView" or display_type == "iCone":
    #add the DTrack daemon and its known targets
    dtrack = avango.daemon.DTrack()
    dtrack.port = "5000"
    devices.append(dtrack)
    dtrack.stations[ 1] = avango.daemon.Station("ve-dtrack-head1")
    dtrack.stations[ 2] = avango.daemon.Station("ve-dtrack-head2")
    dtrack.stations[ 3] = avango.daemon.Station("ve-dtrack-head3")
    dtrack.stations[ 4] = avango.daemon.Station("ve-dtrack-pda1")
    dtrack.stations[ 5] = avango.daemon.Station("ve-dtrack-pda2")
    dtrack.stations[ 6] = avango.daemon.Station("ve-dtrack-head4")
    dtrack.stations[ 7] = avango.daemon.Station("ve-dtrack-head5")
    dtrack.stations[ 8] = avango.daemon.Station("ve-dtrack-head6")
    dtrack.stations[ 9] = avango.daemon.Station("ve-dtrack-head7")
    dtrack.stations[10] = avango.daemon.Station("ve-dtrack-atek")
    dtrack.stations[11] = avango.daemon.Station("ve-dtrack-reddot")
    dtrack.stations[12] = avango.daemon.Station("ve-dtrack-raytac")
    dtrack.stations[13] = avango.daemon.Station("ve-dtrack-spare")
    dtrack.stations[14] = avango.daemon.Station("ve-dtrack-head8")
    dtrack.stations[15] = avango.daemon.Station("ve-dtrack-logitech")
    
    #add the wiimotes
    devices.append(add_wiimote("/dev/input/ve-wiimote1", "wiimote1"))
    devices.append(add_wiimote("/dev/input/ve-wiimote2", "wiimote2"))
    devices.append(add_wiimote("/dev/input/ve-wiimote3", "wiimote3"))
    devices.append(add_wiimote("/dev/input/ve-wiimote4", "wiimote4"))
    

# start daemon (will enter the main loop)
avango.daemon.run(devices)
