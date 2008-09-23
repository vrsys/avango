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
station = avango.daemon.Station('mousestation')

# configure a mouse device
mouse = avango.daemon.HIDInput()
mouse.station = station
mouse.device = '/dev/input/event2'

# map incoming events to station values
mouse.values[0] = "EV_REL::REL_X"
mouse.values[1] = "EV_REL::REL_Y"

# alternative way of mapping events
#mouse.map_to_station_value(0, "EV_REL::REL_X")
#mouse.map_to_station_value(1, "EV_REL::REL_Y")

# start daemon (will enter the main loop)
avango.daemon.run([mouse])
