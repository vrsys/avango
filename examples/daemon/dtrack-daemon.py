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
Learn how to configure a DTrack device that communicates with an instance
of a DTrack tracker running on another workstation.
'''

import avango.daemon

# enable logging for detailed information on device setup
#avango.enable_logging()

# create a station for each target you want to track
s1 = avango.daemon.Station('ve-dtrack-head1')
s2 = avango.daemon.Station('ve-dtrack-head2')
s3 = avango.daemon.Station('ve-dtrack-head3')
s4 = avango.daemon.Station('ve-dtrack-pda1')
s5 = avango.daemon.Station('ve-dtrack-pda2')
s6 = avango.daemon.Station('ve-dtrack-head4')
s7 = avango.daemon.Station('ve-dtrack-head5')
s8 = avango.daemon.Station('ve-dtrack-head6')
s9 = avango.daemon.Station('ve-dtrack-head7')
s10 = avango.daemon.Station('ve-dtrack-atek')
s11 = avango.daemon.Station('ve-dtrack-reddot')
s12 = avango.daemon.Station('ve-dtrack-raytac')
s13 = avango.daemon.Station('ve-dtrack-spare')
s14 = avango.daemon.Station('ve-dtrack-head8')
s15 = avango.daemon.Station('ve-dtrack-logitech')

# create instance of DTrack
dtrack = avango.daemon.DTrack()
dtrack.port = "5000"

# add stations (index should correspond to the number in the DTrack tracker)
dtrack.stations[1] = s1
dtrack.stations[2] = s2
dtrack.stations[3] = s3
dtrack.stations[4] = s4
dtrack.stations[5] = s5
dtrack.stations[6] = s6
dtrack.stations[7] = s7
dtrack.stations[8] = s8
dtrack.stations[9] = s9
dtrack.stations[10] = s10
dtrack.stations[11] = s11
dtrack.stations[12] = s12
dtrack.stations[13] = s13
dtrack.stations[14] = s14
dtrack.stations[15] = s15

# start daemon (will enter the main loop)
avango.daemon.run([dtrack])
