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
Learn how to configure a HIDInput device to collect input data based on
the Linux input system.
'''

import avango.daemon
import os


def main():
    mouse_name = os.popen(
        "ls /dev/input/by-id | grep \"-event-mouse\" | " +
        "sed -e \'s/\"//g\'  | cut -d\" \" -f4"
        ).read()

    mouse_name = mouse_name.split()[0]

    # configure a mouse device
    mouse = avango.daemon.HIDInput()

    # create a station to propagate the input events
    mouse.station = avango.daemon.Station('gua-device-mouse')
    mouse.device = "/dev/input/by-id/" + mouse_name

    # map incoming events to station values
    mouse.values[0] = "EV_REL::REL_X"
    mouse.values[1] = "EV_REL::REL_Y"
    mouse.buttons[0] = "EV_KEY::BTN_LEFT"
    mouse.buttons[1] = "EV_KEY::BTN_RIGHT"

    print("Mouse started at:", mouse_name)

    # start daemon (will enter the main loop)
    avango.daemon.run([mouse])


if __name__ == '__main__':
    main()
