#!/usr/bin/python
# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       #
# angewandten Forschung (FhG), Munich, Germany.                          #
#                                                                        #
# AVANGO is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, version 3.                  #
#                                                                        #
# AVANGO is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
# GNU General Public License for more details.                           #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
##########################################################################

'''
Learn how to configure a Wiimote device connected to the Linux input
system via wminput (from the cwiid library).
'''

import avango.daemon

# enable logging for detailed information on device setup
#avango.enable_logging(2)

# create a station to propagate the input events
station = avango.daemon.Station('wiimotestation')

# configure a wiimote device
# Note: Make sure that the patched 'wminput' is running, since this will
#       propagate the inputs to a corresponding /dev/input/event* link
#       (http://abstrakraft.org/cwiid with patches #34,#35 and #36)
wiimote = avango.daemon.Wiimote()
wiimote.station = station
wiimote.device = '/dev/input/event3'

# map incoming events to station buttons
wiimote.buttons[0] = "EV_KEY::KEY_HOME"     # home
wiimote.buttons[1] = "EV_KEY::KEY_UP"       # up
wiimote.buttons[2] = "EV_KEY::KEY_DOWN"     # down
wiimote.buttons[3] = "EV_KEY::KEY_LEFT"     # left
wiimote.buttons[4] = "EV_KEY::KEY_RIGHT"    # right
wiimote.buttons[5] = "EV_KEY::BTN_LEFT"     # A
wiimote.buttons[6] = "EV_KEY::BTN_RIGHT"    # B
wiimote.buttons[7] = "EV_KEY::KEY_PROG1"    # 1
wiimote.buttons[8] = "EV_KEY::KEY_PROG2"    # 2
wiimote.buttons[9] = "EV_KEY::KEY_BACK"     # -
wiimote.buttons[10] = "EV_KEY::KEY_FORWARD" # +

# start daemon (will enter the main loop)
avango.daemon.run([wiimote])
