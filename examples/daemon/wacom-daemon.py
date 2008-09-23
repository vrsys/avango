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

'''
Learn how to configure a HIDInput device to collect input data based on
the Linux input system.
'''

import avango.daemon

# enable logging for detailed information on device setup
#avango.enable_logging()

# create a station to propagate the input events
station = avango.daemon.Station('wacomstation')

# configure a tablet device
wacom = avango.daemon.WacomIntuos3()
wacom.station = station
wacom.device = '/dev/input/wacom'
#wacom.norm_abs = 'False'

#input events are configures automatically in WacomIntuos3
#the mapping is as following
# map incoming events to station values
#wacom.values[0] = "EV_ABS::ABS_X" #abs x axis pen
#wacom.values[1] = "EV_ABS::ABS_Y" #abs y axis pen
#wacom.values[2] = "EV_ABS::ABS_RX"
#wacom.values[3] = "EV_ABS::ABS_RY"
#wacom.values[4] = "EV_ABS::ABS_RZ"
#wacom.values[5] = "EV_ABS::ABS_THROTTLE"
#wacom.values[6] = "EV_ABS::ABS_WHEEL"
#wacom.values[7] = "EV_ABS::ABS_PRESSURE" #abs pen pressure
#wacom.values[8] = "EV_ABS::ABS_DISTANCE"
#wacom.values[9] = "EV_ABS::ABS_TILT_X" #abs pen tilt X axis
#wacom.values[10] = "EV_ABS::ABS_TILT_Y" #abs pen tilt Y axis
#wacom.values[11] = "EV_REL::REL_WHEEL"

# map incoming key events to station buttons
#wacom.buttons[0] = "EV_KEY::BTN_0"
#wacom.buttons[1] = "EV_KEY::BTN_1"
#wacom.buttons[2] = "EV_KEY::BTN_2"
#wacom.buttons[3] = "EV_KEY::BTN_3"
#wacom.buttons[4] = "EV_KEY::BTN_4"
#wacom.buttons[5] = "EV_KEY::BTN_5"
#wacom.buttons[6] = "EV_KEY::BTN_6"
#wacom.buttons[7] = "EV_KEY::BTN_7"
#wacom.buttons[8] = "EV_KEY::BTN_LEFT"
#wacom.buttons[9] = "EV_KEY::BTN_RIGHT"
#wacom.buttons[10] = "EV_KEY::BTN_MIDDLE"
#wacom.buttons[11] = "EV_KEY::BTN_SIDE"
#wacom.buttons[12] = "EV_KEY::BTN_EXTRA"
#wacom.buttons[13] = "EV_KEY::BTN_TOOL_PEN"
#wacom.buttons[14] = "EV_KEY::BTN_TOOL_RUBBER"
#wacom.buttons[15] = "EV_KEY::BTN_TOOL_BRUSH"
#wacom.buttons[16] = "EV_KEY::BTN_TOOL_PENCIL"
#wacom.buttons[17] = "EV_KEY::BTN_TOOL_AIRBRUSH"
#wacom.buttons[18] = "EV_KEY::BTN_TOOL_FINGER"
#wacom.buttons[19] = "EV_KEY::BTN_TOOL_MOUSE"
#wacom.buttons[20] = "EV_KEY::BTN_TOOL_LENS"
#wacom.buttons[21] = "EV_KEY::BTN_TOUCH"
#wacom.buttons[22] = "EV_KEY::BTN_STYLUS"
#wacom.buttons[23] = "EV_KEY::BTN_STYLUS2"


# alternative way of mapping events
#mouse.map_to_station_value(0, "EV_REL::REL_X")
#mouse.map_to_station_value(1, "EV_REL::REL_Y")

# start daemon (will enter the main loop)
avango.daemon.run([wacom])
