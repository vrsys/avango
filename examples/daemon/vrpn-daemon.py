#!/usr/bin/python
# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2009 Fraunhofer-Gesellschaft zur Foerderung der       #
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
Learn how to configure a VRPN client to handle data sent by a corresponding
VRPN server (in this example 'Tracker0@localhost').
'''

import avango.daemon

# enable logging on a certain level (0-5)
avango.enable_logging(5)

# create a station to propagate input events
station = avango.daemon.Station('vrpn-client')

# configure VRPN client
vrpn = avango.daemon.VRPNClient()
vrpn.station = station
vrpn.server = "Tracker0@localhost"

# start daemon (enter main loop)
avango.daemon.run([vrpn])
