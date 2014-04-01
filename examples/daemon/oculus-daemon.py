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
Learn how to configure an Oculus device to fetch the rotation input
of OculusRifts connected via USB.
'''

import avango.daemon

# enable logging for detailed information on device setup
#avango.enable_logging()

# create a station for each oculus
ovr_station_0 = avango.daemon.Station('oculus-0')
ovr_station_1 = avango.daemon.Station('oculus-1')
ovr_station_2 = avango.daemon.Station('oculus-2')

# create instance of Oculus
oculus = avango.daemon.Oculus()

# add stations
oculus.stations[0] = ovr_station_0
oculus.stations[1] = ovr_station_1
oculus.stations[2] = ovr_station_2

# start daemon (will enter the main loop)
avango.daemon.run([oculus])
